#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


#define WLAN_SSID         "SENAI-LABS"
#define WLAN_PASS         "senai@2022#*@"

#define AIO_SERVER        "io.adafruit.com"
#define AIO_SERVERPORT    1883                   // use 8883 for SSL
#define AIO_USERNAME      "Robotgamert324"
#define AIO_KEY           "aio_uvPF459kBrOpcfeE1C40z0nHAq7M"

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Publish humidadesoloFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidadesolo");


//------------------------------------------------------------------------------------------------------------
/*
#include <Wire.h> // Biblioteca utilizada para fazer a comunicação com o I2C
#include <LiquidCrystal_I2C.h> // Biblioteca utilizada para fazer a comunicação com o display 20x4 

#define col 16 // Serve para definir o numero de colunas do display utilizado
#define lin  2 // Serve para definir o numero de linhas do display utilizado
#define ende  0x27 // Serve para definir o endereço do display.

LiquidCrystal_I2C lcd(ende,col,lin); // Chamada da funcação LiquidCrystal para ser usada com o I2C*/

//------------------------------------------------------------------------------------------------------------

// Define o pino usado para a leitura analógica
const int sensorPin = A0;

// Define a faixa de valores para a umidade do solo
const int HumidadeMinima = 100;      // Umidade mínima
const int HumidadeMaxima = 0;    // Umidade máxima
const int valorMinimo = 569;        // Valor mínimo lido no pino analógico
const int valorMaximo = 1024;     // Valor máximo lido no pino analógico

//------------------------------------------------------------------------------------------------------------
unsigned long startTime = 0; // Tempo inicial
unsigned long currentTime = 0; // Tempo atual
unsigned long elapsedSeconds = 0; // Segundos decorridos
int pinBomba = D5;                       // Pino da bomba de água

//------------------------------------------------------------------------------------------------------------

void MQTT_connect() {

  int8_t ret;

}

void setup() {

  WiFi.begin(WLAN_SSID, WLAN_PASS);

  MQTT_connect();

//--------------------------------------------------------------------------------------------------------------
/*
  lcd.init(); // Serve para iniciar a comunicação com o display já conectado
  lcd.backlight(); // Serve para ligar a luz do display
  lcd.clear(); // Serve para limpar a tela do display*/

//--------------------------------------------------------------------------------------------------------------

  Serial.begin(9600);
 startTime = millis(); // Armazena o tempo inicial

//--------------------------------------------------------------------------------------------------------------

pinMode(pinBomba, OUTPUT);

//--------------------------------------------------------------------------------------------------------------

}

  void loop() {

  MQTT_connect();

//--------------------------------------------------------------------------------------------------------------

  // Lê o valor analógico do sensor
  int sensorValue = analogRead(sensorPin);

  // Converte o valor lido para a umidade do solo na faixa especificada
  int Humidadesolo = map(sensorValue, valorMinimo, valorMaximo, HumidadeMinima, HumidadeMaxima);

//--------------------------------------------------------------------------------------------------------------

  Serial.println("Umi Solo:");  
  Serial.println(Humidadesolo);

  humidadesoloFeed.publish(Humidadesolo);

//--------------------------------------------------------------------------------------------------------------

unsigned long currentTime = millis(); // Obtém o tempo atual
unsigned long elapsedWeeks = (currentTime - startTime) / (1000UL * 60 * 60 * 24 * 7); // Calcula o tempo decorrido em semanas

// Verifica a condição para ativar a bomba de água
  if (Humidadesolo < 2 && elapsedWeeks >= 1){
  
  digitalWrite(pinBomba,  LOW );
  startTime = currentTime; // Reinicia o tempo inicial

  }
   else if(Humidadesolo > 50){

     digitalWrite(pinBomba, HIGH);
     startTime = currentTime; // Reinicia o tempo inicial

    }
}




