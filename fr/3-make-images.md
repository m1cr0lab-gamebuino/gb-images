---
layout: default
lang: fr
lang_ref: make-images
nav_order: 3
title: Création des images
permalink: /fr/creation-des-images
---

# Comment créer tes images pour la META

Quand on se lance dans la création d'un jeu vidéo, une étape importante concerne la conception des éléments graphiques. Fondamentalement, la création d’un jeu vidéo résulte de la réflexion d’un concept qui prend forme grâce à la combinaison de pixels et de code informatique. La programmation peut-être considérée comme un art qui va permettre de donner vie au jeu.

De nombreux outils de création graphique existent pour réaliser tous les éléments graphiques qui vont composer l'univers de ton jeu, qu'ils soient statiques ou animés. Je t'en propose quelques-uns qui ont retenu mon attention (mais il en existe beaucoup d'autres, plus ou moins sophistiqués) :

- [Aseprite][aseprite]
- [Piskel][piskel]
- [Pyxel Edit][pyxeledit]
- [Tiled][tiled]

Je te laisse les découvrir par toi-même. Ça n'est pas l'objet de ce tutoriel…

Pour cadrer un peu mon propos, je vais prendre l'exemple d'un *platformer* (un jeu de plateformes), dans lequel on va devoir contrôler un personnage, notre avatar, pour lui indiquer comment se déplacer dans son environnement. On va donc avoir besoin d’un élément graphique qui soit animé, pour représenter visuellement les actions exécutées par l’avatar. Dans le domaine du jeu vidéo, on appelle ça un **sprite**.

Considérons le sprite suivant, qui est relativement simple, et qui permet d'animer notre avatar par étapes successives pour donner l'illusion qu'il marche ou qu'il court (selon la vitesse à laquelle on enchaîne les étapes) :

![Animation][avatar-animation]{: width="64" height="64" style="margin-right:32px" }
![Spritesheet][avatar-spritesheet]{: width="256" height="64" }
{: .figure style="white-space:nowrap;" }

Décomposition image par image d'une animation
{: .caption }

