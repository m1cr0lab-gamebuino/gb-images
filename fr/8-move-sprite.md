---
layout: default
lang: fr
lang_ref: move-sprite
nav_order: 8
title: Déplacer un sprite
permalink: /fr/deplacer-un-sprite
---

# Comment déplacer ton avatar

La scène de jeu est désormais plantée. Nous allons examiner comment y ajouter notre avatar.

{% include meta-demo.html src="assets/screen/example-11.png" %}

Scène de jeu
{: .caption }

## Définition d'un modèle

Nous devons avant tout neutraliser l'animation automatique du sprite :

assets/rgb565.h
{: .filename }

```cpp
const uint16_t SPRITE_DATA[] = {
	8,      // frame width
	8,      // frame height
	4,      // number of frames
	0,      // frame loop         <--
	0xf81f, // transparent color
	0,      // RGB565 color mode
	// colormap
    // ...
```

Pour placer notre avatar sur la scène de jeu, on doit lui associer des coordonnées horizontale `x` et verticale `y`. Une façon efficace d'implémenter notre avatar consiste à définir un modèle orienté objet très simple de la façon suivante :

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

Ce modèle est pourvu d'un constructeur qui attend les coordonnées initiales du sprite à l'écran :

```cpp
Avatar(int16_t x, int16_t y) : x(x), y(y) {}
```

La liste d'initialisation qui suit la déclaration du constructeur permet d'initialiser ses propriétés internes `x` et `y` avec les arguments reçus. Cette forme syntaxique simplifiée est équivalente à la définition suivante :

```cpp
Avatar(int16_t x, int16_t y) {
    this->x = x;
    this->y = y;
}
```

Le modèle `Avatar` possède également une *méthode* (une fonction) lui permettant de se dessiner à l'écran :

```cpp
void draw() {
    Image sprite(SPRITE_DATA);
    gb.display.drawImage(x, y, sprite);
}
```

L'avatar doit sembler reposer sur la ligne de base qui représente le plancher de la scène de jeu :

![Game scene][game-scene]{: width="500" height="314" }
{: .figure }

Placement de l'avatar sur la scène de jeu
{: .caption }

Il s'agit d'un axe imaginaire dont l'équation est définie par :

```cpp
const uint8_t Y_GROUND = SCREEN_HEIGHT - 2*TILE_HEIGHT;
```

Autrement dit, on peut initialiser notre avatar en créant une instance du modèle `Avatar` et en passant au constructeur ses coordonnées initiales avec les valeurs suivantes :

```cpp
Avatar avatar(.5*(SCREEN_WIDTH - AVATAR_WIDTH), Y_GROUND - AVATAR_HEIGHT);
```

Il suffira ensuite, à partir de la boucle principale, d'invoquer la méthode `draw()` de l'instance `avatar` pour provoquer son affichage à l'éran.

Si on fusionne ces nouveaux éléments avec ce que nous avions implémenté au chapitre précédent, et en restructurant légèrement le code, nous obtenons :

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

Parfait ! L'avatar est désormais plongé dans le décor du jeu :

{% include meta-demo.html src="assets/screen/example-12.png" %}

L'avatar est sur scène !
{: .caption }

Au passage, le sprite ne s'anime pas, et c'est bien ce qu'on voulait.  <i class="far fa-smile"></i>


## Contrôle du mouvement de l'avatar

Commençons par la base : faire en sorte que l'avatar puisse se déplacer horizontalement.

Pour cela, nous allons avoir besoin de lui ajouter une nouvelle propriété qui caractérise sa vitesse horizontale `vx` :

```cpp
struct Avatar {

    int16_t x, y;
    int8_t  vx;

    // ...

};
```

Il faut ensuite prévoir de nouvelles méthodes qui permettent de le mettre en mouvement ou de l'arrêter, à partir des actions du joueur :

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

Ces méthodes permettent de modifier instantanément la vitesse horizontale de l'avatar, mais il faut également prévoir une méthode qui permette de l'appliquer à sa position courante, de manière à ce qu'il se déplace et change donc de position en fonction de sa vitesse horizontale :

```cpp
void update() {
    x += vx;
}
```

Maintenant que notre avatar a la capacité de se déplacer, il nous reste à intercepter les commandes du joueur lorsqu'il appuie sur les boutons de la console, de façon à déclencher la mise en mouvement de l'avatar :

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

