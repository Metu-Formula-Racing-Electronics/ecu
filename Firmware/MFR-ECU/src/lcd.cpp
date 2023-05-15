#include <Arduino.h>
#include "SoftwareSerial.h"
#include "Nextion.h"
#include <SPI.h>
#include <SD.h>

String readLcd(int speed, int rpm, float current)
{
    nexInit();
    
    NexText textSpeedValue = NexText(0,4,"textSpeedValue");
    NexText textRPMValue = NexText(0,4,"textRPMValue");
    NexText textCurrentValue = NexText(0,4,"textCurrentValue");


    char speedTemp[10] = {0}; 
    itoa(speed, speedTemp, 10);

    char rpmTemp[10] = {0}; 
    itoa(rpm, rpmTemp, 10);

    static char currentTemp[6];
    dtostrf(current, 6, 2, currentTemp);
    
    textSpeedValue.setText(speedTemp);
    textRPMValue.setText(rpmTemp);
    textCurrentValue.setText(currentTemp);

    return(speedTemp, rpmTemp, currentTemp);
}
