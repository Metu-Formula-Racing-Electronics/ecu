#include "lcd.h"



void lcd::setup()
{
   nexInit();


    mSpeedTempVal= new char[10];
    mRpmTempVal = new char[10];
    mCurrentTempVal = new char[6];

    mTextSpeedValue = NexText(0,4,"mTextSpeedValue");
    mTextRPMValue = NexText(0,4,"mTextRPMValue");
    mTextCurrentValue = NexText(0,4,"mTextCurrentValue");

}

void lcd::writeSensor(int speed,int rpm,float currentVal)
{

    if(!(mSpeedTempVal==nullptr))
    {
        itoa(speed, mSpeedTempVal, 10);
    }
    
      if(!(mRpmTempVal==nullptr))
    {
        itoa(rpm, mRpmTempVal, 10);
    }
    
      if(!(mCurrentTempVal==nullptr))
    {
        
        dtostrf(currentVal, 6, 2, mCurrentTempVal);

    }

    const char *speedTemp = mSpeedTempVal;
    const char *RPMTemp = mRpmTempVal;
    const char *currentTemp = mCurrentTempVal;


    mTextSpeedValue.setText(speedTemp);
    mTextRPMValue.setText(RPMTemp);
    mTextCurrentValue.setText(currentTemp);


}