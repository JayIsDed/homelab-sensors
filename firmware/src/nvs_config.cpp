#include "nvs_config.h"
#include "config.h"
#include <Preferences.h>

static Preferences prefs;

void nvs_init() {
    prefs.begin(NVS_NAMESPACE, false);
}

SensorConfig nvs_load() {
    SensorConfig cfg;
    cfg.provisioned = prefs.getBool(NVS_KEY_PROVISIONED, false);
    cfg.mqtt_host   = prefs.getString(NVS_KEY_MQTT_HOST, "");
    cfg.mqtt_user   = prefs.getString(NVS_KEY_MQTT_USER, "");
    cfg.mqtt_pass   = prefs.getString(NVS_KEY_MQTT_PASS, "");
    cfg.sensor_id   = prefs.getString(NVS_KEY_SENSOR_ID, "");
    cfg.location    = prefs.getString(NVS_KEY_LOCATION, "unknown");
    cfg.interval_s  = prefs.getUInt(NVS_KEY_INTERVAL, DEFAULT_READ_INTERVAL_S);
    return cfg;
}

void nvs_save(const SensorConfig& cfg) {
    prefs.putBool(NVS_KEY_PROVISIONED, cfg.provisioned);
    prefs.putString(NVS_KEY_MQTT_HOST, cfg.mqtt_host);
    prefs.putString(NVS_KEY_MQTT_USER, cfg.mqtt_user);
    prefs.putString(NVS_KEY_MQTT_PASS, cfg.mqtt_pass);
    prefs.putString(NVS_KEY_SENSOR_ID, cfg.sensor_id);
    prefs.putString(NVS_KEY_LOCATION, cfg.location);
    prefs.putUInt(NVS_KEY_INTERVAL, cfg.interval_s);
}

void nvs_clear() {
    prefs.clear();
}

bool nvs_is_provisioned() {
    return prefs.getBool(NVS_KEY_PROVISIONED, false);
}
