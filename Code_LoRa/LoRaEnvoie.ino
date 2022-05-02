#include <SPI.h>
#include <LoRa.h>

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
  envoyer("a001r000d000g000", 1000);
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
