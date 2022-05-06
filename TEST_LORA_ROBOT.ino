// bibliothèques pour communications LoRa 
#include <SPI.h>
#include <LoRa.h>
// bibliothèque pour controle servo
#include <Servo.h>

// on déclare deux servos (1 et 2)
Servo servo1;
Servo servo2;

// deux constantes pour définir les pins des servos (attentions certains pins sont réservés à la communication LoRa)
const int pin_servo1 = 5;
const int pin_servo2 = 9;

// booléen qui servira de marqueur de passage dans le loop
bool go_robot = true;

// trois constances qui correspondent aux delais durant lesquelles doivent fonctionner les servos pour faire 1 tour 1/2 tour ou 1/4 de tour
const int tour_complet = 780;
const int demi_tour = 400;
const int quart_de_tour = 200;

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
  // on déclare une string qui contiendra le message recu du boitier central
  String message = message_recu(); 

  // dans le cas où il n'y aurait pas de message recu on met message a None
  if (message == "None")
  {
    Serial.println("Pas de message");
    Serial.println();
  }

  // dasn le cas où le message est recu
  else
  {
    Serial.print("message recu : ");
    Serial.println(message);

    /* le message recu est toujours sous la forme a(3 chiffres)r(3 chiffres)d(3 chiffres)g(3 chiffres)
     *  on déclare une variable qui contient une string et on concatène les 3 chiffres qui correspondent
     *  a l'instruction. Par exemple (ex : a003) correspondra à avancer de 3 tours. Ainsi la variable
     *  n_avance = "003". Ensuite on convertit cette valeur en entier et on la stock dans la variable
     *  n_avance_int. On répètera le meme procédé pour les autres instruction.
     */
    String n_avance = concatenation(1,3,message);
    int n_avance_int = n_avance.toInt();
    String n_recule = concatenation(5,7,message);
    int n_recule_int = n_recule.toInt();
    String n_droite = concatenation(9,11,message);
    int n_droite_int = n_droite.toInt();
    String n_gauche = concatenation(13,15,message);
    int n_gauche_int = n_gauche.toInt();

  // la condition est vérifié si et seulement si go_robot est vrai, c'est à dire si le marqueur passage reste inchangé
  
    if (go_robot==true)
    {
      // on appelle les fonctions qui vont faire avancer, reculer, tourner à droite ou tourner à gauche le robot avec le nombre de tour à faire
      avancer(n_avance_int);
      delay(300);
      reculer(n_recule_int);
      delay(300);
      tourne_droit(n_droite_int);
      delay(300);
      tourne_gauche(n_gauche_int);
      delay(300);
      go_robot = false; // on mets le marqueur de passage a false pour ne plus re-executer le déplacement. Si le marqueur reste a true on répètera le déplacement à l'infini
    } 
  }
}

// fonction pour recevoir le message du boitier
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


void avancer(int nombre_tour)
{
  for (int i=1; i<=nombre_tour; i++)
  {
    // on attache les moteurs 1 et 2 à leurs sorties respectives pour lancer la rotation
    servo1.attach(pin_servo1);
    servo2.attach(pin_servo2);
    
    // on fait faire 1 tour aux deux servos DABS LE MEME SENS (d'ou le 1 et le 189)
    servo1.write(1);
    servo2.write(189);
    delay(780);
    
    // on les détache de leurs sorties respectives ce qui a pour effet de stopper la rotation
    servo1.detach();
    servo2.detach();
  }
}

void reculer(int nombre_tour)
{
  for (int i=1;i<=nombre_tour; i++)
  {
    servo1.attach(pin_servo1);
    servo2.attach(pin_servo2);
    
    // on fait faire 1 tour aux deux servos DABS LE MEME SENS (Inverse) (d'ou le 1 et le 189)
    servo1.write(189);
    servo2.write(1);
    delay(tour_complet);
    
    // on les détache de leurs sorties respectives ce qui a pour effet de stopper la rotation
    servo1.detach();
    servo2.detach();
  }
}


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
}


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
}
