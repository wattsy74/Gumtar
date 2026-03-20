#include <Arduino.h>
#include "GuitarController.h"

GuitarController gController;

void setup() {
    gController.begin();
}

void loop() {
    gController.update();
}
