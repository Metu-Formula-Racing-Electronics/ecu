#include <Arduino.h>
#include "sensor.h"
#include "APPS.h"
#define APPS1PIN 36
#define APPS2PIN 39
void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print(analogRead(APPS1PIN));
  Serial.print(" ");
  Serial.println(analogRead(APPS2PIN));
  /*Serial.println(AppsRead());*/
  delay(100);

}