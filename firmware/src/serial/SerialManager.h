#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../config/ConfigManager.h"

// Forward declaration
class GuitarController;

class SerialManager {
public:
    void begin();
    void update(GuitarController& ctrl);
    void sendStatus(uint16_t buttons, uint8_t hat, uint8_t whammy, uint8_t tilt);
    void sendConfig(GuitarConfig& cfg);
    void sendAck(const char* cmd, bool ok, const char* msg = nullptr);

private:
    static constexpr uint8_t MAX_LINE = 200;
    char    _lineBuf[MAX_LINE] = {};
    uint8_t _lineLen           = 0;

    void processLine(GuitarController& ctrl);
};
