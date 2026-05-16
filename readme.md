# fire1 — QMK firmware

Personal [QMK](https://qmk.fm/) firmware for two **different** split ergonomic keyboards. The keymap and layer logic are shared in spirit; **hardware, RGB stack, and drivers are not interchangeable** between targets.

**Maintainer:** [Angel Zaprianov](https://github.com/fire1)  
**Splitkb firmware repo:** [github.com/fire1/fire1_splitkb](https://github.com/fire1/fire1_splitkb)

---

## Keyboards

| | **splitkb** (`fire1/splitkb`) | **lily58** (`fire1/lily58`) |
|---|------------------------------|----------------------------|
| **Hardware** | [splitkb Aurora](https://splitkb.com/) Sofle-class board (ATmega32u4), with personal mods | Standard **Lily58** (Pro Micro, typical matrix) |
| **RGB** | RGB Matrix (~70 LEDs, per-key effects) | RGBlight (12 underglow LEDs, split 6+6) |
| **Display** | 128×32 OLED, custom font | 128×32 OLED on `fire1` keymap only |
| **Extras** | Rotary encoder, **right-hand analog joystick** | No encoder / no joystick |
| **Keymap** | `default` | `fire1` (OLED + WPM) or `fire1_light` (no OLED) |

The Lily58 port drops Aurora-only hardware and adjusts the thumb row (no `TT(_NUM_LR)` on the left, no dedicated `KC_RALT` on the right). See [GEMINI.md](GEMINI.md) for porting notes.

---

## Features

- **Layers:** QWERTY, Lower (mouse / symbols), Raise (numbers / punctuation), Adjust (RGB / media), optional numeric toggle (`_NUM_LR` on splitkb).
- **OLED (splitkb & lily58 `fire1`):** Boot logo, layer indicators, animated keymap graphic, recent-key strip.
- **RGB**
  - **splitkb:** RGB Matrix effects (including custom underglow-style indicators).
  - **lily58:** Yellow **snake** on base layer; solid colors on Lower (green), Raise (blue), Adjust (purple).
- **Custom mod-tap keys** (same behaviour on both boards when mapped):

| Key | Hold | Tap |
|-----|------|-----|
| `LSFT_T(LSFT_LNG)` | Left Shift | Win+Space (language switch) |
| `LALT_T(ALT_TAB)` | Left Alt | Ctrl+Tab |
| `RCTL_T(RCTL_TAB)` | Right Ctrl | Alt+Tab |
| `MT(MOD_LCTL, CTL_SFT)` | Left Ctrl | Ctrl+Shift |

---

## Getting started

### 1. Install QMK

Follow the official guide: [QMK setup](https://docs.qmk.fm/#/getting_started_build_tools).

### 2. Add this keyboard to your QMK tree

Copy the `fire1` folder into your QMK checkout:

```text
qmk_firmware/
└── keyboards/
    └── fire1/
        ├── splitkb/
        ├── lily58/
        └── readme.md   ← you are here
```

Or clone [fire1_splitkb](https://github.com/fire1/fire1_splitkb) and place its contents under `keyboards/fire1/splitkb/` (that directory is also maintained as its own git repository).

> **Important:** Do **not** add a `keyboard.json` at `keyboards/fire1/` itself. QMK expects `keyboard.json` only on each **leaf** keyboard (`splitkb/`, `lily58/`). A parent `keyboard.json` will break builds.

### 3. Build from the QMK root

```bash
cd /path/to/qmk_firmware
```

---

## Build

| Board | Command | Output (example) |
|-------|---------|------------------|
| splitkb Aurora | `qmk compile -kb fire1/splitkb -km default` | `fire1_splitkb_default.hex` |
| Lily58 (OLED) | `qmk compile -kb fire1/lily58 -km fire1` | `fire1_lily58_fire1.hex` |
| Lily58 (light) | `qmk compile -kb fire1/lily58 -km fire1_light` | `fire1_lily58_fire1_light.hex` |

**Make equivalents:**

```bash
make fire1/splitkb:default
make fire1/lily58:fire1
make fire1/lily58:fire1_light
```

---

## Flash

Connect the half you are flashing (for split boards, flash **each** half if your workflow requires it).

```bash
qmk flash -kb fire1/splitkb -km default
qmk flash -kb fire1/lily58 -km fire1
# or
qmk flash -kb fire1/lily58 -km fire1_light
```

Put the controller in the bootloader via **reset button**, **Bootmagic**, or a mapped `QK_BOOT` key. Bootloader type is **Caterina** (Pro Micro class) on both targets — see each `keyboard.json` for USB VID/PID.

More detail: [QMK flashing](https://docs.qmk.fm/#/newbs_flashing), [split keyboards](https://docs.qmk.fm/#/features_split_keyboard).

---

## Project layout

```text
keyboards/fire1/
├── readme.md              # This file
├── GEMINI.md              # Maintainer notes (porting, comparison)
├── AGENTS.md              # Structure notes for tooling
│
├── splitkb/               # Aurora / splitkb target
│   ├── keyboard.json
│   ├── config.h
│   ├── keymaps/default/
│   └── lib/               # OLED, joystick (mice.c), RGB matrix, animation
│
└── lily58/                # Lily58 target
    ├── keyboard.json
    ├── config.h
    ├── keymaps/
    │   ├── fire1/         # OLED + WPM
    │   └── fire1_light/   # No OLED (smaller firmware)
    └── lib/               # OLED animation (no joystick/encoder)
```

Each board has its own `lib/main.c`. Do not merge them blindly — splitkb includes pointing-device and RGB Matrix code that Lily58 does not use.

---

## Troubleshooting

| Issue | Things to try |
|-------|----------------|
| Build error about `keyboard.json` location | Remove any `keyboards/fire1/keyboard.json` at the parent folder; only leaf folders should have it. |
| Wrong or missing RGB on Lily58 | Reflash after build; if colours persist from old settings, clear RGB EEPROM (Adjust-layer RGB keys) or reflash with a full reset. |
| `LALT_T(ALT_TAB)` not working | Ensure firmware includes `case LALT_T(ALT_TAB):` in `process_record_user` (see `lily58/lib/main.c` / `splitkb/lib/main.c`). |
| OLED goes blank after idle | splitkb: `OLED_DISABLE_TIMEOUT` in `splitkb/config.h`; Lily58 uses QMK default timeout unless disabled in config. |

---

## License

Firmware follows [QMK](https://github.com/qmk/qmk_firmware) licensing (**GPL-2.0-or-later**). When you distribute builds, comply with the GPL and preserve copyright notices in source files.

---

## Links

- [QMK documentation](https://docs.qmk.fm/)
- [QMK CLI](https://docs.qmk.fm/#/cli)
- [splitkb shop](https://splitkb.com/)
- [Lily58 (upstream QMK)](https://github.com/qmk/qmk_firmware/tree/master/keyboards/lily58)
