#include <dht.h>

// Declarando pinos
#define SENSOR_TEMP 0 //Sensor de temperatura
#define MODULO_LED 1
#define PIR 2 // Sensor de movimento
#define SENSOR_LUZ 4
#define AQUECEDOR 5

 

int led = 23;
dht temp;

// the setup function runs once when you press reset or power the board


void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(sensorluz, INPUT); //DEFINE O PINO COMO ENTRADA
  pinMode(led, OUTPUT); //DEFINE O PINO COMO ENTRADA
   Serial.begin(9600);
   
}

// the loop function runs over and over again forever
void loop() {
  
  //INFORMA O VALOR DO SENSOR DE TEMP
  temp.read11(sensortemp); //LÊ AS INFORMAÇÕES DO SENSOR
  Serial.println(temp.temperature, 0);

  //INFORMA O VALOR DO SENSOR DE LUMINOSIDADE
  int valor1 = analogRead(sensorluz);
  Serial.print("Sensor luz:");
   Serial.println(valor1, 0);

  //O VALOR 600 PODE SER AJUSTADO
  if(analogRead(sensorluz) > 100){ //SE O VALOR LIDO FOR MAIOR QUE 600, FAZ
    digitalWrite(led, HIGH); //ACENDE O LED
  }  
  else{ //SENÃO, FAZ
    digitalWrite(led, LOW); //APAGA O LED
  }  


  delay(1000);
  
}
