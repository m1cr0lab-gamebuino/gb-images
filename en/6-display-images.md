---
layout: default
lang: en
lang_ref: display-images
nav_order: 6
title: How to display your images
permalink: /en/how-to-dispay-your-images
date: 2021-05-14 10:15 +0400
---

# How to display your images on the META


## Organization of source codes

To avoid having to create many Arduino mini-projects, we will organize our code by isolating each demonstration example in an `examples` folder located at the root of the project, by an `example-##.h` file that we will just have to include into the main `my-stunning-game.ino` sketch. For example, let's start with an `example-00.h` file:

![Project folder][project-example]{: width="316" height="222" .shadow }
{: .figure }

The new content of your project folder
{: .caption }

Don't forget to create an empty file `examples.ino` to be able to open the files contained in the `examples` folder with the code editor of the Arduino IDE.

The main sketch is therefore reduced to the following single precompilation directive:

my-stunning-game.ino
{: .filename }

```cpp
#include "examples/example-00.h"
```

And the source code must now be written in the `example-00.h` file:

examples/example-00.h
{: .filename }

```cpp
#include <Gamebuino-Meta.h>

void setup() {
    gb.begin();
    gb.setFrameRate(32);
}

void loop() {
    gb.waitForUpdate();
}
```

With each new source code `example-01.h`, `example-02.h`, etc., you will only need to update the main sketch with the correct `#include` precompilation directive.


## The `drawImage()` method

The `gb.display` API exposes specialized methods that make it easy to display images on the META screen. The term *method* is used here because it refers to functions that are defined within an object-oriented model (the `Image` class). If you are not familiar with the nuance, it doesn't matter, methods are primarily functions in the semantics of the C++ language. Among these methods, one of them will be of particular interest to us here. It is the `drawImage()` method, which exposes 3 different signatures:

```cpp
void drawImage(int16_t x, int16_t y, Image& img);
void drawImage(int16_t x, int16_t y, Image& img, int16_t w2, int16_t h2);
void drawImage(int16_t x, int16_t y, Image& img, int16_t x2, int16_t y2, int16_t w2, int16_t h2);
```

You can see that they all accept a parameter which is a reference to an instance of the `Image` class. And we saw earlier how to create such an instance by providing the constructor of the `Image` class with an array of data of type `uint16_t` or `uint8_t` depending on the configured display mode.

For example, with the default display mode :

config-gamebuino.h
{: .filename }

```cpp
#define DISPLAY_MODE DISPLAY_MODE_RGB565
```

This is how to display our animated sprite on the screen using the first signature:

examples/example-01.h
{: .filename }

```cpp
#include <Gamebuino-Meta.h>
#include "../assets/rgb565.h"

Image avatar(SPRITE_DATA);

void setup() {
    gb.begin();
    gb.setFrameRate(32);
}

void loop() {
    gb.waitForUpdate();
    gb.display.clear();
    gb.display.drawImage(36, 28, avatar);
}
```

Here, the assets file is included by following the relative path from the location of the `example-01.h` file to the location of the `rgb565.h` file:

```cpp
#include "../assets/rgb565.h"
```

We then create an instance of the class `Image`, named `avatar`, by passing to the constructor the `SPRITE_DATA` array, which defines all the frames of our sprite:

```cpp
Image avatar(SPRITE_DATA);
```

Then we display the sprite in the center of the screen:

```cpp
gb.display.drawImage(36, 28, avatar);
```

Why at coordinates (36,28)?

![Sprite centering][avatar-centering]{: width="437" height="311" }
{: .figure }

In general, it is not recommended to express coordinates in raw form when the positioning is the result of a calculation related to the screen size. It is better to write things like this:

examples/example-01.h
{: .filename }

```cpp
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
```

Remember that:

- `SPRITE_DATA[0]` is the `frame_width` of the sprite
- `SPRITE_DATA[1]` is the `frame_height` of the sprite

Their values are expressed with the `uint16_t` type because it is the common type for all data stored in the `SPRITE_DATA` array, but they can also be stored in constants of type `uint8_t` since they are here lower than 256:

```cpp
const uint8_t AVATAR_WIDTH  = SPRITE_DATA[0];
const uint8_t AVATAR_HEIGHT = SPRITE_DATA[1];
```

These constants are absolutely not necessary here, but they make the code easier to read.

Remember also that we set `frame_loop` to `4` in `SPRITE_DATA`, which means that the animation is automatically set to **8 fps** here:

{% include meta-demo.html src="assets/screen/example-01.gif" %}

Magic, right?
{: .caption }


Now let's look at the second signature of the `drawImage()` method:

```cpp
void drawImage(int16_t x, int16_t y, Image& img, int16_t w2, int16_t h2);
```

