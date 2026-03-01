#pragma once

#include <Arduino.h>

struct SensorReading {
    float temperature;
    float humidity;
    bool valid;
};

bool sensor_init();
SensorReading sensor_read();
