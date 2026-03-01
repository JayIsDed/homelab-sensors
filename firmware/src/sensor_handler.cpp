#include "sensor_handler.h"
#include "config.h"
#include <Adafruit_SHT31.h>
#include <Wire.h>

static Adafruit_SHT31 sht31;

bool sensor_init() {
    Wire.begin(PIN_SDA, PIN_SCL);
    if (!sht31.begin(SHT31_ADDRESS)) {
        Serial.println("[SENSOR] SHT31 not found");
        return false;
    }
    Serial.println("[SENSOR] SHT31 initialized");
    return true;
}

SensorReading sensor_read() {
    SensorReading reading;
    reading.temperature = sht31.readTemperature();
    reading.humidity = sht31.readHumidity();
    reading.valid = !isnan(reading.temperature) && !isnan(reading.humidity);

    if (reading.valid) {
        Serial.printf("[SENSOR] T=%.1f°C H=%.1f%%\n", reading.temperature, reading.humidity);
    } else {
        Serial.println("[SENSOR] Read failed");
    }
    return reading;
}
