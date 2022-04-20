String msg = "a";

void setup() {
  Serial.begin(9600);
  int i;
  for (i=0; i<5; i++) {
    msg += i;
  }
  Serial.println(msg);
}

void loop() {
}
