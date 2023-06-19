#include <dht.h>

// Declarando pinos
#define SENSOR_TEMP 14 //Sensor de temperatura
#define MODULO_LED 1
#define PIR 2 // Sensor de movimento
#define SENSOR_LUZ 4
#define AQUECEDOR 27

int led = 23;
dht temp;

// the setup function runs once when you press reset or power the board


void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  //pinMode(SENSOR_TEMP, INPUT); //DEFINE O PINO COMO ENTRADA
  pinMode(AQUECEDOR, OUTPUT); //DEFINE O PINO COMO ENTRADA
  Serial.begin(9600);
  
   
}

// the loop function runs over and over again forever
void loop() {
  
  
  //INFORMA O VALOR DO SENSOR DE TEMP
  temp.read11(SENSOR_TEMP); //LÊ AS INFORMAÇÕES DO SENSOR
  double temperatura = temp.temperature;
  
  Serial.println(temperatura);
  
  // Desliga Aquecedor
  if (temperatura >= 45.00) {
    digitalWrite(AQUECEDOR, HIGH);
  } else if (temperatura <= 35) {
    //Liga aquecedor
    digitalWrite(AQUECEDOR, LOW);
  }
}
