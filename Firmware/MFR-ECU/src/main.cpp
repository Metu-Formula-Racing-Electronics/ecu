#include <SPI.h>
#include <lcd.h>
#include <analog.h>


int  main() 
{
  Serial.begin(9600);

  lcd* nextion = new lcd();
  analog* pot = new analog(35,10,15);//Test for analog sensor was put in Analog pin 35 in esp 32.

  nextion->setup();
  pot->setup();


  while(true)
  {

      pot->readAnalog(pot->mAnalogPin);

      nextion->writeSensor(pot->mAnalogRawResult,pot->mAnalogFilteredResult,0);

      delay(100);
  }


  return 0;
}