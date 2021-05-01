---
layout: default
lang: fr
nav_order: 4
lang_ref: convert-images
title: Conversion des images
permalink: /fr/conversion-des-images
# last_modified_date: 2021-04-04+04:00
---

# Comment convertir tes images en C++

Avant de découvrir comment transcoder les éléments graphiques que nous avons créés pour pouvoir les manipuler par la programmation et les afficher à l'écran, nous devons examiner comment sont représentées les images en C++ sur la META.

## La classe Image

On a vu, dans la première partie de ce tutoriel, que `gb.display` constituait une API spécialisée permettant, entre autres, d'afficher des images, en s'adaptant à 3 modes d'affichages particuliers, avec des résolutions et des profondeurs de couleurs bien spécifiques.

Nous avons également vu que cette API assurait la gestion d'un tampon graphique, dont on n'avait pas à se soucier directement, et dont la taille était automatiquement adaptée au mode d'affichage configuré pour le projet.

Nous allons voir qu'en fait `gb.display` correspond intrinsèquement à la représentation conceptuelle d'une image et est construite selon un modèle orienté objet précis, qui est décrit par la classe [`Image`][image] fournie par la bibliothèque [Gamebuino-META][gb-meta]. Ce modèle expose des [*constructeurs*][image-constructors] qui permettent d'*instancier* (de créer) facilement des images à partir de paramètres spécifiques.

Parmi les constructeurs proposés, ceux qui vont nous intéresser en premier lieu sont les suivants :

```cpp
// flash constructors
Image(const uint16_t* buffer);

// flash indexed constructors
Image(const uint8_t* buffer);
```

Souviens-toi, dans le mode d'affichage par défaut `DISPLAY_MODE_RGB565`, les couleurs sont stockées sur 2 octets, donc 16 bits. En C++, pour stocker des données de cette forme, on utilise le type `uint16_t`. Dans le cas des modes d'affichages `DISPLAY_MODE_INDEX` et `DISPLAY_MODE_INDEX_HALFRES`, où les couleurs sont indexées dans une palette de 16 couleurs, celles-ci sont stockées sur 1 seul octet, donc 8 bits. En C++, on utilisera alors le type `uint8_t`.

C'est la raison pour laquelle les deux constructeurs ci-dessus peuvent être invoqués avec des tableaux de données de deux types particuliers : `uint16_t` et `uint8_t`. On utilisera le premier pour le mode d'affichage par défaut, et le second pour les modes d'affichage en couleurs indexées.

Voyons maintenant comment sont constitués ces tableaux de données.


## Des tableaux de données pour représenter les images

Pour créer une image qui soit une instance de la classe `Image`, on va devoir passer au constructeur de la classe `Image` un tableau de données comportant tous les paramètres nécessaires à constuire une représentation précise de l'image pour l'afficher correctement à l'écran ensuite. Ce tableau de données est constitué de 2 blocs distincts :

| **Metadata header** | Un en-tête décrivant les méta-données de l'image                |
| **Colormap**        | La liste des codes couleurs caractérisant les pixels de l'image |

Ces données devront **toutes** être exprimées avec le type approprié selon le mode d'affichage choisi :

- `uint16_t` dans le cas de `DISPLAY_MODE_RGB565`
- `uint8_t` dans le cas de `DISPLAY_MODE_INDEX` ou `DISPLAY_MODE_INDEX_HALFRES`

Donc voilà comment se présente le squelette de ce tableau de données :

DISPLAY_MODE_RGB565
{: .code-header }

```cpp
const uint16_t IMAGE_DATA[] = {

    // Metadata header
    // ...

    // Colormap
    // ...
};
```

DISPLAY_MODE_INDEX   ou   DISPLAY_MODE_INDEX_HALFRES
{: .code-header }

```cpp
const uint8_t IMAGE_DATA[] = {

    // Metadata header
    // ...

    // Colormap
    // ...
};
```

