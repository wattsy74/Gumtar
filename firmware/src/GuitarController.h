#pragma once
#include <Arduino.h>
#include "inputs/ButtonManager.h"
#include "inputs/WhammyManager.h"
#include "leds/LEDManager.h"
#include "config/ConfigManager.h"
#include "hid/GuitarHID.h"
#include "serial/SerialManager.h"
#include <ArduinoJson.h>

#ifdef XINPUT_MODE
#include <XInput.h>
#endif

class GuitarController {
public:
    void begin();
    void update();
    void handleCommand(JsonObject cmd);
    void enterConfigMode();
    void exitConfigMode();

    // Accessors for SerialManager
    ButtonManager& getButtons() { return _buttons; }
    WhammyManager& getWhammy()  { return _whammy;  }
    ConfigManager& getConfig()  { return _config;  }

private:
    ButtonManager _buttons;
    WhammyManager _whammy;
    LEDManager    _leds;
    ConfigManager _config;
    GuitarHID     _hid;
    SerialManager _serial;

    bool          _configMode        = false;
    unsigned long _configHoldStart   = 0;
    bool          _configHoldActive  = false;
    static constexpr uint32_t CONFIG_HOLD_MS = 3000;

    // Whammy calibration state
    bool          _calibrating       = false;
    unsigned long _calibrateStart    = 0;
    uint16_t      _calibrateMin      = 1023;
    uint16_t      _calibrateMax      = 0;
    static constexpr uint32_t CALIBRATE_DURATION_MS = 5000;

    void detectBootMode();
    void checkConfigModeEntry();
    uint8_t buildHat() const;
    uint16_t buildButtons() const;
    void sendHIDReport();
};
