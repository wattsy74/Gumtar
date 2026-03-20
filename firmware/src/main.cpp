#include <Arduino.h>
#include <avr/wdt.h>
#include "GuitarController.h"

GuitarController gController;

void setup() {
    wdt_disable();
    gController.begin();
}

void loop() {
    gController.update();
}