L’animation est réalisée à l’aide d’une séquence d’images (qu'on appelle aussi des *frames*), qui sont affichées successivement pendant un court instant, dans un ordre bien précis. C’est exactement le même principe que l’on utilise au cinéma. C’est aussi le même principe qui est utilisé pour créer des GIF animés par exemple. Mais pour animer notre avatar dans le jeu, on a besoin d’avoir un contrôle total sur l’ordre d’enchaînement des frames, ainsi que sur la vitesse d’exécution de l’animation. C'est là que le code intervient.

On va voir ensemble comment donner vie à notre avatar en affichant le sprite à l'écran, dans chacun des modes d'affichage qui sont à notre diposition. Mais avant cela, je te propose d'analyser comment est construit le sprite.


## Anatomie d'un sprite

Commençons par examiner notre sprite d'un peu plus près pour découvrir de quoi il est fait :

![Colormap des frames][avatar-colormap]{: width="682" height="152" }
{: .figure }

Décomposition des images en grilles de pixels
{: .caption }

Il est constitué d'une séquence de 4 frames, chacune inscrite dans une grille de 8x8 pixels. Chaque pixel est caractérisé par une couleur au format `#RRGGBB`. Il s'agit d'un format très courant, que l'on retrouve sur la grande majorité des outils de création graphique, permettant de situer une couleur dans l'espace colorimétrique RGB888. Cela signifie que l'intensité de chacun des canaux primaires rouge, vert et bleu qui composent la couleur est codée sur 1 octet (8 bits). Puisqu'on a 3 canaux, alors il faut 3 octets (3x8 = 24 bits) pour coder la couleur. Chaque octet pouvant être lui-même décomposé en une paire de 4 bits, on obtient alors 3 paires de 4 bits de la forme :

R<sub>1</sub>R<sub>2</sub> G<sub>1</sub>G<sub>2</sub> B<sub>1</sub>B<sub>2</sub>
{: .text-center}

Dans ce format, chaque groupe de 4 bits est simplement écrit sous sa forme hexadécimale.

Prenons par exemple la couleur d'indice **3** <span style="display:inline-block;width:1em;height:1em;margin-bottom:-.2em;border:1px solid #000;background-color:#e8c47d;"></span> `#E8C47D` dans la palette des couleurs qui composent notre sprite. Au passage, pour préciser qu'il s'agit d'une valeur exprimée en hexadécimal, on lui préfèrera l'écriture `0xE8C47D`, qui est celle qu'on utilise avec le langage C++. Cette couleur au format RGB888 peut être décomposée en 3 octets (3x8 bits), comme nous venons de le voir, tout comme en 6 *digits* hexadécimaux (6x4 bits), de la façon suivante :

![Anatomie d'une couleur RGB888][rgb888-color]{: width="578" height="130" }
{: .figure }

Anatomie d'une couleur RGB888
{: .caption }


## Conversion de RGB888 vers RGB565

Si les outils de création graphique nous permettent de travailler dans l'espace colorimétrique RGB888, il n'en demeure pas moins que l'écran de la META ne sait traiter que des couleurs exprimées dans l'espace RGB565. On va donc devoir **convertir** les couleurs de la palette dans cet espace.

Pour cela, nous allons devoir projeter les 3 octets de RGB888 (24 bits) sur les 2 octets de RGB565 (16 bits). La méthode de projection est très simple ici, il suffit d'ôter les bits de poids faible sur chacun des octets représentant les canaux primaires. On retire les trois derniers bits de l'octets du canal rouge (8-3 = 5 bits), les 2 derniers bits de l'octet du canal vert (8-2 = 6 bits) et les 3 derniers bits de l'octet du canal bleu (8-3 = 5 bits) :

![Conversion de RGB888 vers RGB565][rgb888-to-rgb565]{: width="585" height="294" }
{: .figure }

Conversion de RGB888 vers RGB565
{: .caption }

On obtient bien une séquence de 16 bits que l'on peut alors réinterpréter sous la forme de 2 paires de 4 bits, qu'il suffit de réécrire sous leur forme hexadécimale… et le tour est joué !  <i class="far fa-smile-wink"></i>

Maintenant, à toi de jouer : je te propose de convertir chaque couleur de la palette pour t'assurer que tu as bien compris le mécanisme de conversion. Pour vérifier tes résultats, tu peux t'appuyer sur le petit convertisseur automatique suivant (inutile de préfixer la valeur à convertir par `#` ou `0x`) :

<div class="form" style="display:flex;">
    <form style="margin:auto;">
        <input type="text" id="rgb888" name="rgb888" value="" size="7" maxlength="6" onkeypress="return /[0-9a-f]/i.test(event.key)" placeholder="RGB888" />
        <button type="button" id="convert" name="button" class="btn btn-primary">Convert</button>
        <input type="text" id="rgb565" name="rgb565" value="" size="7" placeholder="RGB565" style="cursor:not-allowed" readonly />
    </form>
</div>

<script>
    rgb888.addEventListener('focus', function () {
        rgb565.value = '';
    });
    convert.addEventListener('click', function(event) {
        let c = parseInt(rgb888.value, 16);
        let r = ((c >> 8) & 0xf800) | ((c >> 5) & 0x7e0) | ((c >> 3) & 0x1f);
        rgb888.value = c.toString(16).padStart(6, '0');
        rgb565.value = r.toString(16).padStart(4, '0');
    });
</script>

Une fois les calculs effectués, tu devrais retrouver les valeurs suivantes :

![Palette des couleurs au format RGB565][avatar-colormap-565]{: width="676" height="152" }
{: .figure }

Palette des couleurs au format RGB565
{: .caption }


## Gestion de la transparence

Il reste un dernier point important à aborder pour finaliser notre sprite, il s'agit de la *transparence*. En effet, les frames qui composent le sprite comportent des pixels auxquels on n'a affecté aucune couleur particulière. Et pour cause, puisque ces pixels devront rester transparents à l'affichage. Autrement dit, si le sprite est placé au premier plan de l'affichage, comme sur un calque, on doit pouvoir distinguer ce qui se trouve « derrière » lui, au niveau des zones transparentes.

L'API `gb.display` ne permet pas de gérer finement le niveau de transparence, mais considère qu'un pixel est soit totalement opaque, soit totalement transparent. Mais même si cette propriété reste rudimentaire (et même binaire au sens du tout ou rien), il faut fournir un moyen à `gb.display` de déterminer ce qui doit être considéré comme transparent ou pas.

La solution adoptée est simple : il suffit d'appliquer une couleur spécifique (qui sera considérée comme la couleur de transparence) aux pixels qui doivent être considérés comme transparents et qui ne seront donc pas affichés à l'écran. Cela sous-entend que cette couleur ne doit pas être utilisée en tant que telle sur le sprite. Elle doit être uniquement réservée à la caractérisation des zones transparentes de l'image. En général, on choisit une couleur qui tranche avec le reste de la palette pour bien la distinguer des autres couleurs. Le rose <span style="display:inline-block;width:1em;height:1em;margin-bottom:-.2em;border:1px solid #000;background-color:#f0f;"></span> ou le vert <span style="display:inline-block;width:1em;height:1em;margin-bottom:-.2em;border:1px solid #000;background-color:#0f0;"></span> sont couramment employés pour jouer ce rôle. Mais tu peux choisir la couleur qui te convient.

Ici, nous choisirons le rose <span style="display:inline-block;width:1em;height:1em;margin-bottom:-.2em;border:1px solid #000;background-color:#f0f;"></span> `0xFF00FF` qui correspond à la valeur `0xF81F` dans RGB565. Note bien que cette couleur de transparence **doit** être prise en compte dans la palette des couleurs, même si elle ne sera jamais visible à l'écran. Donc sur une palette limitée à 16 couleurs, pour les modes d'affichage à couleurs indexées, cela a toute son importance, puisqu'il ne te reste que les 15 autres pour colorer tes éléments graphiques !

![Nouvelle palette avec la couleur de transparence][avatar-transparency]{: width="676" height="168" }
{: .figure }

Nouvelle palette avec la couleur de transparence
{: .caption }

Il nous reste à exporter le sprite sur une seule et même planche (qu'on appelle une *spritesheet*), au format PNG par exemple, sur laquelle seront juxtaposées toutes les frames qui composent l'animation. Les logiciels de création de sprites comme [Piskel][piskel] permettent de réaliser cet export très simplement :

![Export de la spritesheet avec Piskel][piskel-export]{: width="640" height="461" .shadow .rounded }
{: .figure }

Export de la spritesheet avec Piskel
{: .caption }

Tu vois qu'on peut paramétrer l'agencement des frames en spécifiant le nombre de colonnes ou de lignes sur lesquelles on souhaite les répartir. Ici, j'ai choisi d'exporter la planche sur 1 colonne de 4 lignes. Mais tu peux choisir la configuration que tu veux.

![Différents agencements de la spritesheet][spritesheet-layout]{: width="347" height="256" }
{: .figure }

Différents agencements de la spritesheet
{: .caption }

Voilà, notre sprite est maintenant finalisé, mais notre travail ne s'arrête pas là…


## Tileset & Tile Mapping

Dans ton jeu, tu devras également prévoir des éléments graphiques pour habiller la scène de jeu avec un décor. Et ces éléments ne seront pas nécessairement animés comme les sprites. La plupart seront même statiques. Certains éléments représenteront des objets avec lesquels ton avatar pourra ou non entrer en interaction. Certains objets ne seront là que pour figurer dans le décor. Le décor pourra être lui-même construit avec des éléments graphiques qui seront réutilisés de nombreuses fois, comme des murs, des dalles, des passerelles, des échelles, etc., comme le montre l'écran ci-dessous :

{% include meta-demo.html src="/assets/screen/game-scene-480x384.png" scale="150" %}

Illustration d'une scène de jeu relativement sommaire
{: .caption }

Tu vas donc devoir préparer ce qu'on appelle des **tiles** (des tuiles) pour tapisser ta scène de jeu en fonction de l'ambiance que tu souhaites lui donner, et de ce que devra y accomplir ton avatar. Cet ensemble de tiles s'appelle un **tileset**, et l'agencement de ces tiles sur la scène de jeu s'appelle une **tilemap**.

La construction d'un tileset repose sur la définition des tiles *unitaires* qui vont permettre de construire l'ensemble du décor. Sur la scène de jeu présentée ci-dessus, on peut identifier 4 tiles élémentaires de 16x8 pixels qui permettent à eux-seuls de reconstituer toute la scène :

![Tileset][tileset]{: width="552" height="650" }
{: .figure }

L'agencement du tileset s'effectue exactement de la même manière que celui d'une spritesheet. En général, on rassemble les tiles qui sont adjacents sur la tilemap pour mieux reconnaître les ensembles qu'ils forment une fois groupés.


## Palette des couleurs finales

La prise en compte du tileset n'a aucune importance du point de vue des couleurs dans le mode d'affichage `DISPLAY_MODE_RGB565`, par contre il en a beaucoup plus dans le cas des modes d'affichages à 16 couleurs indexées. Il faut donc veiller à ce que la définition de tous tes sprites et de tous tes tilesets n'implique pas plus de 16 couleurs au total. C'est bien le cas ici : je n'ai finalement utilisé que 15 couleurs au total. Voici la nouvelle palette de couleurs, au complet, qui regroupe celles du sprite et celles de tous les tiles réunis :

![Palette des couleurs du jeu][game-palette]{: width="82" height="248" }
{: .figure }

Palette des couleurs du jeu
{: .caption }

Tu remarqueras cependant que les indices de couleurs des pixels qui composent le sprite ne sont plus les mêmes avec cette nouvelle palette. J'aurais pu faire en sorte de conserver les mêmes, mais j'ai pensé que ça te plairait d'avoir à recalculer les frames de la spritesheet… Le mobile est purement pédagogique.  <i class="far fa-laugh-squint"></i>


Voilà, tous nos éléments graphiques sont définis. Nous devons ensuite les convertir dans un format que l'on pourra exploiter par la programmation pour les afficher à l'écran avec `gb.display`, et c'est ce que nous allons voir au prochain chapitre.  <i class="far fa-smile-wink"></i>



[aseprite]:            https://www.aseprite.org/
[piskel]:              https://www.piskelapp.com/
[pyxeledit]:           https://pyxeledit.com/
[tiled]:               https://www.mapeditor.org/
[avatar-animation]:    {{ site.baseurl }}/assets/figures/avatar-animation-128x128.gif
[avatar-spritesheet]:  {{ site.baseurl }}/assets/figures/avatar-spritesheet-512x128.png
[avatar-colormap]:     {{ site.baseurl }}/assets/figures/avatar-colormap-1364x304.png
[rgb888-color]:        {{ site.baseurl }}/assets/figures/rgb888-color-1156x260.png
[rgb888-to-rgb565]:    {{ site.baseurl }}/assets/figures/rgb888-to-rgb565-1170x588.png
[avatar-colormap-565]: {{ site.baseurl }}/assets/figures/avatar-colormap-rgb565-1352x304.png
[avatar-transparency]: {{ site.baseurl }}/assets/figures/avatar-transparency-1352x336.png
[piskel-export]:       {{ site.baseurl }}/assets/figures/piskel-export-1280x922.gif
[spritesheet-layout]:  {{ site.baseurl }}/assets/figures/spritesheet-layout-694x512.png
[tileset]:             {{ site.baseurl }}/assets/figures/tileset-1104x1300.png
[game-palette]:        {{ site.baseurl }}/assets/figures/game-palette-164x484.png