#include <Arduino.h>
#include <NEXTlcd.h>


void setup()
{
  Serial.begin(9600);
}

void loop(){

    for (int i = 0; i < 9; i++)
    {
      nextlcd::writeSensor(i * 100, i *10);
      delay(500);
    }
}