#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>

#include "config.h"
#include "nvs_config.h"
#include "sensor_handler.h"
#include "mqtt_handler.h"
#include "ble_prov.h"
#include "led_status.h"

enum State {
    STATE_BOOT,
    STATE_CHECK_NVS,
    STATE_BLE_PROV,
    STATE_SERIAL_CONFIG,
    STATE_WIFI_CONNECT,
    STATE_MQTT_CONNECT,
    STATE_SENSOR_LOOP
};

static State currentState = STATE_BOOT;
static SensorConfig config;
static unsigned long lastReadTime = 0;
static unsigned long stateEntryTime = 0;

// --- Factory Reset Check ---
static bool checkFactoryReset() {
    pinMode(PIN_FACTORY_RESET, INPUT_PULLUP);
    delay(50);
    if (digitalRead(PIN_FACTORY_RESET) == LOW) {
        Serial.println("[BOOT] Factory reset triggered!");
        nvs_clear();
        Serial.println("[BOOT] NVS cleared. Rebooting...");
        delay(500);
        ESP.restart();
        return true;
    }
    return false;
}

// --- Serial Configuration ---
// Reads MQTT host, user, pass, sensor ID, location, interval from serial
static bool serialConfigStep() {
    if (!Serial.available()) return false;

    String line = Serial.readStringUntil('\n');
    line.trim();

    if (line.startsWith("host=")) {
        config.mqtt_host = line.substring(5);
        Serial.printf("[CFG] MQTT host: %s\n", config.mqtt_host.c_str());
    } else if (line.startsWith("user=")) {
        config.mqtt_user = line.substring(5);
        Serial.printf("[CFG] MQTT user: %s\n", config.mqtt_user.c_str());
    } else if (line.startsWith("pass=")) {
        config.mqtt_pass = line.substring(5);
        Serial.println("[CFG] MQTT pass: (set)");
    } else if (line.startsWith("id=")) {
        config.sensor_id = line.substring(3);
        Serial.printf("[CFG] Sensor ID: %s\n", config.sensor_id.c_str());
    } else if (line.startsWith("location=")) {
        config.location = line.substring(9);
        Serial.printf("[CFG] Location: %s\n", config.location.c_str());
    } else if (line.startsWith("interval=")) {
        config.interval_s = line.substring(9).toInt();
        Serial.printf("[CFG] Interval: %ds\n", config.interval_s);
    } else if (line == "save") {
        if (config.mqtt_host.length() > 0 && config.mqtt_user.length() > 0 &&
            config.mqtt_pass.length() > 0 && config.sensor_id.length() > 0) {
            config.provisioned = true;
            nvs_save(config);
            Serial.println("[CFG] Config saved! Rebooting...");
            delay(500);
            ESP.restart();
        } else {
            Serial.println("[CFG] ERROR: host, user, pass, id are all required");
        }
    } else if (line == "show") {
        Serial.printf("[CFG] host=%s user=%s id=%s location=%s interval=%d\n",
                      config.mqtt_host.c_str(), config.mqtt_user.c_str(),
                      config.sensor_id.c_str(), config.location.c_str(),
                      config.interval_s);
    } else {
        Serial.println("[CFG] Commands: host=, user=, pass=, id=, location=, interval=, show, save");
    }
    return false;
}

// --- Build JSON payload ---
static String buildPayload(const SensorReading& reading) {
    JsonDocument doc;
    doc["sensor_id"] = config.sensor_id;
    doc["location"] = config.location;
    doc["temperature"] = serialized(String(reading.temperature, 1));
    doc["humidity"] = serialized(String(reading.humidity, 1));
    doc["wifi_rssi"] = WiFi.RSSI();
    doc["uptime_s"] = millis() / 1000;

    String payload;
    serializeJson(doc, payload);
    return payload;
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n=== Homelab Sensor ===");

    led_init();
    nvs_init();

    if (checkFactoryReset()) return;

    currentState = STATE_CHECK_NVS;
}

void loop() {
    led_update();

    switch (currentState) {
        case STATE_CHECK_NVS: {
            config = nvs_load();
            if (config.provisioned) {
                Serial.println("[BOOT] Config found, connecting...");
                currentState = STATE_WIFI_CONNECT;
                led_set_mode(LED_CONNECTING);
            } else {
                Serial.println("[BOOT] Not provisioned, starting BLE...");
                currentState = STATE_BLE_PROV;
                led_set_mode(LED_PROVISIONING);
                ble_prov_start();
            }
            stateEntryTime = millis();
            break;
        }

        case STATE_BLE_PROV: {
            if (ble_prov_is_done()) {
                Serial.println("[BOOT] WiFi provisioned via BLE");
                Serial.println("[BOOT] Enter MQTT config via serial:");
                Serial.println("  host=mqtt.example.com");
                Serial.println("  user=sensor-01");
                Serial.println("  pass=secretpass");
                Serial.println("  id=sensor-01");
                Serial.println("  location=living-room");
                Serial.println("  interval=60");
                Serial.println("  save");
                currentState = STATE_SERIAL_CONFIG;
            }
            break;
        }

        case STATE_SERIAL_CONFIG: {
            serialConfigStep();
            break;
        }

        case STATE_WIFI_CONNECT: {
            if (WiFi.status() == WL_CONNECTED) {
                Serial.printf("[WIFI] Connected, IP: %s\n", WiFi.localIP().toString().c_str());
                mqtt_init(config.mqtt_host, config.mqtt_user, config.mqtt_pass);
                if (!sensor_init()) {
                    Serial.println("[BOOT] Sensor init failed!");
                    led_set_mode(LED_ERROR);
                    return;
                }
                currentState = STATE_MQTT_CONNECT;
                stateEntryTime = millis();
            } else if (millis() - stateEntryTime > WIFI_CONNECT_TIMEOUT_MS) {
                Serial.println("[WIFI] Connection timeout, retrying...");
                WiFi.begin();  // Retry with stored credentials
                stateEntryTime = millis();
            }
            break;
        }

        case STATE_MQTT_CONNECT: {
            if (mqtt_connect()) {
                Serial.println("[BOOT] Ready! Entering sensor loop.");
                led_set_mode(LED_OK);
                currentState = STATE_SENSOR_LOOP;
                lastReadTime = 0;  // Force immediate first read
            } else if (millis() - stateEntryTime > MQTT_CONNECT_TIMEOUT_MS) {
                Serial.println("[MQTT] Connection timeout, retrying...");
                stateEntryTime = millis();
                delay(MQTT_RETRY_DELAY_MS);
            }
            break;
        }

        case STATE_SENSOR_LOOP: {
            mqtt_loop();

            if (!mqtt_connected()) {
                Serial.println("[MQTT] Disconnected, reconnecting...");
                led_set_mode(LED_CONNECTING);
                currentState = STATE_MQTT_CONNECT;
                stateEntryTime = millis();
                break;
            }

            unsigned long now = millis();
            if (now - lastReadTime >= (unsigned long)config.interval_s * 1000 || lastReadTime == 0) {
                lastReadTime = now;
                SensorReading reading = sensor_read();
                if (reading.valid) {
                    String payload = buildPayload(reading);
                    mqtt_publish(config.sensor_id, payload.c_str());
                } else {
                    led_set_mode(LED_ERROR);
                    delay(2000);
                    led_set_mode(LED_OK);
                }
            }
            break;
        }

        default:
            break;
    }
}
