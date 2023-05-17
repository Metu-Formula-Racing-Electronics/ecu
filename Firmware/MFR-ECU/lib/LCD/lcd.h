#pragma once

#include "Nextion.h"
#include <SD.h>

#include <sensor.h>


class lcd: sensorMFR
{
private:
    NexText mTextSpeedValue;
    NexText mTextRPMValue;
    NexText mTextCurrentValue;

    char* mSpeedTempVal;
    char* mRpmTempVal;
    char* mCurrentTempVal;



public:
    void writeSensor() override {return;}
    void writeSensor(int speed,int rpm,float currentVal); //Now we both override and overload the function write from the base class


    void setup() override;
    void readAnalog(int analogPin) override {return;}
    void readDigital(int digitalPin) override {return;}
    


    lcd(/* args */); //Constructor arguments
    ~lcd();
};



