#define pinPIR 2
#define PIN_LED 23

void setup() {
  pinMode(pinPIR, INPUT);
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(9600);
}
void loop() {
  bool valorPIR = digitalRead(pinPIR);
  
  if (valorPIR) {
     Serial.println("DETECTADO");
     digitalWrite(PIN_LED, 1);
  } else {
     Serial.println("----");
     digitalWrite(PIN_LED, 0);
  }
}