Pour finir, il faut penser à modifier la boucle principale pour que les actions du joueur soient bien prises en compte, d'une part, et que le déplacement de l'avatar soit bien appliqué lorsqu'il est mis en mouvement, d'autre part :

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

Et le tour est joué :

{% include meta-demo.html src="assets/screen/example-13.gif" %}

L'avatar peut maintenant bouger
{: .caption }


Si tu trouves que l'avatar se déplace trop rapidement (ou même trop lentement), tu peux évidemment modifier la valeur affectée à la vitesse horizontale du sprite :

```cpp
void moveToLeft() {
    vx = -2;
}

void moveToRight() {
    vx = 2;
}
```

Il serait d'ailleurs plus judicieux de paramétrer cette vitesse comme une constante en début de code :

```cpp
const int8_t AVATAR_SPEED = 2;
```

Et de la prendre en compte dans les méthodes `moveToLeft()` et `moveToRight()` :

```cpp
void moveToLeft() {
    vx = - AVATAR_SPEED;
}

void moveToRight() {
    vx = AVATAR_SPEED;
}
```


Voilà le code source complet :

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

Bon, c'est déjà un résultat assez satisfaisant… Mais on a quand même un problème :

{% include meta-demo.html src="assets/screen/example-13.gif" %}

Reste dans le champ !
{: .caption }

Il faudrait pouvoir faire en sorte que l'avatar ne puisse pas sortir du cadre de l'écran…


## Imposer les contraintes physiques de la scène de jeu

Rien de plus simple, il suffit de prévoir une fonction `updateGame()` qui s'en charge :

```cpp
void updateGame() {

    if (avatar.x < 0) {

        avatar.x = 0;

    } else if (avatar.x + AVATAR_WIDTH > SCREEN_WIDTH ) {

        avatar.x = SCREEN_WIDTH - AVATAR_WIDTH;

    }

}
```

Puis d'appeler cette fonction dans la boucle principale, juste après avoir mis à jour les propriétés cinématiques de l'avatar :

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

Dernière petite retouche : animer le sprite quand l'avatar se déplace…


## Comment animer le sprite

Cette fois, c'est nous qui allons gérer l'animation en fonction des commandes imposées par le joueur. On ne s'appuiera plus sur la gestion automatisée des animations de `gb.display`, mais sur la méthode `setFrame()` fournie par la classe `Image`. Nous l'avons déjà abordée dans le chapitre précédent lorsqu'il s'agissait de sélectionner la bonne frame sur le tileset.

Définissons une nouvelle constante en début de code qui nous donne le nombre total de frames qui composent le sprite :

```cpp
const uint8_t AVATAR_FRAMES = SPRITE_DATA[2];
```

Nous devons également faire évoluer notre modèle `Avatar` en y ajoutant une propriété `frame` pour stocker l'indice de la frame courante. Et lorsque viendra le moment d'afficher le sprite à l'écran, il suffira de sélectionner cette frame avec la méthode `setFrame()`.

Nous devons également prendre en charge l'orientation horizontale du sprite, pour savoir s'il est orienté vers la gauche ou vers la droite. Souviens-toi, nous avons vu que la méthode `drawImage()` permettait de renverser l'image horizontalement (ou verticalement d'ailleurs). Il suffisait pour cela de lui passer deux arguments supplémentaires `w2`et `h2` :

```cpp
void drawImage(int16_t x, int16_t y, Image& img, int16_t w2, int16_t h2);
```

Il suffirait donc de fixer `w2` avec la valeur `-AVATAR_WIDTH` ou `+AVATAR_WIDTH` selon que le sprite doive être orienté vers la gauche ou vers la droite. On va donc définir une nouvelle propriété `direction` qui déterminera l'orientation horizontale du sprite.

Les modifications à apporter au modèle `Avatar` ne sont finalement pas très nombreuses.

D'abord on ajoute les nouvelles propriétés :

```cpp
struct Avatar {

    int16_t x, y;
    int8_t  vx;
    uint8_t frame;
    int8_t  direction;

    // ...

};
```

Ensuite on modifie le contructeur pour qu'il initialise correctement ces propriétés :

```cpp
Avatar(int16_t x, int16_t y) : x(x), y(y), vx(0), frame(0), direction(1) {}
```

Lorsque l'avatar se déplace vers la gauche ou vers la droite, on met à jour la propriété `direction` :

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

Lorsque l'avatar s'arrête, on positionne la frame courante sur l'indice `0` :

```cpp
void stop() {
    vx = 0;
    frame = 0;
}
```

