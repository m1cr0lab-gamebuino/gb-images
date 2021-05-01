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

    uint8_t aw = 3*AVATAR_WIDTH;
    uint8_t ah = 3*AVATAR_HEIGHT;
    
    gb.display.drawImage(
        .5*(SCREEN_WIDTH  - aw), // x
        .5*(SCREEN_HEIGHT - ah), // y
        avatar,                  // image
        aw,                      // x-stretched
        ah                       // y-stretched
    );

}