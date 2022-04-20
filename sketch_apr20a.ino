#include <VirtualWire.h>
#define RxPin 11
#define TxPin 7
void setup()
{
 pinMode(13, OUTPUT);
 pinMode(11, OUTPUT);
 pinMode(4, OUTPUT);
 pinMode(7,INPUT);
 pinMode(3,OUTPUT);
 Serial.begin(9600);  // Debugging only
 Serial.println("Setup Started");
 vw_set_ptt_inverted(true); // Required for DR3100
 vw_setup(2000);  // Bits per sec
 vw_rx_start();       // Start the receiver PLL running
}
void loop()
{
 digitalWrite(13, true);
 uint8_t buf[VW_MAX_MESSAGE_LEN];
 uint8_t buflen = VW_MAX_MESSAGE_LEN;
 if (vw_get_message(buf, &buflen)) // Non-blocking
 {
   int i;
   digitalWrite(4, true); // Flash a light to show received good message
   Serial.print("From transmitter : ");
   if ((char(buf[0])== '1')) {
    for (i = 1; i < buflen; i++){
      Serial.print(char(buf[i]));  
      }
    const char *msg = "0Yes !";
    digitalWrite(3, HIGH); // Flash a light to show transmitting
    Serial.println(msg);
    vw_send((uint8_t *)msg, strlen(msg));
    vw_wait_tx(); // Wait until the whole message is gone
    digitalWrite(3, LOW);
    delay(200);
   }
   else{
    Serial.print("Code invalide");
   }
   Serial.println("");
   digitalWrite(4, LOW);
 }
 digitalWrite(13, false);
}
