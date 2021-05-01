---
layout: default
lang: fr
lang_ref: display-modes
nav_order: 2
title: Les modes d'affichage
permalink: /fr/les-modes-d-affichage
---

# Les modes d'affichage sur la META

Avant de se plonger dans la programmation et découvrir comment afficher et animer des images avec la META, nous devons examiner le périphérique d'affichage de la console et comprendre ce qui est mis à notre disposition par la bibliothèque [Gamebuino-META][gb-meta] pour exploiter ses capacités.


## Le matériel

La META incorpore un écran TFT 1.8" très répandu, le [ST7735R][ST7735R], que l'on retrouve sur bon nombre de périphériques électroniques, y compris sur d'autres consoles de jeux programmables comme le [PyGamer][pygamer] d'Adafruit.

![Adafruit-618][ada-618]{: .shadow }
![Adafruit-358][ada-4242]{: .shadow }
{: .figure-2col }

Écran TFT 1.8" couleur ST7735R avec une résolution de 160x128
{: .caption }

Ce petit écran dipose d'une mémoire interne pouvant stocker des informations caractérisant jusqu'à 162x132 pixels avec 18 bits de profondeur pour coder la couleur de chaque pixel. Plusieurs modes d'affichage sont disponibles, avec trois espaces colorimétriques différents : RGB444, RGB565 et RGB666. La META a été configurée pour exploiter une résolution de **160x128** pixels dans l'espace colorimétrique **RGB565** (5 bits pour le rouge, 6 bits pour le vert et 5 bits pour le bleu), c'est-à-dire avec une profondeur de 5+6+5 = 16 bits. Par conséquent, chaque pixel de l'écran peut endosser une couleur parmi 2<sup>16</sup> = **65 536**… Ce qui est déjà pas mal, et nous laisse le champ libre pour exprimer notre créativité. Voici un petit aperçu de la diversité des couleurs que l'on [peut obtenir][aoneill] sur la META :

{% include meta-demo.html src="/assets/screen/demo-colors.gif" %}

Effusion de couleurs sur l'écran de la Gamebuino META
{: .caption }

Les nuances entre ces différentes spécifications n'évoquent peut-être pas encore grand chose dans ton esprit, mais rassure-toi, on va éclaircir tout ça par la suite…


## Les modes d'affichage disponibles sur la META

L'exploitation directe d'un écran TFT comme le ST7735R est relativement complexe et nécessite d'intégrer de nombreuses notions qui sont nécessaires pour parcourir et comprendre sa [documentation technique][ST7735R]. Pour nous faciliter la tâche, des développeurs expérimentés ont pris la peine de coder des bibliothèques logicielles spécialisées, qui servent d'interfaces avec ces périphériques complexes, et nous simplifient donc considérablement les choses.

La bibliothèque [Gamebuino-META][gb-meta] fournit une API consistante dédiée à l'affichage et, en particulier, à l'affichage des images : [`gb.display`][gb-display]. Cette API permet de gérer 3 modes d'affichage spécifiques, parmi les 4 disponibles sur la META :

|   | Mode d'affichage                                          | Résolution  |  Espace colorimétrique  |
|:-:|:----------------------------------------------------------|:-----------:|:------------------------|
| 1 | `DISPLAY_MODE_RGB565`                                     |    80x64    | 2<sup>16</sup> couleurs |
| 2 | `DISPLAY_MODE_INDEX`                                      |   160x128   | 16 couleurs indexées    |
| 3 | `DISPLAY_MODE_INDEX_HALFRES`                              |    80x64    | 16 couleurs indexées    |
| 4 | <span style="color:#a00;">Mode réservé aux initiés</span> |   160x128   | 2<sup>16</sup> couleurs |

Le 4e mode est un peu plus complexe à gérer, et s'adresse aux plus expérimentés d'entre nous, car il ne peut être exploité avec l'API `gb.display`. Pour utiliser ce mode, il faudra plutôt se tourner vers une autre API, qui est aussi fournie par la bibliothèque [Gamebuino-META][gb-meta] : [`gb.tft`][gb-tft]. C'est d'ailleurs une adaptation de la bibliothèque [Adafruit-ST7735-library][ada-st7735] d'Adafruit. L'exploitation de l'API `gb.tft` dépasse le cadre de ce tutoriel et nous ne l'aborderons pas ici.

Bien, mais alors pourquoi distinguer tous ces modes d'affichage ? Qu'est-ce qui fait leur spécificité et comment savoir lequel choisir ?


## Impacts sur la gestion de la mémoire

Prenons l'exemple du mode d'affichage n°4 qui nous permet d'exploiter pleinement les capacités du ST7735R sur la META. Dans ce mode, l'affichage est discrétisé par une matrice de 160x128 pixels dans l'espace colorimétrique RGB565. C'est à dire que 5+6+5 = 16 bits sont nécessaires pour stocker le code binaire de la couleur de chaque pixel.

