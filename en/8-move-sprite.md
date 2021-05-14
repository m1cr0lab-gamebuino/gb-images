---
layout: default
lang: en
lang_ref: move-sprite
nav_order: 8
title: How to move your sprite
permalink: /en/how-to-move-your-sprite
date: 2021-05-14 12:00 +0400
---

# How to move your sprite

The game scene is now ready. Let's look at how to add our avatar.

{% include meta-demo.html src="assets/screen/example-11.png" %}

Game scene
{: .caption }

## Definition of a model

First of all we have to neutralize the automatic animation of the sprite:

assets/rgb565.h
{: .filename }

```cpp
const uint16_t SPRITE_DATA[] = {

    // metadata

    8,      // frame width
    8,      // frame height
    4,      // frames
    0,      // frame loop         <--
    0xf81f, // transparent color
    0,      // 16-bits color mode

    // colormap
```

To position the avatar on the game scene, we need to assign it horizontal `x` and vertical `y` coordinates. An efficient way to implement our avatar is to define a very simple object-oriented model as follows:

```cpp
struct Avatar {

    int16_t x, y;

    Avatar(int16_t x, int16_t y) : x(x), y(y) {}

    void draw() {
        Image sprite(SPRITE_DATA);
        gb.display.drawImage(x, y, sprite);
    }

};
```

This model has a constructor that expects the initial coordinates of the sprite on the screen:

```cpp
Avatar(int16_t x, int16_t y) : x(x), y(y) {}
```

The initialization list that follows the declaration of the constructor allows to initialize its internal properties `x` and `y` with the received arguments. This simplified syntactic form is equivalent to the following definition:

```cpp
Avatar(int16_t x, int16_t y) {
    this->x = x;
    this->y = y;
}
```

The `Avatar` model also has a `draw()` method to draw itself on the screen:

```cpp
void draw() {
    Image sprite(SPRITE_DATA);
    gb.display.drawImage(x, y, sprite);
}
```

The avatar should appear to be standing on the base line that represents the floor of the game scene:

![Game scene][game-scene]{: width="500" height="314" }
{: .figure }

Positioning of the avatar on the game scene
{: .caption }

It is an imaginary axis whose equation is defined by :

```cpp
const uint8_t Y_GROUND = SCREEN_HEIGHT - 2*TILE_HEIGHT;
```

In other words, we can initialize the avatar by creating an instance of the `Avatar` model and passing to the constructor its initial coordinates with the following values:

```cpp
Avatar avatar(.5*(SCREEN_WIDTH - AVATAR_WIDTH), Y_GROUND - AVATAR_HEIGHT);
```

Then, from the `loop()` function, we just have to invoke the `draw()` method of the `avatar` instance to display it on the screen.

If we merge these new elements with what we had implemented in the previous chapter, and by slightly restructuring the code, we obtain:

examples/example-12.h
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

// ----------------------------------------------------------------------------
// Definition of the object-oriented model of the avatar
// ----------------------------------------------------------------------------

struct Avatar {

    int16_t x, y;

    Avatar(int16_t x, int16_t y) : x(x), y(y) {}

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
    
    drawTilemap();
    avatar.draw();

}
```

Perfect! The avatar is now immersed in the game scenery:

{% include meta-demo.html src="assets/screen/example-12.png" %}

Avatar is on stage!
{: .caption }

By the way, the sprite does not animate, which is what we wanted.  <i class="far fa-smile"></i>


## Control of the avatar's motion

Let's start with the basics: make sure the avatar can move horizontally.

To do this, we will need to add a new property that characterizes its horizontal velocity `vx`:

```cpp
struct Avatar {

    int16_t x, y;
    int8_t  vx;

    // ...

};
```

We must then provide new methods to set it in motion or stop it, based on the player's actions:

```cpp
void moveToLeft() {
    vx = -2;
}

void moveToRight() {
    vx = 2;
}

void stop() {
    vx = 0;
}
```

These methods allow to modify instantly the horizontal velocity of the avatar, but it is also necessary to provide a method that allows to modify its current position according to its horizontal velocity:

```cpp
void update() {
    x += vx;
}
```

Now that our avatar has the ability to move, we have to intercept the player's commands when he presses the buttons on the console, in order to trigger the avatar to move:

```cpp
void readUserInput() {

    if (gb.buttons.repeat(BUTTON_LEFT, 0)) {

        avatar.moveToLeft();

    } else if (gb.buttons.repeat(BUTTON_RIGHT, 0)) {

        avatar.moveToRight();

    } else if (gb.buttons.released(BUTTON_LEFT) || gb.buttons.released(BUTTON_RIGHT)) {

        avatar.stop();

    }

}
```

Finally, it is necessary to think about modifying the main loop so that the player's actions are taken into account, on the one hand, and that the movement of the avatar is applied when it is set in motion, on the other hand:

```cpp
void loop() {

    gb.waitForUpdate();
    gb.display.clear();

    readUserInput();
    avatar.update();
    
    drawTilemap();
    avatar.draw();

}
```

And that's it:

{% include meta-demo.html src="assets/screen/example-13.gif" %}

Avatar is now moving!
{: .caption }


If you find that the avatar moves too fast (or even too slow), you can of course change the value assigned to the horizontal velocity of the sprite:

```cpp
void moveToLeft() {
    vx = -2;
}

