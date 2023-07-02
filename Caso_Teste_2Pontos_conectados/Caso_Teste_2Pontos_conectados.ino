// Neste código foi exemplificado a conexão de dois pontos de iluminação utilizando apenas um interruptor

// Bibliotecas utilizadas
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "fauxmoESP.h"

// Definições dos pinos de saída
#define RELAY_PIN_1 D0
#define RELAY_PIN_2 D1
#define RELAY_PIN_3 D2
#define RELAY_PIN_4 D3
#define RELAY_PIN_5 D4
#define RELAY_PIN_6 D5

// Definição dos pinos de entrada
#define SWITCH_PIN_1 D6
#define SWITCH_PIN_2 D7
#define SWITCH_PIN_3 D9

// Configurações de Rede e log
#define SERIAL_BAUDRATE 115200
#define WIFI_SSID "Wifi TCC"
#define WIFI_PASS "rede-wifi-TCC"

// Definição dos nomes dos dispositivos
#define OUT_1 "Luz Sala"
#define OUT_2 "Tomada Sala"
#define OUT_3 "Luz Cozinha"
#define OUT_4 "Cafeteira"
#define OUT_5 "Luz Quarto"
#define OUT_6 "Tomada Quarto"

fauxmoESP fauxmo;

// Setup de configuração do Wi-Fi
void wifiSetup() {
  WiFi.mode(WIFI_STA);
  Serial.printf("[WIFI] Conectado ao %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Aguardando conexão
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  // Conectado
  Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}

//Variáveis para controle de estado
bool state_1 = false;
uint8_t state_button_1 = 0;
bool state_2 = false;
uint8_t state_button_2 = 0;
bool state_3 = false;
uint8_t state_button_3 = 0;

//Função que trata ações vindas dos interruptores
void checkSwitchState(uint8_t pinIn, uint8_t* stateButton, bool* state, char* device_name, uint8_t pinOut) {
  uint8_t currentState = digitalRead(pinIn);
  if(currentState != *stateButton) {
    *state = !*state;
    *stateButton = currentState;
    digitalWrite(pinOut, *state ? LOW : HIGH);
    fauxmo.setState(device_name, *state, 128);
  }
}

void setup() {
  // Configuração inicial
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println();

  // Configuração Wi-Fi
  wifiSetup();

  // Definição das portas utilizadas para acionar cargas
  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);
  pinMode(RELAY_PIN_3, OUTPUT);
  pinMode(RELAY_PIN_4, OUTPUT);
  pinMode(RELAY_PIN_5, OUTPUT);
  pinMode(RELAY_PIN_6, OUTPUT);

  // Definição do estado inicial das cargas
  digitalWrite(RELAY_PIN_1, HIGH);
  digitalWrite(RELAY_PIN_2, HIGH);
  digitalWrite(RELAY_PIN_3, HIGH);
  digitalWrite(RELAY_PIN_4, HIGH);
  digitalWrite(RELAY_PIN_5, HIGH);
  digitalWrite(RELAY_PIN_6, HIGH);

  // Definição das entradas dos interruptores
  pinMode(SWITCH_PIN_1, INPUT_PULLUP);
  pinMode(SWITCH_PIN_2, INPUT_PULLUP);
  pinMode(SWITCH_PIN_3, INPUT_PULLUP);

  // Configuração da FAUXMO
  fauxmo.createServer(true);
  fauxmo.setPort(80);
  fauxmo.enable(true);

  // Adiciona os dispositivos
  fauxmo.addDevice(OUT_1);
  fauxmo.addDevice(OUT_2);
  fauxmo.addDevice(OUT_3);
  fauxmo.addDevice(OUT_4);
  fauxmo.addDevice(OUT_5);
  fauxmo.addDevice(OUT_6);

  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
    // Retorno de chamada quando um comando da Alexa é recebido

    Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);

    // Lógica para identificação da carga correspondente
    if (strcmp(device_name, OUT_1)==0) {
      digitalWrite(RELAY_PIN_1, state ? LOW : HIGH);
      state_1 = state;
    } else if (strcmp(device_name, OUT_2)==0) {
      digitalWrite(RELAY_PIN_2, state ? LOW : HIGH);
    } else if (strcmp(device_name, OUT_3)==0) {
      digitalWrite(RELAY_PIN_3, state ? LOW : HIGH);
      state_2 = state;
    } else if (strcmp(device_name, OUT_4)==0) {
      digitalWrite(RELAY_PIN_4, state ? LOW : HIGH);
    } else if (strcmp(device_name, OUT_5)==0) {
      digitalWrite(RELAY_PIN_5, state ? LOW : HIGH);
      state_3 = state;
    } else if (strcmp(device_name, OUT_6)==0) {
      digitalWrite(RELAY_PIN_6, state ? LOW : HIGH);
    }
  });
}

void loop() {
  // Pesquisa manual ao server UDP
  fauxmo.handle();

  checkSwitchState(SWITCH_PIN_1, &state_button_1, &state_1, OUT_2, RELAY_PIN_2);
  checkSwitchState(SWITCH_PIN_1, &state_button_2, &state_2, OUT_4, RELAY_PIN_4);
}
