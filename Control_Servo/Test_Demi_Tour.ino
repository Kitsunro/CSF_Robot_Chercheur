#include <Servo.h>
Servo servo1;
Servo servo2;
bool state = true;

void setup() 
{
  servo1.attach(10);
  servo2.attach(11);
}

void loop() 
{
  if (state)
  {
    servo1.attach(10);
    servo2.attach(11);

    servo1.write(189);
    servo2.write(189);
    delay(200);

    servo1.detach();
    servo2.detach();
    state = false;
  }
}
