#pragma once
#include <Arduino.h>

// ── Raspberry Pi Pico (RP2040) pin assignments ─────────────────────────────
// All GPIO numbers refer to the RP2040 GPIO (GP) numbers, which match the
// pin numbers used by the earlephilhower arduino-pico core.
// These assignments match the production PCB layout.

// Fret buttons (active LOW, internal pullup)
constexpr uint8_t PIN_BTN_GREEN   = 2;   // GP2
constexpr uint8_t PIN_BTN_RED     = 3;   // GP3
constexpr uint8_t PIN_BTN_YELLOW  = 4;   // GP4
constexpr uint8_t PIN_BTN_BLUE    = 5;   // GP5
constexpr uint8_t PIN_BTN_ORANGE  = 6;   // GP6

// Strum switches (active LOW)
constexpr uint8_t PIN_STRUM_UP    = 15;  // GP15
constexpr uint8_t PIN_STRUM_DOWN  = 14;  // GP14

// System buttons (active LOW)
constexpr uint8_t PIN_BTN_START   = 0;   // GP0
constexpr uint8_t PIN_BTN_SELECT  = 1;   // GP1

// D-pad (active LOW)
constexpr uint8_t PIN_DPAD_UP     = 11;  // GP11
constexpr uint8_t PIN_DPAD_DOWN   = 10;  // GP10
constexpr uint8_t PIN_DPAD_LEFT   = 9;   // GP9
constexpr uint8_t PIN_DPAD_RIGHT  = 8;   // GP8

// Menu / guide button (active LOW)
constexpr uint8_t PIN_BTN_MENU    = 7;   // GP7

// Tilt switch (active LOW)
constexpr uint8_t PIN_TILT        = 12;  // GP12

// WS2812B LED data
constexpr uint8_t PIN_LED         = 13;  // GP13

// Whammy potentiometer (analog) — RP2040 ADC3 = GP29 = A3
// analogRead() returns 0–1023 (10-bit) with the arduino-pico core defaults.
constexpr uint8_t PIN_WHAMMY      = A3;  // GP29

// Inline helper — returns true if button is pressed (active LOW logic)
inline bool readBtn(uint8_t pin) {
    return digitalRead(pin) == LOW;
}
