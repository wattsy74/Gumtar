# Hardware Guide

## Bill of Materials

| Qty | Part | Notes |
|-----|------|-------|
| 1 | **Raspberry Pi Pico** (or Pico H / Pico W) | RP2040, native USB, 3.3 V I/O — see note below |
| 1–8 | **WS2812B addressable RGB LEDs** (or strip) | 5 V logic; 5 LEDs recommended |
| 1 | **3.3 V → 5 V level shifter** (e.g. 74AHCT125 or SN74HCT245) | Required for WS2812B data line (RP2040 is 3.3 V logic) |
| 5 | **6 mm tactile pushbutton switches** | Fret buttons (green, red, yellow, blue, orange) |
| 2 | **SPDT or SPST momentary switches** | Strum bar (up + down) |
| 4 | **6 mm tactile pushbuttons** | D-pad (up, down, left, right) |
| 3 | **6 mm tactile pushbuttons** | Start, Select, Menu/Guide |
| 1 | **10 kΩ linear potentiometer** | Whammy bar |
| 1 | **Tilt ball switch (SW-200D or similar)** | Star Power tilt detection |
| 1 | **300–500 Ω resistor** | WS2812B data line series resistor (after level shifter) |
| 1 | **1000 µF / 6.3 V electrolytic capacitor** | LED power supply decoupling |
| 1 | **USB Micro-B cable** | Programming and power |
| 1 | **Guitar body / enclosure** | 3D print, repurposed Guitar Hero shell, etc. |

---

## Microcontroller Board

### Raspberry Pi Pico (recommended)

- **MCU:** RP2040 dual-core ARM Cortex-M0+ @ 133 MHz
- **Flash:** 2 MB  |  **SRAM:** 264 KB
- **Native USB** via TinyUSB — no external USB chip required
- **3.3 V GPIO** — a level shifter is required for WS2812B data
- EEPROM is emulated in flash via the arduino-pico core
- USB VID/PID in the HID build are set to `0x12BA / 0x0100` via `build_flags`

> **3.3 V vs 5 V:** The RP2040 GPIOs are 3.3 V. WS2812B LEDs expect a 5 V data
> signal. A single-channel 74AHCT125 or SN74HCT245 level shifter between GP17
> and the LED strip data input ensures reliable operation.

### Alternatives

| Board | Notes |
|-------|-------|
| Raspberry Pi Pico 2 (RP2350) | Drop-in compatible; same pinout. |
| Raspberry Pi Pico W | Same RP2040 but with Wi-Fi; additional wireless config possible. |
| Adafruit Feather RP2040 | Same core; different pinout — update `pins.h` accordingly. |
| SparkFun Pro Micro (ATmega32U4) | Original target; no longer supported in this branch. |

---

## USB VID / PID Explanation

### HID PC/Mac Mode

The default `hid` environment programs the device with:

- **VID:** `0x12BA` (Licensed by Sony Interactive Entertainment)
- **PID:** `0x0100`

This VID/PID pair is recognised by the PlayStation 3 as a **Guitar Hero Les Paul
controller**, enabling native support in Guitar Hero and Rock Band on PS3. On
PC/Mac it appears as a standard HID gamepad with no special driver required.

> **Note:** Using Sony's VID without a licence is technically not permitted for
> commercial products, but is common practice in the DIY/open-source community
> for personal-use guitar controllers that need PS3 compatibility.

### XInput Mode (Xbox 360)

Compiled with `-DXINPUT_MODE` and the `ArduinoXInput_Pico` library. Windows
recognises the device as an **Xbox 360 gamepad** via the XUSB class driver; no
driver installation is needed on Windows 10/11. Not compatible with PS3.

---

## XInput USB Core Installation (xinput build)

The `xinput` PlatformIO environment requires the
[ArduinoXInput_Pico](https://github.com/dmadison/ArduinoXInput_Pico) library,
which is referenced directly via its Git URL in `platformio.ini`:

```ini
lib_deps =
    https://github.com/dmadison/ArduinoXInput_Pico.git
```

PlatformIO will clone and build the library automatically. Refer to the
[ArduinoXInput_Pico README](https://github.com/dmadison/ArduinoXInput_Pico)
for any additional setup steps required for your OS.

Build with:

```bash
pio run -e xinput
```

---

## Power Budget

| Component | Current (typical) |
|-----------|-------------------|
| Raspberry Pi Pico (active) | ~25 mA |
| 5× WS2812B LEDs (mid brightness) | ~75 mA |
| 5× WS2812B LEDs (full white) | ~300 mA |
| Buttons (all pressed simultaneously) | <5 mA |
| **Total (typical)** | **~105 mA** |
| **Total (max)** | **~330 mA** |

A USB 2.0 port supplies 500 mA; this is comfortably within budget.
Avoid powering more than 8 LEDs at full white from USB alone.
