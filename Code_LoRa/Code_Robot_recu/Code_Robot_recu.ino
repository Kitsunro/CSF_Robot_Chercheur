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
      int n = 1;
      String nombres = "";
      while (isDigit(message.charAt(i+n))){
        nombres.concat(message.charAt(i+n)-48);
        n++;
      }
      int nombres_int = nombres.toInt();
      if ( message.charAt(i) == 'a'){
        if (nombres != 0){
          Serial.println("Avance");
          Serial.println(nombres_int);
        }else{
          Serial.println("Avance");
        }
      }else if (message.charAt(i) == 'r'){
        if (nombres != 0){
          Serial.println("Recule");
          Serial.println(nombres_int);
        }else{
          Serial.println("Recule");
        }
      }else if (message.charAt(i) == 'g'){
        if (nombres != 0){
          Serial.println("Gauche");
          Serial.println(nombres_int);
        }else{
          Serial.println("Gauche");
        }
      }else if (message.charAt(i) == 'd'){
        if (nombres != 0){
          Serial.println("Droite");
          Serial.println(nombres_int);
        }else{
          Serial.println("Droite");
        }
      }else{
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
