#include <SPI.h>
#include <LoRa.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}
void loop() {
  recu();
}

void recu(){
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String message = "";
    int lenm = 0;
    while (LoRa.available()) {
      char Z = (char)LoRa.read();
      message.concat(String(Z));
    }
    Serial.println(message);
    lenm = message.length();
    for (int i = 0 ; i<lenm ; i++){
      if ( message.charAt(i) == 'a'){
        Serial.println("Avance");
      }else if (message.charAt(i) == 'r'){
        Serial.println("Recule");
      }else if (message.charAt(i) == 'g'){
       Serial.println("Gauche");
      }else if (message.charAt(i) == 'd'){
        Serial.println("Droite");
      }else{
        Serial.println("Invalide");
      }
    }
    delay(1500);
    envoie("recu roger");
  }
}

void envoie(String msg){
  LoRa.beginPacket();
  LoRa.print(msg);
  LoRa.endPacket();
}
