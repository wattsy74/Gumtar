#pragma once
#include <Arduino.h>

#ifndef XINPUT_MODE

#include <HID.h>

// HID report structure:
//   bytes 0-1: button bitfield (14 buttons)
//   byte  2  : hat switch (0=Up, 2=Right, 4=Down, 6=Left, diagonals, 8=None)
//   byte  3  : whammy axis (0-255)
//   byte  4  : tilt axis (0-255)
struct GuitarHIDReport {
    uint16_t buttons;
    uint8_t  hat;
    uint8_t  whammy;
    uint8_t  tilt;
};

class GuitarHID {
public:
    void begin();
    void sendReport(uint16_t buttons, uint8_t hat, uint8_t whammy, uint8_t tilt);

private:
    GuitarHIDReport _report = {};
};

#else  // XINPUT_MODE

class GuitarHID {
public:
    void begin() {}
    void sendReport(uint16_t, uint8_t, uint8_t, uint8_t) {}  // no-op; XInput handled in main controller
};

#endif // XINPUT_MODE
