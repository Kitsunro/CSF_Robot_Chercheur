// bibliothèques pour communications LoRa 
#include <SPI.h>
#include <LoRa.h>
// bibliothèque pour controle servo
#include <Servo.h>

// identifiant du robot
const String id = "RB";

// on déclare deux servos (1 et 2)
Servo servo1;
Servo servo2;

// deux constantes pour définir les pins des servos (attentions certains pins sont réservés à la communication LoRa)
const int pin_servo1 = 5;
const int pin_servo2 = 9;

// booléen qui servira de marqueur de passage dans le loop
bool go_robot = true;
bool start = false;

// trois constances qui correspondent aux delais durant lesquelles doivent fonctionner les servos pour faire 1 tour 1/2 tour ou 1/4 de tour
const int tour_complet = 780;
const int demi_tour = 400;
const int quart_de_tour = 250;

void setup()
{ 
  Serial.begin(9600);
  // on vérifie que la connexion entre les deux cartes s'est bien faite
  while (!Serial);

  Serial.println("LoRa Receiver");
  // si la connexion s'est mal passée on l'écrit dans le terminal (915E6 correspond à la fréquence d'envoi)
  if (!LoRa.begin(915E6))
  {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  // on attache les deux servos aux pins qui leur correspondent
  servo1.attach(pin_servo1);
  servo2.attach(pin_servo2);
}


void loop()
{
  digitalWrite(pin_servo1, LOW);
  digitalWrite(pin_servo2, LOW);
  
  // on déclare une string qui contiendra le message recu du boitier central
  String message = message_recu();
  
  // dans le cas où il n'y aurait pas de message recu on met message a None
  if (message == "None")
  {
    Serial.println("Pas de message");
    Serial.println();
  }

  /* 
   *  dans le cas où le message est recu, que les deux premiers caractères correspondent
   *  à l'id du robot et que start est vrai (or start est faux pas défaut).
   */
  else if (message[0] == id[0] and message[1] == id[1] and start)
  {
    go_robot = false;
    Serial.print("message : ");
    Serial.println(message);
    Serial.println();

    // pour chaque caractère du message et tant que le booléen go_robot est faux
    for (int i=2; i<message.length() and go_robot==false; i++)
    {
      Serial.println(message[i]);

     // si le caractère d'indice i du message est égale à "a"
     if (message[i] == 'a')
     {
      /* 
       *  on isole la partie de la chaine qui correspond au nombre d'itération de la fonction avancé grâce à la fonction concatenation.
       *  On affecte cette valeur à la variable n_avance. Ensuite on convertit cette String en entier (int) avec la fonction toInt. On se retrouve donc avec
       *  la variable n_avancer_int qui correspond à l'entier, paramètre du nombre d'itération de la fonction avancer. 
      */
      String n_avance = concatenation(i+1,i+3,message);
      int n_avance_int = n_avance.toInt();
      avancer(n_avance_int);
      delay(300);
     }
     
      // selon le même procédé que pour avancer.
     if (message[i] == 'd')
     {
      String n_droite = concatenation(i+1,i+3,message);
      int n_droite_int = n_droite.toInt();
      tourne_droit(n_droite_int);
      delay(300);
     }
     
       // selon le même procédé que pour avancer.
     if (message[i] == 'g')
     {
      String n_gauche = concatenation(i+1,i+3,message);
      int n_gauche_int = n_gauche.toInt();
      tourne_gauche(n_gauche_int);
      delay(300);
     }
    }
    /* 
     *  une fois qu'on sort de la boucle for, on fait passer go_robot à vrai.
     *  Ainsi, tant que go_robot ne repasse pas à faux, le robot ne peut plus se déplacer.
     */
    delay(1000);
    go_robot=true;
  }

/*
 * Cette condition sert à envoyer une notification de fin de déplacement pour le boitier central.
 * Pour que cette notification soit envoyé il faut que le boitier est envoyé "GO" (envoyé lors de l'allumage du boitier). 
 * Par le suite, le robot envoi la notification de fin de déplacement lorsque start est vrai (il est toujour vrai un efois la première notification envoyé)
 * et lorsque go_robot est vrai (lorsque le robot à fini de se déplacer).
 */
  if (message == "GO" or (start and go_robot))
  {
    start = true;
    go_robot = false;
    envoyer(id + "fini", 800);
  }  
}
/*
 * Fonction envoyé, permet d'envoyer un message texte. Elle prend deux paramètres :
 * le message (String msg) et le temps durant lequel on envoi ce message (int delai).
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


// fonction pour recevoir le message du boitier, elle retourne le message reçu.
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
    /*
    Serial.print("le message est : ");
    Serial.println(msg);
    */
    return msg;
  }

  // si il n'y a pas de packet recu alors on fixe le msg à None
  else
  {
    return "None";
  } 
}

