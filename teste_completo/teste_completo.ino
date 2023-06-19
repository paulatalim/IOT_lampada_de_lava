#include <Adafruit_NeoPixel.h>
#include <PubSubClient.h>
#include <arduino.h>
#include <WiFi.h>
#include <dht.h>

/*** Definicoes para o MQTT ***/
#define TOPICO_SUBSCRIBE_SISTEMA "lampadadelava/atuador/ligadesliga"
#define TOPICO_SUBSCRIBE_LED_AUTOMATICO "lampadadelava/atuador/auto"
#define TOPICO_SUBSCRIBE_LED_MANUAL "lampadadelava/atuador/ledcor"
#define TOPICO_PUBLISH_TEMPERATURA "lampadalava/sensor/temperatura"
#define TOPICO_PUBLISH_LUMINOSIDADE "lampadalava/sensor/luminosidade"
#define TOPICO_PUBLISH_MOVIMENTO "lampadalava/sensor/movimento"
#define ID_MQTT "IoT_PUC_SG_mqtt" //id mqtt (para identificação de sessão)

const char* BROKER_MQTT = "test.mosquitto.org";
int BROKER_PORT = 1883; // Porta do Broker MQTT

// celular wifi
const char* SSID    = "AndroidAPT";
const char* PASSWORD = "nfwb6809";

// Variáveis e objetos globais
WiFiClient espClient; // Cria o objeto espClient
PubSubClient MQTT(espClient); // Instancia o Cliente MQTT passando o objeto espClient

/*** SENSORES E CONTROLES ***/
#define LED_COUNT 16 //Numero de pixels da led
#define SENSORTEMP 14 // Sensor de temperatura
#define SENSORLUZ 32 // Sensor de luminosidade
#define SENSORMOV 39 // Sensor de movimento
#define RELE 27 // rele
#define pin 12 // LED

int luminosidade;
int temperatura;
int movimento;
bool modo_automatico_ativado = true;

unsigned long time_now;
unsigned long time_start_mov = 0;
unsigned long time_start_color = 0;
unsigned long time_start_sensor = 0;
unsigned long intervalo_pir = 300000;
unsigned long intervalo_led_cor = 100;
unsigned long intervalo_leitura_sensor = 1000;
int cont = 0;

dht temp; //Variavel da temperatura
Adafruit_NeoPixel strip(LED_COUNT, pin, NEO_GRB + NEO_KHZ800);// Cria um objeto de pixel da fita de LED

/*** Prototypes ***/
void initWiFi(void);
void initMQTT(void);
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void reconnectMQTT(void);
void reconnectWiFi(void);
void VerificaConexoesWiFIEMQTT(void);

void desligar_sistema () {
  digitalWrite (RELE, HIGH); //liga o RELE
  strip.clear();// Defina todas as cores dos pixels como preto (desligado)
  strip.show(); // Atualize a fita de LED com as novas cores 
}

/**
 Inicializa e conecta-se na rede WI-FI desejada
*/
void initWiFi (void) {
  delay(10);
  Serial.println("------Conexao WI-FI------");
  Serial.print("Conectando-se na rede: ");
  Serial.println(SSID);
  Serial.println("Aguarde");

  reconnectWiFi();
}

/**
  Inicializa parâmetros de conexão MQTT(endereço do
  broker, porta e inicializa a função de callback)
*/
void initMQTT(void) {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);   //informa qual broker e porta deve ser conectado
  MQTT.setCallback(mqtt_callback);            //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}


int red = 0, green = 0, blue = 0;

void led_modo_automatico () {
  // Controla o tempo de cada cor
  if (time_now - time_start_color >= intervalo_led_cor) {
    time_start_color = time_now;

    //Ajuste na cor
    if (green < 255 && blue == 0) {
      green ++;
    } else if (red > 0 && green == 255) {
      red --;
    } else if (red == 0 && blue < 255) {
      blue ++;
    } else if (red == 0 && green > 0) {
      green --;
    } else if (red < 255 && blue == 255) {
      red ++;
    } else if (red == 255 && blue > 0) {
      blue --;
    }

    //Atualiza a cor
    strip.fill(strip.Color(red, green, blue));
    strip.show();
  }
}

int configuracao_cor [3];

void led_modo_manual () {
  strip.fill(strip.Color(configuracao_cor[0], configuracao_cor[1], configuracao_cor[2]));
  strip.show();
}

/** 
  FUNÇÃO CALLBACK
  Esta função é chamada toda vez que uma informação de
  um dos tópicos subescritos chega)
*/
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  String msg;

  /* obtem a string do payload recebido */
  for (int i = 0; i < length; i++) {
    char c = (char)payload[i];
    msg += c;
  }
  
  Serial.print("Chegou a seguinte string via MQTT: ");
  Serial.println(msg);

  /* toma ação dependendo da string recebida */
  if (msg.equals("L")) {
    time_start_mov = time_now;
    Serial.println("LED aceso mediante comando MQTT");
  
  } else if (msg.equals("D")) {
    time_start_mov = time_now + intervalo_pir + 1;
    desligar_sistema();
    Serial.println("Sistema desligado pelo MQTT");

  } else if (msg.equals("A")) {
    modo_automatico_ativado = true;
  
  } else if (msg.length() > 1) {
    modo_automatico_ativado = false;
    int indice = 0;
    String num = "";

    for (int i = 0; i < msg.length(); i++) {

      if (msg.charAt(i) == ',') {
        indice ++;
        configuracao_cor[indice] = num.toInt();
        num = "";
      } else if (msg.charAt(i) != '[' && msg.charAt(i) != ']') {
        num += msg.charAt(i);
      }
      
    }

    strip.fill(strip.Color(configuracao_cor[0], configuracao_cor[1], configuracao_cor[2]));
    strip.show();
  } else {
    Serial.println("Não identificou comando MQTT recebido.");
  }
}

