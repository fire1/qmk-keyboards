// ani.c — replaces all kpr_* constants and setOledKeymap

// kbr_full layout (OLED rotation 270, max 5 chars/line):
//   row 11: 0x86 0x87 0x88 0x89 0xFF
//   row 12: 0xA6 0xA7 0xA8 0xA9
#define KBR_LAST_ROW_START 5
#define KBR_LAST_ROW_LINE  (KBR_POSITION_ROW + 1)

static void drawKbrGlyph(uint8_t index, char ch) {
    uint8_t row = KBR_POSITION_ROW;
    uint8_t col = KBR_POSITION_COL + index;

    if (index >= KBR_LAST_ROW_START) {
        row = KBR_LAST_ROW_LINE;
        col = KBR_POSITION_COL + (index - KBR_LAST_ROW_START);
    }

    oled_set_cursor(col, row);
    oled_write_char(ch, false);
}

// All distinct char values used by the animation
static const char PROGMEM kpr_chars[] = {
    0xC6, // 0: tl1
    0x8A, // 1: tm1
    0xC9, // 2: tm2
    0xAA, // 3: ml1
    0xAB, // 4: ml2
    0xCB, // 5: mr1
    0x8C, // 6: bl1
    0xAC, // 7: br1
    0xAD, // 8: br2
    0xCA, // 9
    0x8B, // 10
};

// Packed table: keycodes 4 (KC_A) → 39 (KC_0)
// Byte format: bits[7:4] = keymap[] index, bits[3:0] = kpr_chars[] index
// 0xFF = no mapping
#define _M(i, v) (((i) << 4) | (v))
#define _X        0xFF
static const uint8_t PROGMEM kpr_table[36] = {
    _M(0, 3), // KC_A  → keymap[0] = ml1
    _M(1, 8), // KC_B  → keymap[1] = br2
    _M(0, 7), // KC_C  → keymap[0] = br1
    _M(1, 4), // KC_D  → keymap[1] = ml2
    _M(0, 1), // KC_E  → keymap[0] = tm1
    _M(1, 4), // KC_F  → keymap[1] = ml2
    _M(1, 5), // KC_G  → keymap[1] = mr1
    _M(2, 3), // KC_H  → keymap[2] = ml1
    _M(2, 1), // KC_I  → keymap[2] = tm1
    _M(2, 9), // KC_J  → keymap[2] = 0xCA
    _M(2, 9), // KC_K  → keymap[2] = 0xCA
    _M(3, 5), // KC_L  → keymap[3] = mr1
    _X,       // KC_M  → (no mapping)
    _M(2, 6), // KC_N  → keymap[2] = bl1
    _M(3,10), // KC_O  → keymap[3] = 0x8B
    _M(3,10), // KC_P  → keymap[3] = 0x8B
    _M(0, 0), // KC_Q  → keymap[0] = tl1
    _M(1, 2), // KC_R  → keymap[1] = tm2
    _M(0, 3), // KC_S  → keymap[0] = ml1
    _M(1, 2), // KC_T  → keymap[1] = tm2
    _M(2, 1), // KC_U  → keymap[2] = tm1
    _M(0, 7), // KC_V  → keymap[0] = br1
    _M(0, 1), // KC_W  → keymap[0] = tm1
    _M(0, 6), // KC_X  → keymap[0] = bl1
    _M(2, 0), // KC_Y  → keymap[2] = tl1
    _M(0, 6), // KC_Z  → keymap[0] = bl1
    _M(0, 0), // KC_1  → keymap[0] = tl1
    _M(0, 1), // KC_2  → keymap[0] = tm1
    _M(0, 1), // KC_3  → keymap[0] = tm1
    _M(1, 2), // KC_4  → keymap[1] = tm2
    _M(1, 2), // KC_5  → keymap[1] = tm2
    _M(2, 0), // KC_6  → keymap[2] = tl1
    _M(2, 1), // KC_7  → keymap[2] = tm1
    _M(2, 1), // KC_8  → keymap[2] = tm1
    _M(3,10), // KC_9  → keymap[3] = 0x8B
    _M(3,10), // KC_0  → keymap[3] = 0x8B
};
#undef _M
#undef _X

void setOledKeymap(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed && keycode >= 4 && keycode < 40) {
        uint8_t entry = pgm_read_byte(&kpr_table[keycode - 4]);
        if (entry != 0xFF) {
            keymap[entry >> 4] = pgm_read_byte(&kpr_chars[entry & 0x0F]);
        }
    }
}