# Agents: `keyboards/fire1` (QMK)

## Critical: two different keyboards, two different PCs

**`fire1/splitkb` and `fire1/lily58` target different hardware.** They are not firmware variants for the same PCB.

| | **`splitkb/`** | **`lily58/`** |
|---|----------------|----------------|
| **Board** | Aurora / splitkb ecosystem (maintainer’s modded split, e.g. joystick on right half). | Commercial / community **Lily58** (typical Lily58 matrix, wire, LEDs). |
| **Lighting** | **RGB Matrix** (many LEDs, WS2812; effects in quantum rgb matrix). | **RGBlight** (**12** SK6812/WS2812-style LEDs via `keyboard.json`; underglow style). |
| **Extras** | **OLED**, **encoder**, **right-side analog joystick** (`POINTING_DEVICE_*`). | No joystick, no encoder. **OLED** only on **`fire1`** keymap (off on **`fire1_light`**). |
| **MCU / bootloader** | See [`splitkb/keyboard.json`](splitkb/keyboard.json) (example: Caterina-ish Pro Micro class). | See [`lily58/keyboard.json`](lily58/keyboard.json) (Lily58 defaults). |
| **Code** | [`splitkb/lib/`](splitkb/lib/) (incl. [`mice.c`](splitkb/lib/mice.c)), [`splitkb/config.h`](splitkb/config.h). | [`lily58/lib/`](lily58/lib/) **must not** pull in Aurora-only drivers; port is layout/OLED logic only. |

**Implication for agents:** never assume a change in one folder applies physically to the other. Shared **intent** is mainly the **keymap / layer / custom keycode behaviour** ported from Aurora to Lily58; **pins, LED drivers, pointing device, and matrix handling stay per-keyboard.**

The folder `keyboards/fire1/` is only a **workspace grouping**. It intentionally has **no** `keyboard.json` at this directory root — QMK requires that file only on each **leaf** keyboard path; a parent `keyboard.json` would collide with subtree discovery (`Invalid keyboard.json location`).

Porting deltas (thumb clusters, stripping joystick/encoder) and feature tables: **[`GEMINI.md`](GEMINI.md)**. User-facing build/flash/layout: **[`readme.md`](readme.md)**.

---

## Targets and build commands

Run from **`qmk_firmware` root**:

| Target | Path | Typical build |
|--------|------|----------------|
| **splitkb Aurora** | `fire1/splitkb` | `qmk compile -kb fire1/splitkb -km default` |
| **Lily58 OLED** | `fire1/lily58` | `qmk compile -kb fire1/lily58 -km fire1` |
| **Lily58 minimal** | `fire1/lily58` | `qmk compile -kb fire1/lily58 -km fire1_light` |

Flash: `qmk flash -kb … -km …`. Bootloader and USB IDs are per **`keyboard.json`**.

---

## What lives where

- **`splitkb/`** — Hardware-accurate firmware for **that** PCB stack: OLED, RGB Matrix, encoder, joystick routing, Aurora-specific libs.
- **`lily58/`** — Separate **Lily58** definition; OLED optional by keymap; **RGBlight** count/pin come from **`lily58/keyboard.json`**, not from splitkb.
- **`splitkb/` has a nested `.git`** — Embedded repo (`fire1_splitkb` upstream). Do not assume `qmk_firmware` git tracks all files under `splitkb/`.

---

## Conventions agents should follow

1. **`lib/main.c` is per-keyboard.** Do not copy splitkb joystick/RGB-matrix init into lily58 or vice versa without explicit hardware rationale.
2. **Config and `rules.mk` are per-keyboard.** `RGB_MATRIX_*` belongs to splitkb; `RGBLIGHT_*` / rgblight config belongs to lily58 unless `keyboard.json` already defines LEDs.
3. **Keymaps can resemble each other logically** (layers, custom keycodes such as **`LSFT_LNG`**, **`ALT_TAB`**, **`RCTL_TAB`**) — that is behavioural porting, not shared electrical design.
4. **After edits, compile the specific target** (`fire1/splitkb:default`, `fire1/lily58:fire1`, …). AVR size matters more on lily58/light.
5. If you touch **both** trees for one behaviour, verify **both** keyboards still build; hardware constraints differ.

---

## Maintenance note (what earlier fixes were keyed to)

These were **scoped by directory**, precisely because hardware differs:

- **`splitkb/`** — OLED `keymap[]` NUL-termination for `oled_write`, parent-folder `keyboard.json` removal (QMK path rule), **`_LOWER` key matrix count** (`LAYOUT` width), OLED timeout/`config.h` as applicable to **that** firmware only.
- **`lily58/`** — RGB config aligned with **`keyboard.json`** (no duplicate `RGBLED_NUM` / deprecated pin defines), **`_LOWER` mouse keys** using QMK **`MS_*`** symbols, OLED-only globals in **`lib/main.c`** so **`fire1_light`** builds without a display — all **lily58** tree only.

Nothing in lily58 enables splitkb’s joystick/RGB Matrix; nothing in splitkb was replaced with lily58-specific rgblight limits.

---

## Optional: repo-wide agent rules

If you work from the **`qmk_firmware` tree**, the Cursor/home project may add extra tooling rules at the workspace root—not part of this file.
