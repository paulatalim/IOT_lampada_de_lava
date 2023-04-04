// 
//
#include <Adafruit_NeoPixel.h>
#define LED_COUNT   16 // Defina o número de LEDs na sua fita de LED
#define pin  11 // Defina o pino de dados que está conectado à fita de LED

Adafruit_NeoPixel strip(LED_COUNT, pin, NEO_GRB + NEO_KHZ800);// Crie um objeto de pixel da fita de LED

int rele = 7;
int sensorluz = A3;//Sensor de luminoziadade
int sensortemp = A5; //Sensor de temperatura
int valor_do_sensor1; //valor do sensor de luminosidade
int valor_do_sensor2; //valor do sensor de temperatura
int entrada_monitor  ; //Casso necessite de uma entrada de valores
   

int cont = 0;


void setup()
{
  Serial.begin(9600);
  
  pinMode(rele, OUTPUT);//Saida rele
  
  strip.begin(); // Inicialize a fita de LED
  strip.clear();// Defina todas as cores dos pixels como preto (desligado)
  strip.show();// Atualize a fita de LED com as cores definidas
 
}


void loop()
{
  //PREDEFINIÇÕES
  int sensormov = digitalRead(9); //Sensor de movimento
  valor_do_sensor1 = analogRead(sensorluz); //passando o valor do sensor para uma variavel
  valor_do_sensor2 = analogRead(sensortemp);//passando o valor do sensor para uma variavel
 
  //INFORMA O VALOR DO FOTORESISTOR
  Serial.print("\n");
  Serial.print("Valor do sensor ambiental = "); 
  Serial.println(valor_do_sensor1); 
   
  //INFORMA O VALOR DO SENSOR DE TEMP
  Serial.print("\n");
  Serial.print("Valor do sensor de Temperatura = "); 
  Serial.println(valor_do_sensor2); 
 
  //INFORMA O VALOR DO CONT
  Serial.print("\n");
  Serial.print("Valor cont = "); 
  Serial.println(cont); 
  
  //ESCOLHA
  //Serial.print("\n");
  //Serial.print("Valor da escolha = "); 
  //escolha = Serial.parseInt();
  //Serial.println(escolha);
   
  //if(escolha == 1){
    //Se for detectado movimento e a falta de luz
    if (sensormov == HIGH && valor_do_sensor1 >= 74){

     // long tempo = millis();
        //TEMPERATURA   
        if(valor_do_sensor2 >=280){ //Quando a temperatura passar de 100 graus c 
          digitalWrite(rele, LOW); //Desligar a fonte de calor

        }else{
          digitalWrite(rele, HIGH); //Ligar a fonte de calor
        }


         //COR DA LED
         if(cont%2==0){
           strip.fill(strip.Color(255, 0, 0)); // Defina a cor vermelha para todos os pixels na fita de LED
           strip.show();// Atualize a fita de LED com as novas cores 

         }else{
            if(cont%3 == 0){
             strip.fill(strip.Color(0, 255, 0)); // Defina a cor verde para todos os pixels na fita de LED
             strip.show();// Atualize a fita de LED com as novas cores 

            }else{
             strip.fill(strip.Color(0, 0, 255)); // Defina a cor azul para todos os pixels na fita de LED
             strip.show();// Atualize a fita de LED com as novas cores 
            }
          }


        delay(3000); // Espere mais 8 segundo antes de repetir o loop
        cont++;

     //Senao
    }else{ 

      strip.clear(); // Defina todas as cores dos pixels como preto (desligado)
      strip.show();// Atualize a fita de LED com as cores definida

      digitalWrite(rele, LOW); //Desligar a fonte de calor
    }

  }
  
//}

//if(escolha == 2){
  //digitalWrite(rele, HIGH); //Desligar a fonte de calor
  
//}
//}

//void setColor(){
  //redColor = random( 255,0,0);
  //greenColor = random(0,255,0);
  //blueColor = random(0,0, 255);
  
//}

