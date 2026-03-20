#include "LEDManager.h"
#include "../pins.h"

// FastLED requires a compile-time pin — we use a template wrapper.
// The actual data pin is PIN_LED (A4 = 22 on Pro Micro).
static CRGB _ledBuffer[LEDManager::MAX_LEDS];

void LEDManager::begin(uint8_t numLeds) {
    _numLeds = (numLeds > MAX_LEDS) ? MAX_LEDS : numLeds;

    FastLED.addLeds<WS2812B, PIN_LED, GRB>(_ledBuffer, MAX_LEDS);
    FastLED.setBrightness(200);
    FastLED.clear(true);

    // Default base colour: fret colours cycling per LED
    const CRGB defaults[MAX_LEDS] = {
        CRGB(0, 255, 0),    // green
        CRGB(255, 0, 0),    // red
        CRGB(255, 255, 0),  // yellow
        CRGB(0, 0, 255),    // blue
        CRGB(255, 100, 0),  // orange
        CRGB(0, 255, 0),
        CRGB(255, 0, 0),
        CRGB(255, 255, 0)
    };
    for (uint8_t i = 0; i < MAX_LEDS; i++) {
        _baseColours[i] = defaults[i];
        _leds[i]        = defaults[i];
    }
}

void LEDManager::update(bool strumEvent) {
    switch (_effect) {
        case LEDEffect::SOLID:       runSolid();              break;
        case LEDEffect::PULSE:       runPulse();              break;
        case LEDEffect::RAINBOW:     runRainbow();            break;
        case LEDEffect::STRUM_FLASH: runStrumFlash(strumEvent); break;
    }

    for (uint8_t i = 0; i < _numLeds; i++) {
        _ledBuffer[i] = _leds[i];
    }
    FastLED.show();
}

void LEDManager::setLED(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
    if (index >= MAX_LEDS) return;
    _baseColours[index] = CRGB(r, g, b);
    _leds[index]        = CRGB(r, g, b);
}

void LEDManager::setAll(CRGB colour) {
    for (uint8_t i = 0; i < MAX_LEDS; i++) {
        _baseColours[i] = colour;
        _leds[i]        = colour;
    }
}

void LEDManager::setEffect(LEDEffect effect) {
    _effect = effect;
}

void LEDManager::setBaseColours(CRGB* colours) {
    for (uint8_t i = 0; i < MAX_LEDS; i++) {
        _baseColours[i] = colours[i];
        _leds[i]        = colours[i];
    }
}

// ── Private effect implementations ──────────────────────────────────────────

void LEDManager::runSolid() {
    for (uint8_t i = 0; i < _numLeds; i++) {
        _leds[i] = _baseColours[i];
    }
}

void LEDManager::runPulse() {
    unsigned long now = millis();
    if ((now - _lastPulseUpdate) >= PULSE_STEP_MS) {
        _lastPulseUpdate = now;
        _pulsePhase++;  // wraps naturally at 255
    }

    // sin8 returns 0-255; use it as brightness multiplier
    uint8_t brightness = sin8(_pulsePhase);

    for (uint8_t i = 0; i < _numLeds; i++) {
        _leds[i] = _baseColours[i];
        _leds[i].nscale8(brightness);
    }
}

void LEDManager::runRainbow() {
    unsigned long now = millis();
    if ((now - _lastRainbowUpdate) >= RAINBOW_STEP_MS) {
        _lastRainbowUpdate = now;
        _rainbowHue++;
    }

    fill_rainbow(_leds, _numLeds, _rainbowHue, 256 / _numLeds);
}

void LEDManager::runStrumFlash(bool strumEvent) {
    unsigned long now = millis();

    if (strumEvent) {
        _strumFlashTime = now;
    }

    if (_strumFlashTime != 0 && (now - _strumFlashTime) < STRUM_FLASH_DURATION_MS) {
        // Flash all white
        for (uint8_t i = 0; i < _numLeds; i++) {
            _leds[i] = CRGB::White;
        }
    } else {
        // Return to base colours
        for (uint8_t i = 0; i < _numLeds; i++) {
            _leds[i] = _baseColours[i];
        }
        _strumFlashTime = 0;
    }
}
