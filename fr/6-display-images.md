---
layout: default
lang: fr
lang_ref: display-images
nav_order: 6
title: Affichage des images
permalink: /fr/affichage-des-images
date: 2021-05-14 10:15 +0400
---

# Comment afficher tes images sur la META


## Organisation des codes sources

Pour éviter d'avoir à créer de nombreux mini-projets Arduino, on va organiser notre code en isolant chaque exemple de démonstration dans un dossier `examples` situé à la racine du projet, par un fichier `example-##.h` qu'il suffira d'intégrer dans le sketch principal `my-stunning-game.ino`. Par exemple, commençons avec un fichier `example-00.h` :

![Dossier projet][project-example]{: width="316" height="222" .shadow }
{: .figure }

Le nouveau contenu du dossier de ton projet
{: .caption }

N'oublie pas de créer un fichier vide `examples.ino` pour pouvoir ouvrir les fichiers contenus dans le dossier `examples` avec l'éditeur de code de l'IDE Arduino.

Le sketch principal se réduit à la seule directive de précompilation suivante :

my-stunning-game.ino
{: .filename }

```cpp
#include "examples/example-00.h"
```

Et le code source doit maintenant être écrit dans le fichier `example-00.h` :

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

À chaque nouveau code source `example-01.h`, `example-02.h`, etc., il te suffira de mettre à jour le sketch principal avec la bonne directive de précompilation `#include`.


## La méthode `drawImage()`

L'API `gb.display` expose des méthodes spécialisées qui permettent d'afficher facilement des images sur l'écran de la META. On emploie ici le terme *méthode*, car il désigne des fonctions qui sont définies dans le cadre d'un modèle orienté objet (la classe `Image`). Si la nuance ne t'est pas familière, ça n'est pas grave, les méthodes restent avant tout des fonctions dans la sémantique du langage C++. Parmi ces méthodes, l'une d'entre elles va particulièrement nous intéresser ici. Il s'agit de la méthode `drawImage()`, qui expose 3 signatures différentes :

```cpp
void drawImage(int16_t x, int16_t y, Image& img);
void drawImage(int16_t x, int16_t y, Image& img, int16_t w2, int16_t h2);
void drawImage(int16_t x, int16_t y, Image& img, int16_t x2, int16_t y2, int16_t w2, int16_t h2);
```

Tu vois qu'elle prennent toutes comme argument une référence vers une instance de la classe `Image`. Et nous avons vu précédemment comment la créer en fournissant au constructeur de la classe `Image` un tableau de données de type `uint16_t` ou `uint8_t` en fonction du mode d'affichage configuré.

Par exemple, avec le mode d'affichage par défaut :

config-gamebuino.h
{: .filename }

```cpp
#define DISPLAY_MODE DISPLAY_MODE_RGB565
```

Voilà comment afficher notre sprite animé à l'écran en utilisant la première signature :

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

Ici, le fichier d'assets est incorporé en suivant le chemin relatif qui relie l'emplacement du fichier `example-01.h` à celui du fichier `rgb565.h` :

```cpp
#include "../assets/rgb565.h"
```

On crée ensuite une instance de la classe `Image`, nommée `avatar`, en passant au constructeur le tableau de données `SPRITE_DATA`, qui définit toutes les frames de notre sprite :

```cpp
Image avatar(SPRITE_DATA);
```

Puis on affiche le sprite au centre de l'écran :

```cpp
gb.display.drawImage(36, 28, avatar);
```

Pourquoi aux coordonnées (36,28) ?

![Centrage du sprite][avatar-centering]{: width="437" height="311" }
{: .figure }

De manière générale, il est peu recommandé d'exprimer des coordonées sous forme brute quand le positionnement est le résultat d'un calcul relatif à la dimension de l'écran. Il vaut mieux écrire les choses ainsi :

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

Souviens-toi que :

- `SPRITE_DATA[0]` correspond à la largeur `frame_width` du sprite
- `SPRITE_DATA[1]` correspond à la hauteur `frame_height` du sprite

Leurs valeurs sont exprimées avec le type `uint16_t` car c'est le type commun à toutes les données stockées dans le tableau `SPRITE_DATA`, mais elles peuvent être stockées dans des constantes de type `uint8_t` puisqu'elles sont inférieures à 256 :

```cpp
const uint8_t AVATAR_WIDTH  = SPRITE_DATA[0];
const uint8_t AVATAR_HEIGHT = SPRITE_DATA[1];
```

Ces constantes ne sont absolument pas nécessaires ici, mais elles facilitent la lecture du code.

Souviens-toi également que nous avions défini `frame_loop` avec la valeur de `4` dans `SPRITE_DATA`, ce qui implique que l'animation est automatiquement réglée à la fréquence de **8 fps** ici :

{% include meta-demo.html src="assets/screen/example-01.gif" %}

Magique, nan ?
{: .caption }


