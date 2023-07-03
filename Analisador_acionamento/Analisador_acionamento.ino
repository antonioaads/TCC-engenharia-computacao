#include <Arduino.h>
const int ledPin = LED_BUILTIN; 
int ledState = LOW; 
unsigned long previousMillis = 0; 
unsigned long startMillis = 0; 
const long interval = 1000;
bool started = false;
bool finished = false;
#define SERIAL_BAUDRATE 115200

void setup() {
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println();
  pinMode(ledPin, OUTPUT);
  pinMode(2, INPUT);
  digitalWrite(13, HIGH);
}

void loop() {
  unsigned long currentMillis = millis();
  if(finished == false){
    if (currentMillis - previousMillis >= interval && started == false) {
      started = true;
      startMillis = currentMillis;
      digitalWrite(13, LOW);
    }

    int sensorVal = digitalRead(2);
    if (sensorVal == HIGH) {
      finished = true;
      Serial.print(millis() - startMillis);
      Serial.println();
      digitalWrite(13, HIGH);
    } 
  }else {
    delay(10000);
    finished = false;
    started = false;
    previousMillis = millis();
  }
}