Le référentiel spatial et colorimétrique de l'écran se présente de la façon suivante :

![META screen encoding][meta-screen]{: width="390" height="292" }
{: .figure }

Discrétisation de l'affichage sur la META
{: .caption }

Chaque pixel est caractérisé par :

- une coordonnée horizontale **x** qui prend ses valeurs dans l'intervalle [0, 159],
- une coordonnée verticale **y** qui prend ses valeurs dans l'intervalle [0, 127],
- une couleur encodée sur 16 bits, c'est-à-dire 2 octets.

L'origine du référentiel spatial se trouve dans le coin supérieur gauche, aux coordonnées (0, 0).

Afficher un pixel à l'écran, aux coordonnées **(x, y)**, revient à affecter une couleur à cette unité de surface. Autrement dit, nous aurons besoin d'un espace mémoire de 2 octets pour écrire et stocker les 16 bits du code couleur associé à ce pixel. Dans la mesure où la couleur de chaque pixel devra être stockée en mémoire, on peut en déduire la taille de cet espace de la façon suivante :

160 x 128 x 16 bits = 160 x 128 x 2 octets = 40 960 octets = 40 ko
{: .formula }

Cet espace mémoire correspond à une zone de *préparation* de l'affichage que l'on appelle un **tampon graphique**. Cette zone sert à collecter les informations qui devront ensuite être transmises au contrôleur d'affichage pour être finalement visibles à l'écran et se traduire par une coloration particulière de chaque unité lumineuse qui compose l'écran. L'écran TFT fait partie de la famille des écrans LCD dans lesquels chaque pixel est commandé par un minuscule semi-conducteur qui contrôle la quantité de lumière devant passer. La lumière est émise à l'aide de 3 diodes électroluminescentes : une pour le rouge, une pour le vert et une pour le bleu. C'est le mélange de ces 3 couleurs primaires, avec des niveaux d'intensité lumineuse différenciés qui permet d'obtenir la couleur désirée, par composition additive.

Le tampon graphique servant à préparer l'affichage est nécessairement stocké en RAM durant l'exécution du programme, et est susceptible de changer d'état à tout moment, au fil de l'exécution des instructions qui viendront le modifier dynamiquement pour afficher ce que tu souhaites à l'écran, au moment où tu le souhaites.

La bibliothèque [Gamebuino-META][gb-meta] se charge elle-même de gérer ce tampon graphique et expose des routines simplifiées qui te permettent facilement de tracer une ligne, dessiner un rectangle ou un cercle, ou même carrément reproduire une image que tu auras dessinée dans ton éditeur de sprite préféré. C'est justement le rôle de l'API spécialisée `gb.display`. Elle se charge de traduire les directives graphiques que tu invoques par la mise à jour appropriée de l'état du tampon graphique. Tu n'as pas à t'en soucier, même s'il reste toujours possible d'écrire directement sur ce tampon si tu sais comment faire.

Bien, revenons à notre petit calcul… Si l'on souhaite exploiter la totalité de l'espace colorimétrique RGB565 avec une résolution de 160x128 pixels, nous avons besoin d'un tampon graphique de **40 ko**… Or, tu n'es pas censé ignorer que la META ne dispose, en tout et pour tout, que de **32 ko** de RAM. On est donc confronté à un problème de taille : comment faire rentrer 40 ko dans 32 ko ?

Le tampon graphique de `gb.display` ne le permet pas, tout simplement. C'est précisément pour cette raison que ce mode d'affichage ne peut être géré par l'API `gb.display`. Ce n'est pas impossible dans l'absolu… mais il faut ruser… et passer par l'API `gb.tft`, beaucoup plus rudimentaire, et donc plus compliquée à exploiter. C'est la raison pour laquelle ce mode d'affichage est réservé aux initiés.  <i class="far fa-smile-wink"></i>

Bon, et qu'en est-il des autres modes ?

### DISPLAY_MODE_RGB565

