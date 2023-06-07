#include <Arduino.h>
#include "sensor.h"
#include "APPS.h"
void setup() {
  Serial.begin(9600);
}

void loop() {
  
  Serial.println(AppsRead());
  delay(100);

}