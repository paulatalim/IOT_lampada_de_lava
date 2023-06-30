#define led 13

void setup () {
  pinMode(led, OUTPUT);
}

//Variaveis para marcar o tempo
unsigned long time_now;
unsigned long time_start = 0;

//Estipula o tempo do sensor
unsigned long intervalo_pir = 1000;
bool estado_sensor = false;

void loop() {
  //marca o tempo de agora
  time_now = millis();
  
  //Verifica o intervalo
  if (time_now - time_start >= intervalo_pir) {
    //Reinicia o tempo de inicio
    //Marca a ultima reicidencia
    time_start = millis();
    
    //Troca o estado da led
    estado_sensor = !estado_sensor;
    digitalWrite(led, estado_sensor);
  }
}