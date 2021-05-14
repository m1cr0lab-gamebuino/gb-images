---
layout: default
lang: fr
lang_ref: tile-mapping
nav_order: 7
title: Tile Mapping
permalink: /fr/tilemapping
date: 2021-05-14 12:00 +0400
---

# Comment décorer la scène de jeu

Le moment est venu de dresser le décor en utilisant notre tileset :

![Tileset][tileset-frames]{: width="552" height="650" }
{: .figure }

Chaque tile est défini par une frame dans le tableau `TILESET_DATA`. On a 4 tiles, donc 4 frames, que l'on peut représenter par un indice compris entre `0` et `3`. À la différence du sprite, les frames du tileset ne sont pas destinées à être animées. Au contraire, on va avoir besoin de désigner précisément quelle frame afficher à tel endroit sur l'écran.

C'est la raison pour laquelle nous avions défini la propriété `frame_loop` avec la valeur `0` :

assets/rgb565.h
{: .filename }

```cpp
const uint16_t TILESET_DATA[] = {

    // metadata

    16,     // frame width
    8,      // frame height
    4,      // frames
    0,      // frame loop
    0xf81f, // transparent color
    0,      // 16-bits color mode

    // colormap
```

Ceci permet de désactiver le processus automatique d'animation géré par `gb.display`.

Pour afficher une frame en particulier, nous allons utiliser la méthode `setFrame()` de la classe `Image` :

```cpp
void setFrame(uint16_t frame);
```

Par exemple, pour tapisser tout l'écran avec les tiles d'indice `0` et `2`, on peut faire la chose suivante :

examples/example-10.h
{: .filename }

```cpp
#include <Gamebuino-Meta.h>
#include "../assets/rgb565.h"

const uint8_t SCREEN_WIDTH  = 80;
const uint8_t SCREEN_HEIGHT = 64;

const uint8_t TILE_WIDTH  = TILESET_DATA[0];
const uint8_t TILE_HEIGHT = TILESET_DATA[1];

const uint8_t TILES_WIDE = SCREEN_WIDTH  / TILE_WIDTH;
const uint8_t TILES_HIGH = SCREEN_HEIGHT / TILE_HEIGHT;

Image tileset(TILESET_DATA);

void setup() {
    gb.begin();
    gb.setFrameRate(32);
}

void loop() {

    gb.waitForUpdate();
    gb.display.clear();

    for (uint8_t j=0; j<TILES_HIGH; ++j) {
        for (uint8_t i=0; i<TILES_WIDE; ++i) {

            tileset.setFrame(2*(j & 0x1));

            gb.display.drawImage(
                i*TILE_WIDTH,  // x
                j*TILE_HEIGHT, // y
                tileset        // image
            );

        }
    }

}
```

On détermine le nombre de tiles à afficher en fonction de leurs dimensions horizontale et verticale :

- sur la largeur de l'écran : `TILES_WIDE`
- sur la hauteur de l'écran : `TILES_HIGH`

```cpp
const uint8_t TILE_WIDTH  = TILESET_DATA[0];
const uint8_t TILE_HEIGHT = TILESET_DATA[1];

const uint8_t TILES_WIDE = SCREEN_WIDTH  / TILE_WIDTH;
const uint8_t TILES_HIGH = SCREEN_HEIGHT / TILE_HEIGHT;
```

Cela revient à découper l'écran sous la forme d'une grille dont les cellules sont les tiles :

![Grille de tiles][tile-grid]{: width="450" height="314" }
{: .figure }

Puis on instancie en mémoire une image contenant notre tileset :

```cpp
Image tileset(TILESET_DATA);
```

Ensuite, il ne reste plus qu'à parcourir chaque cellule de la grille :

```cpp
// the tile grid is crossed in height:
for (uint8_t j=0; j<TILES_HIGH; ++j) {

    // and in width:
    for (uint8_t i=0; i<TILES_WIDE; ++i) {

        // ...

    }
}
```

Et pour chaque coordonnée `(i,j)` sur la grille de tiles, on détermine les coordonnées `(x,y)` de l'image à afficher à l'écran. Mais juste avant de l'afficher, on sélectionne la bonne frame dans le tileset :

```cpp
tileset.setFrame(2*(j & 0x1));

gb.display.drawImage(
    i*TILE_WIDTH,  // x
    j*TILE_HEIGHT, // y
    tileset        // image
);
```

Si le numéro de ligne `j` est pair, on sélectionne la frame d'indice `0`, et si `j` est impair, on sélectionne la frame d'indice `2` :

{% include meta-demo.html src="assets/screen/example-10.png" %}

Tilemap calculé dynamiquement
{: .caption }

Ici le *tilemap*, autrement dit la cartographie de l'écran, est obtenu par une procédure de calcul au moment de l'exécution. Ce procédé peut être utile dans certains cas, mais on a coûtume d'établir cette cartographie dans un tableau de données, en indiquant explicitement quelle frame afficher sur quelle cellule de la grille :

```cpp
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
```

Le tableau `TILEMAP` établit tout simplement le plan d'agencement des tiles à l'écran.

Intégrons ce nouveau procédé dans notre code :

examples/example-11.h
{: .filename }

```cpp
#include <Gamebuino-Meta.h>
#include "../assets/rgb565.h"

const uint8_t SCREEN_WIDTH  = 80;
const uint8_t SCREEN_HEIGHT = 64;

const uint8_t TILE_WIDTH  = TILESET_DATA[0];
const uint8_t TILE_HEIGHT = TILESET_DATA[1];

const uint8_t TILES_WIDE = SCREEN_WIDTH  / TILE_WIDTH;
const uint8_t TILES_HIGH = SCREEN_HEIGHT / TILE_HEIGHT;

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

Image tileset(TILESET_DATA);

void setup() {
    gb.begin();
    gb.setFrameRate(32);
}

void loop() {

    gb.waitForUpdate();
    gb.display.clear();

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
```

Et on obtient la scène du jeu définie par le tableau `TILEMAP` :

{% include meta-demo.html src="assets/screen/example-11.png" %}

Tilemap prédéfini
{: .caption }

On n'a plus qu'à y intégrer notre avatar et examiner comment contrôler ses mouvements et les animations du sprite qui en découlent. C'est précisément ce qu'on va faire au chapitre suivant.  <i class="far fa-smile-wink"></i>



[tileset-frames]: {{ site.baseurl }}/assets/figures/tileset-1104x1300.png
[tile-grid]:      {{ site.baseurl }}/assets/figures/tile-grid-900x628.png