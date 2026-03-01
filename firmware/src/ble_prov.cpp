#include "ble_prov.h"
#include "config.h"
#include <WiFi.h>
#include <WiFiProv.h>

static bool provDone = false;

static void prov_event_handler(arduino_event_t* sys_event) {
    switch (sys_event->event_id) {
        case ARDUINO_EVENT_PROV_START:
            Serial.println("[BLE] Provisioning started");
            break;
        case ARDUINO_EVENT_PROV_CRED_RECV:
            Serial.println("[BLE] Credentials received");
            break;
        case ARDUINO_EVENT_PROV_CRED_SUCCESS:
            Serial.println("[BLE] Provisioning successful");
            provDone = true;
            break;
        case ARDUINO_EVENT_PROV_CRED_FAIL:
            Serial.println("[BLE] Provisioning failed");
            break;
        case ARDUINO_EVENT_PROV_END:
            Serial.println("[BLE] Provisioning ended");
            break;
        default:
            break;
    }
}

void ble_prov_start() {
    provDone = false;
    WiFi.onEvent(prov_event_handler);

    // Generate unique device name from MAC
    uint8_t mac[6];
    WiFi.macAddress(mac);
    char deviceName[32];
    snprintf(deviceName, sizeof(deviceName), "%s-%02X%02X",
             BLE_DEVICE_PREFIX, mac[4], mac[5]);

    Serial.printf("[BLE] Starting provisioning as '%s'\n", deviceName);
    Serial.printf("[BLE] PoP PIN: %s\n", BLE_POP_PIN);

    WiFiProv.beginProvision(
        NETWORK_PROV_SCHEME_BLE,
        NETWORK_PROV_SCHEME_HANDLER_FREE_BLE,
        NETWORK_PROV_SECURITY_1,
        BLE_POP_PIN,
        deviceName
    );
}

bool ble_prov_is_done() {
    return provDone;
}
