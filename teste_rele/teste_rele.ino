#define rele 0

void setup() {
  pinMode(rele, OUTPUT);
}

void loop() {
  digitalWrite(rele, HIGH);
  delay(5000);
  
  digitalWrite(rele, LOW);
  delay(3000);
}
