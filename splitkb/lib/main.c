
#include QMK_KEYBOARD_H
#include <stdio.h>

#define L_BASE 0
#define L_LOWER (1 << 1)
#define L_RAISE (1 << 2)
#define L_ADJUST (1 << 3)
#define L_ADJUST_TRI (L_ADJUST | L_RAISE | L_LOWER)

#ifdef OLED_ENABLE

#    define KBR_POSITION_ROW 11
#    define KBR_POSITION_COL 1

#    define OLED_DIRTY_HEADER  (1 << 0)
#    define OLED_DIRTY_LAYERS  (1 << 1)
#    define OLED_DIRTY_KBD     (1 << 2)
#    define OLED_DIRTY_KEYLOG  (1 << 3)
#    define OLED_DIRTY_KBD_ALL (1 << 4)

static const char codeToChar[60] = {' ', ' ', ' ', ' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', (char)28, 0x7F, (char)27, (char)29, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ';', '\'', ' ', ',', '.', '/', ' ', ' ', ' '};
static char       keylog[5]      = {' ', ' ', ' ', ' '};

static const char PROGMEM qmk_logo[] = {0x80, 0x81, 0x82, 0x83, 0xFF, 0xA0, 0xA1, 0xA2, 0xA3, 0xFF, 0xC0, 0xC1, 0xC2, 0xC3};
static const char PROGMEM win_logo[] = {0x97, 0x98, 0xFF, 0x20, 0x20, 0xB7, 0xB8, 0xFF, 0xFF};
static const char PROGMEM mac_logo[] = {0x95, 0x96, 0xFF, 0x20, 0x20, 0xB6, 0xFF, 0xFF, 0xFF};
static const char PROGMEM kbr_full[] = {0x86, 0x87, 0x88, 0x89, 0xFF, 0xA6, 0xA7, 0xA8, 0xA9};
static const char PROGMEM blank[]    = "     ";

/* Glyph string for oled_write(); must be NUL-terminated (oled_write uses strlen). */
static char keymap[sizeof(kbr_full) + 1] = {0};

#    include "lib/ani.c"

static uint8_t      keypressIndex   = 0;
static uint16_t     privateTimer    = 0;
static bool         isInitDrawRun   = false;
static uint8_t      oled_dirty_mask = 0;
static layer_state_t last_layer_state = 0;
static bool         last_caps_lock  = false;
static char         rendered_keymap[sizeof(kbr_full)];
static char         rendered_keylog[4];

static bool is_layer_eql(uint16_t state) {
    return layer_state == state;
}

static void oled_request(uint8_t parts) {
    oled_dirty_mask |= parts;
}

static void drawBoot(void) {
    if (keymap_config.swap_lctl_lgui) {
        oled_set_cursor(2, 1);
        oled_write_P(mac_logo, false);
    } else {
        oled_set_cursor(2, 1);
        oled_write_P(win_logo, false);
    }
    oled_set_cursor(0, 3);
    oled_write_P(blank, false);
    oled_set_cursor(0, 4);
    oled_write_P(blank, false);
}

static void drawLogo(void) {
    oled_set_cursor(1, 1);
    oled_write_P(qmk_logo, false);
    oled_set_cursor(0, 4);
    oled_write_P(blank, false);
}

static void drawLayers(led_t ledUsbState) {
    oled_set_cursor(0, 5);
    switch (layer_state) {
        case L_BASE:
            if (ledUsbState.caps_lock) {
                oled_write_P(PSTR("CPSLK"), true);
            } else {
                oled_write_P(PSTR("QWRTY"), false);
            }
            break;
        case L_ADJUST:
        case L_ADJUST_TRI:
            oled_write_P(PSTR("ADJST"), true);
            break;
    }

    oled_set_cursor(0, 6);
    oled_write_P(PSTR("-----"), false);
    oled_set_cursor(0, 7);
    oled_write_P(PSTR("LOWER"), is_layer_eql(L_LOWER));
    oled_set_cursor(0, 8);
    oled_write_P(PSTR("RAISE"), is_layer_eql(L_RAISE));
}

static void drawKeyboardGlyphs(bool force_all) {
    for (uint8_t i = 0; i < sizeof(kbr_full); i++) {
        if (force_all || keymap[i] != rendered_keymap[i]) {
            drawKbrGlyph(i, keymap[i]);
            rendered_keymap[i] = keymap[i];
        }
    }
}

