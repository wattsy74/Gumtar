#include "GuitarController.h"
#include "pins.h"

void GuitarController::begin() {
    _config.begin();
    GuitarConfig& cfg = _config.get();

    detectBootMode();

    _buttons.begin();
    _whammy.begin(cfg.whammyMin, cfg.whammyMax, cfg.whammyDeadzone);
    _leds.begin(cfg.numLeds);
    _leds.setEffect(static_cast<LEDEffect>(cfg.ledEffect));
    _leds.setBaseColours(cfg.ledColours);

#ifndef XINPUT_MODE
    _hid.begin();
#else
    XInput.setRange(AXIS_LEFT_X, 0, 255);
    XInput.setRange(AXIS_LEFT_Y, 0, 255);
    XInput.begin();
#endif

    _serial.begin();
}

void GuitarController::update() {
    _buttons.update();
    _whammy.update();

    // Handle whammy calibration
    if (_calibrating) {
        uint16_t raw = analogRead(PIN_WHAMMY);
        if (raw < _calibrateMin) _calibrateMin = raw;
        if (raw > _calibrateMax) _calibrateMax = raw;

        if ((millis() - _calibrateStart) >= CALIBRATE_DURATION_MS) {
            _calibrating = false;
            GuitarConfig& cfg = _config.get();
            cfg.whammyMin = _calibrateMin;
            cfg.whammyMax = _calibrateMax;
            _whammy.begin(cfg.whammyMin, cfg.whammyMax, cfg.whammyDeadzone);
            _serial.sendAck("calibrate_whammy", true, "done");
        }
    }

    bool strumEvent = _buttons.justPressed(BTN_STRUM_UP) || _buttons.justPressed(BTN_STRUM_DOWN);
    _leds.update(strumEvent);

    if (!_configMode) {
        sendHIDReport();
    }

    checkConfigModeEntry();

    if (_configMode) {
        _serial.update(*this);
    }
}

void GuitarController::detectBootMode() {
    // Read pins directly at boot (before ButtonManager init)
    pinMode(PIN_BTN_GREEN,  INPUT_PULLUP);
    pinMode(PIN_BTN_RED,    INPUT_PULLUP);
    pinMode(PIN_BTN_YELLOW, INPUT_PULLUP);
    pinMode(PIN_BTN_BLUE,   INPUT_PULLUP);
    pinMode(PIN_BTN_ORANGE, INPUT_PULLUP);
    delay(10);  // allow pullups to settle

    GuitarConfig& cfg = _config.get();

    // Green+Red+Yellow → XInput
    if (readBtn(PIN_BTN_GREEN) && readBtn(PIN_BTN_RED) && readBtn(PIN_BTN_YELLOW)) {
        cfg.mode = 2;
    }
    // Blue+Orange → PS3
    else if (readBtn(PIN_BTN_BLUE) && readBtn(PIN_BTN_ORANGE)) {
        cfg.mode = 1;
    }
    // else: keep saved mode
}

void GuitarController::checkConfigModeEntry() {
    bool holdActive = _buttons.isPressed(BTN_SELECT) && _buttons.isPressed(BTN_START);

    if (holdActive) {
        if (!_configHoldActive) {
            _configHoldActive = true;
            _configHoldStart  = millis();
        } else if (!_configMode && (millis() - _configHoldStart) >= CONFIG_HOLD_MS) {
            enterConfigMode();
        }
    } else {
        _configHoldActive = false;
    }
}

void GuitarController::enterConfigMode() {
    _configMode = true;
    // Flash blue on all LEDs to indicate config mode
    _leds.setAll(CRGB(0, 0, 255));
    _leds.setEffect(LEDEffect::PULSE);
    _serial.sendAck("config_mode", true, "entered");
}

void GuitarController::exitConfigMode() {
    _configMode = false;
    GuitarConfig& cfg = _config.get();
    _leds.setEffect(static_cast<LEDEffect>(cfg.ledEffect));
    _leds.setBaseColours(cfg.ledColours);
    _serial.sendAck("config_mode", false, "exited");
}

