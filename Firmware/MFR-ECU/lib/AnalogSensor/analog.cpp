 #include <analog.h>    
    
    
void analog::setup()
{
        
        pinMode(mAnalogPin, INPUT);
        mAnalogBuffer = new uint32_t[mFilterLength]{0}; 

        mAnalogBufferIncrement  = 0;
        mAnalogRawResult = 0;
        mAnalogFilteredResult = 0;


}

void analog::readAnalog(int analogPin)
{
    mAnalogRawResult = digitalRead(mAnalogPin);
    mAnalogFilteredResult = readAverageAnalog(mAnalogRawResult);
}

void analog::printResults()
{
    Serial.print(mAnalogRawResult);    // Print Raw Reading
    Serial.print(',');                // Comma Separator
    Serial.println(mAnalogFilteredResult);
}


uint32_t  analog::readAverageAnalog(int analogRaw) //For Noise Reduction(Basic)
{

    int i = 0;
    uint32_t sum = 0;

    mAnalogBuffer[mAnalogBufferIncrement++] = mAnalogRawResult;
    if(mAnalogBufferIncrement==mFilterLength)
    {
        mAnalogBufferIncrement = 0;

    }
    for (size_t i = 0; i < mFilterLength; i++)
    {
        sum +=mAnalogBuffer[i];
    }
    return (sum / mFilterLength);

    
}