> **Remarque importante**
> 
> Le tableau de données `IMAGE_DATA` est défini comme une **constante** par le mot-clef `const` du C++. En effet, les données qu'il contient ne sont pas destinées à être modifiées pendant l'exécution du programme. L'avantage avec des données constantes, c'est que le compilateur fera en sorte qu'elles soient hébergées dans la mémoire Flash du micro-contrôleur. Elles n'occuperont aucune place en RAM en tant que telles.


## Les métadonnées de l'image

### Avec le mode d'affichage RGB565

Dans le cas du mode d'affichage `DISPLAY_MODE_RGB565`, le bloc des métadonnées caractérisant l'image se présente sous la forme suivante :

| frame_width       | Largeur de chaque frame en nombre de pixels                                     |
| frame_height      | Hauteur de chaque frame en nombre de pixels                                     |
| frames            | Nombre de frames constituant l'image                                            |
| frame_loop        | Nombre de boucles d'affichage caractérisant la persistance de la frame courante |
| transparent_color | Couleur de transparence de l'image                                              |
| color_mode        | 0                                                                               |

Je suppose qu'il n'est pas utile de fournir davantage d'explications, si ce n'est pour préciser un peu les choses concernant la propriété `frame_loop`. Cette propriété n'a de sens que pour des images animées comme les sprites. Pour des images statiques comme les tiles qui constituent le décor de la scène de jeu, cette propriété n'a pas de sens et doit être fixée avec la valeur `0`.

Pour bien comprendre ce que signifie `frame_loop` et ce qu'implique la valeur qu'on lui affecte, partons d'un programme typique pour la META :

```cpp
#include <Gamebuino-Meta.h>

void setup() {
    gb.begin();
}

void loop() {
    gb.waitForUpdate();
}
```

L'instruction `gb.waitForUpdate()` permet, entre autres, de s'assurer que la fréquence d'affichage est maintenue à 25 rafraîchissements par secondes. Elle déclenche différentes routines de contrôle assurant le fonctionnement intrinsèque de la console, comme la gestion des animations automatisées avec `frame_loop`, le transfert du tampon graphique de `gb.display` vers l'écran ou la lecture de l'état des boutons, et bien d'autres choses encore.

La séquence des routines déclenchées pour réaliser toutes ces opérations prend évidemment un certain temps à s'exécuter, même si ça nous semble très rapide. La durée totale d'exécution de cette séquence est mesurée précisément. Pour maintenir une fréquence régulière à 25 rafraîchissements par secondes, il faut qu'un cycle complet de la boucle `loop()` soit exécuté en 1000 / 25 = 40 ms. Si d'aventure l'exécution du cycle prend moins de temps que prévu, `gb.waitForUpdate()` va ordonner au micro-contrôleur de faire une pause en attendant que les 40 ms soient bien écoulées. C'est ainsi que la fréquence d'affichage est régulée.

Si par contre, la durée d'exécution d'un cycle complet est supérieure à 40 ms, c'est que la fréquence de rafraîchissement est réglée avec une valeur trop élevée pour que le micro-contrôleur ait le temps de la satisfaire. Il va se retrouver débordé par la somme de tâches à accomplir dans le temps imparti et tu observeras un effet de *lag* plus ou moins important sur l'écran de la console.

Pour ajuster la fréquence d'exécution des cycles de la fonction `loop()`, la classe `Gamebuino` (dont `gb` est une instance) expose une fonction qui permet de la fixer à la valeur de ton choix pourvu qu'elle n'excède pas la valeur de **50 frames par seconde** (*fps*) :

```cpp
void Gamebuino::setFrameRate(uint8_t fps)
```

Elle est exprimée en **fps** en référence au nombre de frames, constituées par les états successifs du tampon graphique, qui pourront être envoyées au périphérique d'affichage (l'écran) par le micro-contrôleur, chaque seconde, dans la mesure où il est en capacité de maintenir cette fréquence sans être débordé.