uint8_t GuitarController::buildHat() const {
    bool up    = _buttons.isPressed(BTN_DPAD_UP)    || _buttons.isPressed(BTN_STRUM_UP);
    bool down  = _buttons.isPressed(BTN_DPAD_DOWN)  || _buttons.isPressed(BTN_STRUM_DOWN);
    bool left  = _buttons.isPressed(BTN_DPAD_LEFT);
    bool right = _buttons.isPressed(BTN_DPAD_RIGHT);

    if (up    && right) return 1;
    if (right && down)  return 3;
    if (down  && left)  return 5;
    if (left  && up)    return 7;
    if (up)    return 0;
    if (right) return 2;
    if (down)  return 4;
    if (left)  return 6;
    return 8;  // centred / no press
}

uint16_t GuitarController::buildButtons() const {
    const GuitarConfig& cfg = _config.get();
    uint16_t btns = 0;

    for (uint8_t i = 0; i < BTN_COUNT; i++) {
        uint8_t hidIdx = cfg.buttonMap[i];
        if (hidIdx == 0xFF) continue;  // hat-handled or unmapped
        if (hidIdx >= 14)   continue;
        if (_buttons.isPressed(i)) {
            btns |= (1u << hidIdx);
        }
    }
    return btns;
}

void GuitarController::sendHIDReport() {
    uint16_t buttons = buildButtons();
    uint8_t  hat     = buildHat();
    uint8_t  whammy  = _whammy.getValue();
    uint8_t  tilt    = _buttons.isPressed(BTN_TILT) ? 255 : 0;

    const GuitarConfig& cfg = _config.get();
    if (cfg.tiltInvert) tilt = 255 - tilt;

#ifndef XINPUT_MODE
    _hid.sendReport(buttons, hat, whammy, tilt);
#else
    // Map to XInput
    XInput.setButton(BUTTON_A, (buttons >> 0) & 1);  // green
    XInput.setButton(BUTTON_B, (buttons >> 1) & 1);  // red
    XInput.setButton(BUTTON_Y, (buttons >> 2) & 1);  // yellow
    XInput.setButton(BUTTON_X, (buttons >> 3) & 1);  // blue
    XInput.setButton(BUTTON_LB,(buttons >> 4) & 1);  // orange
    XInput.setButton(BUTTON_START, (buttons >> 6) & 1);
    XInput.setButton(BUTTON_BACK,  (buttons >> 7) & 1);
    XInput.setButton(BUTTON_LOGO,  (buttons >> 8) & 1);

    // D-pad / strum
    XInput.setDpad(hat == 0 || hat == 1 || hat == 7,   // up
                   hat == 4 || hat == 3 || hat == 5,   // down
                   hat == 6 || hat == 5 || hat == 7,   // left
                   hat == 2 || hat == 1 || hat == 3);  // right

    XInput.setAxis(AXIS_LEFT_X,  whammy);
    XInput.setAxis(AXIS_LEFT_Y,  tilt);
    XInput.send();
#endif
}