static void drawKeylog(void) {
    for (uint8_t i = 0; i < 4; i++) {
        if (keylog[i] != rendered_keylog[i]) {
            oled_set_cursor(1 + i, 14);
            oled_write_char(keylog[i], false);
            rendered_keylog[i] = keylog[i];
        }
    }
}

static void resetKeymap(void) {
    memcpy_P(keymap, kbr_full, sizeof(kbr_full));
    keymap[sizeof(kbr_full)] = '\0';
    keypressIndex = 0;
}

oled_rotation_t oled_init_kb(oled_rotation_t rotation) {
    memcpy_P(keymap, kbr_full, sizeof(kbr_full));
    keymap[sizeof(kbr_full)] = '\0';
    privateTimer      = timer_read();
    last_layer_state  = layer_state;
    oled_dirty_mask   = OLED_DIRTY_HEADER | OLED_DIRTY_LAYERS | OLED_DIRTY_KBD_ALL | OLED_DIRTY_KEYLOG;
    return OLED_ROTATION_270;
}

bool oled_task_kb(void) {
    if (!is_keyboard_master()) {
        return false;
    }

    uint8_t work = oled_dirty_mask;

    if (!isInitDrawRun && timer_elapsed(privateTimer) > 5000) {
        isInitDrawRun = true;
        work |= OLED_DIRTY_HEADER;
    }

    if (keypressIndex > 0 && timer_elapsed(privateTimer) > 1200) {
        resetKeymap();
        work |= OLED_DIRTY_KBD_ALL;
    }

    led_t led = host_keyboard_led_state();
    if (layer_state != last_layer_state) {
        last_layer_state = layer_state;
        work |= OLED_DIRTY_LAYERS;
    }
    if (led.caps_lock != last_caps_lock) {
        last_caps_lock = led.caps_lock;
        if (layer_state == L_BASE) {
            work |= OLED_DIRTY_LAYERS;
        }
    }

    if (!work) {
        return false;
    }

    oled_dirty_mask &= ~work;

    if (work & OLED_DIRTY_HEADER) {
        if (isInitDrawRun) {
            drawLogo();
        } else {
            drawBoot();
        }
    }

    if (work & OLED_DIRTY_LAYERS) {
        drawLayers(led);
    }

    if (work & (OLED_DIRTY_KBD | OLED_DIRTY_KBD_ALL)) {
        drawKeyboardGlyphs((work & OLED_DIRTY_KBD_ALL) != 0);
    }

    if (work & OLED_DIRTY_KEYLOG) {
        drawKeylog();
    }

    return false;
}

static void setScreenKeys(uint16_t keycode) {
    char name = ' ';
    if (keycode < 60) {
        name = codeToChar[keycode];
    }
    keylog[3] = keylog[2];
    keylog[2] = keylog[1];
    keylog[1] = keylog[0];
    keylog[0] = name;
}

#endif /* OLED_ENABLE */

void default_rgb_layer(void) {
#ifdef RGB_MATRIX_ENABLE
    rgb_matrix_mode_noeeprom(RGB_MATRIX_DEFAULT_MODE);
#endif
}

void keyboard_post_init_user(void) {
#ifdef RGB_MATRIX_ENABLE
    rgb_matrix_enable();
    default_rgb_layer();
#    if defined(RGBLIGHT_DEFAULT_SPD)
    rgb_matrix_set_speed_noeeprom(RGBLIGHT_DEFAULT_SPD);
#    endif
#endif
}

