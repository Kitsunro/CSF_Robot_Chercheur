#include <VirtualWire.h>
// #define TxPin 12
#define RxPin 8
const int led_verte = 4;
const int led_rouge = 3;

void setup() 
{
/*
 // Initialisation du transmetteur 
 pinMode(led_verte, OUTPUT);
 pinMode(led_rouge, OUTPUT);
 pinMode(TxPin, INPUT);
 Serial.begin(9600);    // Debugging only
 Serial.println("Setup Transmitter");
 // Initialise the IO and ISR
 vw_set_ptt_inverted(true); // Required for DR3100
 vw_setup(2000);  // Bits per sec
*/

 // Initialisation du récepteur pinMode(11, OUTPUT);
 pinMode(8, OUTPUT);
 Serial.println("Setup Receiver");
 vw_set_ptt_inverted(true); // Required for DR3100
 vw_setup(2000); // Bits per sec
 vw_rx_start(); // Start the receiver PLL running
 
}


void loop() 
{
/*
// Algo transmetteur

 String s = "";
 int i;
 int j = 77;
 for (i=0; i<j; i++) {
  s += 0;
 }
 const char* msg = s.c_str();
 */
 /*
 const char* msg = "1 la planète Mars n'a rien à voir avec la barre chocolatée";
 
 digitalWrite(led_verte, HIGH);
 digitalWrite(led_rouge, LOW);
 Serial.println(msg);
 vw_send((uint8_t *)msg, strlen(msg));
 vw_wait_tx(); // Wait until the whole message is gone
 digitalWrite(led_verte, LOW);
 digitalWrite(led_rouge, LOW);
 delay(200);
*/

 // Algo receveur
 uint8_t buf[VW_MAX_MESSAGE_LEN];
 uint8_t buflen = VW_MAX_MESSAGE_LEN;
 Serial.println("Loop");
 if (vw_get_message(buf, &buflen)) // Non-blocking 
 {
   int i;
   digitalWrite(led_rouge, HIGH);
   digitalWrite(led_verte, LOW);
   // Message with a good checksum received, dump it.
   Serial.print("From transmitter : ");
   
   for (i = 0; i < buflen; i++)
   {
    //Serial.print(buf[i], HEX);       →sert a afficher les valeurs en héxa
    Serial.print(char(buf[i]));
    //Serial.print(" ");
   }
   
   Serial.println("");
   digitalWrite(led_rouge, LOW);
   digitalWrite(led_verte, LOW);
 }
 
}
