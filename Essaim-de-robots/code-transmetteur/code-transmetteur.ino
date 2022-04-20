#include <VirtualWire.h>
#define TxPin 12

void setup()
{
 pinMode(13, OUTPUT);
 pinMode(12, INPUT);
 Serial.begin(9600);    // Debugging only
 Serial.println("Setup Transmitter");
 // Initialise the IO and ISR
 vw_set_ptt_inverted(true); // Required for DR3100
 vw_setup(2000);  // Bits per sec
}

void loop()
{
 String s = "";
 int i;
 int j = 78;
 for (i=0; i<j; i++) {
  s += 0;
 }
 
 const char* msg = s.c_str();
 Serial.print("var msg :");
 
 digitalWrite(13, true); // Flash a light to show transmitting
 Serial.println(msg);
 vw_send((uint8_t *)msg, strlen(msg));
 vw_wait_tx(); // Wait until the whole message is gone
 digitalWrite(13, false);
 delay(200);
}
