#pragma once

#include <Arduino.h>

void mqtt_init(const String& host, const String& user, const String& pass);
bool mqtt_connect();
bool mqtt_publish(const String& sensor_id, const char* payload);
void mqtt_loop();
bool mqtt_connected();
