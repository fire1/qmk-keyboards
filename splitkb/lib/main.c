
#include QMK_KEYBOARD_H
#include <stdio.h>

#define L_BASE 0
#define L_LOWER (1 << 1)
#define L_RAISE (1 << 2)
#define L_ADJUST (1 << 3)
#define L_ADJUST_TRI (L_ADJUST | L_RAISE | L_LOWER)

#define KBR_POSITION_ROW 11
#define KBR_POSITION_COL 1

#define KBR_POSITION KBR_POSITION_COL, KBR_POSITION_ROW

// static const char codeToMap[30]  = {' ', ' ', ' ', ' ', 0xAA, 0xAC,0xAC};
static const char codeToChar[60] = {' ', ' ', ' ', ' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', (char)28, 0x7F, (char)27, (char)29, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ';', '\'', ' ', ',', '.', '/', ' ', ' ', ' '};
static char       keylog[5]      = {' ', ' ', ' ', ' '};

//
// Logo char definitions
static const char PROGMEM qmk_logo[] = {0x80, 0x81, 0x82, 0x83, 0xFF, 0xA0, 0xA1, 0xA2, 0xA3, 0xFF, 0xC0, 0xC1, 0xC2, 0xC3};

static const char PROGMEM win_logo[] = {0x97, 0x98, 0xFF, 0x20, 0x20, 0xB7, 0xB8, 0xFF, 0xFF};

static const char PROGMEM mac_logo[] = {0x95, 0x96, 0xFF, 0x20, 0x20, 0xB6, 0xFF, 0xFF, 0xFF};

static const char PROGMEM kbr_full[] = {0x86, 0x87, 0x88, 0x89, 0xFF, 0xA6, 0xA7, 0xA8, 0xA9};

// Blank space
static const char PROGMEM blank[] = "     ";

/* Glyph string for oled_write(); must be NUL-terminated (oled_write uses strlen). */
static char keymap[sizeof(kbr_full) + 1] = {0};
#include "lib/ani.c"
static uint8_t  keypressIndex = 0;
static uint16_t privateTimer  = 0;
static bool     isInitDrawRun = false;
static bool     isInitDrawKbd = false;

static bool is_layer_eql(uint16_t state) {
    if (layer_state == state) {
        return true;
    }

    return false;
}

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

/**
 * @brief Change RGB color
 *
 * @param h
 * @param s
 * @param v
 */
void change_rgb_layer(uint8_t h, uint8_t s, uint8_t v) {
#ifdef RGB_MATRIX_ENABLE
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv_noeeprom(h, s, v);
#endif
}
//
// Suspend handler
/*
void suspend_power_down_kb(void) {
    rgb_matrix_set_suspend_state(true);
    oled_off();
}

void suspend_wakeup_init_kb(void) {
    rgb_matrix_set_suspend_state(false);
    oled_on();
}
*/

