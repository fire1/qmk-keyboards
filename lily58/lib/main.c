#include QMK_KEYBOARD_H
#include <stdio.h>

#define L_BASE 0
#define L_LOWER (1 << 1)
#define L_RAISE (1 << 2)
#define L_ADJUST (1 << 3)
#define L_ADJUST_TRI (L_ADJUST | L_RAISE | L_LOWER)

#ifdef OLED_ENABLE
static const char codeToChar[60] = {' ', ' ', ' ', ' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', (char)28, 0x7F, (char)27, (char)29, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ';', '\'', ' ', ',', '.', '/', ' ', ' ', ' '};
static char keylog[5] = {' ', ' ', ' ', ' '};
static const char PROGMEM qmk_logo[] = {0x80, 0x81, 0x82, 0x83, 0xFF, 0xA0, 0xA1, 0xA2, 0xA3, 0xFF, 0xC0, 0xC1, 0xC2, 0xC3};
static const char PROGMEM win_logo[] = {0x97, 0x98, 0xFF, 0x20, 0x20, 0xB7, 0xB8, 0xFF, 0xFF};
static const char PROGMEM mac_logo[] = {0x95, 0x96, 0xFF, 0x20, 0x20, 0xB6, 0xFF, 0xFF, 0xFF};
static const char PROGMEM kbr_full[] = {0x86, 0x87, 0x88, 0x89, 0xFF, 0xA6, 0xA7, 0xA8, 0xA9};
static const char PROGMEM blank[] = "     ";
static char keymap[sizeof(kbr_full) + 1] = {0};

#include "ani.c"

static uint8_t  keypressIndex = 0;
static uint16_t privateTimer   = 0;
static bool     isInitDrawRun  = false;
static bool     isInitDrawKbd  = false;

static bool is_layer_eql(uint16_t state) {
    return layer_state == state;
}

void drawBoot(void) {
    oled_set_cursor(2, 1);
    if (keymap_config.swap_lctl_lgui) {
        oled_write_P(mac_logo, false);
    } else {
        oled_write_P(win_logo, false);
    }
    oled_set_cursor(0, 3); oled_write_P(blank, false);
    oled_set_cursor(0, 4); oled_write_P(blank, false);
}

void drawLogo(void) {
    oled_set_cursor(1, 1);
    oled_write_P(qmk_logo, false);
    oled_set_cursor(0, 4); oled_write_P(blank, false);
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
        case L_ADJUST:
        case L_ADJUST_TRI:
            oled_write_P(PSTR("ADJST"), true);
            break;
    }
    oled_set_cursor(0, 6); oled_write_P(PSTR("-----"), false);
    oled_set_cursor(0, 7); oled_write_P(PSTR("LOWER"), is_layer_eql(L_LOWER));
    oled_set_cursor(0, 8); oled_write_P(PSTR("RAISE"), is_layer_eql(L_RAISE));
}

void drawKeyboard(void) {
    oled_set_cursor(1, 11);
    oled_write(keymap, false);
    oled_set_cursor(0, 13); oled_write_P(blank, false);
    oled_set_cursor(1, 14); oled_write(keylog, false);
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

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    memcpy_P(keymap, kbr_full, sizeof(kbr_full));
    keymap[sizeof(kbr_full)] = '\0';
    privateTimer = timer_read();
    return OLED_ROTATION_270;
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        renderMaster(host_keyboard_led_state());
    }
    return false;
}

void setScreenKeys(uint16_t keycode, keyrecord_t *record) {
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

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef OLED_ENABLE
    setScreenKeys(keycode, record);
    setOledKeymap(keycode, record);
    if (keypressIndex > 5) resetKeymap();
    if (isInitDrawRun) privateTimer = timer_read();
    keypressIndex++;
#endif

    switch (keycode) {
        case LSFT_LNG:
        case LSFT_T(LSFT_LNG):
            if (record->event.pressed) {
                if (record->tap.count == 0) register_mods(MOD_BIT(KC_LSFT));
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
                if (record->tap.count > 0) layer_invert(_ADJUST);
            }
            return false;
        case CTL_SFT:
        case MT(MOD_LCTL, CTL_SFT):
            if (record->event.pressed) {
                if (record->tap.count > 0) register_mods(MOD_BIT(KC_LCTL) | MOD_BIT(KC_LSFT));
                else register_mods(MOD_BIT(KC_LCTL));
            } else {
                unregister_mods(MOD_BIT(KC_LCTL) | MOD_BIT(KC_LSFT));
            }
            return false;
    }
    return true;
}
