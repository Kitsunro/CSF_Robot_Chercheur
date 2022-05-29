#include <SPI.h>
#include <LoRa.h>

int coord_RA[2] = {0,0};
int coord_RB[2] = {0, 11};
bool state_bot = false;
int y = 0;

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
    EXPLORATION();
  }
}


bool moove(String id1, String id2, int delai)
{
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

    if (notif_fin == id1 + "fini")
    {
      state_id1 = false;
    }

    if (notif_fin == id2 + "fini")
    {
      state_id2 = false;
    }

    else
    {
      state_id1 = true;
      state_id2 = true;
    }

    return state_id1 or state_id2;
  }
}


void EXPLORATION()
{
  while (y<20)
  {
    bool m_RA = moove("RA", "RB", 1200);
    Serial.println(m_RA);
    Serial.println(y);
    delay(500);
    if (y%2==0 and y<20 and m_RA == false)
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

    else if (y%2!=0 and y<20 and m_RA == false)
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
