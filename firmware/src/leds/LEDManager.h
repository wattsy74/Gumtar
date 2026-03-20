#pragma once
#include <Arduino.h>
#include <FastLED.h>

enum class LEDEffect : uint8_t {
    SOLID       = 0,
    PULSE       = 1,
    RAINBOW     = 2,
    STRUM_FLASH = 3
};

class LEDManager {
public:
    static constexpr uint8_t MAX_LEDS = 8;

    void begin(uint8_t numLeds);
    void update(bool strumEvent);
    void setLED(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
    void setAll(CRGB colour);
    void setEffect(LEDEffect effect);
    void setBaseColours(CRGB* colours);

private:
    CRGB      _leds[MAX_LEDS]       = {};
    CRGB      _baseColours[MAX_LEDS]= {};
    uint8_t   _numLeds              = 1;
    LEDEffect _effect               = LEDEffect::SOLID;

    // STRUM_FLASH state
    unsigned long _strumFlashTime   = 0;
    static constexpr uint16_t STRUM_FLASH_DURATION_MS = 80;

    // PULSE state
    uint8_t   _pulsePhase           = 0;
    unsigned long _lastPulseUpdate  = 0;
    static constexpr uint8_t PULSE_STEP_MS = 20;

    // RAINBOW state
    uint8_t   _rainbowHue           = 0;
    unsigned long _lastRainbowUpdate= 0;
    static constexpr uint8_t RAINBOW_STEP_MS = 20;

    void runSolid();
    void runPulse();
    void runRainbow();
    void runStrumFlash(bool strumEvent);
};
