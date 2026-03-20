#pragma once
#include <Arduino.h>

#ifndef XINPUT_MODE

#include <Adafruit_TinyUSB.h>

// HID report structure — must match the custom HID descriptor below:
//   bytes 0-1: button bitfield (14 buttons, top 2 bits always 0)
//   byte  2  : hat switch (low nibble 0-8, high nibble 0 = padding)
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
    Adafruit_USBD_HID _usb_hid;
    GuitarHIDReport   _report = {};
};

#else  // XINPUT_MODE

class GuitarHID {
public:
    void begin() {}
    void sendReport(uint16_t, uint8_t, uint8_t, uint8_t) {}  // no-op; XInput handled in GuitarController
};

#endif // XINPUT_MODE