It introduces two new arguments that allow to stretch the original image on a rectangle of width `w2` and height `h2`. In other words, this version allows you to enlarge or shrink the image according to its horizontal and/or vertical dimensions. For example, if you want to enlarge the image uniformly by a factor of 3, simply multiply its original width and height by 3:

examples/example-02.h
{: .filename }

```cpp
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
```

{% include meta-demo.html src="assets/screen/example-02.gif" %}

Enlargement by a factor of 3
{: .caption }

It is quite possible to apply different stretching factors to the width and height of the image. But in this case, the image will necessarily be distorted, since the ratio width / height of the original image will not be respected. On the other hand, if you look closely at the signature of the `drawImage()` method, you can see that the `w2` and `h2` arguments can be **negative**. And this is very interesting, because it means that we can not only stretch the image but also **reverse** it. For example, if we multiply the initial width of the image by the `-1` factor, we get a horizontal reversal:

examples/example-03.h
{: .filename }

```cpp
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
        avatar,                             // image
      - AVATAR_WIDTH,                       // reversed horizontally
        AVATAR_HEIGHT                       // only
    );

}
```

{% include meta-demo.html src="assets/screen/example-03.gif" %}

Horizontal reversal:
{: .caption }

You can imagine that this subtlety will be very useful to handle the movement of the avatar. We won't need to modify the sprite to add images that move to the left. It will be enough to apply a horizontal reversal, and the trick will be done.  <i class="far fa-smile-wink"></i>

We still need to examine the last signature of the `drawImage()` method:

```cpp
void drawImage(int16_t x, int16_t y, Image& img, int16_t x2, int16_t y2, int16_t w2, int16_t h2);
```

This one will allow us to crop the display area of the image by specifying the coordinates of the upper left corner and the dimensions of the cropping rectangle. For example, let's imagine that we want to erase the head of our avatar:

examples/example-04.h
{: .filename }

```cpp
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
```

{% include meta-demo.html src="assets/screen/example-04.gif" %}

Crop the area to be displayed
{: .caption }

You can see that the possibilities are multiple with these 3 variants of the `drawImage()` method.


## How it works with indexed display modes

Now let's switch to an indexed color display mode:

config-gamebuino.h
{: .filename }

```cpp
#define DISPLAY_MODE DISPLAY_MODE_INDEX_HALFRES
```

And let's see what happens when we display our sprite:

examples/example-05.h
{: .filename }

```cpp
#include <Gamebuino-Meta.h>
#include "../assets/indexed.h"

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
```

{% include meta-demo.html src="assets/screen/example-05.gif" %}

<i class="far fa-surprise"></i>  Oops
{: .caption }

<i class="far fa-frown-open"></i>  However, we have integrated the right asset file:

```cpp
#include "../assets/indexed.h"
```

Yes, but?… We must have forgotten something…

Remember, by default the **Gamebuino palette** is applied to the indexed color display modes. So don't forget to apply **your** color palette to the `gb.display` image during initialization:

```cpp
gb.display.setPalette(PALETTE);
```

Okay, let's put this into action right now:

examples/example-05.h
{: .filename }

```cpp
#include <Gamebuino-Meta.h>
#include "../assets/indexed.h"

const uint8_t SCREEN_WIDTH  = 80;
const uint8_t SCREEN_HEIGHT = 64;

const uint8_t AVATAR_WIDTH  = SPRITE_DATA[0];
const uint8_t AVATAR_HEIGHT = SPRITE_DATA[1];

Image avatar(SPRITE_DATA);

void setup() {
    gb.begin();
    gb.setFrameRate(32);
    gb.display.setPalette(PALETTE);
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
```

This time, everything is fine:

{% include meta-demo.html src="assets/screen/example-02.gif" %}{% comment %} <!-- example-06 is same as 02 --> {% endcomment %}

Don't forget to set the right palette
{: .caption }

Don't forget that the color palette applied to the `gb.display` image is global. Therefore all the images you display with the `drawImage()` method will necessarily share this unique palette. However, it is possible to modify this palette with the `setPalette()` method at any time, but the repercussions will be propagated to all the images displayed.

It is quite different in the case of the RGB565 display mode. And I propose to examine some of the tricks that come with it.


## How to play with palettes in RGB565

If the color palette is common to all assets in the case of indexed color display modes, it is possible to dissociate them for each asset, and to handle them independently of the others, in the case of `DISPLAY_MODE_RGB565`. Let's go back to this mode:

config-gamebuino.h
{: .filename }

```cpp
#define DISPLAY_MODE DISPLAY_MODE_RGB565
```

But let's continue to use the assets as they have been defined for the indexed color display modes by the `assets/indexed.h` file.

