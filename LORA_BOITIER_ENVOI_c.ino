#include <SPI.h>
#include <LoRa.h>
#include <stdlib.h>
#include <time.h>

//coordonnées de RA, RB dans un tableau
int coord_RA[2] = {0,0};
int coord_RB[2] = {0, 11};

bool state_bot = false;
int y = 0;
int compteur_telecommand = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("LoRa Sender");
  
  if (!LoRa.begin(915E6))
  {
    Serial.println("Starting LoRa failed!");
  }
  envoyer("GO", 500);
}

void loop()
{
  if (state_bot == false)
  {
    //EXPLORATION();
    //RANDOM();
    //TELECOMMAND("RB",1,0,0,20,0);
    envoyer("RBa003",300);
  }
}

/*
 * Fonction moove, retourne false si les deux robots sont prêt à recevoir une commande.
 */
bool moove(String id1, String id2, int delai)
{
  // On utilise millis pour compter le temps entre deux mesure
  unsigned long Time1 = millis();
  float past = 0;
  bool state_id1;
  bool state_id2;
  
  while (past<delai)
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

/*
 * Fonction EXPLORATION, elle fait se déplacer les deux robots en zigzag pour qu'ils couvrent toute une zone.
 * On veut qu'une fois sur deux, le robot se déplace jusqu'à la coordonnée (0,y), puis ensuite (10,y).
 * Celà va avoir pour effet de le faire se déplacer un zigzag. On va donc s'interresser à la parité de y pour realiser cette alternance un certain nombre de fois (repeat).
 */
void EXPLORATION()
{
  // nombre de répétitions de la boucle
  const int repeat = 10;
  
  while (y<repeat)
  {
    // on déclare un booléen (m_bot) qui contient la valeur renvoyé par la fonction moove avec les paramètres (id1 = RA, id2 = RB, delai = 1,2s).
    bool m_bot = moove("RA", "RB", 1200);
    Serial.println(m_RA);
    Serial.println(y);
    delay(500);

    // dans le cas où y est un nombre pair, inférieure au nombre de répétition et que les deux robots sont prêts à reçevoir des commandes.
    if (y%2==0 and y<repeat and m_bot == false)
    {
      Serial.println("MARQUEUR DE PASSAGE PAIR 0");
      envoyer("RA" + coord(coord_RA[0], coord_RA[1], 0, y),300);
      Serial.println("MARQUEUR DE PASSAGE PAIR 1");
      coord_RA[0] = 0;
      coord_RA[1] = y;
      
      envoyer("RB" + coord(coord_RB[0], coord_RB[1], 11, y),300);
      coord_RB[0] = 11;
      coord_RB[1] = y;

      y++;
    }

    // même cas que pour le if mais avec y impair
    else if (y%2!=0 and y<repeat and m_bot == false)
    {
      Serial.println("MARQUEUR DE PASSAGE IMPAIR 0");
      envoyer("RA" + coord(coord_RA[0], coord_RA[1], 10, y), 300);
      Serial.println("MARQUEUR DE PASSAGE IMPAIR 1");
      coord_RA[0] = 10;
      coord_RA[1] = y;

      envoyer("RB" + coord(coord_RB[0], coord_RB[1], 19, y), 300);
      coord_RB[0] = 19;
      coord_RB[1] = y;

      y++;
    }
  }
}

/*
 * Fonction permettant de générer un entier aléatoire enter a et b
 */
int alea( int a, int b)
{
  int n_alea ;
  n_alea = a + (int)((float)rand() * (b-a+1) / (RAND_MAX-1)) ;
  return n_alea;
}

/*
 * Fonction RANDOM. Elle permet de générer des coordonnées aléatoire pour les deux robots et de les envoyer, tout cela un certain nombre de fois.
 * Elle est construite un peu comme la fonction EXPLORATION.
 */
void RANDOM()
{
  const int repeat = 10;
  const int a = 0;
  const int b = 20;
  
  while (y<repeat)
  {
    bool m_bots = moove("RA", "RB", 1200);
    Serial.println(m_bots);
    Serial.println(y);
    delay(500);
    
    if (y<repeat and m_bots == false)
    {
      int coord_xa = alea(a,b);
      int coord_ya = alea(a,b);
      envoyer("RA" + coord(coord_RA[0], coord_RA[1], coord_xa, coord_ya),300);
      coord_RA[0] = coord_xa;
      coord_RA[1] = coord_ya;

      int coord_xb = alea(a,b);
      int coord_yb = alea(a,b);
      envoyer("RB" + coord(coord_RB[0], coord_RB[1], coord_xb, coord_yb),300);
      coord_RB[0] = coord_xb;
      coord_RB[1] = coord_yb;

      y++;
    }
  }
}

/*
 * Fonction TELECOMMAND. Elle permet d'envoyer une instruction de déplacement vers des coordonnées précises, un certain nombre de fois et vers un destinataire précis.
 */
void TELECOMMAND(String destinataire, const int nombre_iteration, const int x1, const int y1, const int x2, const int y2)
{
  if (compteur_telecommand <= nombre_iteration)
  {
    envoyer(destinataire + coord(x1,y1,x2,y2), 300);
    compteur_telecommand++;
  }
}


/*
 * Permet d'envoyer un message texte (même fonction que pour les robots).
 */
void envoyer(String msg, int delai)
{
  unsigned long Time1 = millis();
  float past = 0;
  
  while (past<delai)
  {
    unsigned long Time2 = millis();
    past = Time2 - Time1;
    LoRa.beginPacket();
    LoRa.print(msg);
    LoRa.endPacket();
  
    Serial.print("Sending packet: ");
    Serial.println(msg);
  }
}

/*
 * Même fonction que pour les robots.
 */
String message_recu()
{
  // test si il y a un packet (message) qui est recu
  int packetSize = LoRa.parsePacket();
  delay(100);
  if (packetSize)
  {
    /* on défini une variable msg qui contiendra le message recu. Tant que le message n'est pas fini,
     * on ajoute la variable caractère (qui contient l'un après l'autre tous les caractères du message recu)
     * à la variable msg grace a la fonction concat.
     */
    String msg = "";
    while (LoRa.available())
    {
      char caractere = (char)LoRa.read();
      msg.concat(String(caractere));
    }
    
    Serial.print("le message est : ");
    Serial.println(msg);
    
    return msg;
  }

  // si il n'y a pas de packet recu alors on fixe le msg à None
  else
  {
    return "None";
  } 
}

/*
 * Fonction permettant de calculer les déplacement que doivent accomplir les robots en fonction de la coordonnée de départ et de la coordonnée d'arrivée.
 * Elle retourne le message a envoyer de la forme => "directionXXXdirectionXXXdirectionXXX...". exemple : a001d012a014g158", 
 * avec a pour avancer, d pour tourner à droite, g pour reculer. Les robots sont aussi capable de reculer, mais cette fonctionalité n'est pas utiliser dans le projet.
 */
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

/*
 * Fonction permettant de construire les instructions. Elle prend trois paramètres :
 * la variable dans laquelle elle va "ranger" l'instruction sous forme de texte, l'action que le robot doit executer (avancer (a), tourner à gauche (g) ou tourner à droite (d),
 * le nombre de répétition à faire.
 */
String instruction(String msg, String action, int repeats_number)
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
}