Supposons, par exemple, que nous fixions cette fréquence à 32 fps :

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

Dans ce cas la fonction `loop()` sera exécutée 32 fois par secondes. Autrement dit un cycle complet durera 1000 / 32 = 31,25 ms.

Revenons maintenant à la métadonnée `frame_loop` de l'image. Elle permet simplement de fixer le nombre de cycles pendant lesquels chaque frame de l'animation restera affichée à l'écran avant de passer à la frame suivante. Cette gestion est automatisée et prise en charge par `gb.waitForUpdate()`. Autrement dit, si `frame_loop = 4`, chaque frame de l'animation restera affichée à l'écran pendant 4 cycles avant que la frame suivante ne soit affichée à son tour pour 4 nouveaux cycles, etc.

Par conséquent, si la fréquence globale de rafraîchissement de l'écran est fixée à 32 fps, alors l'animation progressera au rythme de 32 / 4 = 8 fps. `frame_loop` permet donc de régler la vitesse de la tête de lecture de l'animation, par-rapport à la fréquence d'affichage globale. Et chaque image animée peut évidemment être paramétrée avec une vitesse différente, puisque la métadonnée `frame_loop` est définie pour une image donnée.

Dans les cas où `frame_loop` est fixée à la valeur `0`, l'image est alors figée sur sa première frame. On verra plus loin comment spécifier explicitement l'indice de la frame que l'on souhaite afficher. Par ailleurs, si l'image ne comporte que des tiles censés composer le décor de la scène de jeu, il est inutile de fixer une valeur différente de `0` à `frame_loop`, puisqu'aucune animation n'est censée se jouer. Chaque tile est indépendant, même s'il fait partie d'un même tileset. Là encore, on verra plus loin comment désigner un tile en particulier parmi tous ceux qui composent le tileset.

Pour se fixer les idées, voici comment déclarer les métadonnées de notre sprite :

```cpp
const uint16_t SPRITE_DATA[] = {
    
    // Metadata header

    8,      // frame_width
    8,      // frame_height
    4,      // frames
    4,      // frame_loop
    0xf81f, // transparent_color
    0,      // color_mode
    
    // Colormap
    // ...
};
```

Et voici comment déclarer les métadonnées de notre tileset :

```cpp
const uint16_t TILESET_DATA[] = {

    // Metadata header

    16,     // frame_width
    8,      // frame_height
    4,      // frames
    0,      // frame_loop
    0xf81f, // transparent_color
    0,      // color_mode
    
    // Colormap
    // ...
};
```

Tu peux constater au passage qu'avec le langage C++ la notation hexadécimale peut être être écrite en minuscules ou en majuscules. Le compilateur les interprétera de la même façon.


### Avec les modes d'affichages à couleurs indexées

Dans le cas des modes `DISPLAY_MODE_INDEX` et `DISPLAY_MODE_INDEX_HALFRES`, les métadonnées sont globalement les mêmes, à la seule différence que le nombre de frames constituant l'image est ici décomposé sur une **paire** d'`uint8_t` :

| frame_width        | Largeur de chaque frame en nombre de pixels                                     |
| frame_height       | Hauteur de chaque frame en nombre de pixels                                     |
| frames_lower_byte  | Nombre de frames constituant l'image (octet des bits de poids faibles)          |
| frames_higher_byte | Nombre de frames constituant l'image (octet des bits de poids fort)             |
| frame_loop         | Nombre de boucles d'affichage caractérisant la persistance de la frame courante |
| transparent_color  | Couleur de transparence de l'image                                              |
| color_mode         | 1                                                                               |

