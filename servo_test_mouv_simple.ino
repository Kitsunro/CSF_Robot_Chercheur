#include <Servo.h>
Servo servo1;
Servo servo2;
bool state = true;
int tour_complet = 780;
int demi_tour = 400;
int quart_de_tour = 200;

void setup() 
{
  Serial.begin(9600);
  servo1.attach(10);
  servo2.attach(11);
}

void loop() 
{
  if (state)
  {
    avancer(3);
    delay(300);
    reculer(3);
    delay(300);
    tourne_gauche(3);
    delay(300);
    tourne_droit(3);
    state = false;
  }
}

// fonction avancer
void avancer(int nombre_tour)
{
  for (int i=1; i<=nombre_tour; i++)
  {
    // on attache les moteurs 1 et 2 à leurs sorties respectives pour lancer la rotation
    servo1.attach(10);
    servo2.attach(11);
    
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
    servo1.attach(10);
        servo2.attach(11);
    
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
    servo1.attach(10);
    servo2.attach(11);

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
    servo1.attach(10);
    servo2.attach(11);

    servo1.write(189);
    servo2.write(189);
    delay(demi_tour);

    servo1.detach();
    servo2.detach();
  }
}