Examinons maintenant la deuxième signature de la méthode `drawImage()` :

```cpp
void drawImage(int16_t x, int16_t y, Image& img, int16_t w2, int16_t h2);
```

Elle introduit deux nouveaux arguments qui permettent d'étirer l'image originale sur un rectangle de largeur `w2` et de hauteur `h2`. Autrement dit, cette version permet d'agrandir ou de rétrécir l'image selon ses dimensions horizontale et/ou verticale. Par exemple, si on souhaite agrandir uniformément l'image par un facteur 3, il suffit de multiplier sa largeur et sa hauteur initiales par 3 :

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

Agrandissement par un facteur 3
{: .caption }

On peut tout à fait appliquer des facteurs d'étirement différents sur la largeur et la hauteur de l'image. Mais dans ce cas, l'image sera nécessairement déformée, puisque le ratio largeur / hauteur de l'image originale ne sera pas respecté. Par contre, si tu examines bien la signature de la méthode `drawImage()`, tu vois que les arguments `w2` et `h2` peuvent tout à fait être **négatifs**. Et ça c'est très intéressant, car cela signifie qu'on peut non seulement étirer l'image mais également **la renverser**. Par exemple, si on multiplie la largeur initiale de l'image par le facteur `-1`, on obtient un renversement horizontal :

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

Renversement horizontal
{: .caption }

Tu te doutes que cette subtilité va nous être très utile pour gérer le déplacement de l'avatar. On n'aura pas besoin de modifier le sprite pour y ajouter des images décrivant un mouvement vers la gauche. Il suffira tout simplement d'appliquer un renversement horizontal, et le tour sera joué.  <i class="far fa-smile-wink"></i>


Il nous reste à examiner la dernière signature de la méthode `drawImage()` :

```cpp
void drawImage(int16_t x, int16_t y, Image& img, int16_t x2, int16_t y2, int16_t w2, int16_t h2);
```

Celle-ci va nous permettre de recadrer la zone d'affichage de l'image en spécifiant les coordonnées du coin supérieur gauche et les dimensions du rectangle de recadrage. Par exemple, imaginons qu'on souhaite effacer la tête de notre avatar :

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

Recadrage de la zone à afficher
{: .caption }

Tu vois que les possibilités sont multiples avec ces 3 variantes de la méthode `drawImage()`.


## Comment ça marche avec les modes d'affichage à couleurs indexées

Passons maintenant dans un mode d'affichage à couleurs indexées :

config-gamebuino.h
{: .filename }

```cpp
#define DISPLAY_MODE DISPLAY_MODE_INDEX_HALFRES
```

Et voyons ce qui se passe quand on affiche notre sprite :

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

<i class="far fa-surprise"></i>  Oups
{: .caption }

<i class="far fa-frown-open"></i>  On a pourtant pensé à intégrer le bon fichier d'assets :

```cpp
#include "../assets/indexed.h"
```

Oui, mais ?… On a forcément oublié un truc…

Souviens-toi, par défaut c'est la **palette Gamebuino** qui est appliquée sur les modes d'affichage à couleurs indexées. Il ne faut donc surtout pas oublier d'appliquer **notre palette** de couleurs à l'image `gb.display` lors de l'initialisation :

```cpp
gb.display.setPalette(PALETTE);
```

Ok, mettons ça en application tout de suite :

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

Cette fois-ci, tout va bien :

{% include meta-demo.html src="assets/screen/example-02.gif" %}{% comment %} <!-- example-06 is same as 02 --> {% endcomment %}

N'oublie pas de fixer la bonne palette
{: .caption }

Ne perds pas de vue que la palette de couleurs appliquée à l'image `gb.display` est globale. Par conséquent toutes les images que tu afficheras avec la méthode `drawImage()` devront nécessairement partager cette palette unique. Il est néanmoins tout à fait possible de modifier cette palette avec la méthode `setPalette()` à tout moment, mais les répercussions se propageront sur toutes les images affichées.

Il en est tout autrement dans le cas du mode d'affichage RGB565. Et je te propose d'examiner quelques astuces qui en découlent.


## Comment jouer avec les palettes en RGB565

Si la palette de couleurs est commune à tous les assets dans le cas des modes d'affichage à couleurs indexées, il est par contre possible de jouer avec celle de chaque asset, indépendamment des autres, dans le cas de `DISPLAY_MODE_RGB565`. Repassons donc dans ce mode :

config-gamebuino.h
{: .filename }

```cpp
#define DISPLAY_MODE DISPLAY_MODE_RGB565
```

Mais continuons d'utiliser les assets tels qu'ils ont été définis pour les modes d'affichage à couleurs indexées par le fichier `assets/indexed.h`.

Le tampon graphique de `gb.display` peut stocker n'importe-quelle couleur de l'espace RGB565, mais on peut tout à fait lui transférer une image en couleurs indexées avec la méthode `drawImage()`. Pour cela, il suffit simplement de lui appliquer la palette souhaitée juste avant le transfert.