void GuitarController::handleCommand(JsonObject cmd) {
    const char* command = cmd[F("cmd")] | "";
    GuitarConfig& cfg   = _config.get();

    if (strcmp_P(command, PSTR("get_config")) == 0) {
        _serial.sendConfig(cfg);

    } else if (strcmp_P(command, PSTR("get_status")) == 0) {
        _serial.sendStatus(buildButtons(), buildHat(), _whammy.getValue(),
                           _buttons.isPressed(BTN_TILT) ? 255 : 0);

    } else if (strcmp_P(command, PSTR("set_led")) == 0) {
        uint8_t idx = cmd[F("index")] | 0;
        uint8_t r   = cmd[F("r")]     | 0;
        uint8_t g   = cmd[F("g")]     | 0;
        uint8_t b   = cmd[F("b")]     | 0;
        if (idx < 8) {
            cfg.ledColours[idx] = CRGB(r, g, b);
            _leds.setLED(idx, r, g, b);
        }
        _serial.sendAck("set_led", true);

    } else if (strcmp_P(command, PSTR("set_leds")) == 0) {
        JsonArray arr = cmd[F("leds")].as<JsonArray>();
        uint8_t i = 0;
        for (JsonObject led : arr) {
            if (i >= 8) break;
            uint8_t r = led[F("r")] | 0;
            uint8_t g = led[F("g")] | 0;
            uint8_t b = led[F("b")] | 0;
            cfg.ledColours[i] = CRGB(r, g, b);
            _leds.setLED(i, r, g, b);
            i++;
        }
        _serial.sendAck("set_leds", true);

    } else if (strcmp_P(command, PSTR("set_led_effect")) == 0) {
        const char* eff = cmd[F("effect")] | "solid";
        LEDEffect e = LEDEffect::SOLID;
        if      (strcmp_P(eff, PSTR("pulse"))        == 0) e = LEDEffect::PULSE;
        else if (strcmp_P(eff, PSTR("rainbow"))      == 0) e = LEDEffect::RAINBOW;
        else if (strcmp_P(eff, PSTR("strum_flash"))  == 0) e = LEDEffect::STRUM_FLASH;
        cfg.ledEffect = static_cast<uint8_t>(e);
        _leds.setEffect(e);
        _serial.sendAck("set_led_effect", true);

    } else if (strcmp_P(command, PSTR("set_whammy_range")) == 0) {
        cfg.whammyMin = cmd[F("min")] | 0;
        cfg.whammyMax = cmd[F("max")] | 1023;
        _whammy.begin(cfg.whammyMin, cfg.whammyMax, cfg.whammyDeadzone);
        _serial.sendAck("set_whammy_range", true);

    } else if (strcmp_P(command, PSTR("set_whammy_deadzone")) == 0) {
        cfg.whammyDeadzone = cmd[F("deadzone")] | 0;
        _whammy.begin(cfg.whammyMin, cfg.whammyMax, cfg.whammyDeadzone);
        _serial.sendAck("set_whammy_deadzone", true);

    } else if (strcmp_P(command, PSTR("set_tilt_invert")) == 0) {
        cfg.tiltInvert = cmd[F("invert")] | false;
        _serial.sendAck("set_tilt_invert", true);

    } else if (strcmp_P(command, PSTR("set_button_map")) == 0) {
        const char* btnName = cmd[F("button")] | "";
        uint8_t     action  = cmd[F("action")]  | 0xFF;
        // Map name to index
        const char* const names[] = {
            "green","red","yellow","blue","orange",
            "strum_up","strum_down","start","select",
            "dpad_up","dpad_down","dpad_left","dpad_right",
            "menu","tilt"
        };
        for (uint8_t i = 0; i < BTN_COUNT; i++) {
            if (strcmp(btnName, names[i]) == 0) {
                cfg.buttonMap[i] = action;
                break;
            }
        }
        _serial.sendAck("set_button_map", true);

    } else if (strcmp_P(command, PSTR("set_mode")) == 0) {
        const char* mode = cmd[F("mode")] | "hid_pc";
        if      (strcmp_P(mode, PSTR("hid_pc"))  == 0) cfg.mode = 0;
        else if (strcmp_P(mode, PSTR("hid_ps3")) == 0) cfg.mode = 1;
        else if (strcmp_P(mode, PSTR("xinput"))  == 0) cfg.mode = 2;
        _serial.sendAck("set_mode", true, "reboot required");

    } else if (strcmp_P(command, PSTR("save_config")) == 0) {
        _config.save();
        _serial.sendAck("save_config", true);

    } else if (strcmp_P(command, PSTR("factory_reset")) == 0) {
        _config.reset();
        GuitarConfig& newCfg = _config.get();
        _whammy.begin(newCfg.whammyMin, newCfg.whammyMax, newCfg.whammyDeadzone);
        _leds.setEffect(static_cast<LEDEffect>(newCfg.ledEffect));
        _leds.setBaseColours(newCfg.ledColours);
        _serial.sendAck("factory_reset", true);

    } else if (strcmp_P(command, PSTR("reboot")) == 0) {
        _serial.sendAck("reboot", true);
        delay(100);
        // AVR soft reset via watchdog
        wdt_enable(WDTO_15MS);
        while (true) {}

    } else if (strcmp_P(command, PSTR("calibrate_whammy")) == 0) {
        _calibrating    = true;
        _calibrateStart = millis();
        _calibrateMin   = 1023;
        _calibrateMax   = 0;
        _serial.sendAck("calibrate_whammy", true, "move whammy fully for 5s");

    } else if (strcmp_P(command, PSTR("exit_config")) == 0) {
        exitConfigMode();

    } else {
        _serial.sendAck(command, false, "unknown command");
    }
}
