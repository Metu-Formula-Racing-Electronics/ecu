#include <Arduino.h>
#include "SoftwareSerial.h"
#include "Nextion.h"
#include <SPI.h>
#include <SD.h>
#include "lcd.h"

NexText textSpeedValue;
NexText textRPMValue;
NexText textCurrentValue;

void startLcd()
{
    nexInit();
    
    textSpeedValue = NexText(0,4,"textSpeedValue");
    textRPMValue = NexText(0,4,"textRPMValue");
    textCurrentValue = NexText(0,4,"textCurrentValue");
}

void writeLcd(int speed, int rpm, float current)
{
    char speedTemp[10] = {0}; 
    itoa(speed, speedTemp, 10);

    char rpmTemp[10] = {0}; 
    itoa(rpm, rpmTemp, 10);

    static char currentTemp[6];
    dtostrf(current, 6, 2, currentTemp);
    
    textSpeedValue.setText(speedTemp);
    textRPMValue.setText(rpmTemp);
    textCurrentValue.setText(currentTemp);

}
