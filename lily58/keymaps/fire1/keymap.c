#include QMK_KEYBOARD_H

// Keyboard layers
enum layer_number {
  _QWERTY = 0,
  _LOWER,
  _RAISE,
  _ADJUST,
  _NUM_LR
};

// Custom keycodes
enum custom_keycodes {
  LSFT_LNG = SAFE_RANGE,
  ALT_TAB,
  RCTL_TAB,
  CTL_SFT,
  CTL_TGL
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
 [_QWERTY] = LAYOUT(
  KC_ESC,   KC_1,   KC_2,   KC_3,   KC_4,   KC_5,                      KC_6,   KC_7,   KC_8,   KC_9,   KC_0,   KC_DEL,
  KC_TAB,   KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,                      KC_Y,   KC_U,   KC_I,   KC_O,   KC_P,   KC_MINS,
  LSFT_T(LSFT_LNG), KC_A, KC_S, KC_D, KC_F, KC_G,                      KC_H,   KC_J,   KC_K,   KC_L,   KC_SCLN, MT(MOD_RSFT, KC_QUOT),
  KC_LCTL,  KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,   KC_MUTE,   KC_F6,  KC_N,   KC_M,   KC_COMM, KC_DOT, KC_SLSH, MT(MOD_LCTL, KC_CAPS),
                    KC_LGUI, LALT_T(ALT_TAB), MO(_LOWER), KC_SPC, KC_ENT, MO(_RAISE), RCTL_T(RCTL_TAB), MT(MOD_RGUI, KC_APP)
 ),

 [_LOWER] = LAYOUT(
  _______,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                     KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_INS,
  _______,  KC_ESC,  _______, MS_UP, _______, _______,                   KC_CIRC, KC_AMPR, KC_PAST, KC_PLUS, KC_BSPC, KC_EQL,
  _______,  MT(MOD_LCTL, CTL_SFT), MS_LEFT, MS_DOWN, MS_RGHT, MS_WHLU,     KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_END,  _______,
  _______,  KC_LALT, CTL_SFT, MS_BTN2, MS_BTN1, MS_WHLD, _______, _______, _______, KC_HOME, KC_LABK, KC_RABK, KC_BSLS, _______,
                             _______, _______, _______, _______, _______, _______, _______, _______
 ),

 [_RAISE] = LAYOUT(
  KC_ESC,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                     KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_DEL,
  KC_TAB,   KC_GRV,  KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                   KC_CIRC, KC_AMPR, KC_ASTR, KC_EXLM, KC_QUES,  _______,
  _______,  KC_LPRN, KC_RPRN, KC_LCBR, KC_RCBR, KC_TILD,                   KC_5,    KC_6,    KC_7,    KC_8,    KC_9,     _______,
  _______,  KC_EXLM, KC_UNDS, KC_LBRC, KC_RBRC, _______, KC_PGUP, KC_PGDN, KC_0,    KC_1,    KC_2,    KC_3,    KC_4,     _______,
                             _______, _______, _______, _______, _______, _______, _______, _______
 ),

 [_ADJUST] = LAYOUT(
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_SLEP,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_DEL,  XXXXXXX,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_MUTE, KC_VOLD, KC_VOLU, XXXXXXX, XXXXXXX,
                             _______, _______, _______, _______, _______, _______, _______, _______
 ),

 [_NUM_LR] = LAYOUT(
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                     KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  XXXXXXX,
  XXXXXXX, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    XXXXXXX,
  XXXXXXX, KC_F11,  KC_F12,  KC_PLUS, KC_ASTR, XXXXXXX, XXXXXXX, XXXXXXX, KC_MINS, KC_SLSH, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                             _______, _______, _______, _______, _______, _______, _______, _______
 )
};

#ifdef RGBLIGHT_ENABLE
/** Base layer: yellow snake animation. Layers: solid colors (need explicit static mode or sethsv cannot show them). */
void set_layer_color(uint8_t layer) {
    switch (layer) {
        case _RAISE:
            rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
            rgblight_sethsv_noeeprom(HSV_BLUE);
            break;
        case _LOWER:
            rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
            rgblight_sethsv_noeeprom(HSV_GREEN);
            break;
        case _ADJUST:
        case _NUM_LR:
            rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
            rgblight_sethsv_noeeprom(HSV_PURPLE);
            break;
        default:
#    ifdef RGBLIGHT_EFFECT_SNAKE
            rgblight_mode_noeeprom(RGBLIGHT_MODE_SNAKE);
            rgblight_sethsv_noeeprom(HSV_SNAKE_BASE);
#    else
            rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
            rgblight_sethsv_noeeprom(HSV_SNAKE_BASE);
#    endif
            break;
    }
}
#endif

void keyboard_post_init_user(void) {
#ifdef RGBLIGHT_ENABLE
    rgblight_enable();
    rgblight_mode(RGBLIGHT_MODE_SNAKE);
    rgblight_sethsv(HSV_SNAKE_BASE);
    set_layer_color(get_highest_layer(layer_state));
#endif
}

layer_state_t layer_state_set_user(layer_state_t state) {
    state = update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
#ifdef RGBLIGHT_ENABLE
    set_layer_color(get_highest_layer(state));
#endif
    return state;
}

#include "lib/main.c"
