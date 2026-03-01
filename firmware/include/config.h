#pragma once

// --- Pin Definitions (XIAO ESP32-C6) ---
#define PIN_SDA           6    // D4/GPIO6 — I2C SDA
#define PIN_SCL           7    // D5/GPIO7 — I2C SCL
#define PIN_LED           15   // Onboard LED
#define PIN_FACTORY_RESET 2    // D0/GPIO2 — Ground to factory reset

// --- I2C ---
#define SHT31_ADDRESS     0x44

// --- MQTT Defaults ---
#define MQTT_PORT         8883
#define MQTT_TOPIC_PREFIX "homelab/sensors"

// --- Sensor ---
#define DEFAULT_READ_INTERVAL_S 60  // Seconds between sensor reads
#define SENSOR_WARMUP_MS        50  // ms delay after SHT31 wakeup

// --- WiFi ---
#define WIFI_CONNECT_TIMEOUT_MS 15000
#define WIFI_RETRY_DELAY_MS     5000

// --- MQTT ---
#define MQTT_CONNECT_TIMEOUT_MS 10000
#define MQTT_RETRY_DELAY_MS     5000
#define MQTT_BUFFER_SIZE        512

// --- BLE Provisioning ---
#define BLE_DEVICE_PREFIX "Sensor"  // BLE name: Sensor-XXXX
#define BLE_POP_PIN       "12345678"

// --- NVS Keys ---
#define NVS_NAMESPACE     "sensor_cfg"
#define NVS_KEY_PROVISIONED "prov"
#define NVS_KEY_MQTT_HOST   "mqtt_host"
#define NVS_KEY_MQTT_USER   "mqtt_user"
#define NVS_KEY_MQTT_PASS   "mqtt_pass"
#define NVS_KEY_SENSOR_ID   "sensor_id"
#define NVS_KEY_LOCATION    "location"
#define NVS_KEY_INTERVAL    "interval"

// --- LED Patterns (ms) ---
#define LED_BLINK_PROV     150   // Fast blink: provisioning mode
#define LED_BLINK_CONNECT  500   // Medium blink: connecting
#define LED_PULSE_OK       2000  // Slow pulse: connected & reporting
#define LED_BLINK_ERROR    100   // Very fast: error