La métadonnée `frames` qu'on a vue auparavant est ici scindée en deux valeurs : `frames_lower_byte` et `frames_higher_byte`. Et ça nous est imposé par la nature même du tableau de données de l'image qui ne peut être constitué que de valeurs de type `uint8_t`. En effet, `uint8_t` permet de coder la valeur d'un entier non signé sur 8 bits. Par conséquent, seules des valeurs comprises entre **0** et **255** peuvent être représentées par un `uint8_t` (2<sup>8</sup> = 256 valeurs possibles… mais pas une de plus). Ce type de données semble donc contraignant en nous imposant de nous limiter à des images ne comportant pas plus de 255 frames au total.

C'est là que l'astuce réside : en doublant le nombre d'octets pour stocker le nombre de frames, on s'autorise cette fois à coder jusqu'à 2<sup>16</sup> - 1 = 65 535 frames sur une même image ! Bon, ça semble énorme vu comme ça, mais c'est nécessaire pour franchir la barre des 255 frames.

Supposons, par exemple, que nous souhaitions encoder un tileset comportant **718** tiles. C'est tout à fait envisageable sur un jeu complet. En exprimant ce nombre sur 2 octets, sous ses formes binaire et hexadécimale, on obtient la répartition suivante :


![First frame of sprite][16-bits-integer]{: width="394" height="106" }
{: .figure }

Et du même coup, on peut en déduire que :

- `frames_lower_byte = 0xCE`
- `frames_higher_byte = 0x02`

Pour des nombres inférieurs à 256, `frames_higher_byte` sera toujours égal à `0x00`.

Voici par exemple comment déclarer les métadonnées de notre sprite :

```cpp
const uint8_t SPRITE_DATA[] = {
    
    // Metadata header

    8,    // frame_width
    8,    // frame_height
    0x04, // frames_lower_byte
    0x00, // frames_higher_byte
    4,    // frame_loop
    0xe,  // transparent_color
    1,    // color_mode
    
    // Colormap
    // ...
};
```

Et voici comment déclarer les métadonnées de notre tileset :

```cpp
const uint8_t TILESET_DATA[] = {

    // Metadata header

    16,   // frame_width
    8,    // frame_height
    0x04, // frames_lower_byte
    0x00, // frames_higher_byte
    0,    // frame_loop
    0xe,  // transparent_color
    1,    // color_mode
    
    // Colormap
    // ...
};
```


## La colormap de l'image

Le deuxième bloc du tableau de données, qui vient après les métadonnées de l'image, constitue ce que l'on appelle la *colormap* de l'image, c'est-à-dire la liste des codes couleurs des pixels constituant chacune des frames de l'image. Ces codes couleurs sont écrits les uns à la suite des autres, dans le sens de lecture des pixels de gauche à droite, puis de haut en bas, dans l'ordre de succession des frames.

Reprenons la spritesheet de notre avatar, en tenant compte de la modification de la palette des couleurs qui intègre maintenant les couleurs du tileset que nous avons ajouté :

![La colormap du sprite][avatar-final]{: width="396" height="314" }
{: .figure }

La colormap du sprite
{: .caption }

On peut maintenant compléter le tableau de données `SPRITE_DATA` en y ajoutant sa colormap.


**DISPLAY_MODE_RGB565**

La conversion est ici assez simple, il suffit de suivre les pixels dans l'ordre, en remplaçant l'indice de la couleur par sa valeur RGB565 :

