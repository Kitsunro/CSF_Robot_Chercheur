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
  String message = coord(5,3,1,0);
  envoyer(message, 500);
}


void envoyer(String msg, int delai)
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
    }
  }
}

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

    if (x1 == x2 or y1 == y2)
    {
    }
    
    return msg;
}

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
