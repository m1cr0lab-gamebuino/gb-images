/**
 * ----------------------------------------------------------------------------
 * Handling images on the Gamebuino META
 * © 2021 Stéphane Calderoni
 * ----------------------------------------------------------------------------
 */

#include <Gamebuino-Meta.h>

const char *FORMAT = "FREE RAM: %u";

void setup() {
    gb.begin();
}

void loop() {
    gb.waitForUpdate();
    gb.display.clear();
    gb.display.setFontSize(1);
    gb.display.printf(2, 2, FORMAT, gb.getFreeRam());
}