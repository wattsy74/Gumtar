#include "WhammyManager.h"
#include "../pins.h"

void WhammyManager::begin(uint16_t minVal, uint16_t maxVal, uint8_t deadzone) {
    _minVal   = minVal;
    _maxVal   = maxVal;
    _deadzone = deadzone;

    // Pre-fill sample buffer with current ADC reading
    uint16_t initial = analogRead(PIN_WHAMMY);
    for (uint8_t i = 0; i < SAMPLES; i++) {
        _samples[i] = initial;
    }
    _sampleIdx = 0;
}

void WhammyManager::update() {
    _samples[_sampleIdx] = analogRead(PIN_WHAMMY);
    _sampleIdx = (_sampleIdx + 1) % SAMPLES;

    uint32_t sum = 0;
    for (uint8_t i = 0; i < SAMPLES; i++) {
        sum += _samples[i];
    }
    uint16_t avg = (uint16_t)(sum / SAMPLES);

    // Apply deadzone around min (resting position)
    uint16_t effectiveMin = _minVal + _deadzone;
    if (avg <= effectiveMin) {
        _value = 0;
        return;
    }

    uint16_t range = (_maxVal > effectiveMin) ? (_maxVal - effectiveMin) : 1;
    uint32_t mapped = ((uint32_t)(avg - effectiveMin) * 255) / range;
    if (mapped > 255) mapped = 255;
    _value = (uint8_t)mapped;
}

uint8_t WhammyManager::getValue() const {
    return _value;
}
