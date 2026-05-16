#pragma once

#define MASTER_LEFT
#define SPLIT_USB_DETECT

/* Tapping term is overridden per-keymap in keymaps/<name>/config.h */

#ifdef RGBLIGHT_ENABLE
#    define RGBLIGHT_SPLIT
/** Lily58 underglow: 12 LEDs wired as six per half (keyboard.json rgblight.led_count: 12). */
#    ifndef RGBLED_SPLIT
#        define RGBLED_SPLIT \
            { 6, 6 }
#    endif

/* Per-half LED wire order: see lily58.c (ws2812_set_color). */

/* Base-layer snake: warm orange (requested color: 19, 255, 170). */
#    define HSV_SNAKE_BASE 19, 255, 170
#endif