/* fonction qui sert a concaténer certaines valeurs d'une String.
 *  Cette fonction prend 3 arguments : le premier correspond à l'indice du début de la partie de la chaine
 *  que l'on souhaite concaténer (pour l'isoler du reste), le deuxière correspond à l'indice de fin et le dernier à la chaine que 
 *  l'on souhaite traiter.
 */
String concatenation(int debut, int fin, String message)
{
  String msg = "";
  for (int i = debut; i<=fin; i++)
  {
    msg.concat(message.charAt(i));
  }
  return msg;
}


/*
 * Fonction avancer, prend en paramètre le nombre de tour (c'est à dire le nombre d'itération).
 */
void avancer(int nombre_tour)
{
  for (int i=1; i<=nombre_tour; i++)
  {
    // on attache les moteurs 1 et 2 à leurs sorties respectives pour lancer la rotation
    servo1.attach(pin_servo1);
    servo2.attach(pin_servo2);

    // on fait faire 1 quart de tour aux deux servos DANS LE MEME SENS (d'ou le 1 et le 189)
    servo1.write(1);
    servo2.write(179);
    delay(quart_de_tour);

    // on les détache de leurs sorties respectives ce qui a pour effet de stopper la rotation
    servo1.detach();
    servo2.detach();

  }
  Serial.print("AVANCE DE : ");
  Serial.println(nombre_tour);
  Serial.println();
}

// Selon une logique semblable 
void reculer(int nombre_tour)
{
  
  for (int i=1;i<=nombre_tour; i++)
  {
    servo1.attach(pin_servo1);
    servo2.attach(pin_servo2);
    
    // on fait faire 1 tour aux deux servos DABS LE MEME SENS (Inverse) (d'ou le 1 et le 189)
    servo1.write(189);
    servo2.write(1);
    delay(quart_de_tour);
    
    // on les détache de leurs sorties respectives ce qui a pour effet de stopper la rotation
    servo1.detach();
    servo2.detach();
  }
  Serial.print("RECULE DE : ");
  Serial.println(nombre_tour);
  Serial.println();
}


// Selon une logique semblable 
void tourne_gauche(int nombre_demi_tour)
{
  for (int i=1;i<=nombre_demi_tour;i++)
  {
    servo1.attach(pin_servo1);
    servo2.attach(pin_servo2);

    servo1.write(1);
    servo2.write(1);
    delay(demi_tour);

    servo1.detach();
    servo2.detach();
  }
  Serial.print("TOURNE A GAUCHE DE : ");
  Serial.println(nombre_demi_tour);
  Serial.println();
}

// Selon une logique semblable 
void tourne_droit(int nombre_demi_tour)
{
  for (int i=1;i<=nombre_demi_tour;i++)
  {
    servo1.attach(pin_servo1);
    servo2.attach(pin_servo2);

    servo1.write(189);
    servo2.write(189);
    delay(demi_tour);

    servo1.detach();
    servo2.detach();
  }
  Serial.print("TOURNE A DROITE DE : ");
  Serial.println(nombre_demi_tour);
  Serial.println();
}