void change_rgb_layer(uint8_t h, uint8_t s, uint8_t v) {
#ifdef RGB_MATRIX_ENABLE
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv_noeeprom(h, s, v);
#endif
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef OLED_ENABLE
    if (record->event.pressed) {
        setScreenKeys(keycode);
        setOledKeymap(keycode, record);
        if (keypressIndex > 5) {
            resetKeymap();
            oled_request(OLED_DIRTY_KBD_ALL);
        }
        if (isInitDrawRun) {
            privateTimer = timer_read();
        }
        keypressIndex++;
        oled_request(OLED_DIRTY_KBD | OLED_DIRTY_KEYLOG);
    }
#endif

    switch (keycode) {
        case LSFT_LNG:
        case LSFT_T(LSFT_LNG):
            if (record->event.pressed) {
                if (record->tap.count == 0) {
                    register_mods(MOD_BIT(KC_LSFT));
                }
            } else {
                unregister_mods(MOD_BIT(KC_LSFT));
                if (record->tap.count > 0) {
                    clear_mods();
                    register_code(KC_LGUI);
                    tap_code(KC_SPC);
                    unregister_code(KC_LGUI);
                }
            }
            return false;

        case RCTL_TAB:
        case RCTL_T(RCTL_TAB):
            if (record->event.pressed) {
                if (record->tap.count > 0) {
                    register_code(KC_LALT);
                    tap_code(KC_TAB);
                    unregister_code(KC_LALT);
                } else {
                    register_mods(MOD_BIT(KC_RCTL));
                }
            } else {
                unregister_mods(MOD_BIT(KC_RCTL));
            }
            return false;

        case ALT_TAB:
        case LALT_T(ALT_TAB):
            if (record->event.pressed) {
                if (record->tap.count > 0) {
                    register_code(KC_LCTL);
                    tap_code(KC_TAB);
                    unregister_code(KC_LCTL);
                } else {
                    register_mods(MOD_BIT(KC_LALT));
                }
            } else {
                unregister_mods(MOD_BIT(KC_LALT));
            }
            return false;

        case CTL_TGL:
        case LCTL_T(CTL_TGL):
            if (record->event.pressed) {
                register_code(KC_LCTL);
            } else {
                unregister_code(KC_LCTL);
                if (record->tap.count > 0) {
                    layer_invert(_ADJUST);
                }
            }
            return false;

        case CTL_SFT:
        case MT(MOD_LCTL, CTL_SFT):
            if (record->event.pressed) {
                if (record->tap.count > 0) {
                    register_mods(MOD_BIT(KC_LCTL) | MOD_BIT(KC_LSFT));
                } else {
                    register_mods(MOD_BIT(KC_LCTL));
                }
            } else {
                unregister_mods(MOD_BIT(KC_LCTL) | MOD_BIT(KC_LSFT));
            }
            return false;
    }
    return true;
}

#ifdef RGB_MATRIX_ENABLE
static uint8_t  snake_pos   = 0;
static uint16_t snake_timer = 0;
static uint8_t  snake_hue   = 0;

static const uint8_t PROGMEM underglow_leds[] = {0, 1, 2, 3, 4, 5, 35, 36, 37, 38, 39, 40};

bool rgb_matrix_indicators_user(void) {
    if (timer_elapsed(snake_timer) > 80) {
        snake_timer = timer_read();
        snake_pos   = (snake_pos + 1) % 12;
        if (snake_pos == 0) {
            snake_hue += 32;
            rgb_matrix_sethsv_noeeprom(snake_hue, 255, rgb_matrix_get_val());
        }
    }

    for (uint8_t i = 0; i < 4; i++) {
        uint8_t led_idx = pgm_read_byte(&underglow_leds[(snake_pos + i) % 12]);
        uint8_t val     = (i + 1) * 60;
        rgb_matrix_set_color(led_idx, hsv_to_rgb((hsv_t){snake_hue, 255, val}).r, hsv_to_rgb((hsv_t){snake_hue, 255, val}).g, hsv_to_rgb((hsv_t){snake_hue, 255, val}).b);
    }
    return false;
}
#endif

void suspend_power_down_user(void) {
#ifdef OLED_ENABLE
    oled_off();
#endif
#ifdef RGB_MATRIX_ENABLE
    rgb_matrix_set_suspend_state(true);
    rgb_matrix_disable_noeeprom();
#endif
#ifdef RGBLIGHT_ENABLE
    rgblight_disable_noeeprom();
#endif
}

void suspend_wakeup_init_user(void) {
#ifdef RGBLIGHT_ENABLE
    rgblight_enable_noeeprom();
#endif
#ifdef RGB_MATRIX_ENABLE
    rgb_matrix_set_suspend_state(false);
    rgb_matrix_enable_noeeprom();
#endif
#ifdef OLED_ENABLE
    oled_on();
#endif
    default_rgb_layer();
}

bool shutdown_user(bool jump_to_bootloader) {
    if (jump_to_bootloader) {
#ifdef OLED_ENABLE
        oled_on();
#endif
#ifdef RGB_MATRIX_ENABLE
        rgb_matrix_set_color_all(RGB_RED);
#endif
    } else {
#ifdef OLED_ENABLE
        oled_off();
#endif
#ifdef RGB_MATRIX_ENABLE
        rgb_matrix_set_color_all(RGB_OFF);
#endif
    }
    return false;
}
