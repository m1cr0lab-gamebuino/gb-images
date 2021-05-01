---
layout: default
lang: fr
lang_ref: finishing-touch
nav_order: 9
title: La touche finale
permalink: /fr/la-touche-finale
---

# La touche finale

Maintenant que tu sais comment animer un sprite, image par image, en contrôlant l'enchaînement des frames qui le composent, ne perds pas de vue que `gb.display` peut aussi prendre en charge l'animation de certains élements de la scène de jeu de façon automatique. Les deux approches peuvent tout à fait coexister simultanément. Pour illustrer cela, je te propose que nous ajoutions deux torches animées au décor pour le rendre un peu plus vivant.


## Préparation des éléments graphiques

Voici la spritesheet qu'on utilisera pour chaque torche :

![Spritesheet d'une torche][torch-spritesheet]{: width="320" height="256" .shadow }
{: .figure }

Spritesheet d'une torche
{: .caption }

Je te propose de la télécharger et de la ranger dans ton dossier `artwork` avec les autres assets :

[Torch spritesheet][torch]{: .btn .btn-primary download="" }
{: .text-center }

Ensuite, dépose-la sur **Image Transcoder**, que j'ai préparamétré ci-dessous, pour la convertir en code C++, et colle le code obtenu à la suite du fichier `assets/rgb565.h` en nommant le tableau de données `TORCH_DATA`.

<iframe src="https://gamebuino.m1cr0lab.com/tools/img2code/?fw=8&fh=16&as=2&tc=0xff00ff" height="800"></iframe>

Sinon télécharge carrément la nouvelle version du fichier à ranger dans ton dossier `assets` :

[rgb565.h][assets-rgb565]{: .btn .btn-purple download="rgb565.h" style="margin-right:.5em;" }
{: .text-center }


## Animation des torches

On va avoir besoin d'une nouvelle variable globale `torch`, instance de la classe `Image` construite sur la base du nouveau tableau de données `TORCH_DATA`, que tu peux définir juste avant celle de l'avatar :

```cpp
Image torch(TORCH_DATA);
Avatar avatar(.5*(SCREEN_WIDTH - AVATAR_WIDTH), Y_GROUND - AVATAR_HEIGHT);
```

Ensuite on va définir une fonction `drawTorches()` chargée d'afficher les deux torches sur la scène de jeu, à des emplacements bien choisis :

```cpp
void drawTorches() {
    gb.display.drawImage(12, 6, torch);
    gb.display.drawImage(60, 6, torch);
}
```

Nul besoin d'en faire plus, `gb.display` se chargera de leur animation toute seule, puisque la propriété `frame_loop` est fixée à `2` dans le tableau `TORCH_DATA` :

assets/rgb565.h
{: .filename }

```cpp
const uint16_t TORCH_DATA[] = {
    8,      // frame width
    16,     // frame height
    10,     // number of frames
    2,      // frame loop
    0xf81f, // transparent color
    0,      // RGB565 color mode
    // colormap
    // ...
```

Il ne nous reste plus qu'à invoquer la fonction `drawTorches()`, dans la boucle principale, pour afficher les torches sur la scène de jeu :

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

Et voilà ! Ça ne demandait pas de gros efforts, mais ça donne une toute autre ambiance au décor :

{% include meta-demo.html src="assets/screen/example-16.gif" %}

La scène finale
{: .caption }

On va s'arrêter là-dessus. Voici le code complet de cette petite touche finale :

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