void drawBoot(void) {
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

void drawLogo(void) {
    oled_set_cursor(1, 1);
    oled_write_P(qmk_logo, false);
    oled_set_cursor(0, 4);
   oled_write_P(blank, false);
}

void handleLayers(led_t ledUsbState) {
    oled_set_cursor(0, 5);
    switch (layer_state) {
        case L_BASE:
            if (ledUsbState.caps_lock) {
                oled_write_P(PSTR("CPSLK"), true);
            } else {
                oled_write_P(PSTR("QWRTY"), false);
            }
            break;

            /*case L_LOWER:
                break;
            case L_RAISE:
                break;*/

            /* Too big, no size for the firmware
        case _NUM_LR:
            oled_write_P(PSTR("FUNUM"), true);
            break;
 */
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

void drawKeyboard(void) {
    oled_set_cursor(KBR_POSITION);
    oled_write(keymap, false);

    oled_set_cursor(0, 13);
   oled_write_P(blank, false);

    oled_set_cursor(1, 14);
    oled_write(keylog, false);
}

void resetKeymap(void) {
    memcpy_P(keymap, kbr_full, sizeof(kbr_full));
    keymap[sizeof(kbr_full)] = '\0';
    keypressIndex = 0;
}

void renderMaster(led_t ledUsbState) {

    if (!isInitDrawRun) {
        if (timer_elapsed(privateTimer) > 5000) {
            drawLogo();
            isInitDrawRun = true;
        } else {
            drawBoot();
        }
    }


    handleLayers(ledUsbState);

    if (keypressIndex > 0 && timer_elapsed(privateTimer) > 1200) resetKeymap();

    if (keypressIndex > 0 || !isInitDrawKbd) {
        isInitDrawKbd = true;
        drawKeyboard();
    }
}

//
// Setup screen
oled_rotation_t oled_init_kb(oled_rotation_t rotation) {
    memcpy_P(keymap, kbr_full, sizeof(kbr_full));
    keymap[sizeof(kbr_full)] = '\0';
    privateTimer = timer_read();
    return OLED_ROTATION_270;
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
#if OLED_TIMEOUT > 0
        /* Keep timeout aligned with OLED refresh cycles (typing gaps alone miss some frames). */
        oled_on();
#endif
        renderMaster(host_keyboard_led_state());
    }

    return false;
}

void setScreenKeys(uint16_t keycode, keyrecord_t *record) {
    char name = ' ';
    if (keycode < 60) {
        name = codeToChar[keycode];
    }

    // update keylog and shift it
    keylog[3] = keylog[2];
    keylog[2] = keylog[1];
    keylog[1] = keylog[0];
    keylog[0] = name;
}

/**
 * @brief OLED keyboard animation of the keypresses
 *      Replacment of tabdance
 * @param keycode
 * @param record
 * @return true
 * @return false
 */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Your existing OLED and Screen logic
    setScreenKeys(keycode, record);
    setOledKeymap(keycode, record);

    if (keypressIndex > 5) resetKeymap();
    if (isInitDrawRun) privateTimer = timer_read();
    keypressIndex++;

    // --- CUSTOM LOGIC ---
    switch (keycode) {


        case LSFT_LNG:
        case LSFT_T(LSFT_LNG):
            if (record->event.pressed) {
                // Handle the HOLD (Shift) manually to ensure no F7 is sent on press
                if (record->tap.count == 0) {
                    register_mods(MOD_BIT(KC_LSFT));
                }
            } else {
                unregister_mods(MOD_BIT(KC_LSFT)); // Always unregister on release
                if (record->tap.count > 0) {
                    // Logic for Language Change (TAP)
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
                    // TAP → Alt+Tab (moved from left key)
                    register_code(KC_LALT);
                    tap_code(KC_TAB);
                    unregister_code(KC_LALT);
                } else {
                    // HOLD → Right Ctrl (unchanged)
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
                    // TAP → Ctrl+Tab (moved from right key)
                    register_code(KC_LCTL);
                    tap_code(KC_TAB);
                    unregister_code(KC_LCTL);
                } else {
                    // HOLD → Left Alt (unchanged)
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
        //
        // Holding ctr+shift
        // case CTL_SFT:
        //     if (record->event.pressed) {
        //         register_mods(MOD_BIT(KC_LCTL) | MOD_BIT(KC_LSFT));
        //     } else {
        //         unregister_mods(MOD_BIT(KC_LCTL) | MOD_BIT(KC_LSFT));
        //     }
        //     return false;
        //
        // Ctrl hold and Ctrl+Shift+Tap hold state
        case CTL_SFT:
        case MT(MOD_LCTL, CTL_SFT):
            if (record->event.pressed) {
                if (record->tap.count > 0) {
                    // tap → hold: Ctrl+Shift
                    register_mods(MOD_BIT(KC_LCTL) | MOD_BIT(KC_LSFT));
                } else {
                    // plain hold: Ctrl only
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
// Snake state
static uint8_t snake_pos = 0;
static uint16_t snake_timer = 0;
static uint8_t snake_hue = 0;

// Underglow LED indices (Aurora usually has 6 underglow LEDs per side at the start of the chain)
static const uint8_t PROGMEM underglow_leds[] = {0, 1, 2, 3, 4, 5, 35, 36, 37, 38, 39, 40};

bool rgb_matrix_indicators_user(void) {
    if (timer_elapsed(snake_timer) > 80) { // Snake speed
        snake_timer = timer_read();
        snake_pos = (snake_pos + 1) % 12;
        if (snake_pos == 0) {
            snake_hue += 32; // Shift color every loop
            rgb_matrix_sethsv_noeeprom(snake_hue, 255, rgb_matrix_get_val());
        }
    }

    for (uint8_t i = 0; i < 4; i++) { // Snake length of 4
        uint8_t led_idx = pgm_read_byte(&underglow_leds[(snake_pos + i) % 12]);
        uint8_t val = (i + 1) * 60; // Tail fade
        rgb_matrix_set_color(led_idx, hsv_to_rgb((hsv_t){snake_hue, 255, val}).r, hsv_to_rgb((hsv_t){snake_hue, 255, val}).g, hsv_to_rgb((hsv_t){snake_hue, 255, val}).b);
    }
    return false;
}
#endif

//
// Handle power states of the keybard
//

// todo
void suspend_power_down_user(void) {
    // Force OLED off
    oled_off();

    // Force LEDs off
#ifdef RGB_MATRIX_ENABLE
    rgb_matrix_set_suspend_state(true);
    rgb_matrix_disable_noeeprom();
#endif
#ifdef RGBLIGHT_ENABLE
    rgblight_disable_noeeprom();
#endif
}

void suspend_wakeup_init_user(void) {
    // Re-enable LEDs
#ifdef RGBLIGHT_ENABLE
    rgblight_enable_noeeprom();
#endif
#ifdef RGB_MATRIX_ENABLE
    rgb_matrix_set_suspend_state(false);
    rgb_matrix_enable_noeeprom();
#endif

    // Re-enable OLED
    oled_on();
    
    // Restore default layer color
    default_rgb_layer();
}
/*
bool shutdown_kb(bool jump_to_bootloader) {
    if (!shutdown_user(jump_to_bootloader)) {
        return false;
    }

    if (jump_to_bootloader) {
        // red for bootloader
        oled_off();
        rgb_matrix_set_color_all(RGB_OFF);
    } else {
        // off for soft reset
        oled_on();
        rgb_matrix_set_color_all(RGB_GREEN);
    }
    // force flushing -- otherwise will never happen
    //  rgb_matrix_update_pwm_buffers();
    return true;
}
*/
bool shutdown_user(bool jump_to_bootloader) {
    if (jump_to_bootloader) {
        // red for bootloader
        oled_on();
#ifdef RGB_MATRIX_ENABLE
        rgb_matrix_set_color_all(RGB_RED);
#endif
    } else {
        // off for soft reset
        oled_off();
#ifdef RGB_MATRIX_ENABLE
        rgb_matrix_set_color_all(RGB_OFF);
#endif
    }
    // force flushing -- otherwise will never happen
    // rgb_matrix_update_pwm_buffers();
    // false to not process kb level
    return false;
}
