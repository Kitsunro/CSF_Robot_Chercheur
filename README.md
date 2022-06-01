# **ESSAIM DE ROBOTS.**

Ce projet est réalisé dans le cadre de l'UE Communication Sans Fil au cours de la première année de licence à l'Université de Valrose à Nice.

## Description du projet
### Contexte
Ce projet est une introduction à la robotique en essaim. Cette branche de la robotique a pour but de concevoir des systèmes plus ou moins intelligents composés de plusieurs individus simplistes. C’est par la communication et l’effet de groupe, tout ça en étant régis par des règles simples, que l’essaim va se comporter intelligemment.

### Principe du projet
La problématique du projet est la suivante : *Comment concevoir un système répondant aux grands principes de la robotique en essaim tout en étant capable d’être facilement amélioré ?*
Dans ce projet, nous nous intéressons particulièrement à la gestion de plusieurs robots. Chaque individu de l’essaim est une copie simpliste des autres.
Ainsi, nous faisons se déplacer dans une arène deux robots pour quadriller le plus vite possible cet espace.


## Fabrication
### Matériels nécessaires
#### Pour un robot :
- Carte [UCA](https://github.com/FabienFerrero/UCA21) ou [carte Arduino](https://www.gotronic.fr/art-carte-arduino-nano-12422.htm) (NANO ou UNO)
- Si vous utiliser une carte arduino il vous faudra un module de [communication LoRa](https://www.gotronic.fr/art-shield-firebeetle-lora-tel0121-27836.htm).
- 2 Servomoteur [FS90R](https://www.gotronic.fr/art-servomoteur-fs90r-25838.htm)
- Boitier imprimer en 3d
- [Roues pour les servomoteurs](https://www.gotronic.fr/art-roue-pour-servomoteur-fs90r-25856.htm)
- Fils de connexions
- Breadboard

#### Pour le boitier de commande :
- Carte [UCA](https://github.com/FabienFerrero/UCA21) ou [carte Arduino](https://www.gotronic.fr/art-carte-arduino-nano-12422.htm) (NANO ou UNO)
- Si vous utiliser une carte arduino il vous faudra un module de [communication LoRa](https://www.gotronic.fr/art-shield-firebeetle-lora-tel0121-27836.htm).

### Construction des robots
#### Etape 1 :
Commençons par imprimer en 3d le modèle du boitier qui acceuillera les robots par la suite.
Pour cela, vous pouvez télécharger le [modèle 3d](https://github.com/Kitsunro/CSF_Robot_Chercheur/blob/main/Impression3D/Boitier1.STL) présent sur le github ou confectionner le votre.
![](https://github.com/Kitsunro/CSF_Robot_Chercheur/blob/main/Impression3D/Capture%20d%E2%80%99%C3%A9cran%202022-05-30%20185653.png)

#### Etape 2 :
Ensuite nous allons passer au montage du projet, c'est à dire que nous allons assembler nos différents éléments pour composer les robots.
Je précise que vous pouvez construire autant de robots que vous le voulez, le programme étant sensiblement le même pour chacun des robots, la gestion de plusieurs robots se fera surtout dans le programme du boitier central, mais nous verrons ça tout à l'heure.
Commençons l'assemblage.
#### Pour cette assamblage il vous faudra un peu de matériel :
- de la colle (je vous conseille [celle-ci](https://www.cdiscount.com/telephonie/accessoires-portable-gsm/tubes-de-colle-glue-adhesif-b-7000-15-ml-vitre-cha/f-1442034-auc2009365374519.html?idOffre=1094297931#mpos=0|mp) qui fonctionne très bien avec le plastique)
- un tourne vis cruciforme pour fixer les roues aux servomoteurs

Nous allons commencer par coller les deux servomoteurs FS90R sur les deux emplacements (de chaque côté du boitier) prévu à cette effet. La tête du servomoteur passe par le trou. Nous vesserons les roues une fois la colle séché. Les deux excroissances sur les côtés entourées en rouge restent à l'interieure du boitier.
![](https://github.com/Kitsunro/CSF_Robot_Chercheur/blob/main/Impression3D/servo_roue_modifi%C3%A9.jpg)

Une fois que les servos sont collés, il faut viser les roues. Attention, ne forcez pas trop pour viser, vous risquez de decoller le servos du support.
##### Voilà ! Vous avez un robot prêt a être programmé. Nous allons donc pouvoir passer à cette deuxième partie : le code.

#### Etape 3 :
Nous allons maintenent coder les robots et le boitier de contrôle. C'est ici que réside le gros du travail mais pas de panique, nous allons détaillé tout ça, pas à pas.
Nous voulons être en mesure de commander deux robots identiques avec un boitier. Ces robots vont évoluer (par soucis de simplicité) dans une zone d'1 mètre carré quadrillée d'un repère orthonormé où 1 unité vaut 5cm.
Ainsi, les robots se déplaceront d'une point A vers un point B. **Nous tenons là notre premier objectif, il nous faut être capable de calculer des instructions de déplacement à partir des coordonnées d'un point A de départ et des coordonnées d'arrivées d'une point B dans le boitier, puis d'envoyer ces instructions aux robots lorsque cela sont immobiles.**

Pour résoudre ce problème nous allons mettre en place plusieurs fonctions que j'expliquerait plutard.
##### On commence par détailler le code du boitier. Certains bout de code ne sont compréhensible qu'en ayant connaissance du programme des robots, dans le cas échéant j'essairai d'expliquer au mieux le pourquoi du comment, mais tout s'éclaircira avec le programme des robots que l'on expliquera plus bas.

Après avoir créer votre nouveau fichier .ino, vous allez dans un premier temps configurer la communication entre les cartes.
Nous allons utiliser la technologie LoRa *(et non LoRaWAN qui est un protocole bien spécifique de communication LoRa)*. Pour se faire, il vous faudra télécharger la bibliothèque (library en anglais) [LoRa](https://www.arduino.cc/reference/en/libraries/lora/). Télécharger la version 0.8.0 (la dernière au moment de ce *tuto*).
Vous avez normalement un dossier .zip, il faudra en extraire les fichiers dans le dossier *libraries* d'Arduino (pour moi par exemple, le chemin d'accès est : C:\Program Files (x86)\Arduino\libraries).

Maintenant que vous avez la bonne library, nous allons pouvoir commencer à programmer. Si vous utiliser (comme moi) la carte UCA, sachez qu'il vous faudra configurer votre IDE Arduino pour pouvoir l'utiliser. Dans ce cas, je vous renvoi vers [cette page Github](https://github.com/FabienFerrero/UCA21) qui explique comment faire.
<br/>**Maintenant, commençons à coder !**

On commence par le programme du **boitier**. Dans un premier temps, il nous faut importer toutes les bibliothèques dont on va avoir besoin :
- `#include <SPI.h>`
- `#include <LoRa.h>`
- `#include <stdlib.h>`
- `#include <time.h>`

Les deux premières bibliothèques servent à la communication LoRa, le troisième proposent des fonctions qui nous seront utiles pour convertir des types particulier ou générer des nombres aléatoires et la dernière, `time.h`, sert à gérer le temps, comme son nom l'indique. Bien sûr, si vous voulez rajouter des fonctionnalités particulières ou des capteurs, vous pouvez rajouter des biblithèques comme vous le souhaitez, les possibilités sont infinies.

Passons à la suite de notre programme qui pour l'instant ne fait pas grand chose d'intéressant. On va déclarer plusieurs variables qui nous seront bien utile par la suite. Je ne vais pas entrer tout de suite dans le détail de chacunes mais ça va venir.
<pre><code>//coordonnées de RA, RB dans un tableau
int coord_RA[2] = {0,0};
int coord_RB[2] = {0, 11};

bool state_bot = false;
int y = 0;
int compteur_telecommand = 0;</code></pre>

C'est bon, une fois toutes les déclarations faites nous allons pouvoir entrer dans le vif du sujet : le `setup`.
Ici le but est de configurer la communication Lora. C'est là que nous allons indiquer la fréquence que l'on va utiliser, initialiser le `Serial` (qui nous sera très utile ensuite pour comprendre ce qui se passe) et gérer les erreurs de connexion sur lesquels on pourrait tomber. Encore une fois, je ne vais pas entrer dans le détail du fonctionnement, mais rien de sorcier, on ne fait que réutiliser la syntaxe des exemble *LoRa Sender* et *LoRa Receiver* de la bibliothèque que l'on a téléchargé plus haut.
<pre><code>void setup()
{
  Serial.begin(9600);
  Serial.println("LoRa Sender");
  
  if (!LoRa.begin(915E6))
  {
    Serial.println("Starting LoRa failed!");
  }
  Serial.println("Test");
  envoyer("GO", 500);
}</pre></code>

Le `loop`, autrement dit la boucle infinie que va exécuter la carte est extrêment simple. On va seulement appeler les différentes fonctionnalités que l'on a programmer dans les fonctions que nous allons détailler par la suite. Je vous mets quand même le programme en bas pour montrer de quoi ça à l'air mais il n'y a rien de particulier à comprendre.
<br/>Vous vous demandez peut-être à quoi sert la ligne `envoyer("GO", 500);`. C'est typiquement une partie compliqué à comprendre sans connaître le code des robots mais pour ne pas vous laissez complètement dans le brouillard, le boitier va envoyer "GO" au deux robots, ce qui leurs signalera que celui-ci est lancé et que le *jeu* commence. Ce sera en quelque sorte l'élément déclancheur qui permettra aux robots de rentrer par la suite dans les différentes boucles qui les feront fonctionner.
<pre><code>void loop()
{
  if (state_bot == false)
  {
    //EXPLORATION();
    //RANDOM();
    //TELECOMMAND("RA",1,0,0,20,0);
  }
}</pre></code>

Vous pouvez voir les trois fonctionnalités `EXPLORATION` `RANDOM` et `TELECOMMAND`. **Elles sont toutes les trois en commentaire donc si vous lancer le programme tel quel, rien ne se passera**.

##### Nous venons de voir toutes la partie *initialisation* du programme du boitier. Désormais il nous faut nous pencher sur la partie *fonctionnement*.

Le boitier peut commander 3 fonctions différentes, mais pour l'instant n'oublions pas que nous voulons seulement répondre à la question :
> Comment être capable de calculer des instructions de déplacement à partir des coordonnées d'un point A de départ et des coordonnées d'arrivées d'un point B dans le boitier, puis d"envoyer ces instructions aux robots lorsque cela sont immobiles ?

Pour faire celà, il faut répondre à plusieurs questions au préalable :
1. Selon quelle logique les robots vont-ils se déplacer d'un point A vers un point B ?
2. Sous quelle forme les instructions doivent-elles être envoyés aux robots ?
3. Comment mettre en place un algorithme simple réalisant ce calcul ?
5. Comment être au courant que les robots sont immobiles et ainsi que les informations peuvent être envoyées ?

#### Commençons par répondre à la première question.
Dans un premier temps, on pourrait dire que par soucis d'efficacité les robots vont se déplacer en lignes droites du point de départ jusqu'au point d'arrivé. Cependant, ça soulève un problème : il faudra gérer des angles et des calculs trigonométriques qui vont complexifier l'instruction à envoyé.
Il existe une autre solution plus rapide et plus simple à mettre en place : un déplacement en escalier.
Dans ce cas, la seule information à connaitre est la valeur absolu de la différence entre l'abscice du point A et du point B et la valeur absolu de la différence entre l'ordonnée du point A et du point B. Les robots se déplaceront donc selon cette logique :
![](https://github.com/Kitsunro/CSF_Robot_Storm/blob/main/codes_tests/Schema_logique_deplacement.jpeg)

Les robots devront donc pourvoir avancer, tourner à gauche et tourner à droite un certain nombre de fois. Ainsi, une simple combinaison de virage à gauche ou à droite de 90 degrés et d'avancé tout droit permettra d'arriver à destination à tout les coups.
De plus, ilsera aisé de maintenir une même orientation des robots à la fin de leurs déplacements, je m'explique.
Lorsqu'il sera necessaire d'additionner les déplacements les uns derrière les autres (*par exemple en allant d'un point A, à un point B, à un point C ect*), un robot devra toujours terminer son déplacement das la même *orientation* qu'au départ. C'est à dire que s'il commence à se déplacer en *regardant* vers le haut, il devra finir son déplacement en *regardant* vers le haut. Sinon, au fil des itérations, les robots n'ayant aucune conscience de leurs *présences dans l'espace*, ceux-ci vont se décaler et ce décallage, comme vous pouvez l'imaginer, faussera tout le processus.
<br/>, Ainsi, avec ce mode de fonctionnement, les robots n'aurons qu'à tourner sur eux-même le même nombre de fois dans un sens comme dans l'autre pour maintenir une orientation corecte (*puisque un virage sera foncément un quart de tour, plus de problème d'addition d'angles*).

#### Maintenant nous pouvons passer à la deuxième question : Sous quelle forme devront être envoyé ces informations ?
Ici il est plus rapide de répondre. LoRa peut envoyer des messages sous forme de chaine de caractère (`String`). Même si la communication est enfaite coder en hexadécimal, ici on peut considérer que l'on envoie et que l'on reçoit des chaines de caractères.
</br> On va donc envoyer les instruction sous la forme suivante : `d001a010g002a199`, c'est à dire dans cet exemple:
- tourner à droite 1 fois
- avancer de 10
- tourner à gauche 2 fois
- avancer de 199

#### Passons à la programmation.
Cette fonction permet de calculer les déplacements que doivent accomplir les robots en fonction des coordonnées de départs et des coordonnées d'arrivées.
</br> Elle retourne le message a envoyer de la forme => "directionXXXdirectionXXXdirectionXXX...". exemple : a001d012a014g158", 
avec a pour avancer, d pour tourner à droite, g pour reculer.
</br> Les robots sont aussi capable de reculer, mais cette fonctionalité n'est pas utiliser dans le projet.

<pre><code>
String coord(int x1, int y1, int x2, int y2)
{
  String msg = "";

    if (x1 > x2)
    {
      msg = "" + instruction(msg, "g", 1);
      msg = "" + instruction(msg, "a", abs(x1-x2));
      msg = "" + instruction(msg, "d", 1);
    }

    if (x1 < x2)
    {
      msg = "" + instruction(msg, "d", 1);
      msg = "" + instruction(msg, "a", abs(x1-x2));
      msg = "" + instruction(msg, "g", 1);
    }

    if (y1 > y2)
    {
      msg = "" + instruction(msg, "d", 2);
      msg = "" + instruction(msg, "a", abs(y1-y2));
      msg = "" + instruction(msg, "d", 2);
    }

    if (y1 < y2)
    {
      msg = "" + instruction(msg, "a", abs(y1-y2));
    }

    if (x1 == x2 and y1 == y2)
    {
    }

    return msg;
}
</pre></code>

La logique est la suivante : on imagime que *l'arène* est divisée en 4 parties. On imagine (comme sur le dessin ci-dessous) le robot au milieu de l'arène au point de coordonnées (x1,y1), 
- le coin supérieur droit correspond à des coordonnées x2 et y2 supérieures au coordonnées de départ (x1 et y1).
- le coin inférieure gauche correspond à l'exact opposé, c'est à dire des coordonnées en x2 et y2 inférieures aux coordonnées de départ.
- pour le coin inférieur droit, on a x2 qui est supérieur à x1 mais y2 et inférieur à y1.
- et pour le dernier coin, supérieur gauche, on a x2 qui est inférieur à x1 et y2 qui est supérieur à y1.

Ainsi, on va comparer les différents points de coordonnées pour *localiser* la zone de la destination et pour se placer sur un point précis dans cette zone, on va calculer la différence entre les abscisses x1 et x2 et les ordonnées y1 et y2, ce qui va indiquer sur la distance du déplacement vertical et horizontal.

![](https://github.com/Kitsunro/CSF_Robot_Storm/blob/main/codes_tests/Schema-fonction-coord().jpg)

Vous aurez sûrement remarqué l'utilisation d'une fonction dont je n'ai pas encore parlé : `instruction()`.
</br> Cette fonction permet de construire les instructions. Elle prend trois paramètres :
- la variable dans laquelle elle va *ranger* l'instruction sous forme de texte
- l'action que le robot doit executer (avancer (a), tourner à gauche (g) ou tourner à droite (d)
- le nombre de répétition à faire.
Il faut comprendre que le plus complexe dans cette fonction est la gestion du nombre d'itération car en fonction du nombre d'unité (1 ou 10 ou 100), la chaîne de caractère ne sera pas construite pareil (*car je le rappel : l'information doit être de la forme direction1 puis 3 chiffres, direction2 puis à nouveau 3 chiffres et ainsi de suite*).

<pre><code>String instruction(String msg, String action, int repeats_number)
{
  if (repeats_number >= 100)
  {
    msg = msg + action + repeats_number;
    return msg;
  }
  
  else if (repeats_number >= 10)
  {
   msg = msg + action + "0" + repeats_number;
   return msg;
  }
  
  else if (repeats_number < 10)
  {
    msg = msg + action + "00" + repeats_number;
    return msg;
  }
}</pre></code>
 
 
#### On peut désormais se concentrer sur la dernière de nos 5 questions
Enfin nous y sommes. Après ça nous aurons notre base : **des robots qui se déplacent d'un point A à un point B**. Une fois que cela sera fait, le reste semblera beaucoup plus facile.
</br> **Comment pouvons nous être notifié que les robots ont arrêté de se déplacer et sont prêts à recevoir de nouvelles instructions ?**
</br> Une solution serait de programmer une sorte de *notification de fin de déplacement*. Pour faire cela, il va falloir parler un peu des codes des robots. Sans entrer dans le détail (nous le verrons après), les robots vont fonctionner selon ces étapes :
1. Ecouter les éventuelles messages qu'ils pourraient recevoir.
  - Dans le cas où ils ne recoivent rien, le message est égal à `Pas de message`.
  - Dans le cas où ils recoivent un message, le message est égal à la chaine de caractère (`String`) qui a été reçu.
2. Vérifier si le message est bien destiné au robot qui la reçu.
  - Etant donné que tout notre système va émettre sur la même fréquence (ce qui est la principale faiblesse du projet), il va falloir faire le tri dans ce qui arrive aux *oreilles* de nos petits robots.
3. Exécuter les instructions du message reçu.
4. Renvoyer un message à destination du boitier central signalant la fin des déplacements(immobilité) du robot en question (c'est à dire *signé* ce message)

</br> Nous allons donc programmer une fonction qui devra renvoyer une valeur précise (un booléen par exemple) si les **deux** robots sont immobiles.
On appelle cette fonction `moove`.

<pre><code>/*
 * Fonction moove, retourne false si les deux robots sont prêt à recevoir une commande.
 */
bool moove(String id1, String id2, int delai)
{
  // On utilise millis pour compter le temps entre deux mesure
  unsigned long Time1 = millis();
  float past = 0;
  bool state_id1;
  bool state_id2;
  
  while (past < delai)
  {
    unsigned long Time2 = millis();
    past = Time2 - Time1;
    
    String notif_fin = message_recu();
    Serial.println();
    Serial.print("message_notif dans le moove                       : ");
    Serial.println(notif_fin);

    // si on reçoit la notification de fin du robot id1
    if (notif_fin == id1 + "fini")
    {
      state_id1 = false;
    }

    // si on reçoit la notification de fin du robot id2
    if (notif_fin == id2 + "fini")
    {
      state_id2 = false;
    }

    // dans le cas où l'on n'a pas reçu une notif de fin des deux robots, les états des deux robots sont à vrai
    else
    {
      state_id1 = true;
      state_id2 = true;
    }

    // on retourne vrai dans tous les cas sauf celui où les deux notifs sont reçus (table de vérité du ou logique).
    return state_id1 or state_id2;
  }
}
</pre></code>

</br> **Voilà ! Notre objectif est enfin atteint, nous sommes capable de calculer des instructions de déplacement à partir des coordonnées d'un point A et d'un point B dans le boitier, puis d'envoyer ces instructions aux robots lorsque cela sont immobiles.**
</br> J'attire votre attention sur le fait que c'est bien dans le boitier que nous faisons les calculs. Les robots ne font qu'exécuter les instructions qu'ils reçoiovent.
</br> Désormais, il va être bien plus simple et plus *propre* de programmer les fonctionnalités de nos robots.
#### Commençons par la plus simple des 3 : <code>TELECOMMAND()</code>



