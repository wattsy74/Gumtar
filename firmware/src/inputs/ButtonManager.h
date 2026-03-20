#pragma once
#include <Arduino.h>

enum Button : uint8_t {
    BTN_GREEN = 0,
    BTN_RED,
    BTN_YELLOW,
    BTN_BLUE,
    BTN_ORANGE,
    BTN_STRUM_UP,
    BTN_STRUM_DOWN,
    BTN_START,
    BTN_SELECT,
    BTN_DPAD_UP,
    BTN_DPAD_DOWN,
    BTN_DPAD_LEFT,
    BTN_DPAD_RIGHT,
    BTN_MENU,
    BTN_TILT,
    BTN_COUNT
};

class ButtonManager {
public:
    void begin();
    void update();

    bool isPressed(uint8_t btn) const;
    bool justPressed(uint8_t btn) const;
    bool justReleased(uint8_t btn) const;

private:
    static constexpr uint8_t DEBOUNCE_MS = 5;

    // Physical pin for each button index (must match Button enum order)
    static const uint8_t _pins[BTN_COUNT];

    bool          _state[BTN_COUNT]       = {};
    bool          _prevState[BTN_COUNT]   = {};
    bool          _rawState[BTN_COUNT]    = {};
    unsigned long _lastChangeTime[BTN_COUNT] = {};
};
