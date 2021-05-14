---
layout: default
lang: en
lang_ref: finishing-touch
nav_order: 9
title: Finishing touch
permalink: /en/finishing-touch
date: 2021-05-14 12:10 +0400
---

# The finishing touch

Now that you know how to animate a sprite, frame by frame, by controlling the sequence of frames that compose it, don't forget that `gb.display` can also take care of the animation of some elements of the game scene automatically. The two approaches can coexist simultaneously. To illustrate this, I suggest that we add two animated torches to the scenery to make it a bit more lively.


## Preparation of graphic assets

Here is the spritesheet we will use for each torch:

![Torch spritesheet][torch-spritesheet]{: width="320" height="256" .shadow }
{: .figure }

Torch spritesheet
{: .caption }

I suggest you download it and put it in your `artwork` folder with the other assets:

[Torch spritesheet][torch]{: .btn .btn-primary download="" }
{: .text-center }

Then drop it on **Image Transcoder** to convert it to C++ code, and paste the resulting code after the `assets/rgb565.h` file, naming the data array `TORCH_DATA`. Set the `frame_loop = 2` property to automate the torch animation.

<iframe src="https://gamebuino.m1cr0lab.com/tools/img2code/?fw=8&fh=16&as=2&tc=0xff00ff" height="800"></iframe>

If you're lazy, just download the new version of the file and put it in your `assets` folder:

[rgb565.h][assets-rgb565]{: .btn .btn-purple download="rgb565.h" style="margin-right:.5em;" }
{: .text-center }


## Torch animation

We'll need a new global variable `torch`, an instance of the `Image` class built on the basis of the new `TORCH_DATA` array, which you can define just before the one for the avatar:

```cpp
Image torch(TORCH_DATA);
Avatar avatar(.5*(SCREEN_WIDTH - AVATAR_WIDTH), Y_GROUND - AVATAR_HEIGHT);
```

Then we will define a function `drawTorches()` in charge of displaying the two torches on the game scene, at the chosen locations:

```cpp
void drawTorches() {
    gb.display.drawImage(12, 6, torch);
    gb.display.drawImage(60, 6, torch);
}
```

No need to do more, `gb.display` will take care of their animation by itself, since the `frame_loop` property is set to `2` in the `TORCH_DATA` array:

assets/rgb565.h
{: .filename }

```cpp
const uint16_t TORCH_DATA[] = {

    // metadata

    8,      // frame width
    16,     // frame height
    10,     // frames
    2,      // frame loop
    0xf81f, // transparent color
    0,      // 16-bits color mode

    // colormap
```

Now we just have to invoke the `drawTorches()` function, in the main loop, to display the torches on the game scene:

```cpp
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
```

And that's it! It didn't take much effort, but it gives a whole new vibe to the scenery:

{% include meta-demo.html src="assets/screen/example-16.gif" %}

The final stage
{: .caption }

Here's the full code for this nice finishing touch:

examples/example-16.h
{: .filename }

```cpp
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
// Hnadling user input
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
```




[torch-spritesheet]: {{ site.baseurl }}/assets/figures/torch-spritesheet-640x512.png
[torch]:             {{ site.baseurl }}/assets/download/torch-5x2.png
[assets-rgb565]:     {{ site.baseurl }}/assets/download/rgb565-final.h