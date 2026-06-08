// Copyright 2023 Angel Zaprianov (@fire1)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

//
// Oled custom font
#ifdef OLED_ENABLE
#    define OLED_DISPLAY_128X32
#    define OLED_FONT_H "lib/font.c"
/* Turn display off after 1 min idle; QMK wakes it on key/encoder/joystick activity. */
#    define OLED_TIMEOUT 60000
/* Fail fast on I2C errors so matrix scanning is not blocked for 100 ms (QMK default). */
#    define OLED_I2C_TIMEOUT 5
/* AVR: bus speed via F_SCL (100 kHz is more tolerant of wiring/noise than 400 kHz). */
#    define F_SCL 100000UL
/* oled_task_kb tick only; actual pixels are written when state changes, not every tick. */
#    define OLED_UPDATE_INTERVAL 50
/* One dirty OLED block per main-loop iteration — spreads I2C across scans. */
#    define OLED_UPDATE_PROCESS_LIMIT 1
#endif

#define TAPPING_TERM 175
#define TAPPING_TERM_PER_KEY
// #define RGBLIGHT_SLEEP
// #define RGB_DISABLE_WHEN_USB_SUSPENDED

#ifdef RGB_MATRIX_ENABLE
#    define RGBLIGHT_DEFAULT_SPD 125 // Range is 0-255
#    define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#    define RGB_DISABLE_WHEN_USB_SUSPENDED // turn off effects when suspended
#    define RGB_MATRIX_TIMEOUT 1800000         // 30min
#endif

#define SPLIT_USB_DETECT

/* disable debug print */
// #define NO_DEBUG

/* disable print */
// #define NO_PRINT
#ifdef POINTING_DEVICE_ENABLE
#    define SPLIT_POINTING_ENABLE // enables transmitting the pointing device status to the master side of the split keyboard
#    define ANALOG_JOYSTICK_X_AXIS_PIN F4
#    define ANALOG_JOYSTICK_Y_AXIS_PIN B6
#    define ANALOG_JOYSTICK_CLICK_PIN B2
#    define ANALOG_JOYSTICK_AXIS_MIN 120
#    define ANALOG_JOYSTICK_AXIS_MAX 900
#    define ANALOG_JOYSTICK_SPEED_REGULATOR 38
#    define ANALOG_JOYSTICK_READ_INTERVAL 2
#    define POINTING_DEVICE_RIGHT
#    define POINTING_DEVICE_INVERT_X
// #    define POINTING_DEVICE_INVERT_Y
#    define ANALOG_JOYSTICK_SPEED_MAX 2
#endif
