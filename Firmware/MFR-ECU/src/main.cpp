#include <Arduino.h>
#include "sensor.h"
#include "APPS.h"
#define APPS1PIN 36
#define APPS2PIN 39
void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println(AppsRead());
  delay(100);

}