```cpp
const uint16_t SPRITE_DATA[] = {
    
    // Metadata header

    8,      // frame_width
    8,      // frame_height
    4,      // frames
    4,      // frame_loop
    0xf81f, // transparent_color
    0,      // color_mode
    
    // Colormap

    // Frame 0
    0xf81f, 0xf81f, 0x632c, 0xad55, 0xad55, 0xad55, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xee2f, 0xff36, 0xff36, 0xff36, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xee2f, 0x0000, 0xff36, 0x0000, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xee2f, 0xff36, 0xff36, 0xff36, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x7afa, 0xb4df, 0xb4df, 0xb4df, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xff36, 0xb4df, 0xb4df, 0xb4df, 0xff36, 0xf81f,
    0xf81f, 0xf81f, 0x7afa, 0xb4df, 0xb4df, 0xb4df, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x6217, 0xf81f, 0xf81f, 0x6217, 0xf81f, 0xf81f,

    // Frame 1
    0xf81f, 0xf81f, 0x632c, 0xad55, 0xad55, 0xad55, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xee2f, 0xff36, 0xff36, 0xff36, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xee2f, 0x0000, 0xff36, 0x0000, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xee2f, 0xff36, 0xff36, 0xff36, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x7afa, 0xb4df, 0xb4df, 0xb4df, 0xf81f, 0xf81f,
    0xf81f, 0xff36, 0x7afa, 0xb4df, 0xb4df, 0xb4df, 0x7afa, 0xff36,
    0xf81f, 0xf81f, 0x7afa, 0xb4df, 0xb4df, 0xb4df, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0x7afa, 0x6217, 0xf81f, 0xf81f, 0xf81f,

    // Frame 2
    0xf81f, 0xf81f, 0x632c, 0xad55, 0xad55, 0xad55, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xee2f, 0xff36, 0xff36, 0xff36, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xee2f, 0x0000, 0xff36, 0x0000, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xee2f, 0xff36, 0xff36, 0xff36, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x7afa, 0xb4df, 0xb4df, 0xb4df, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xff36, 0xb4df, 0xb4df, 0xb4df, 0xff36, 0xf81f,
    0xf81f, 0xf81f, 0x7afa, 0xb4df, 0xb4df, 0xb4df, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x6217, 0xf81f, 0xf81f, 0x6217, 0xf81f, 0xf81f,

    // Frame 3
    0xf81f, 0xf81f, 0x632c, 0xad55, 0xad55, 0xad55, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xee2f, 0xff36, 0xff36, 0xff36, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xee2f, 0x0000, 0xff36, 0x0000, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xee2f, 0xff36, 0xff36, 0xff36, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x7afa, 0xb4df, 0xb4df, 0xb4df, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x7afa, 0x7afa, 0xff36, 0xb4df, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x7afa, 0xb4df, 0xb4df, 0xb4df, 0xf81f, 0xf81f,
    0xf81f, 0x6217, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x7afa, 0xf81f
};
```


**DISPLAY_MODE_INDEX et DISPLAY_MODE_INDEX_HALFRES**

Ici, on ne reporte pas directement les codes couleurs RGB565, mais leurs indices selon l'ordre défini par la palette. Il faut donc coder également la palette elle-même. Pour cela, la bibliothèque [Gamebuino-META][gb-meta] offre une classe [`Color`][color] permettant d'interpréter un code hexadécimal RGB565 comme une couleur (au sens sémantique).

Par ailleurs, n'oublie pas que, dans la colormap, les indices des couleurs doivent être regroupés par paires de pixels voisins sur un seul octet. L'expression de la colormap est du coup beaucoup plus concise :

