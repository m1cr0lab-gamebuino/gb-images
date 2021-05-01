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
const uint8_t AVATAR_FRAMES = SPRITE_DATA[2];

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

const int8_t AVATAR_SPEED =  2;
const int8_t AVATAR_JUMP  = -5;

const int8_t GRAVITY = 1;

// ----------------------------------------------------------------------------
// Definition of the object-oriented model of the avatar
// ----------------------------------------------------------------------------

struct Avatar {

    int16_t x, y;
    int8_t  vx, vy;
    uint8_t frame;
    int8_t  direction;
    bool    jumping;

    Avatar(int16_t x, int16_t y) : x(x), y(y), vx(0), vy(0), frame(0), direction(1), jumping(false) {}

    void moveToLeft() {
        vx = - AVATAR_SPEED;
        direction = -1;
    }

    void moveToRight() {
        vx = AVATAR_SPEED;
        direction = 1;
    }

    void stop() {
        vx = 0;
        vy = 0;
        frame = 0;
        jumping = false;
    }

    void jump() {
        vy = AVATAR_JUMP;
        jumping = true;
    }

    void update() {

        x += vx;
        y += vy;

        if (jumping) {
            
            frame = 3;
            
        } else if (vx && (gb.frameCount & 0x1)) {
            
            ++frame %= AVATAR_FRAMES;
            
        }

    }

    void draw() {
        Image sprite(SPRITE_DATA);
        sprite.setFrame(frame);
        gb.display.drawImage(x, y, sprite, direction * AVATAR_WIDTH, AVATAR_HEIGHT);
    }

};

// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------

Image torch(TORCH_DATA);
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

void drawTorches() {
    gb.display.drawImage(12, 6, torch);
    gb.display.drawImage(60, 6, torch);
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

        if (!avatar.jumping) avatar.stop();

    }

    if (gb.buttons.pressed(BUTTON_A) && !avatar.jumping) {

        avatar.jump();

    }

}

// ----------------------------------------------------------------------------
// Handling physical constraints of the game scene
// ----------------------------------------------------------------------------

void updateGame() {

    if (avatar.x < 0) {

        avatar.x = 0;

    } else if (avatar.x + AVATAR_WIDTH > SCREEN_WIDTH ) {

        avatar.x = SCREEN_WIDTH - AVATAR_WIDTH;

    }

    if (avatar.jumping) {

        avatar.vy += GRAVITY;

        if (avatar.y + AVATAR_HEIGHT > Y_GROUND) {

            avatar.stop();
            avatar.y = Y_GROUND - AVATAR_HEIGHT;

        }

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
    updateGame();
    
    drawTilemap();
    drawTorches();
    avatar.draw();

}