// Copyright 2026 Angel Zaprianov (@fire1)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"
#include "ws2812.h"

extern ws2812_led_t ws2812_leds[];

/*
 * Mixed underglow wiring on some Lily58 builds: left (master) GRB, right (slave) RGB.
 * Linked with -Wl,--wrap=ws2812_set_color (see rules.mk).
 */
void __wrap_ws2812_set_color(int index, uint8_t red, uint8_t green, uint8_t blue) {
    if (is_keyboard_left()) {
        ws2812_leds[index].g = green;
        ws2812_leds[index].r = red;
        ws2812_leds[index].b = blue;
    } else {
        ws2812_leds[index].g = red;
        ws2812_leds[index].r = green;
        ws2812_leds[index].b = blue;
    }
#if defined(WS2812_RGBW)
    ws2812_rgb_to_rgbw(&ws2812_leds[index]);
#endif
}

void __wrap_ws2812_set_color_all(uint8_t red, uint8_t green, uint8_t blue) {
    for (int i = 0; i < WS2812_LED_COUNT; i++) {
        __wrap_ws2812_set_color(i, red, green, blue);
    }
}
