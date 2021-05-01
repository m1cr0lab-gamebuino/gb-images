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
const uint8_t NB_TINTS      = 6;

const uint8_t AVATAR_WIDTH  = SPRITE_DATA[0];
const uint8_t AVATAR_HEIGHT = SPRITE_DATA[1];

Color tint[] = { PINK, LIGHTBLUE, LIGHTGREEN, YELLOW, ORANGE, RED };
Color palette[16];
Image avatar(SPRITE_DATA);

void updatePalette(Color color) {

    uint16_t c = (uint16_t)color;

    uint8_t r1 = .8f * (c >> 11);
    uint8_t g1 = .8f * ((c >> 5) & 0x3f);
    uint8_t b1 = .8f * (c & 0x1f);

    uint8_t r2 = .8f * r1;
    uint8_t g2 = .8f * g1;
    uint8_t b2 = .8f * b1;

    palette[0xb] = (Color)((r2 << 11) | (g2 << 5) | b2);
    palette[0xc] = (Color)((r1 << 11) | (g1 << 5) | b1);
    palette[0xd] = color;

}

void setup() {
    gb.begin();
    gb.setFrameRate(32);
    memcpy(palette, PALETTE, 16*sizeof(Color));
}

void loop() {

    gb.waitForUpdate();
    gb.display.clear();

    gb.display.setPalette(palette);

    for (uint8_t i=0; i<NB_TINTS; ++i) {
        updatePalette(tint[i]);
        gb.display.drawImage(
            (i+1)*SCREEN_WIDTH/(NB_TINTS+1) - .5*AVATAR_WIDTH,
            .5*(SCREEN_HEIGHT - AVATAR_HEIGHT),
            avatar
        );
    }
    
}