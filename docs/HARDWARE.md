# Hardware Guide

## Bill of Materials

| Qty | Part | Notes |
|-----|------|-------|
| 1 | **Arduino Pro Micro 5 V / 16 MHz** (SparkFun DEV-12640 or clone) | ATmega32U4, native USB HID support |
| 1–8 | **WS2812B addressable RGB LEDs** (or strip) | 5 V logic, 60 mA/LED max at full white; 5 LEDs recommended |
| 5 | **6 mm tactile pushbutton switches** | Fret buttons (green, red, yellow, blue, orange) |
| 2 | **SPDT or SPST momentary switches** | Strum bar (up + down) |
| 4 | **6 mm tactile pushbuttons** | D-pad (up, down, left, right) |
| 3 | **6 mm tactile pushbuttons** | Start, Select, Menu/Guide |
| 1 | **10 kΩ linear potentiometer** | Whammy bar |
| 1 | **Tilt ball switch (SW-200D or similar)** | Star Power tilt detection |
| 1 | **0.1 µF ceramic capacitor** | Bypass cap on Pro Micro VCC |
| 1 | **300–500 Ω resistor** | WS2812B data line series resistor |
| 1 | **1000 µF / 6.3 V electrolytic capacitor** | LED power supply decoupling |
| 1 | **USB Micro-B cable** | Programming and power |
| 1 | **Guitar body / enclosure** | 3D print, repurposed Guitar Hero shell, etc. |

---

## Microcontroller Board

### SparkFun Pro Micro 5 V (recommended)

- **MCU:** ATmega32U4 @ 16 MHz
- **Flash:** 32 KB  |  **SRAM:** 2.5 KB  |  **EEPROM:** 1 KB
- **Native USB** — no external USB chip required
- **VID/PID** in the HID build are overridden to `0x12BA / 0x0100` via `build_flags`

### Alternatives

| Board | Notes |
|-------|-------|
| Arduino Leonardo | Same ATmega32U4 chip; use `board = leonardo` in `platformio.ini`. More GPIO pins, larger form factor. |
| SparkFun Pro Micro 3.3 V / 8 MHz | **Not recommended** — WS2812B requires 5 V logic; level-shifting required. |
| Raspberry Pi Pico / RP2040 | **Requires significant porting** — different USB stack (TinyUSB), different FastLED support, no AVR-specific watchdog. Not currently supported. |
| Teensy 3.x / 4.x | Capable but not tested; requires custom PlatformIO environment. |

---

## USB VID / PID Explanation

### HID PC/Mac Mode

The default `hid` environment programs the device with:

- **VID:** `0x12BA` (Licensed by Sony Interactive Entertainment)
- **PID:** `0x0100`

This VID/PID pair is recognised by the PlayStation 3 as a **Guitar Hero Les Paul controller**, enabling native support in Guitar Hero and Rock Band on PS3. On PC/Mac it appears as a standard HID gamepad with no special driver required.

> **Note:** Using Sony's VID without a licence is technically not permitted for commercial products, but is common practice in the DIY/open-source community for personal-use guitar controllers that need PS3 compatibility.

### HID PS3 Mode

Same VID/PID as above; the `mode` field in config distinguishes PC and PS3 for any software-side differences.

### XInput Mode (Xbox 360)

Compiled with `-DXINPUT_MODE` and the `dmadison/ArduinoXInput` library. Windows recognises the device as an **Xbox 360 gamepad** via the `XUSB` class; no driver installation needed on Windows 10/11. Not compatible with PS3.

---

## XInput USB Core Installation

The `xinput` PlatformIO environment requires the **ArduinoXInput USB core** to replace the default Arduino USB core:

1. Open **Arduino IDE** → Preferences → Additional Board Manager URLs and add:
   ```
   https://raw.githubusercontent.com/dmadison/ArduinoXInput_AVR/master/package_arduinoxinput_avr_index.json
   ```
2. Open **Tools → Board → Board Manager**, search for `XInput`, and install **ArduinoXInput AVR Boards**.
3. In PlatformIO, set `board = sparkfun_promicro16` and ensure `dmadison/ArduinoXInput` is in `lib_deps`.
4. Build with `pio run -e xinput`.

> ⚠️ You **cannot** upload using the standard Arduino bootloader while the XInput USB core is active. Use a second Arduino as ISP or a USBasp programmer if you need to recover.

---

## Power Budget

| Component | Current (typical) |
|-----------|-------------------|
| Pro Micro (active) | ~25 mA |
| 5× WS2812B LEDs (mid brightness) | ~75 mA |
| 5× WS2812B LEDs (full white) | ~300 mA |
| Buttons (all pressed simultaneously) | <5 mA |
| **Total (typical)** | **~100 mA** |
| **Total (max)** | **~330 mA** |

A USB 2.0 port supplies 500 mA; this is comfortably within budget. Avoid powering more than 8 LEDs at full white from USB alone.
