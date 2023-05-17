#pragma once

#include <Arduino.h>
class sensorMFR
{

public:
        virtual void setup() = 0;
        virtual void writeSensor() = 0;
        virtual void readAnalog(int analogPin) = 0;
        virtual void readDigital(int digitalPin) = 0;


};





