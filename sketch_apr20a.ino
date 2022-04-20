#include <VirtualWire.h>
#define RxPin 11
void setup()
{
 pinMode(13, OUTPUT);
 pinMode(11, OUTPUT);
 Serial.begin(9600);  // Debugging only
 Serial.println("Setup Receiver");
 vw_set_ptt_inverted(true); // Required for DR3100
 vw_setup(2000);  // Bits per sec
 vw_rx_start();       // Start the receiver PLL running
}
void loop()
{
 uint8_t buf[VW_MAX_MESSAGE_LEN];
 uint8_t buflen = VW_MAX_MESSAGE_LEN;
 if (vw_get_message(buf, &buflen)) // Non-blocking
 {
   int i;
   digitalWrite(13, true); // Flash a light to show received good message
   // Message with a good checksum received, dump it.
   Serial.print("From transmitter : ");
   if ((char(buf[0])== '0')) {
    for (i = 2; i < buflen; i++){
       //Serial.print(buf[i], HEX);       →sert a afficher les valeurs en héxa
      Serial.print(char(buf[i]));
       //Serial.print(" ");
    }
   }else{
    Serial.print("Code invalide");
   }
   Serial.println("");
   digitalWrite(13, false);
 }
}
