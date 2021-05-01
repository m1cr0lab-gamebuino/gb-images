/**
 * ----------------------------------------------------------------------------
 * Handling images on the Gamebuino META
 * © 2021 Stéphane Calderoni
 * ----------------------------------------------------------------------------
 */

#include <Gamebuino-Meta.h>
#include "../assets/rgb565.h"

void setup() {
    gb.begin();
    gb.display.init("splash.bmp");
}

void loop() {
    
    gb.waitForUpdate();
    gb.display.nextFrame();
    
    gb.display.print(8, 16, "My Stunning Game");
    gb.display.drawImage(36, 40, SPRITE_DATA);
    
}