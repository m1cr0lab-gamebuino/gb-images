/**
 * ----------------------------------------------------------------------------
 * Handling images on the Gamebuino META
 * © 2021 Stéphane Calderoni
 * ----------------------------------------------------------------------------
 */

#include <Gamebuino-Meta.h>
#include "../assets/indexed.h"

const uint8_t SCREEN_WIDTH  = 80;
const uint8_t SCREEN_HEIGHT = 64;

const uint8_t AVATAR_WIDTH  = SPRITE_DATA[0];
const uint8_t AVATAR_HEIGHT = SPRITE_DATA[1];

Color palette[16];
Image avatar(SPRITE_DATA);

void setup() {
    gb.begin();
    gb.setFrameRate(32);
    memcpy(palette, PALETTE, 16*sizeof(Color));
}

void loop() {

    gb.waitForUpdate();
    gb.display.clear();

    gb.display.setPalette(palette);
    palette[0x0] = (Color)((uint8_t)(0x1f*(1+sin(4*PI*(gb.frameCount & 0x3f)/0x3f))) << 5);

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