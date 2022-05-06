
#include <SPI.h>
#include <LoRa.h>
//#include <String.h>

 

void setup()
{
  Serial.begin(9600);
  Serial.println("LoRa Sender");
  
  if (!LoRa.begin(915E6))
  {
    Serial.println("Starting LoRa failed!");
  }
}

void loop()
{
  envoyer("a000r001d000g000", 2000);
}
void envoyer(char msg[], int delai)
{
  LoRa.beginPacket();
  LoRa.print(msg);
  LoRa.endPacket();
  delay(delai);
  
  Serial.print("Sending packet: ");
  Serial.println(msg);
}

void recevoir()
{
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    Serial.println("Entree dans le if");
    Serial.print("Message recu : ");
    while (LoRa.available() == true)
    {
      Serial.print((char)LoRa.read());
      /*
      char caractere = LoRa.read();
      String msg = "" + caractere;
      Serial.println(msg);
      */
    }
  }
}


/*
void instruction(int x1, int y1, int x2, int y2)
{
  int diff_x = x1 - x2;
  int diff_y = y1 - y2;
  
  diff_x = abs(diff_x);
  diff_y = abs(diff_y);
  
  String diff_x_s(toString(diff_x));
  String diff_y_s(toString(diff_y));

  Serial.println(diff_x_s);
  Serial.println(diff_y_s);
  
  if (x1 > x2)
  {
    envoyer("a000r000d000g001", 100)
    envoyer("a000r000d000g000", 100)
    envoyer("a010r000d001g000", 100)
  }

  if (x1 < x2)
  {
    envoyer("a000r000d001g000", 100)
    envoyer("a000r000d000g000", 100)
    envoyer("a000r000d000g001", 100)  
  }

  if (y1 > y2)
  {
    envoyer("a000r000d002g000", 100)
    envoyer("a000r000d000g000", 100)
    envoyer("a000r000d002g000", 100)
  }

  if (y1 < y2)
  {
    envoyer("a000r000d000g000", 100)
  }

  else
  {
  }
}
*/
