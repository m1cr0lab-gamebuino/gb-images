/**
 * ----------------------------------------------------------------------------
 * Handling images on the Gamebuino META
 * © 2021 Stéphane Calderoni
 * ----------------------------------------------------------------------------
 */

#include <Gamebuino-Meta.h>
#include "../assets/rgb565.h"

// ----------------------------------------------------------------------------
// Global constants
// ----------------------------------------------------------------------------

const uint8_t SCREEN_WIDTH  = 80;
const uint8_t SCREEN_HEIGHT = 64;

const uint8_t AVATAR_WIDTH  = SPRITE_DATA[0];
const uint8_t AVATAR_HEIGHT = SPRITE_DATA[1];

const uint8_t TILE_WIDTH  = TILESET_DATA[0];
const uint8_t TILE_HEIGHT = TILESET_DATA[1];

const uint8_t TILES_WIDE = SCREEN_WIDTH  / TILE_WIDTH;
const uint8_t TILES_HIGH = SCREEN_HEIGHT / TILE_HEIGHT;

const uint8_t Y_GROUND = SCREEN_HEIGHT - 2*TILE_HEIGHT;

const uint8_t TILEMAP[] = {
    0, 0, 0, 0, 0,
    2, 2, 2, 2, 2,
    0, 0, 0, 0, 0,
    2, 2, 2, 2, 2,
    0, 0, 0, 0, 0,
    2, 2, 2, 2, 2,
    1, 1, 1, 1, 1,
    3, 3, 3, 3, 3
};

const int8_t AVATAR_SPEED = 2;

// ----------------------------------------------------------------------------
// Definition of the object-oriented model of the avatar
// ----------------------------------------------------------------------------

struct Avatar {

    int16_t x, y;
    int8_t  vx;

    Avatar(int16_t x, int16_t y) : x(x), y(y), vx(0) {}

    void moveToLeft() {
        vx = - AVATAR_SPEED;
    }

    void moveToRight() {
        vx = AVATAR_SPEED;
    }

    void stop() {
        vx = 0;
    }

    void update() {
        x += vx;
    }

    void draw() {
        Image sprite(SPRITE_DATA);
        gb.display.drawImage(x, y, sprite);
    }

};

// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------

Avatar avatar(.5*(SCREEN_WIDTH - AVATAR_WIDTH), Y_GROUND - AVATAR_HEIGHT);

// ----------------------------------------------------------------------------
// Graphics rendering
// ----------------------------------------------------------------------------

void drawTilemap() {

    Image tileset(TILESET_DATA);

    for (uint8_t j=0; j<TILES_HIGH; ++j) {
        for (uint8_t i=0; i<TILES_WIDE; ++i) {

            tileset.setFrame(TILEMAP[i + j * TILES_WIDE]);

            gb.display.drawImage(
                i*TILE_WIDTH,  // x
                j*TILE_HEIGHT, // y
                tileset        // image
            );

        }
    }

}

// ----------------------------------------------------------------------------
// Handling user input
// ----------------------------------------------------------------------------

void readUserInput() {

    if (gb.buttons.repeat(BUTTON_LEFT, 0)) {

        avatar.moveToLeft();

    } else if (gb.buttons.repeat(BUTTON_RIGHT, 0)) {

        avatar.moveToRight();

    } else if (gb.buttons.released(BUTTON_LEFT) || gb.buttons.released(BUTTON_RIGHT)) {

        avatar.stop();

    }

}

// ----------------------------------------------------------------------------
// Initialization
// ----------------------------------------------------------------------------

void setup() {

    gb.begin();
    gb.setFrameRate(32);

}

// ----------------------------------------------------------------------------
// Main control loop
// ----------------------------------------------------------------------------

void loop() {

    gb.waitForUpdate();
    gb.display.clear();

    readUserInput();
    avatar.update();
    
    drawTilemap();
    avatar.draw();

}