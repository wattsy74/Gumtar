#include "GuitarHID.h"

#ifndef XINPUT_MODE

// USB HID descriptor for a gamepad:
//   14 buttons, 1 hat switch (4-bit), whammy axis (8-bit), tilt axis (8-bit)
static const uint8_t _hidDescriptor[] PROGMEM = {
    0x05, 0x01,       // Usage Page (Generic Desktop)
    0x09, 0x05,       // Usage (Gamepad)
    0xA1, 0x01,       // Collection (Application)

    // 14 buttons
    0x05, 0x09,       //   Usage Page (Buttons)
    0x19, 0x01,       //   Usage Minimum (Button 1)
    0x29, 0x0E,       //   Usage Maximum (Button 14)
    0x15, 0x00,       //   Logical Minimum (0)
    0x25, 0x01,       //   Logical Maximum (1)
    0x75, 0x01,       //   Report Size (1)
    0x95, 0x0E,       //   Report Count (14)
    0x81, 0x02,       //   Input (Data, Variable, Absolute)

    // 2-bit padding to align to byte boundary
    0x75, 0x02,       //   Report Size (2)
    0x95, 0x01,       //   Report Count (1)
    0x81, 0x03,       //   Input (Constant)

    // Hat switch (4-bit, values 0-7 for directions, 8=no press)
    0x05, 0x01,       //   Usage Page (Generic Desktop)
    0x09, 0x39,       //   Usage (Hat Switch)
    0x15, 0x00,       //   Logical Minimum (0)
    0x25, 0x07,       //   Logical Maximum (7)
    0x46, 0x3B, 0x01, //   Physical Maximum (315)
    0x65, 0x14,       //   Unit (Degrees)
    0x75, 0x04,       //   Report Size (4)
    0x95, 0x01,       //   Report Count (1)
    0x81, 0x42,       //   Input (Data, Variable, Absolute, Null State)

    // 4-bit padding
    0x65, 0x00,       //   Unit (None)
    0x75, 0x04,       //   Report Size (4)
    0x95, 0x01,       //   Report Count (1)
    0x81, 0x03,       //   Input (Constant)

    // Whammy axis (8-bit)
    0x09, 0x30,       //   Usage (X)  — repurposed for whammy
    0x15, 0x00,       //   Logical Minimum (0)
    0x26, 0xFF, 0x00, //   Logical Maximum (255)
    0x75, 0x08,       //   Report Size (8)
    0x95, 0x01,       //   Report Count (1)
    0x81, 0x02,       //   Input (Data, Variable, Absolute)

    // Tilt axis (8-bit)
    0x09, 0x31,       //   Usage (Y)  — repurposed for tilt
    0x15, 0x00,       //   Logical Minimum (0)
    0x26, 0xFF, 0x00, //   Logical Maximum (255)
    0x75, 0x08,       //   Report Size (8)
    0x95, 0x01,       //   Report Count (1)
    0x81, 0x02,       //   Input (Data, Variable, Absolute)

    0xC0              // End Collection
};

static HIDSubDescriptor _hidNode(_hidDescriptor, sizeof(_hidDescriptor));

void GuitarHID::begin() {
    HID().AppendDescriptor(&_hidNode);
}

void GuitarHID::sendReport(uint16_t buttons, uint8_t hat, uint8_t whammy, uint8_t tilt) {
    // Hat: HID standard uses 0-7 for 8 directions, 8 = centred.
    // We clamp to 4 bits; value 8 is treated as null state by the descriptor.
    uint8_t hatNibble = (hat > 8) ? 8 : hat;

    _report.buttons = buttons;
    _report.hat     = hatNibble;
    _report.whammy  = whammy;
    _report.tilt    = tilt;

    HID().SendReport(0, &_report, sizeof(_report));
}

#endif // XINPUT_MODE
