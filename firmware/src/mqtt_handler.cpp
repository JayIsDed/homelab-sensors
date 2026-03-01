#include "mqtt_handler.h"
#include "config.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <LittleFS.h>

static WiFiClientSecure tlsClient;
static PubSubClient mqttClient(tlsClient);
static String mqttUser;
static String mqttPass;

static void loadCACert() {
    if (!LittleFS.begin(true)) {
        Serial.println("[MQTT] LittleFS mount failed");
        return;
    }
    File caFile = LittleFS.open("/ca_cert.pem", "r");
    if (!caFile) {
        Serial.println("[MQTT] CA cert not found, using insecure");
        tlsClient.setInsecure();
        return;
    }
    String caCert = caFile.readString();
    caFile.close();
    tlsClient.setCACert(caCert.c_str());
    Serial.println("[MQTT] CA cert loaded");
}

void mqtt_init(const String& host, const String& user, const String& pass) {
    mqttUser = user;
    mqttPass = pass;
    loadCACert();
    mqttClient.setServer(host.c_str(), MQTT_PORT);
    mqttClient.setBufferSize(MQTT_BUFFER_SIZE);
    Serial.printf("[MQTT] Configured: %s:%d user=%s\n", host.c_str(), MQTT_PORT, user.c_str());
}

bool mqtt_connect() {
    if (mqttClient.connected()) return true;

    Serial.println("[MQTT] Connecting...");
    String clientId = "sensor-" + String((uint32_t)ESP.getEfuseMac(), HEX);

    if (mqttClient.connect(clientId.c_str(), mqttUser.c_str(), mqttPass.c_str())) {
        Serial.println("[MQTT] Connected");
        return true;
    }
    Serial.printf("[MQTT] Failed, rc=%d\n", mqttClient.state());
    return false;
}

bool mqtt_publish(const String& sensor_id, const char* payload) {
    String topic = String(MQTT_TOPIC_PREFIX) + "/" + sensor_id + "/data";
    bool ok = mqttClient.publish(topic.c_str(), payload);
    if (ok) {
        Serial.printf("[MQTT] Published to %s\n", topic.c_str());
    } else {
        Serial.println("[MQTT] Publish failed");
    }
    return ok;
}

void mqtt_loop() {
    mqttClient.loop();
}

bool mqtt_connected() {
    return mqttClient.connected();
}
