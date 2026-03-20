# Gumtar 🎸

**Gumtar** is an open-source Arduino firmware and browser-based configurator for
DIY guitar controllers. It turns an Arduino Pro Micro into a USB HID gamepad
(PC, Mac, PS3) or Xbox 360 XInput device — complete with fret buttons, strum
bar, whammy bar, tilt (star power), D-pad, and a WS2812B LED strip with
animated effects.

---

## Features

- **5 fret buttons** (green, red, yellow, blue, orange)
- **Strum bar** (up + down), **D-pad**, **Start/Select/Menu** buttons
- **Whammy bar** — 10-bit ADC with rolling average, configurable range & deadzone
- **Tilt / Star Power** — tilt ball switch with optional axis inversion
- **WS2812B LED strip** — up to 8 LEDs, four effects: Solid, Pulse, Rainbow, Strum Flash
- **Three USB modes:**
  - `hid` — standard HID gamepad (PC, Mac, PS3 Guitar Hero Les Paul)
  - `xinput` — Xbox 360 XInput controller (Windows)
- **Boot-time mode selection** — hold fret buttons at power-on to override saved mode
- **Config mode** — hold Start+Select for 3 seconds to enter serial config mode
- **Web app configurator** — browser-based (Web Serial API, Chrome/Edge)
  - Live status display (buttons, hat, whammy, tilt)
  - LED colour and effect editor
  - Button-to-HID remapping
  - Whammy calibration (manual and auto)
  - Settings: mode, tilt invert, factory reset, reboot
- **EEPROM persistence** — all settings survive power cycles
- **JSON serial protocol** — easy to integrate with custom tools

---

## Repository Layout

```
firmware/           PlatformIO project (Arduino C++)
  platformio.ini
  src/
    main.cpp
    pins.h
    GuitarController.{h,cpp}
    inputs/         ButtonManager, WhammyManager
    leds/           LEDManager (FastLED)
    config/         ConfigManager (EEPROM + ArduinoJson)
    hid/            GuitarHID (HID descriptor)
    serial/         SerialManager (JSON protocol)
  test/             README for host-based unit tests

web-app/
  index.html        Self-contained configurator (Web Serial API)

docs/
  HARDWARE.md       Bill of materials, board alternatives, power budget
  WIRING.md         Full wiring tables for every component
```

---

## Quick Start

### 1. Install PlatformIO

```bash
pip install platformio
```

or use the [PlatformIO IDE](https://platformio.org/install/ide) extension for VS Code.

### 2. Clone and build

```bash
git clone https://github.com/your-org/Gumtar.git
cd Gumtar/firmware
pio run -e hid                       # build HID firmware
pio run -e hid --target upload       # flash to connected Pro Micro
```

### 3. Wire the hardware

See **[docs/WIRING.md](docs/WIRING.md)** for full wiring tables.
See **[docs/HARDWARE.md](docs/HARDWARE.md)** for the bill of materials.

---

## USB Mode Selection

### Saved mode (web app / serial)

Use the **Settings** tab in the web app, or send:

```json
{"cmd":"set_mode","mode":"hid_pc"}
{"cmd":"save_config"}
```

Valid values: `hid_pc`, `hid_ps3`, `xinput`.

### Boot-time override (hold on power-on)

| Buttons held at power-on | Mode selected |
|--------------------------|---------------|
| Green + Red + Yellow     | XInput        |
| Blue + Orange            | PS3 / HID     |
| *(none)*                 | Saved mode    |

The boot-time selection is **not** saved to EEPROM — the next reboot will
use the saved mode again.

---

## Entering Config Mode

Hold **Start + Select** for 3 seconds. The LEDs will pulse blue to confirm.
In config mode, HID reports are suspended and the device accepts JSON commands
over USB serial at 115200 baud.

To exit, send `{"cmd":"exit_config"}` or reboot.

---

## Web App Usage

1. Open `web-app/index.html` in **Chrome** or **Edge** (version 89+).
2. Click **Connect** and select the serial port for your Gumtar device.
3. The app reads the current configuration automatically.

| Tab | What it does |
|-----|-------------|
| **Status** | Live button, hat, whammy, and tilt display (auto-refreshes at ~7 Hz) |
| **LEDs** | Per-LED colour picker, strip length, effect selector |
| **Buttons** | HID button index remapping table |
| **Whammy** | Min/max range sliders, deadzone, one-click auto-calibration |
| **Settings** | USB mode, tilt invert, factory reset, reboot |
| **Log** | Raw JSON serial log |

Click **Save All to EEPROM** (footer) or the per-tab Save buttons to persist
changes across power cycles.

---

## Serial JSON Protocol

All commands are newline-terminated JSON objects sent at 115200 baud.

### Commands (sent to device)

| `cmd` | Extra fields | Description |
|-------|-------------|-------------|
| `get_config` | — | Returns full config object |
| `get_status` | — | Returns current button/axis state |
| `set_led` | `index`, `r`, `g`, `b` | Set one LED colour |
| `set_leds` | `leds` (array) | Set all LED colours |
| `set_led_effect` | `effect` | Set animation (`solid`/`pulse`/`rainbow`/`strum_flash`) |
| `set_whammy_range` | `min`, `max` | Set ADC range |
| `set_whammy_deadzone` | `deadzone` | Set deadzone (0–255) |
| `set_tilt_invert` | `invert` (bool) | Invert tilt axis |
| `set_button_map` | `button`, `action` | Remap one button to HID index |
| `set_mode` | `mode` | Set USB mode (reboot required) |
| `save_config` | — | Write config to EEPROM |
| `factory_reset` | — | Restore defaults and save |
| `reboot` | — | Soft-reset the device |
| `calibrate_whammy` | — | Start 5-second whammy auto-calibration |
| `exit_config` | — | Leave config mode |

### Responses (from device)

| `type` | Fields | Description |
|--------|--------|-------------|
| `config` | all config fields | Response to `get_config` |
| `status` | `buttons`, `hat`, `whammy`, `tilt` | Response to `get_status` |
| `ack` | `cmd`, `ok`, `msg?` | Acknowledgement for every command |

---

## Documentation

- [Hardware guide (BOM, board alternatives, XInput setup)](docs/HARDWARE.md)
- [Wiring guide (pin tables for every component)](docs/WIRING.md)

---

## Licence

MIT — see `LICENSE` for details.