The graphic buffer of `gb.display` can store any color in the RGB565 space, but it is possible to transfer an image in indexed colors with the `drawImage()` method. To do this, simply apply the desired palette just before transferring.

Let's suppose we want to change the skin color of our avatar. We could apply the following changes to the sprite's palette:

- color index `0x6` : {% include square.html color="#e8c47d" %} `0xEE2F` &rArr; {% include square.html color="#7a6552" %} `0x7B2A`
- color index `0x7` : {% include square.html color="#ffe6b3" %} `0xFF36` &rArr; {% include square.html color="#b8977a" %} `0xBCAF`

examples/example-06.h
{: .filename }

```cpp
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
    palette[0x6] = (Color)0x7b2a;
    palette[0x7] = (Color)0xbcaf;

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
```

{% include meta-demo.html src="assets/screen/example-06.gif" %}

Dynamic modification of the sprite palette
{: .caption }

During initialization we copy the original `PALETTE` of the sprite into a palette array that we can modify during the execution of the program:

```cpp
memcpy(palette, PALETTE, 16*sizeof(Color));
```

Then, in the `loop()` function, just before displaying the sprite with `drawImage()`, we assign the dynamic palette to `gb.display`, which we can then modify freely:

```cpp
gb.display.setPalette(palette);
palette[0x6] = (Color)0x7b2a;
palette[0x7] = (Color)0xbcaf;
```


You can see that you can do some pretty funny things very easily, starting from the same colormap, but applying changes to the palette just before transferring the image to `gb.display`.

For example, we might want to dress the sprite with different colors:

examples/example-07.h
{: .filename }

```cpp
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
```

{% include meta-demo.html src="assets/screen/example-07.gif" %}

Dynamic sprite skinning
{: .caption }

The principle is always the same: assign the dynamic palette to `gb.display`, then modify it before transferring the sprite with `drawImage()`.

You can even assign a new color to the palette, which changes with each display cycle. For example, to make the sprite look like a cyborg, we can do the following:

examples/example-08.h
{: .filename }

```cpp
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
    palette[0xf] = (Color)((uint8_t)(0x1f*(1+sin(4*PI*(gb.frameCount & 0x3f)/0x3f))) << 5);

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
```

{% include meta-demo.html src="assets/screen/example-08.gif" %}

Periodic variation of a color in the palette
{: .caption }

One could also apply cyclic permutations on the color palette. This is exactly the technique used by talented artists like [Mark Ferrari][mark-ferrari] to animate [beautiful pixel-art scenes][game-effects], using only cycles of colors defined on a palette.

![Stunning Pixel Art color cycling animation][pixel-art]{: width="320" height="240" .shadow }
{: .figure }

Stunning Pixel Art color cycling animation
{: .caption }

The possibilities are huge, even on a small screen like the META. Now you have the keys in hand… Trust your imagination.

We have just reviewed the different ways to display our sprite on the screen with the `drawImage()` method. We didn't worry at all about handling the animation, since it is implemented automatically thanks to the `frame_loop` property that we have defined in the `SPRITE_DATA` array. Until now, it had the value of `4`, which translated into an animation at the frequency of **8 fps**.

If you want to double this frequency to **16 fps** and give the impression that our avatar is running, simply divide `frame_loop` by `2`.

Let's go back to RGB565 mode:

config-gamebuino.h
{: .filename }

```cpp
#define DISPLAY_MODE DISPLAY_MODE_RGB565
```

Then let's change the animation frequency of the sprite:

assets/rgb565.h
{: .filename }

```cpp
#pragma once

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

And let's see what we get with that:

examples/example-09.h
{: .filename }

```cpp
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
```

{% include meta-demo.html src="assets/screen/example-09.gif" %}

Doubled animation frequency
{: .caption }


This feature is quite useful for cyclic animations where you don't need any control. Just let `gb.display` handle the animation automatically at the chosen frequency. This can be useful for example for assets like a torch whose flame would flicker over time. We wouldn't have to worry explicitly about its animation in the main control loop of the game. `gb.display` would handle it automatically for us.

But in the case of our avatar, it is the user's interactions with the console buttons that should govern the sprite animations. We must therefore have total control over these animations. This is of course possible and we will see it later. But before that, I suggest that we examine how to set up the scenery of the game using the images in our tileset. See you in the next chapter for that.



[project-example]:  {{ site.baseurl }}/assets/figures/project-examples-632x524.png
[avatar-centering]: {{ site.baseurl }}/assets/figures/avatar-screen-centering-874x622.png
[pixel-art]:        {{ site.baseurl }}/assets/figures/seascape-640x480.gif
[mark-ferrari]:     https://www.markferrari.com/
[game-effects]:     http://www.effectgames.com/demos/canvascycle/