/**
  Reconecta-se ao broker MQTT (caso ainda não esteja conectado ou em caso de a conexão cair)
  em caso de sucesso na conexão ou reconexão, o subscribe dos tópicos é refeito.
*/
void reconnectMQTT(void) {
  while (!MQTT.connected()) {
    Serial.print("* Tentando se conectar ao Broker MQTT: ");
    Serial.println(BROKER_MQTT);
    
    if (MQTT.connect(ID_MQTT)) {
      Serial.println("Conectado com sucesso ao broker MQTT!");
      MQTT.subscribe(TOPICO_SUBSCRIBE_SISTEMA);
      MQTT.subscribe(TOPICO_SUBSCRIBE_LED_AUTOMATICO);
      MQTT.subscribe(TOPICO_SUBSCRIBE_LED_MANUAL);
    
    } else {
      Serial.println("Falha ao reconectar no broker.");
      Serial.println("Havera nova tentativa de conexao em 2s");
      delay(2000);
    }
  }  
}

/**
  Reconecta-se ao WiFi
*/
void reconnectWiFi(void) {
  //se já está conectado à rede WI-FI, nada é feito.
  //Caso contrário, são efetuadas tentativas de conexão
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  WiFi.begin(SSID, PASSWORD); // Conecta na rede WI-FI

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Conectado com sucesso na rede ");
  Serial.print(SSID);
  Serial.println("\nIP obtido: ");
  Serial.println(WiFi.localIP());
}

/**
  Verifica o estado das conexões WiFI e ao broker MQTT.
  Em caso de desconexão (qualquer uma das duas), a conexão
  é refeita.
*/
void VerificaConexoesWiFIEMQTT(void) {
  if (!MQTT.connected()) {
    reconnectMQTT(); //se não há conexão com o Broker, a conexão é refeita
  }

  reconnectWiFi(); //se não há conexão com o WiFI, a conexão é refeita
}

void setup() {
  Serial.begin(9600); //Monitor serial
  pinMode(SENSORLUZ, INPUT); //DEFINE O PINO COMO ENTRADA
  pinMode(RELE, OUTPUT); //DEFINE O PINO COMO SAIDA
  pinMode(SENSORMOV, INPUT); //DEFINE O PINO COMO ENTRADA

  //COMANDOS LED
  strip.begin(); // Inicialize a fita de LED
  strip.clear(); // Defina todas as cores dos pixels como preto (desligado)
  strip.show(); // Atualize a fita de LED com as cores definidas

  /* Inicializa a conexao wi-fi */
  initWiFi();

  /* Inicializa a conexao ao broker MQTT */
  initMQTT();
}

void loop() {
  char temperatura_str[10] = {0};
  char luminosidade_str[10] = {0};

  /* garante funcionamento das conexões WiFi e ao broker MQTT */
  VerificaConexoesWiFIEMQTT();

  time_now = millis();

  //Controla o tempo de leitura dos sensores
  if (time_now - time_start_sensor >= intervalo_leitura_sensor) {
    time_start_sensor = time_now;

    //INFORMA O VALOR DO FOTORESISTOR
    luminosidade = analogRead(SENSORLUZ);
    Serial.println();
    Serial.print("Valor do sensor de luminosidade = "); 
    Serial.println(luminosidade);
    sprintf(luminosidade_str, "%d", luminosidade);
    MQTT.publish(TOPICO_PUBLISH_LUMINOSIDADE, luminosidade_str, 1);

    //INFORMA O VALOR DO SENSOR DE TEMPERATURA
    temp.read11(SENSORTEMP); //LÊ AS INFORMAÇÕES DO SENSOR
    temperatura = temp.temperature;	
    Serial.print("Valor do sensor de Temperatura = "); 
    Serial.println(temperatura , 0);
    sprintf (temperatura_str, "%d", temperatura);
    MQTT.publish(TOPICO_PUBLISH_TEMPERATURA, temperatura_str, 1); 

    //INFORMA O VALOR DO SENSOR DE MOVIMENTO
    movimento = digitalRead(SENSORMOV);
    Serial.print("Movimento: "); 
    if (movimento == HIGH) {
      time_start_mov = time_now;
      // time_start_color = time_now;
      MQTT.publish(TOPICO_PUBLISH_MOVIMENTO, "1", 1); 
      Serial.println("1");
    } else {
      MQTT.publish(TOPICO_PUBLISH_MOVIMENTO, "0", 1); 
      Serial.println("0");
    }
  }
  
  if (time_now - time_start_mov <= intervalo_pir) {
    // Controla o aquecedor
    if (temperatura <= 35) {
      // Liga o aquecedor
      digitalWrite (RELE, LOW);
    } else if (temperatura >= 45) {
      // Desliga o aquecedor
      digitalWrite (RELE, HIGH);
    }
    
    // Controla a led
    if (luminosidade <= 1900) {
      if (modo_automatico_ativado) {
        led_modo_automatico();
      } else {
        led_modo_manual();
      }
    } else {
      strip.clear();// Defina todas as cores dos pixels como preto (desligado)
      strip.show(); // Atualize a fita de LED com as novas cores 
    }
  } else {
    desligar_sistema();
  }

  MQTT.loop();
}