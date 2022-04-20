#include <VirtualWire.h>
#define TxPin 12
int liste[5];

void setup()
{
 pinMode(13, OUTPUT);
 pinMode(12, INPUT);
 Serial.begin(9600);    // Debugging only
 Serial.println("Setup Transmitter");
 // Initialise the IO and ISR
 vw_set_ptt_inverted(true); // Required for DR3100
 vw_setup(2000);  // Bits per sec
 
 liste[0]=12;
 Serial.println(liste[0]);
}

void loop()
{
 const char *msg =  liste[0];
Serial.println(liste[0]);;
 digitalWrite(13, true); // Flash a light to show transmitting
 vw_send((uint8_t *)msg, strlen(msg));
 vw_wait_tx(); // Wait until the whole message is gone
 digitalWrite(13, false);
 delay(200);
}
