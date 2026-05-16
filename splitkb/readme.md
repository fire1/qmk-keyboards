# fire1_splitkb — QMK firmware (Aurora / splitkb)

[QMK](https://qmk.fm/) firmware for a **splitkb Aurora** Sofle-class keyboard (ATmega32u4), with personal mods: **OLED**, **RGB Matrix**, **rotary encoder**, and a **right-hand analog joystick**.

**Maintainer:** [Angel Zaprianov](https://github.com/fire1)  
**Related:** Lily58 port and full project docs live in the parent [fire1](../readme.md) folder when used inside `qmk_firmware/keyboards/fire1/`.

---

## Hardware

- **Board:** [splitkb Aurora](https://splitkb.com/) (Sofle-style split)
- **MCU:** ATmega32u4, Caterina bootloader
- **RGB:** WS2812 RGB Matrix (~70 LEDs)
- **Display:** 128×32 OLED (custom font in `lib/font.c`)
- **Input:** Left encoder (volume); right analog joystick (see `lib/mice.c`)

Joystick wiring is a personal mod; hardware details are not published.

---

## Install in QMK

1. [Set up QMK](https://docs.qmk.fm/#/getting_started_build_tools).
2. Copy this repository into your QMK tree:

```text
qmk_firmware/keyboards/fire1/splitkb/
```

(If you cloned this repo as-is, place these files under that path — the QMK keyboard path is `fire1/splitkb`, not `fire1_splitkb`.)

---

## Build & flash

From the **QMK repository root**:

```bash
qmk compile -kb fire1/splitkb -km default
qmk flash -kb fire1/splitkb -km default
```

```bash
make fire1/splitkb:default
make fire1/splitkb:default:flash
```

Output: `fire1_splitkb_default.hex`

Flash **both halves** if required for your split workflow. Enter the bootloader via reset, Bootmagic, or `QK_BOOT`.

---

## Layers & custom keys

| Layer | Purpose |
|-------|---------|
| QWERTY | Default typing |
| Lower | Mouse keys, arrows, symbols |
| Raise | Numbers, punctuation |
| Adjust | RGB, sleep, media |
| NUM_LR | Toggle-style function layer (splitkb thumb) |

| Key | Hold | Tap |
|-----|------|-----|
| `LSFT_T(LSFT_LNG)` | Left Shift | Win+Space |
| `LALT_T(ALT_TAB)` | Left Alt | Ctrl+Tab |
| `RCTL_T(RCTL_TAB)` | Right Ctrl | Alt+Tab |

Logic is in `lib/main.c` (OLED animation, custom keycodes, RGB indicators).

---

## Layout

```text
keymaps/default/keymap.c   # Keymap
lib/main.c                 # OLED, process_record_user, RGB
lib/mice.c                 # Joystick
lib/ani.c                  # OLED keypress animation
config.h                   # Tapping term, OLED, RGB Matrix
keyboard.json              # Pinout & features
```

---

## License

GPL-2.0-or-later (same as QMK). See file headers in source.
