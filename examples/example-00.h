/**
 * ----------------------------------------------------------------------------
 * Handling images on the Gamebuino META
 * © 2021 Stéphane Calderoni
 * ----------------------------------------------------------------------------
 */

#include <Gamebuino-Meta.h>

void setup() {
    gb.begin();
    gb.setFrameRate(32);
}

void loop() {
    gb.waitForUpdate();
}