```cpp
const Color PALETTE[] = {
    (Color)0x0000, // color 0x0
    (Color)0x18c3, // color 0x1
    (Color)0x3186, // color 0x2
    (Color)0x4228, // color 0x3
    (Color)0x632c, // color 0x4
    (Color)0xad55, // color 0x5
    (Color)0xee2f, // color 0x6
    (Color)0xff36, // color 0x7
    (Color)0x0862, // color 0x8
    (Color)0x10e4, // color 0x9
    (Color)0x1926, // color 0xa
    (Color)0x6217, // color 0xb
    (Color)0x7afa, // color 0xc
    (Color)0xb4df, // color 0xd
    (Color)0xf81f, // color 0xe
    (Color)0x0000  // color 0xf
};

const uint8_t SPRITE_DATA[] = {

    // Metadata header

    8,    // frame_width
    8,    // frame_height
    0x04, // frames_lower_byte
    0x00, // frames_higher_byte
    4,    // frame_loop
    0xe,  // transparent_color
    1,    // color_mode
    
    // Colormap

    // Frame 0
    0xee, 0x45, 0x55, 0xee,
    0xee, 0x67, 0x77, 0xee,
    0xee, 0x60, 0x70, 0xee,
    0xee, 0x67, 0x77, 0xee,
    0xee, 0xcd, 0xdd, 0xee,
    0xee, 0x7d, 0xdd, 0x7e,
    0xee, 0xcd, 0xdd, 0xee,
    0xee, 0xbe, 0xeb, 0xee,

    // Frame 1
    0xee, 0x45, 0x55, 0xee,
    0xee, 0x67, 0x77, 0xee,
    0xee, 0x60, 0x70, 0xee,
    0xee, 0x67, 0x77, 0xee,
    0xee, 0xcd, 0xdd, 0xee,
    0xe7, 0xcd, 0xdd, 0xc7,
    0xee, 0xcd, 0xdd, 0xee,
    0xee, 0xec, 0xbe, 0xee,

    // Frame 2
    0xee, 0x45, 0x55, 0xee,
    0xee, 0x67, 0x77, 0xee,
    0xee, 0x60, 0x70, 0xee,
    0xee, 0x67, 0x77, 0xee,
    0xee, 0xcd, 0xdd, 0xee,
    0xee, 0x7d, 0xdd, 0x7e,
    0xee, 0xcd, 0xdd, 0xee,
    0xee, 0xbe, 0xeb, 0xee,

    // Frame 3
    0xee, 0x45, 0x55, 0xee,
    0xee, 0x67, 0x77, 0xee,
    0xee, 0x60, 0x70, 0xee,
    0xee, 0x67, 0x77, 0xee,
    0xee, 0xcd, 0xdd, 0xee,
    0xee, 0xcc, 0x7d, 0xee,
    0xee, 0xcd, 0xdd, 0xee,
    0xeb, 0xee, 0xee, 0xce
};
```

Parfait… maintenant je te laisse convertir notre magnifique tileset pour t'assurer que tu as bien compris tout le processus de conversion :

![Tileset][tileset-frames]{: width="552" height="650" }
{: .figure }

<i class="far fa-4x fa-laugh-squint"></i>
{: .text-center }


Je plaisante… Tu peux le faire si ça t'amuse, mais tu conviendras sans trop de difficultés que tout ce processus de conversion représente pas mal de boulot, même si c'est amusant intellectuellement, n'est-ce pas ? Je te laisse imaginer ce que ça implique sur un jeu complet avec de très nombreux éléments graphiques… Ce serait beaucoup plus pratique si on pouvait s'appuyer sur un outil de conversion automatique, nan ? Et ben, rassure-toi… cet outil existe.  <i class="far fa-smile-wink"></i>

Et je te propose de le découvrir au chapitre suivant…



[gb-meta]:            https://github.com/Gamebuino/Gamebuino-META
[image]:              https://github.com/Gamebuino/Gamebuino-META/blob/master/src/utility/Graphics/Image.h
[image-constructors]: https://github.com/Gamebuino/Gamebuino-META/blob/f89185a785bdb03d14098c7faaa67eba3d3b630c/src/utility/Graphics/Image.h#L68-L90
[color]:              https://github.com/Gamebuino/Gamebuino-META/blob/f89185a785bdb03d14098c7faaa67eba3d3b630c/src/utility/Graphics/Graphics.h#L13-L30
[piskel]:             https://www.piskelapp.com/
[avatar-final]:       {{ site.baseurl }}/assets/figures/avatar-final-792x628.png
[tileset-frames]:     {{ site.baseurl }}/assets/figures/tileset-1104x1300.png
[16-bits-integer]:    {{ site.baseurl }}/assets/figures/16-bits-integer-788x212.png
