#include "led_status.h"
#include "config.h"
#include <Arduino.h>

static LedMode currentMode = LED_OFF;
static unsigned long lastToggle = 0;
static bool ledState = false;

void led_init() {
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);
}

void led_set_mode(LedMode mode) {
    currentMode = mode;
    if (mode == LED_OFF) {
        digitalWrite(PIN_LED, LOW);
        ledState = false;
    }
}

void led_update() {
    if (currentMode == LED_OFF) return;

    unsigned long interval;
    switch (currentMode) {
        case LED_PROVISIONING: interval = LED_BLINK_PROV;    break;
        case LED_CONNECTING:   interval = LED_BLINK_CONNECT; break;
        case LED_OK:           interval = LED_PULSE_OK;      break;
        case LED_ERROR:        interval = LED_BLINK_ERROR;    break;
        default: return;
    }

    unsigned long now = millis();
    if (now - lastToggle >= interval) {
        lastToggle = now;
        ledState = !ledState;
        digitalWrite(PIN_LED, ledState ? HIGH : LOW);
    }
}
