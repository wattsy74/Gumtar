#include "ConfigManager.h"

static const CRGB DEFAULT_LED_COLOURS[8] PROGMEM = {
    CRGB(0, 255, 0),
    CRGB(255, 0, 0),
    CRGB(255, 255, 0),
    CRGB(0, 0, 255),
    CRGB(255, 100, 0),
    CRGB(0, 255, 0),
    CRGB(255, 0, 0),
    CRGB(255, 255, 0)
};

void ConfigManager::loadDefaults() {
    _config.magic          = CONFIG_MAGIC;
    _config.mode           = 0;  // HID_PC
    _config.whammyMin      = 0;
    _config.whammyMax      = 1023;
    _config.whammyDeadzone = 10;
    _config.tiltInvert     = false;
    _config.numLeds        = 5;
    _config.ledEffect      = 0;  // SOLID

    for (uint8_t i = 0; i < 8; i++) {
        _config.ledColours[i] = DEFAULT_LED_COLOURS[i];
    }

    // Default HID button mapping
    _config.buttonMap[BTN_GREEN]      = 0;
    _config.buttonMap[BTN_RED]        = 1;
    _config.buttonMap[BTN_YELLOW]     = 2;
    _config.buttonMap[BTN_BLUE]       = 3;
    _config.buttonMap[BTN_ORANGE]     = 4;
    _config.buttonMap[BTN_STRUM_UP]   = 0xFF; // handled via hat
    _config.buttonMap[BTN_STRUM_DOWN] = 0xFF; // handled via hat
    _config.buttonMap[BTN_START]      = 6;
    _config.buttonMap[BTN_SELECT]     = 7;
    _config.buttonMap[BTN_DPAD_UP]    = 0xFF; // handled via hat
    _config.buttonMap[BTN_DPAD_DOWN]  = 0xFF; // handled via hat
    _config.buttonMap[BTN_DPAD_LEFT]  = 0xFF; // handled via hat
    _config.buttonMap[BTN_DPAD_RIGHT] = 0xFF; // handled via hat
    _config.buttonMap[BTN_MENU]       = 8;
    _config.buttonMap[BTN_TILT]       = 5;
}

void ConfigManager::begin() {
    uint8_t magic;
    EEPROM.get(0, magic);

    if (magic == CONFIG_MAGIC) {
        EEPROM.get(0, _config);
    } else {
        loadDefaults();
        save();
    }
}

void ConfigManager::save() {
    EEPROM.put(0, _config);
}

void ConfigManager::reset() {
    loadDefaults();
    save();
}

GuitarConfig& ConfigManager::get() {
    return _config;
}
