#include <SPI.h>
#include <LoRa.h>

int counter = 0;
int cont = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  LoRa.beginPacket();
  LoRa.print("ar");
  LoRa.endPacket();
  counter++;
  bool messagerecu = false;
  while(!messagerecu){
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
      String message = "";
      while (LoRa.available()) {
        char Z = (char)LoRa.read();
        message.concat(String(Z));
   }
    Serial.println(message);
    messagerecu = true;
  }
 }
  delay(1000);
}