C'est le **mode d'affichage par défaut** (qui sera appliqué automatiquement si tu n'en spécifies aucun). Dans ce mode, pour contourner la problématique de l'espace mémoire insuffisant, la surface du référentiel spatial de l'écran est divisée par 4 en divisant ses dimensions horizontale et verticale par 2. Le tampon graphique est donc réduit à un espace mémoire permettant de gérer un affichage dans une résolution de 80x64 pixels, toujours dans l'espace colorimétrique RGB565, donc avec une profondeur de 16 bits par pixel :

80 x 64 x 16 bits = 80 x 64 x 2 octets = 10 240 octets = 10 ko
{: .formula }

Surface de pixels divisée par 4 &rArr; taille du tampon graphique divisée par 4… logique.  <i class="far fa-laugh-squint"></i>

L'API `gb.display` se chargera automatiquement de recouvrir les 160x128 pixels de l'écran avec les 80x64 pixels du tampon graphique par simple projection. Autrement dit, chaque pixel du tampon sera reproduit sur 4 pixels de l'écran, comme le montre la figure suivante :

![Projection du tampon graphique][screen-buffer]{: width="160" height="128" }
{: .figure }


### DISPLAY_MODE_INDEX

Ici, l'astuce est différente : on ne touche pas aux dimensions spatiales (on reste en 160x128), mais on négocie sur l'espace colorimétrique en diminuant drastiquement le nombre de couleurs disponibles. En effet, on va se limiter à une *palette* de 16 couleurs (chacune pouvant être choisie dans l'espace RGB565). Chaque couleur est associée à un *index* compris entre 0 et 15 dans une table de référence. Et c'est précisément cet index qu'on va reporter sur le tampon graphique aux coordonnées du pixel qui porte la couleur associée (1 seul octet suffit ici pour encoder un entier entre 0 et 15). Mais l'astuce va plus loin : pour coder un nombre entier entre 0 et 15, en réalité, on a juste besoin de 4 bits (2<sup>4</sup> = 16 possibilités, donc 16 couleurs), n'est-ce pas ? Or, sur un octet (8 bits), on peut stocker 2 paquets de 4 bits. Autrement dit, avec un seul octet, on peut coder les couleurs respectives de 2 pixels voisins ! Du coup, ça permet encore de diviser par 2 le nombre d'octets nécessaires pour encoder les couleurs des 160x128 pixels, puisqu'on les code par paires de 4 bits. Par conséquent :

160 x 128 x 4 bits = 160 x 128 x &frac12; octets = 10 240 octets = 10 ko
{: .formula }

Et on retombe sur nos pattes, comme dans le cas précédent.

### DISPLAY_MODE_INDEX_HALFRES

Ici, le raisonnement est le même avec une surface d'affichage divisée par 4 (on repasse en 80x64) :

80 x 64 x 4 bits = 80 x 64 x &frac12; octets = 2 560 octets = 2.5 ko
{: .formula }

Sans surprise : surface divisée par 4 &rArr; taille du tampon divisée par 4.   <i class="fas fa-medal"></i>

La recopie sur l'écran se fait par projection, comme dans le cas de `DISPLAY_MODE_RGB565`.


## Récapitulatif

Pour résumer, voici les contraintes techniques que tu dois garder en tête avant de choisir le mode d'affichage le plus approprié pour réaliser ton jeu. En effet, tu dois le choisir une bonne fois pour toutes, dès le départ. Il sera difficile de revenir en arrière…


| Mode d'affichage                                          | Résolution  |  Espace colorimétrique  |            Taille du tampon            |
|:----------------------------------------------------------|:-----------:|:------------------------|:--------------------------------------:|
| `DISPLAY_MODE_RGB565`                                     |    80x64    | 2<sup>16</sup> couleurs |                  10 ko                 |
| `DISPLAY_MODE_INDEX`                                      |   160x128   | 16 couleurs indexées    |                  10 ko                 |
| `DISPLAY_MODE_INDEX_HALFRES`                              |    80x64    | 16 couleurs indexées    |                  2.5 ko                |
| <span style="color:#a00;">Mode réservé aux initiés</span> |   160x128   | 2<sup>16</sup> couleurs | <span style="color:#a00;">40 ko</span> |
{: .indexed }

> **Attention**
>
> Avec les modes d’affichage à couleurs indexées, tu n’as que 16 couleurs en tout et pour tout ! Ça veut dire que tous les éléments graphiques de ton jeu devront partager l’unique palette de couleurs que tu auras définie. Donc, n’oublie pas d’en tenir compte au moment où tu dessineras tes sprites et les éléments de décor de ton jeu…
>
> Et oui… on économise de l’espace mémoire, mais on perd du même coup le potentiel des couleurs qu’offre l'espace RGB565…


## Initialisation du projet

Pour démarrer notre projet, je te propose de créer un nouveau sketch avec l'IDE Arduino, et de le nommer `my-stunning-game`. Ceci aura pour effet de générer le dossier suivant :

![Création du projet][project-init]{: width="316" height="222" .shadow .rounded }
{: .figure }

L'IDE Arduino impose de nommer le dossier du projet avec le même nom que le sketch principal.


## Comment configurer le mode d'affichage de ton projet

Le mode d'affichage doit être défini dès le départ, quand tu démarres ton projet. Pour cela, tu dois créer un fichier de configuration, à la racine de ton projet, nommé `config-gamebuino.h`.

![Configuration du projet][project-config]{: width="316" height="222" .shadow .rounded }
{: .figure }

Tu dois ensuite y définir la macro `DISPLAY_MODE`, en lui associant la valeur correspondant au mode d'affichage que tu souhaites appliquer à ton projet. La bibliothèque [Gamebuino-META][gb-meta] fournit les trois macros suivantes pour cela :

- `DISPLAY_MODE_RGB565`
- `DISPLAY_MODE_INDEX`
- `DISPLAY_MODE_INDEX_HALFRES`

Pour appliquer le mode d'affichage 80x64 à 2<sup>16</sup> couleurs (RGB565 au complet), déclare simplement :

config-gamebuino.h
{: .filename }

```cpp
#define DISPLAY_MODE DISPLAY_MODE_RGB565
```

Il s'agit ici du mode par défaut. Donc si ton projet ne comporte pas de fichier `config-gamebuino.h`, ou si la macro `DISPLAY_MODE` n'y est pas précisée, alors le mode d'affichage sera automatiquement défini par `DISPLAY_MODE_RGB565`.

Pour appliquer le mode d'affichage 160x128 à 16 couleurs indexées, il faudra déclarer :

config-gamebuino.h
{: .filename }

```cpp
#define DISPLAY_MODE DISPLAY_MODE_INDEX
```

Dernière possibilité, pour appliquer le mode d'affichage 80x64 à 16 couleurs indexées, déclare :

config-gamebuino.h
{: .filename }

```cpp
#define DISPLAY_MODE DISPLAY_MODE_INDEX_HALFRES
```

## Demonstration

Voici un petit code de démonstration pour vérifier l'espace mémoire disponible en RAM :

my-stunning-game.ino
{: .filename }

```cpp
#include <Gamebuino-Meta.h>

const char *FORMAT = "FREE RAM: %u";

void setup() {
    gb.begin();
}

void loop() {
    gb.waitForUpdate();
    gb.display.clear();
    gb.display.setFontSize(1);
    gb.display.printf(2, 2, FORMAT, gb.getFreeRam());
}
```

Teste le dans chacun des modes d'affichage, et tu devrais obtenir les résultats suivants :

{% include meta-demo.html src="assets/screen/free-ram-DISPLAY_MODE_RGB565.png" %}

DISPLAY_MODE_RGB565
{: .caption }

{% include meta-demo.html src="assets/screen/free-ram-DISPLAY_MODE_INDEX.png" %}

DISPLAY_MODE_INDEX
{: .caption }

{% include meta-demo.html src="assets/screen/free-ram-DISPLAY_MODE_INDEX_HALFRES.png" %}

DISPLAY_MODE_INDEX_HALFRES
{: .caption }

Les valeurs observées sont identiques pour `DISPLAY_MODE_RGB565` et `DISPLAY_MODE_INDEX`, ce qui est logique puisque la taille du tampon graphique est identique dans les deux cas. Par contre, si tu calcules la différence entre la valeur observée pour `DISPLAY_MODE_INDEX_HALFRES` et celle pour les deux cas précédents, tu remarqueras que :

22 839 - 15 159 = 7 680 octets = 7.5 ko
{: .formula }

Ce différentiel correspond exactement à la différence de taille de leurs tampons respectifs :

10 ko - 2.5 ko = 7.5 ko
{: .formula }

Maintenant que les choses sont plus claires concernant le matériel de la META et sur les possibilités offertes par l'API `gb.display`, nous allons voir dans le prochain chapitre comment préparer les images que l'on souhaitera afficher à l'écran.



[gb-meta]:     https://github.com/Gamebuino/Gamebuino-META
[ST7735R]:     https://www.displayfuture.com/Display/datasheet/controller/ST7735R.pdf
[pygamer]:     https://www.adafruit.com/product/4242
[pokitto]:     https://i.ytimg.com/vi/iaM5j0Jr4f8/maxresdefault.jpg
[ada-618]:     https://cdn-shop.adafruit.com/970x728/618-07.jpg
[ada-4242]:    https://cdn-shop.adafruit.com/970x728/4242-04.jpg
[aoneill]:     https://gamebuino.com/fr/creations/high-resolution-without-gb-display
[gb-display]:  https://github.com/Gamebuino/Gamebuino-META/blob/master/src/utility/Graphics/Graphics.h
[gb-tft]:      https://github.com/Gamebuino/Gamebuino-META/blob/master/src/utility/Display-ST7735/Display-ST7735.h
[ada-st7735]:  https://github.com/adafruit/Adafruit-ST7735-Library

[meta-screen]:    {{ site.baseurl }}/assets/figures/screen-encoding-780x584.png
[screen-buffer]:  {{ site.baseurl }}/assets/figures/screen-buffer-320x256.png
[project-init]:   {{ site.baseurl }}/assets/figures/project-init-632x444.png
[project-config]: {{ site.baseurl }}/assets/figures/project-config-632x444.png