Supposons que nous ayons envie de changer la couleur de peau de notre avatar. Nous pourrions appliquer les modifications suivantes sur la palette du sprite :

- couleur d'indice `0x6` : {% include square.html color="#e8c47d" %} `0xEE2F` &rArr; {% include square.html color="#7a6552" %} `0x7B2A`
- couleur d'indice `0x7` : {% include square.html color="#ffe6b3" %} `0xFF36` &rArr; {% include square.html color="#b8977a" %} `0xBCAF`

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

Modification dynamique de la palette du sprite
{: .caption }

Lors de l'initialisation on recopie la palette originale `PALETTE` du sprite dans un tableau `palette` qu'on va pouvoir modifier pendant l'exécution du programme :

```cpp
memcpy(palette, PALETTE, 16*sizeof(Color));
```

Ensuite, dans la boucle principale, juste avant d'afficher le sprite avec `drawImage()`, on affecte la palette dynamique à `gb.display`, que l'on peut alors modifier librement :

```cpp
gb.display.setPalette(palette);
palette[0x6] = (Color)0x7b2a;
palette[0x7] = (Color)0xbcaf;
```


Tu vois qu'on peut faire des choses assez marrantes très simplement, en partant d'une même colormap, mais en appliquant des modifications à la palette juste avant de transférer l'image sur `gb.display`.

Par exemple, on pourrait souhaiter habiller le sprite avec différentes couleurs :

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

Habillage dynamique du sprite
{: .caption }

Le principe est toujours le même : affecter la palette dynamique à `gb.display`, puis la modifier avant de transférer le sprite avec `drawImage()`.

On peut même affecter une nouvelle couleur à la palette, qui change à chaque cycle d'affichage. Par exemple, pour donner un air de cyborg au sprite, on peut faire la chose suivante :

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

Variation périodique d'une couleur de la palette
{: .caption }

On pourrait également appliquer des permutations cycliques sur la palette de couleurs. C'est exactement cette technique qui est utilisée par des artistes talentueux comme [Mark Ferrari][mark-ferrari] pour animer de [magnifiques scènes][game-effects] de pixel-art, en utilisant uniquement des cycles de couleurs définies sur une palette.

![Stunning Pixel Art][pixel-art]{: width="320" height="240" .shadow }
{: .figure }

Animation d'une toile de pixels par simples permutations cycliques des couleurs
{: .caption }

Les possibilités sont vastes, même sur un petit écran comme celui de la META. Maintenant t'as les clefs en main… Fais confiance à ton imagination.

On vient de passer en revue les différentes manières d'afficher notre sprite à l'écran avec la méthode `drawImage()`. On ne s'est pas du tout préoccupé de gérer l'animation, puisqu'elle est mise en oeuvre de manière automatique grâce à la propriété `frame_loop` que nous avons définie dans le tableau `SPRITE_DATA`. Jusque là, elle avait la valeur de `4`, ce qui se traduisait par une animation à la fréquence de **8 fps**.

Si on souhaite doubler cette fréquence pour la passer à **16 fps** et donner l'impression que notre avatar est en train de courir, il suffit de diviser `frame_loop` par `2`.

Repasssons en mode RGB565 :

config-gamebuino.h
{: .filename }

```cpp
#define DISPLAY_MODE DISPLAY_MODE_RGB565
```

Puis modifions la fréquence d'animation du sprite :

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

Et voyons ce que l'on obtient avec ça :

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

Modification de la fréquence d'animation
{: .caption }


Cette fonctionnalité est assez pratique pour des animations cycliques sur lesquelles on n'a pas besoin de contrôle. Il suffit de laisser `gb.display` gérer l'animation automatiquement à la fréquence choisie. Ça peut être utile par exemple pour des assets comme une torche dont la flamme vascillerait imperturbablement au cours du temps. On n'aurait pas besoin de se préoccuper explicitement de son animation dans la boucle de contrôle principale du jeu. `gb.display` se chargerait de gérer ça automatiquement pour nous.

Mais dans le cas de notre avatar, ce sont les interactions de l'utilisateur avec les boutons de la console qui doivent gouverner les animations du sprite. Nous devons donc avoir un contôle total sur ces animations. C'est évidement possible et nous le verrons un peu plus loin. Mais auparavant, je te propose qu'on examine comment mettre en place le décor de la scène de jeu en utilisant les images de notre tileset. Allez, on se retrouve au prochain chapitre pour cela.



[project-example]:  {{ site.baseurl }}/assets/figures/project-examples-632x524.png
[avatar-centering]: {{ site.baseurl }}/assets/figures/avatar-screen-centering-874x622.png
[pixel-art]:        {{ site.baseurl }}/assets/figures/seascape-640x480.gif
[mark-ferrari]:     https://www.markferrari.com/
[game-effects]:     http://www.effectgames.com/demos/canvascycle/