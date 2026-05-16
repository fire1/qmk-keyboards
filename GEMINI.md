# fire1 QMK Workspace

This repository contains two distinct keyboard configurations managed within a single workspace: **splitkb** (Aurora Selfie) and **lily58**.

## Project Structure

```text
/home/fire1/qmk_firmware/keyboards/fire1/
├── splitkb/            # Master keyboard (currently used)
│   ├── keymaps/default/ # Main keymap (contains master logic)
│   └── lib/            # Hardware-specific lib (joystick, encoders, RGB Matrix)
├── lily58/             # Ported keyboard (reorganized & functional)
│   ├── keymaps/
│   │   ├── fire1/      # OLED enabled variant
│   │   └── fire1_light/# OLED disabled variant (TAP_DANCE also disabled)
│   └── lib/            # Ported lib (Stripped of splitkb-only hardware)
└── GEMINI.md           # This file
```

## Keyboards Comparison

| Feature | splitkb (Aurora) | lily58 |
|---------|-----------------|--------|
| **Matrix** | 5x6 + Thumb | 5x6 + Thumb |
| **Pointing Device**| Analog Joystick (F4/B6) | None |
| **Encoder** | Rotary Encoder (B2/B6) | None |
| **RGB** | RGB Matrix (70 LEDs) | RGBLIGHT (12 LEDs) |
| **OLED** | 128x32 (Custom Font) | 128x32 (Stock Font) |

## Keymap Porting Logic

The `lily58` keymap is a port of the `splitkb` layout. Due to layout constraints, the following modifications were made to the `lily58` variants:
1. **Left Side (Master)**: Removed `TT(_NUM_LR)` from the thumb cluster.
2. **Right Side (Slave)**: Removed `KC_RALT` from the thumb cluster.
3. **Hardware Independence**: The `lily58/lib` logic has been stripped of joystick and encoder code to prevent compilation errors.

## Compilation Commands

To compile these keyboards, use the following QMK commands from the `qmk_firmware` root:

**splitkb (Aurora Selfie):**
```bash
qmk compile -kb fire1/splitkb -km default
```

**lily58 (OLED version):**
```bash
qmk compile -kb fire1/lily58 -km fire1
```

**lily58 (Light version - No OLED, No Tap Dance):**
```bash
qmk compile -kb fire1/lily58 -km fire1_light
```

## Maintenance Notes
- **lib/main.c**: Each keyboard has its own `lib/main.c`. Do not share them directly as they handle hardware initialization (like the joystick) differently.
- **Custom Keycodes**: Both keyboards share logic for custom keycodes like `LSFT_LNG` (Shift on hold, Language change on tap) and `ALT_TAB` / `RCTL_TAB` (swapped tap behaviors).
- **TAP_DANCE**: Disabled for all `lily58` builds to save firmware space and per user request.
