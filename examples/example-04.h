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

    uint8_t y = 4; // just below the head
    uint8_t h = AVATAR_HEIGHT - y;

    gb.display.drawImage(
        .5*(SCREEN_WIDTH  - AVATAR_WIDTH),  // x
        .5*(SCREEN_HEIGHT - h),             // y
        avatar,                             // image
        0,                                  // x2 = x
        y,                                  // y2 = y+4
        AVATAR_WIDTH,                       // w2 = w
        h                                   // h2 = h-4
    );

}