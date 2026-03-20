# Wiring Guide

All connections refer to the **Raspberry Pi Pico (RP2040)**. Pin numbers are
the GP (GPIO) numbers labelled on the Pico board, which match the
`firmware/src/pins.h` definitions.

> **3.3 V logic notice:** The Pico's GPIOs operate at **3.3 V**. WS2812B LEDs
> require a 5 V data signal. Insert a 74AHCT125 or SN74HCT245 level shifter
> between GP17 and the LED strip data input. All button and switch inputs are
> fine at 3.3 V — the internal pull-ups pull to 3.3 V and most switches are
> passive (connected to GND when pressed).

---

## Fret Buttons (active LOW, internal pull-up)

Wire one leg of each button to the listed GP pin and the other leg to **GND**.
No external resistors needed — the firmware enables `INPUT_PULLUP`.

| Button | Pico Pin | GPIO |
|--------|----------|------|
| Green  | Pin 4    | GP2  |
| Red    | Pin 5    | GP3  |
| Yellow | Pin 6    | GP4  |
| Blue   | Pin 7    | GP5  |
| Orange | Pin 9    | GP6  |

---

## Strum Bar (active LOW)

| Switch     | Pico Pin | GPIO | Notes                   |
|------------|----------|------|-------------------------|
| Strum Up   | Pin 10   | GP7  | Upper leaf/microswitch  |
| Strum Down | Pin 11   | GP8  | Lower leaf/microswitch  |

Wire one contact to the GP pin, other to **GND**.

---

## System Buttons (active LOW)

| Button | Pico Pin | GPIO |
|--------|----------|------|
| Start  | Pin 12   | GP9  |
| Select | Pin 14   | GP10 |

---

## D-Pad (active LOW)

| Direction | Pico Pin | GPIO |
|-----------|----------|------|
| Up        | Pin 15   | GP11 |
| Down      | Pin 16   | GP12 |
| Left      | Pin 17   | GP13 |
| Right     | Pin 19   | GP14 |

---

## Menu / Guide Button (active LOW)

| Button | Pico Pin | GPIO |
|--------|----------|------|
| Menu   | Pin 20   | GP15 |

---

## Tilt Switch (active LOW)

| Component      | Pico Pin | GPIO | Notes                                             |
|----------------|----------|------|---------------------------------------------------|
| Tilt (SW-200D) | Pin 21   | GP16 | Wire one leg to GP16, other to GND. Mount so the switch closes when the guitar neck tilts upward. |

---

## Whammy Potentiometer (analog)

The potentiometer acts as a voltage divider between 3.3 V and GND.

```
Pico 3V3 (Pin 36) ──┬── Pot leg 1 (one end)
                    │
                    ├── Pot wiper ──→ GP26 / A0 (Pin 31)
                    │
Pico GND (Pin 38) ──┴── Pot leg 3 (other end)
```

| Connection | Pico Pin | GPIO / Net | Notes                |
|------------|----------|------------|----------------------|
| Pot wiper  | Pin 31   | GP26 (A0)  | Centre pin of pot    |
| Pot end 1  | Pin 36   | 3V3        | One outer pin of pot |
| Pot end 2  | Pin 38   | GND        | Other outer pin      |

`analogRead()` returns 0–1023 (10-bit) by default with the arduino-pico core.
Use the **Auto Calibrate** feature in the web app after wiring to set the
exact min/max ADC values for your specific potentiometer travel.

---

## WS2812B LED Strip / Ring

> **Level shifter required.** The RP2040 outputs 3.3 V; WS2812B requires
> ≥ 3.5 V on the data line. A 74AHCT125 (or equivalent) between GP17 and
> the strip's DI pin ensures reliable operation.

```
Pico VSYS (Pin 39) ───────────────────┬───── LED strip VCC (5 V from USB)
                                      │
                            1000 µF cap (across VCC/GND of strip)
                                      │
Pico GND (Pin 38) ────────────────────┴───── LED strip GND

Pico GP17 (Pin 22) ──[74AHCT125]──[330 Ω]──── LED strip Data In (DI)
```

| Connection    | Pico Pin | GPIO  | Notes                                     |
|---------------|----------|-------|-------------------------------------------|
| Data In (DI)  | Pin 22   | GP17  | Via level shifter then 300–500 Ω resistor |
| VCC           | Pin 39   | VSYS  | 5 V from USB (through Pico regulator path)|
| GND           | Pin 38   | GND   | Common ground                             |

> **Important:** The 300–500 Ω series resistor on the data line prevents
> ringing and protects the first LED from overvoltage spikes. The 1000 µF
> bulk capacitor absorbs inrush current when LEDs switch on.

---

## Full Pin Summary

| Pico Pin | GPIO  | Function           | Direction | Pull-up |
|----------|-------|--------------------|-----------|---------|
| 4        | GP2   | Fret Green         | Input     | Yes     |
| 5        | GP3   | Fret Red           | Input     | Yes     |
| 6        | GP4   | Fret Yellow        | Input     | Yes     |
| 7        | GP5   | Fret Blue          | Input     | Yes     |
| 9        | GP6   | Fret Orange        | Input     | Yes     |
| 10       | GP7   | Strum Up           | Input     | Yes     |
| 11       | GP8   | Strum Down         | Input     | Yes     |
| 12       | GP9   | Start              | Input     | Yes     |
| 14       | GP10  | Select             | Input     | Yes     |
| 15       | GP11  | D-Pad Up           | Input     | Yes     |
| 16       | GP12  | D-Pad Down         | Input     | Yes     |
| 17       | GP13  | D-Pad Left         | Input     | Yes     |
| 19       | GP14  | D-Pad Right        | Input     | Yes     |
| 20       | GP15  | Menu/Guide         | Input     | Yes     |
| 21       | GP16  | Tilt switch        | Input     | Yes     |
| 22       | GP17  | WS2812B Data       | Output    | No      |
| 31       | GP26 (A0) | Whammy pot wiper | Analog in | No   |

---

## Grounding Notes

- Use a **star ground** topology: run separate GND wires from a Pico GND pin
  to each button cluster, the whammy pot, and the LED strip GND. Join them
  at the Pico GND pin rather than daisy-chaining.
- Keep the WS2812B data wire short and routed away from the analog whammy
  wire to minimise noise pickup on the ADC.
- The Pico has multiple GND pins (3, 8, 13, 18, 23, 28, 33, 38); use the
  nearest one for each group of components.