void moveToRight() {
    vx = 2;
}
```

It would be better to set this velocity as a constant at the beginning of the code:

```cpp
const int8_t AVATAR_SPEED = 2;
```

And to take it into account in the methods `moveToLeft()` and `moveToRight()`:

```cpp
void moveToLeft() {
    vx = - AVATAR_SPEED;
}

void moveToRight() {
    vx = AVATAR_SPEED;
}
```


Here is the complete source code:

examples/example-13.h
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
// Hnadling user input
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
```

Well, that's already a pretty good result… But we still have a problem:

{% include meta-demo.html src="assets/screen/example-13.gif" %}

Stay in the field!
{: .caption }

It would be necessary to make sure that the avatar cannot leave the frame of the screen…


## Set the physical constraints of the game scene

You just have to provide an `updateGame()` function that takes care of it:

```cpp
void updateGame() {

    if (avatar.x < 0) {

        avatar.x = 0;

    } else if (avatar.x + AVATAR_WIDTH > SCREEN_WIDTH ) {

        avatar.x = SCREEN_WIDTH - AVATAR_WIDTH;

    }

}
```

Then call this function from the main loop, just after updating the kinematic properties of the avatar:

```cpp
void loop() {

    gb.waitForUpdate();
    gb.display.clear();

    readUserInput();
    avatar.update();
    updateGame();
    
    drawTilemap();
    avatar.draw();

}
```

Last touch up: animate the sprite when the avatar moves…


## How to animate the sprite

This time, we will handle the animation according to the commands imposed by the player. We won't rely on the animations automated by `gb.display`, but on the `setFrame()` method provided by the `Image` class. We have already discussed it in the previous chapter when we had to select the right frame on the tileset.

Let's define a new constant at the beginning of the code that stores the total number of frames that make up the sprite:

```cpp
const uint8_t AVATAR_FRAMES = SPRITE_DATA[2];
```

We also need to evolve our `Avatar` model by adding a `frame` property to store the index of the current frame. And when the time comes to display the sprite on the screen, we will just have to select this frame with the method `setFrame()`.

We also need to take care of the horizontal direction of the sprite, so that we know whether it is oriented to the left or to the right. Remember, we saw that the `drawImage()` method could flip the image horizontally (or vertically). All we had to do was to pass it two additional arguments `w2` and `h2`:

```cpp
void drawImage(int16_t x, int16_t y, Image& img, int16_t w2, int16_t h2);
```

It would be enough to set `w2` with the value `-AVATAR_WIDTH` or `+AVATAR_WIDTH` depending on whether the sprite should be oriented to the left or to the right. We will therefore define a new `direction` property which will determine the horizontal orientation of the sprite.

The changes to be made to the Avatar model are not very important.

First we add the new properties:

```cpp
struct Avatar {

    int16_t x, y;
    int8_t  vx;
    uint8_t frame;
    int8_t  direction;

    // ...

};
```

Then we modify the constructor so that it initializes these properties correctly:

```cpp
Avatar(int16_t x, int16_t y) : x(x), y(y), vx(0), frame(0), direction(1) {}
```

When the avatar moves to the left or to the right, the `direction` property is updated:

```cpp
void moveToLeft() {
    vx = - AVATAR_SPEED;
    direction = -1;
}

void moveToRight() {
    vx = AVATAR_SPEED;
    direction = 1;
}
```

When the avatar stops, the current frame is set to index `0`:

```cpp
void stop() {
    vx = 0;
    frame = 0;
}
```

And at the time of taking into account its motion with the `update()` method, we take advantage of it to calculate the index of the current frame:

```cpp
void update() {

    x += vx;

    if (vx && (gb.frameCount & 0x1)) {
        ++frame %= AVATAR_FRAMES;
    }

}
```

Thus, when its horizontal velocity is non-zero and the number of cycles `gb.frameCount` is odd, i.e. every other time when the `loop()` function is executed, we go to the next frame, looping on the first one when we have reached the last.

