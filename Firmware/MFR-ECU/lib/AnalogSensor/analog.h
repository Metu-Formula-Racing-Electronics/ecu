#pragma once


#include <sensor.h>
#include "esp_adc_cal.h"


class analog: sensorMFR
{
private:

uint32_t* mAnalogBuffer;

int mAnalogBufferIncrement;


public:


    int mAnalogPin;
    int mWidthBits;
    int mFilterLength;


    int mAnalogRawResult;
    int mAnalogFilteredResult;

    void writeSensor() override {return;}     
    void setup() override;
    void readAnalog(int analogPin) override;
    void readDigital(int digitalPin) override {return;}
    
    uint32_t readAverageAnalog(int analogRaw);
    
    void printResults();

    analog(int analogPin,int widthBits,int filterLength);
    ~analog();
};

analog::analog(int analogPin,int widthBits,int filterLength) //Constructing pin, bits and filter lengths for analog sensor class.
{   
    mAnalogPin = analogPin;
    mWidthBits = widthBits;
    mFilterLength = filterLength;

    if(mWidthBits>0)
    {
        analogSetWidth(mWidthBits);
    }

}

analog::~analog()
{
}


