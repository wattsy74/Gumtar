#include "SerialManager.h"
#include "../GuitarController.h"

void SerialManager::begin() {
    Serial.begin(115200);
}

void SerialManager::update(GuitarController& ctrl) {
    while (Serial.available()) {
        char c = (char)Serial.read();

        if (c == '\n' || c == '\r') {
            if (_lineLen > 0) {
                _lineBuf[_lineLen] = '\0';
                processLine(ctrl);
                _lineLen = 0;
            }
        } else if (_lineLen < (MAX_LINE - 1)) {
            _lineBuf[_lineLen++] = c;
        }
    }
}

void SerialManager::processLine(GuitarController& ctrl) {
    StaticJsonDocument<512> doc;
    DeserializationError err = deserializeJson(doc, _lineBuf);

    if (err) {
        sendAck("parse", false, "JSON parse error");
        return;
    }

    ctrl.handleCommand(doc.as<JsonObject>());
}

void SerialManager::sendStatus(uint16_t buttons, uint8_t hat, uint8_t whammy, uint8_t tilt) {
    StaticJsonDocument<256> doc;
    doc[F("type")]    = F("status");
    doc[F("buttons")] = buttons;
    doc[F("hat")]     = hat;
    doc[F("whammy")]  = whammy;
    doc[F("tilt")]    = tilt;
    serializeJson(doc, Serial);
    Serial.print('\n');
}

void SerialManager::sendConfig(GuitarConfig& cfg) {
    StaticJsonDocument<512> doc;
    doc[F("type")]            = F("config");
    doc[F("mode")]            = cfg.mode;
    doc[F("whammyMin")]       = cfg.whammyMin;
    doc[F("whammyMax")]       = cfg.whammyMax;
    doc[F("whammyDeadzone")]  = cfg.whammyDeadzone;
    doc[F("tiltInvert")]      = cfg.tiltInvert;
    doc[F("numLeds")]         = cfg.numLeds;
    doc[F("ledEffect")]       = cfg.ledEffect;

    JsonArray leds = doc.createNestedArray(F("leds"));
    for (uint8_t i = 0; i < 8; i++) {
        JsonObject led = leds.createNestedObject();
        led[F("r")] = cfg.ledColours[i].r;
        led[F("g")] = cfg.ledColours[i].g;
        led[F("b")] = cfg.ledColours[i].b;
    }

    JsonArray bmap = doc.createNestedArray(F("buttonMap"));
    for (uint8_t i = 0; i < BTN_COUNT; i++) {
        bmap.add(cfg.buttonMap[i]);
    }

    serializeJson(doc, Serial);
    Serial.print('\n');
}

void SerialManager::sendAck(const char* cmd, bool ok, const char* msg) {
    StaticJsonDocument<128> doc;
    doc[F("type")] = F("ack");
    doc[F("cmd")]  = cmd;
    doc[F("ok")]   = ok;
    if (msg) doc[F("msg")] = msg;
    serializeJson(doc, Serial);
    Serial.print('\n');
}
