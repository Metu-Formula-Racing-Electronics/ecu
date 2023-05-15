#include <Arduino.h>
#include "sensor.h"
#include "mp3.h"
#include "SPIFFS.h"

void setup() {
  //TEST CODE FOR THE SPIFFS 
  // Serial.begin(115200);

  // if (!SPIFFS.begin(true)) {
  //   Serial.println("An Error has occurred while mounting SPIFFS");
  //   return;
  // }
 
  // File root = SPIFFS.open("/");
 
  // File file = root.openNextFile();
 
  // while(file){
 
  //     Serial.print("FILE: ");
  //     Serial.println(file.name());
 
  //     file = root.openNextFile();
  // }

  mp3_setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  //master brachteyim
  // mp3_loop();
  mp3_loop();
}