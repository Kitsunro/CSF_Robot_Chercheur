#include <SPI.h>
#include <LoRa.h>
#include <Servo.h>

Servo servo1;
Servo servo2;

int pin_servo1 = 7;
int pin_servo2 = 8;

bool go_robot = true;

int tour_complet = 780;
int demi_tour = 400;
int quart_de_tour = 200;

void setup()
{ 
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");

  if (!LoRa.begin(915E6))
  {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  
  servo1.attach(pin_servo1);
  servo2.attach(pin_servo2);
}


void loop()
{
  String message = message_recu(); 
  
  if (message == "None")
  {
    Serial.println("Pas de message");
    Serial.println();
  }

  else
  {
    Serial.print("message recu : ");
    Serial.println(message);

    String n_avance = concatenation(1,3,message);
    int n_avance_int = n_avance.toInt();
    String n_recule = concatenation(5,7,message);
    int n_recule_int = n_recule.toInt();
    String n_droite = concatenation(9,11,message);
    int n_droite_int = n_droite.toInt();
    String n_gauche = concatenation(13,15,message);
    int n_gauche_int = n_gauche.toInt();

    if (go_robot==true)
    {
      avancer(n_avance_int);
      delay(300);
      reculer(n_recule_int);
      delay(300);
      tourne_droit(n_droite_int);
      delay(300);
      tourne_gauche(n_gauche_int);
      delay(300);
      go_robot = false;
    } 
  }
}

String message_recu()
{
  int packetSize = LoRa.parsePacket();
  Serial.println(packetSize);
  if (packetSize)
  {
    String msg = "";
    while (LoRa.available())
    {
      char caractere = (char)LoRa.read();
      msg.concat(String(caractere));
    }
    return msg;
  }
  else
  {
    return "None";
  }
}

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
