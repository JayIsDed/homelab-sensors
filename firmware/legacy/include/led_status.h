#pragma once

enum LedMode {
    LED_OFF,
    LED_PROVISIONING,  // Fast blink
    LED_CONNECTING,    // Medium blink
    LED_OK,            // Slow pulse
    LED_ERROR          // Very fast blink
};

void led_init();
void led_set_mode(LedMode mode);
void led_update();  // Call in loop
