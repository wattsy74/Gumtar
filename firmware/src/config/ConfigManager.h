#pragma once
#include <Arduino.h>
#include <EEPROM.h>
#include <FastLED.h>
#include "../inputs/ButtonManager.h"

constexpr uint8_t CONFIG_MAGIC = 0x47;  // 'G' for Gumtar

struct GuitarConfig {
    uint8_t  magic;
    uint8_t  mode;            // 0=HID_PC, 1=HID_PS3, 2=XINPUT
    uint16_t whammyMin;
    uint16_t whammyMax;
    uint8_t  whammyDeadzone;
    bool     tiltInvert;
    uint8_t  numLeds;
    uint8_t  ledEffect;       // 0=SOLID, 1=PULSE, 2=RAINBOW, 3=STRUM_FLASH
    CRGB     ledColours[8];
    uint8_t  buttonMap[BTN_COUNT]; // HID button index for each physical button
};

class ConfigManager {
public:
    void begin();
    void save();
    void reset();
    GuitarConfig& get();

private:
    GuitarConfig _config;
    void loadDefaults();
};
