/**
 * ----------------------------------------------------------------------------
 * Handling images on the Gamebuino META
 * © 2021 Stéphane Calderoni
 * ----------------------------------------------------------------------------
 */

#include <Gamebuino-Meta.h>

Image image("gamebuino.bmp");

void setup() {
    gb.begin();
    gb.display.drawImage(0, 0, image);
}

void loop() {
    gb.waitForUpdate();
}