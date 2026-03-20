#pragma once
#include <Arduino.h>

class WhammyManager {
public:
    void begin(uint16_t minVal, uint16_t maxVal, uint8_t deadzone);
    void update();
    uint8_t getValue() const;

private:
    static constexpr uint8_t SAMPLES = 4;

    uint16_t _samples[SAMPLES] = {};
    uint8_t  _sampleIdx        = 0;
    uint16_t _minVal           = 0;
    uint16_t _maxVal           = 1023;
    uint8_t  _deadzone         = 0;
    uint8_t  _value            = 0;
};
