#pragma once
#include <Arduino.h>

// Fret buttons (active LOW, internal pullup)
constexpr uint8_t PIN_BTN_GREEN   = 2;
constexpr uint8_t PIN_BTN_RED     = 3;
constexpr uint8_t PIN_BTN_YELLOW  = 4;
constexpr uint8_t PIN_BTN_BLUE    = 5;
constexpr uint8_t PIN_BTN_ORANGE  = 6;

// Strum switches (active LOW)
constexpr uint8_t PIN_STRUM_UP   = 7;
constexpr uint8_t PIN_STRUM_DOWN = 8;

// System buttons (active LOW)
constexpr uint8_t PIN_BTN_START  = 9;
constexpr uint8_t PIN_BTN_SELECT = 10;

// D-pad (active LOW)
constexpr uint8_t PIN_DPAD_UP    = 14;
constexpr uint8_t PIN_DPAD_DOWN  = 15;
constexpr uint8_t PIN_DPAD_LEFT  = 16;
constexpr uint8_t PIN_DPAD_RIGHT = 18;  // A0

// Menu / guide button (active LOW)
constexpr uint8_t PIN_BTN_MENU   = 19;  // A1

// Tilt switch (active LOW)
constexpr uint8_t PIN_TILT       = 20;  // A2

// Whammy potentiometer (analog)
constexpr uint8_t PIN_WHAMMY     = A3;  // 21

// WS2812B LED data
constexpr uint8_t PIN_LED        = A4;  // 22 — SDA on Pro Micro, usable as GPIO

// Inline helper — returns true if button is pressed (active LOW logic)
inline bool readBtn(uint8_t pin) {
    return digitalRead(pin) == LOW;
}