Et au moment de prendre en compte son déplacement avec la méthode `update()`, on en profite pour calculer l'indice de la frame courante :

```cpp
void update() {

    x += vx;

    if (vx && (gb.frameCount & 0x1)) {
        ++frame %= AVATAR_FRAMES;
    }

}
```

Ainsi, lorsque sa vitesse horizontale est non nulle et que le nombre de cycles `gb.frameCount` est impair, c'est-à-dire une fois sur deux lorsque la fonction `loop()` est exécutée, on passe à la frame suivante, en bouclant sur la première lorsqu'on a atteint la dernière.

Il ne reste plus qu'à sélectionner la bonne frame avant d'afficher le sprite avec la bonne orientation :

```cpp
void draw() {
    Image sprite(SPRITE_DATA);
    sprite.setFrame(frame);
    gb.display.drawImage(x, y, sprite, direction * AVATAR_WIDTH, AVATAR_HEIGHT);
}
```

Et voilà ! Cette fois, on a exactement ce qu'on voulait :

{% include meta-demo.html src="assets/screen/example-14.gif" %}

L'avatar est maintenant animé lorsqu'il se déplace
{: .caption }


Pas trop perdu ? On a appliqué pas mal de modifications depuis la dernière démo. Voilà donc le code source complet qui prend en compte toutes les nouveautés que nous avons apportées jusque-là :

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

Maintenant qu'on peut déplacer notre avatar horizontalement sur la scène de jeu, on pourrait faire en sorte qu'il puisse effectuer des sauts. Tu vas voir que c'est pas très compliqué.


## Comment effectuer des sauts

Cette fois, il faut ajouter à notre avatar la capacité de se déplacer dans la dimension verticale :

- sous l'impulsion d'un saut
- sous l'effet de la gravité

Définissons donc deux nouvelles constantes :

```cpp
const int8_t AVATAR_JUMP = -5;
const int8_t GRAVITY     = 1;
```

L'axe vertical est orienté vers le bas, dans l'ordre croissant des ordonnées.

Puis ajoutons une composante verticale à la vitesse de notre modèle `Avatar`, ainsi qu'un témoin `jumping` qui indique qu'un saut est en cours :

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

En effet, le témoin `jumping` est utile pour contraindre certaines actions du joueur comme l'empêcher de stopper son avatar en plein saut !

Voyons justement ce qui se passe au niveau de la gestion des contrôles :

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

On ajoute une nouvelle commande qui permet au joueur de sauter lorsqu'il appuie sur `BUTTON_A`. Mais on ajoute une contrainte pour l'empêcher de sauter lorsqu'un saut est déjà en cours. On ajoute également une contrainte lorsqu'il relâche `BUTTON_LEFT` ou `BUTTON_RIGHT` pour empêcher que l'avatar puisse s'arrêter en plein vol.

Il faut également réviser la fonction `updateGame()` qui gère les contraintes physiques de la scène de jeu. En effet, on doit y ajouter la prise en compte de l'effet de la gravité :

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

Revenons à notre modèle `Avatar` et modifions le constructeur pour l'initialisation des nouvelles propriétés `vy` et `jumping` :

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

Puis ajoutons la méthode `jump()` qui permet d'exécuter le saut :

```cpp
void jump() {
    vy = AVATAR_JUMP;
    jumping = true;
}
```

On doit également modifier la méthode `stop()` pour prendre en compte les nouvelles propriétés :

```cpp
void stop() {
    vx = 0;
    vy = 0;
    frame = 0;
    jumping = false;
}
```

Pour terminer, nous devons réviser la méthode `update()` pour :

- ajuster la position du sprite dans les deux dimensions horizontale et verticale
- figer l'animation du sprite sur la frame d'indice `3` pendant un saut

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

Et voilà le travail :

{% include meta-demo.html src="assets/screen/example-15.gif" %}

Jumpin' Jack Flash
{: .caption }

Ça commence à ressembler à un vrai jeu de plateformes, nan ?

Voici le code source complet, qui reprend tout ce que nous avons accompli jusque-là :

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

Tu connais maintenant toutes les ficelles qui permettent d'animer un sprite facilement, et de donner vie à ton avatar dans un décor construit de pixels. Pour clore ce tutoriel, on va ajouter une petite touche finale, que je te propose de découvrir dans le prochain chapitre.


[game-scene]: {{ site.baseurl }}/assets/figures/game-scene-1000x628.png