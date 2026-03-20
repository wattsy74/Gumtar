#include "ButtonManager.h"
#include "../pins.h"

const uint8_t ButtonManager::_pins[BTN_COUNT] = {
    PIN_BTN_GREEN,
    PIN_BTN_RED,
    PIN_BTN_YELLOW,
    PIN_BTN_BLUE,
    PIN_BTN_ORANGE,
    PIN_STRUM_UP,
    PIN_STRUM_DOWN,
    PIN_BTN_START,
    PIN_BTN_SELECT,
    PIN_DPAD_UP,
    PIN_DPAD_DOWN,
    PIN_DPAD_LEFT,
    PIN_DPAD_RIGHT,
    PIN_BTN_MENU,
    PIN_TILT
};

void ButtonManager::begin() {
    for (uint8_t i = 0; i < BTN_COUNT; i++) {
        pinMode(_pins[i], INPUT_PULLUP);
        _state[i]          = false;
        _prevState[i]      = false;
        _rawState[i]       = false;
        _lastChangeTime[i] = 0;
    }
}

void ButtonManager::update() {
    unsigned long now = millis();

    for (uint8_t i = 0; i < BTN_COUNT; i++) {
        _prevState[i] = _state[i];

        bool raw = readBtn(_pins[i]);

        if (raw != _rawState[i]) {
            _rawState[i]       = raw;
            _lastChangeTime[i] = now;
        }

        // Commit debounced state after DEBOUNCE_MS has elapsed
        if ((now - _lastChangeTime[i]) >= DEBOUNCE_MS) {
            _state[i] = _rawState[i];
        }
    }
}

bool ButtonManager::isPressed(uint8_t btn) const {
    if (btn >= BTN_COUNT) return false;
    return _state[btn];
}

bool ButtonManager::justPressed(uint8_t btn) const {
    if (btn >= BTN_COUNT) return false;
    return _state[btn] && !_prevState[btn];
}

bool ButtonManager::justReleased(uint8_t btn) const {
    if (btn >= BTN_COUNT) return false;
    return !_state[btn] && _prevState[btn];
}
