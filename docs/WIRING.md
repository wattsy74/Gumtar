# Wiring Guide

All connections refer to the **SparkFun Pro Micro 5 V (ATmega32U4)**. Pin numbers
are the digital/analogue identifiers printed on the board, which match the
`firmware/src/pins.h` definitions.

---

## Fret Buttons (active LOW, internal pull-up)

Wire one leg of each button to the listed pin and the other leg to **GND**.
No external resistors needed — the firmware enables `INPUT_PULLUP`.

| Button | Pro Micro Pin | Colour convention |
|--------|---------------|-------------------|
| Green  | `2`           | Green wire        |
| Red    | `3`           | Red wire          |
| Yellow | `4`           | Yellow wire       |
| Blue   | `5`           | Blue wire         |
| Orange | `6`           | Orange wire       |

---

## Strum Bar (active LOW)

| Switch     | Pro Micro Pin | Notes                    |
|------------|---------------|--------------------------|
| Strum Up   | `7`           | Upper leaf/microswitch   |
| Strum Down | `8`           | Lower leaf/microswitch   |

Wire one contact to the pin, other to **GND**.

---

## System Buttons (active LOW)

| Button | Pro Micro Pin |
|--------|---------------|
| Start  | `9`           |
| Select | `10`          |

---

## D-Pad (active LOW)

| Direction | Pro Micro Pin | AVR label |
|-----------|---------------|-----------|
| Up        | `14`          | MISO      |
| Down      | `15`          | SCLK      |
| Left      | `16`          | MOSI      |
| Right     | `18` (A0)     | A0        |

> Pins 14–16 are the SPI header pins on the Pro Micro; they work fine as GPIO.

---

## Menu / Guide Button (active LOW)

| Button | Pro Micro Pin | AVR label |
|--------|---------------|-----------|
| Menu   | `19` (A1)     | A1        |

---

## Tilt Switch (active LOW)

| Component    | Pro Micro Pin | AVR label |
|--------------|---------------|-----------|
| Tilt (SW-200D) | `20` (A2)   | A2        |

Wire one leg to the pin, other leg to **GND**. Mount the switch so it closes
when the guitar neck tilts upward.

---

## Whammy Potentiometer (analog)

The potentiometer acts as a voltage divider between VCC and GND.

```
Pro Micro VCC (5V) ──┬── Pot leg 1 (one end)
                     │
                     ├── Pot wiper ──→ Pin A3 (21)
                     │
Pro Micro GND ───────┴── Pot leg 3 (other end)
```

| Connection   | Pro Micro Pin | Notes                    |
|--------------|---------------|--------------------------|
| Pot wiper    | `A3` (21)     | Centre pin of pot        |
| Pot end 1    | `VCC` (5 V)   | One outer pin of pot     |
| Pot end 2    | `GND`         | Other outer pin of pot   |

Use the **Auto Calibrate** feature in the web app after wiring to set the
exact min/max ADC values for your specific potentiometer travel.

---

## WS2812B LED Strip / Ring

```
Pro Micro 5V ─────────────────┬───── LED strip VCC (5V)
                              │
                    1000 µF cap (across VCC/GND of strip)
                              │
Pro Micro GND ────────────────┴───── LED strip GND

Pro Micro Pin A4 (22) ──[330Ω]──── LED strip Data In (DI)
```

| Connection   | Pro Micro Pin | Notes                                    |
|--------------|---------------|------------------------------------------|
| Data In (DI) | `A4` (22)     | Via 300–500 Ω series resistor            |
| VCC          | `VCC` (5 V)   | Add 1000 µF cap across VCC/GND near LEDs |
| GND          | `GND`         | Common ground                            |

> **Important:** The 300–500 Ω series resistor on the data line prevents ringing
> and protects the first LED from overvoltage spikes. The 1000 µF bulk capacitor
> absorbs the inrush current when LEDs switch on and prevents voltage sag that
> can crash the microcontroller.

> **Note:** Pin `A4` (22) is also the I2C SDA pin on the Pro Micro. It is fully
> usable as a digital output when I2C is not in use. FastLED's template pin
> argument is set to this pin at compile time.

---

## Full Pin Summary

| Pin      | Function           | Direction | Pull-up |
|----------|--------------------|-----------|---------|
| `2`      | Fret Green         | Input     | Yes     |
| `3`      | Fret Red           | Input     | Yes     |
| `4`      | Fret Yellow        | Input     | Yes     |
| `5`      | Fret Blue          | Input     | Yes     |
| `6`      | Fret Orange        | Input     | Yes     |
| `7`      | Strum Up           | Input     | Yes     |
| `8`      | Strum Down         | Input     | Yes     |
| `9`      | Start              | Input     | Yes     |
| `10`     | Select             | Input     | Yes     |
| `14`     | D-Pad Up           | Input     | Yes     |
| `15`     | D-Pad Down         | Input     | Yes     |
| `16`     | D-Pad Left         | Input     | Yes     |
| `18 / A0`| D-Pad Right        | Input     | Yes     |
| `19 / A1`| Menu/Guide         | Input     | Yes     |
| `20 / A2`| Tilt switch        | Input     | Yes     |
| `A3 / 21`| Whammy pot wiper   | Analog in | No      |
| `A4 / 22`| WS2812B Data       | Output    | No      |

---

## Grounding Notes

- Use a **star ground** topology: run separate GND wires from the Pro Micro GND
  pin to each button cluster, the whammy pot, and the LED strip GND. Join them
  at the Pro Micro GND pin rather than daisy-chaining.
- Keep the WS2812B data wire short and away from the analog whammy wire to
  minimise noise pickup on the ADC.
