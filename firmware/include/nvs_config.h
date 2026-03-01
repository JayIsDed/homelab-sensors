#pragma once

#include <Arduino.h>

struct SensorConfig {
    bool provisioned;
    String mqtt_host;
    String mqtt_user;
    String mqtt_pass;
    String sensor_id;
    String location;
    uint32_t interval_s;
};

void nvs_init();
SensorConfig nvs_load();
void nvs_save(const SensorConfig& cfg);
void nvs_clear();
bool nvs_is_provisioned();
