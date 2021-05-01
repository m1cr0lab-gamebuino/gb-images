/**
 * ----------------------------------------------------------------------------
 * Handling images on the Gamebuino META
 * © 2021 Stéphane Calderoni
 * ----------------------------------------------------------------------------
 */

#include <Gamebuino-Meta.h>
#include "../assets/rgb565.h"

const uint8_t SCREEN_WIDTH  = 80;
const uint8_t SCREEN_HEIGHT = 64;

const uint8_t AVATAR_WIDTH  = SPRITE_DATA[0];
const uint8_t AVATAR_HEIGHT = SPRITE_DATA[1];

Image avatar(SPRITE_DATA);

void setup() {
    gb.begin();
    gb.setFrameRate(32);
}

void loop() {
    gb.waitForUpdate();
    gb.display.clear();
    gb.display.drawImage(
        .5*(SCREEN_WIDTH  - AVATAR_WIDTH),  // x
        .5*(SCREEN_HEIGHT - AVATAR_HEIGHT), // y
        avatar                              // image
    );
}