Finally, it only remains to select the right frame before displaying the sprite with the right orientation:

```cpp
void draw() {
    Image sprite(SPRITE_DATA);
    sprite.setFrame(frame);
    gb.display.drawImage(x, y, sprite, direction * AVATAR_WIDTH, AVATAR_HEIGHT);
}
```

And here it is! This time we got exactly what we wanted:

{% include meta-demo.html src="assets/screen/example-14.gif" %}

The avatar is now animated when it moves
{: .caption }


Not too lost? We've applied a lot of changes since the last demo. So here is the complete source code that takes into account all the new features we have made so far:

examples/example-14.h
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

const int8_t AVATAR_SPEED = 2;

// ----------------------------------------------------------------------------
// Definition of the object-oriented model of the avatar
// ----------------------------------------------------------------------------

struct Avatar {

    int16_t x, y;
    int8_t  vx;
    uint8_t frame;
    int8_t  direction;

    Avatar(int16_t x, int16_t y) : x(x), y(y), vx(0), frame(0), direction(1) {}

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
        frame = 0;
    }

    void update() {

        x += vx;

        if (vx && (gb.frameCount & 0x1)) {
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
// Hnadling user input
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
// Handling physical constraints of the game scene
// ----------------------------------------------------------------------------

void updateGame() {

    if (avatar.x < 0) {

        avatar.x = 0;

    } else if (avatar.x + AVATAR_WIDTH > SCREEN_WIDTH ) {

        avatar.x = SCREEN_WIDTH - AVATAR_WIDTH;

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
    avatar.draw();

}
```

Now that we can move the avatar horizontally on the game scene, we could make it jump. You'll see that it's not very complicated.


## How to do jumps

This time we need to add to our avatar the ability to move in the vertical dimension:

- under the impulse of a jump
- under the effect of gravity

So let's define two new constants:

```cpp
const int8_t AVATAR_JUMP = -5;
const int8_t GRAVITY     = 1;
```

The vertical axis is oriented downward, in ascending order of ordinates.

Then let's add a vertical component to the velocity of the `Avatar` model, as well as a `jumping` flag which indicates that a jump is in progress:

```cpp
struct Avatar {

    int16_t x, y;
    int8_t  vx, vy;
    uint8_t frame;
    int8_t  direction;
    bool    jumping;

    // ...

};
```

Indeed, the `jumping` flag is useful to constrain some actions of the player like preventing him from stopping his avatar in the middle of a jump!

Let's see what happens with the control management:

```cpp
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
```

We add a new command that allows the player to jump when he presses `BUTTON_A`. But we add a constraint to prevent him from jumping when a jump is already in progress. We also add a constraint when he releases `BUTTON_LEFT` or `BUTTON_RIGHT` to prevent the avatar from stopping in mid-air.

We also need to modify the `updateGame()` function which handles the physical constraints of the game scene. Indeed, we have to add the effect of gravity:

```cpp
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
```

Let's go back to our `Avatar` model and modify the constructor to initialize the new properties `vy` and `jumping` :

```cpp
Avatar(int16_t x, int16_t y)
: x(x)
, y(y)
, vx(0)
, vy(0)
, frame(0)
, direction(1)
, jumping(false)
{}
```

Then let's add the `jump()` method:

```cpp
void jump() {
    vy = AVATAR_JUMP;
    jumping = true;
}
```

We must also modify the `stop()` method to take into account the new properties:

```cpp
void stop() {
    vx = 0;
    vy = 0;
    frame = 0;
    jumping = false;
}
```

Finally, we need to modify the `update()` method to :

- adjust the position of the sprite in both horizontal and vertical dimensions
- freeze the animation of the sprite on the frame of index `3` during a jump

```cpp
void update() {

    x += vx;
    y += vy;

    if (jumping) {
        
        frame = 3;
        
    } else if (vx && (gb.frameCount & 0x1)) {
        
        ++frame %= AVATAR_FRAMES;
        
    }

}
```

And here's the job:

{% include meta-demo.html src="assets/screen/example-15.gif" %}

Jumpin' Jack Flash
{: .caption }

It's starting to look like a real platformer, isn't it?

Here is the complete source code, which includes everything we have done so far:

examples/example-15.h
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

    Avatar(int16_t x, int16_t y) 
    : x(x)
    , y(y)
    , vx(0)
    , vy(0)
    , frame(0)
    , direction(1)
    , jumping(false)
    {}

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
    avatar.draw();

}
```

Now you know all the tricks to animate a sprite easily, and to give life to your avatar in a scenery made of pixels. To finish this tutorial, let's add a final touch in the next chapter.



[game-scene]: {{ site.baseurl }}/assets/figures/game-scene-1000x628.png