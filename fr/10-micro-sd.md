---
layout: default
lang: fr
lang_ref: micro-sd
nav_order: 10
title: La carte micro SD
permalink: /fr/la-carte-micro-sd
---

# Charger des images depuis la carte micro SD

Je n'ai pas abordé la question jusque-là, car son intérêt est assez limité, mais il existe néanmoins la possibilité de charger des images depuis la carte micro SD insérée dans le lecteur de ta Gamebuino.

Les formats d'images supportés sont les Windows Bitmap (BMP) à 4, 24 ou 32 bits de profondeur :

- Les BMP 4 bits seront convertis en images à 16 couleurs indexées.
- Les BMP 24 bits seront convertis en images RGB565.
- Les BMP 32 bits seront également convertis en images RGB565, à la différence près que le canal alpha (l'octet représentant le niveau de transparence de la couleur) sera converti avec la dichotomie suivante :
    - Si la transparence est supérieure à 50% (donc si la valeur de l'octet est supérieure à 127), alors le pixel sera considéré comme totalement transparent.
    - Sinon le pixel sera considéré comme totalement opaque.

La conversion est automatique et déclenchée lors de la première exécution de ton programme, tu n'as pas à t'en préoccuper. Il faudra néanmoins patienter lors du premier lancement pour que le processus de conversion ait le temps de s'effectuer.

Voici comment créer une image à partir d'un fichier BMP stocké sur la carte micro SD :

```cpp
Image image("path/to/file.bmp");
```

Le chemin d'accès au fichier `file.bmp` doit être exprimé de manière relative au contenu du dossier dans lequel est stocké le fichier binaire de ton programme.


## Hébergement des ressources graphiques sur la carte micro SD

Durant toute la phase de programmation, ton code est compilé puis transféré sur la mémoire Flash de la Gamebuino par l'intermédiaire de la connexion USB, sans que tu n'aies besoin de prévoir un dossier spécifique, à ce stade, pour l'héberger sur la carte micro SD. Tu peux néanmoins créer ce dossier de manière anticipée si tu souhaites y héberger des ressources multimedia destinées à être exploitées par ton programme. Par défaut, le nom de ce dossier est le même que celui de ton sketch `.ino`. Dans notre cas, on peut donc dès à présent créer un dossier `my-stunning-game` à la racine de la carte micro SD pour y héberger les images au format BMP que nous allons exploiter.

![Le dossier de ton projet sur la carte micro SD][root]{: width="316" height="301" .shadow .rounded }
{: .figure }

Le dossier de ton projet sur la carte micro SD
{: .caption }

Télécharge ensuite ces deux fichiers BMP :

[gamebuino.bmp][gamebuino]{: .btn .btn-purple download="" style="margin-right:.5em;" }
[splash.bmp][splash]{: .btn .btn-purple download="" }
{: .text-center }

Et range les dans le dossier `my-stunning-game` sur ta carte micro SD :

![Ressources graphiques du projet sur la carte micro SD][folder]{: width="316" height="339" .shadow .rounded }
{: .figure }

Ressources graphiques du projet sur la carte micro SD
{: .caption }

Tu peux maintenant insérer la carte micro SD dans ta Gamebuino.


## Chargement et affichage des images stockées sur la carte micro SD

Le fichier `gamebuino.bmp` est une version du logo Gamebuino adaptée pour recouvrir l'écran de la META. Pour afficher ce logo au lancement du programme, on peut procéder ainsi :

examples/example-17.h
{: .filename }

```cpp
#include <Gamebuino-Meta.h>

Image image("gamebuino.bmp");

void setup() {
    gb.begin();
    gb.display.drawImage(0, 0, image);
}

void loop() {
    gb.waitForUpdate();
}
```

{% include meta-demo.html src="assets/screen/example-17.png" %}

Le logo chargé depuis la carte micro SD
{: .caption }

Tu dois prendre en compte le fait que le chargement de l'image occupera nécessairement de l'espace en RAM pour créer l'instance `image` de la classe `Image`. Il est en général préférable de stocker ses images en mémoire Flash en les convertissant en tableaux de données constantes comme nous l'avons vu dans les chapitres précédents. D'autant que le chargement des images à partir de la carte micro SD sera beaucoup plus lent qu'à partir de la mémoire Flash.

Mais il peut s'avérer très utile de pouvoir stocker une **séquence animée** trop lourde pour être hébergée directement dans la mémoire Flash, mais que l'on souhaite afficher malgré tout dans le jeu comme un écran d'accueil ou un écran de transition entre deux niveaux par exemple. Dans ce cas, on utilisera une fonctionnalité particulière offerte par `gb.display` qui consiste à *streamer* l'animation directement depuis la carte micro SD.

Pour activer cette possibilité, il va falloir ajouter une directive de configuration :

config-gamebuino.h
{: .filename }

```cpp
#define DISPLAY_MODE DISPLAY_MODE_RGB565
#define STRICT_IMAGES 1
```

Le fichier `splash.bmp` est une séquence animée de 97 frames avec une résolution de 80x64 pixels. Tu te doutes bien qu'une telle séquence ne pourrait évidemment pas être stockée en RAM, ni même sur la mémoire Flash. Pourtant, tu vas voir qu'on peut quand même l'afficher à l'écran en invoquant une méthode d'initialisation de `gb.display` particulière.

Il existe en réalité [4 constructeurs ou initialiseurs][init] de la classe `Image`, tous spécialisés pour engendrer des instances associées à la lecture de fichiers BMP sur la carte micro SD :

```cpp
// SD constructors
Image(char* filename, uint8_t fl = DEFAULT_FRAME_LOOP);
void init(char* filename, uint8_t fl = DEFAULT_FRAME_LOOP);
Image(uint16_t w, uint16_t h, char* filename, uint8_t fl = DEFAULT_FRAME_LOOP);
void init(uint16_t w, uint16_t h, char* filename, uint8_t fl = DEFAULT_FRAME_LOOP);
```

Voyons par exemple comment utiliser l'un d'entre eux avec le fichier `splash.bmp` :

examples/example-18.h
{: .filename }

```cpp
#include <Gamebuino-Meta.h>
#include "../assets/rgb565.h"

void setup() {
    gb.begin();
    gb.display.init("splash.bmp");
}

void loop() {

    gb.waitForUpdate();
    gb.display.nextFrame();

    gb.display.print(8, 16, "My Stunning Game");
    gb.display.drawImage(36, 40, SPRITE_DATA);

}
```

Compile et transfère ce programme sur ta Gamebuino. Tu devrais constater qu'elle semble bloquer sur l'écran d'initialisation qui précède le lancement effectif de ton programme. C'est normal… et ça va même durer plusieurs secondes ! Elle est justement en train de convertir ton fichier BMP au format RGB565 qui est directement exploitable par `gb.display`. Le résultat de la conversion est ensuite enregistré sur la carte micro SD dans un fichier binaire `splash.GMV`, au format singulier **GMV**, qui est propre à la META. Par conséquent, lors du prochain lancement du programme, la conversion ne sera plus à faire et le fichier GMV sera directement lu pour initialiser l'image `gb.display` sans délai.

Une fois la conversion terminée, ton programme se lance, et tu devrais obtenir ceci :

{% include meta-demo.html src="assets/screen/example-18.gif" %}

Un écran d'accueil animé par lecture d'un fichier sur la carte micro SD
{: .caption }

La capture que j'ai réalisée ici est animée à la fréquence par défaut de 25 fps. Tu verras que sur ta console, le raffraîchissement est un peu plus lent. En effet, la lecture continuelle du fichier GMV sur la carte micro SD est moins rapide que si les frames étaient directement lues en mémoire Flash. D'ailleurs, tu devrais pouvoir observer un clignottement permanent de la petite LED témoin de couleur orange qui est située à proximité du lecteur micro SD.

Eteins ta Gamebuino et insère la carte micro SD dans un lecteur connecté à ton ordinateur. Si tu regardes le contenu du dossier `my-stunning-game`, tu devrais constater l'apparition des nouveaux fichiers GMV :

![Apparition des fichiers GMV sur la carte micro SD][gmv-files]{: width="316" height="377" .shadow .rounded }
{: .figure }

Apparition des fichiers GMV sur la carte micro SD
{: .caption }


Il est peu probable que tu utilises ce procédé pour un écran de jeu à proprement parler, mais plutôt pour des séquences intermédiaires du fait de la lenteur afférente au chargement des images sur la carte micro SD.

Cette technique reste toutefois très utile pour animer des séquences dont le poids proscrit leur chargement en RAM. Chaque frame de la séquence est chargée une à une et directement transférée sur le tampon graphique de `gb.display` sans occuper davantage de mémoire. C'est l'avantage de cette technique de *streaming*.

À chaque cycle de la fonction `loop()` on déclenche le chargement de la frame suivante par :

```cpp
gb.display.nextFrame();
```

Puis, on peut tracer tout ce qu'on veut par-dessus avec les méthodes standards de `gb.display`…

> **Remarque**
> 
> C'est précisément cette technique qui est employée par le `loader` pour afficher l'écran de démarrage `TITLESCREEN.BMP` que l'on retrouve dans la plupart des dossiers d'applications pour la META. Tu ne le savais peut-être pas, mais tu peux effectivement avoir un écran de démarrage animé si ton fichier BMP incorpore une séquence de frames de la taille de l'écran de ta Gamebuino…  <i class="far fa-smile-wink"></i>



[root]:      {{ site.baseurl }}/assets/figures/micro-sd-root-632x602.png
[folder]:    {{ site.baseurl }}/assets/figures/micro-sd-game-folder-632x678.png
[gmv-files]: {{ site.baseurl }}/assets/figures/micro-sd-gmv-632x754.png
[gamebuino]: {{ site.baseurl }}/assets/download/gamebuino.bmp
[splash]:    {{ site.baseurl }}/assets/download/splash.bmp
[init]:      https://github.com/Gamebuino/Gamebuino-META/blob/f89185a785bdb03d14098c7faaa67eba3d3b630c/src/utility/Graphics/Image.h#L86-L90