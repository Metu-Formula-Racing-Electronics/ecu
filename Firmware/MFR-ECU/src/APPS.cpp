#include "sensor.h"
#include <Arduino.h>

#define APPS1PIN 36
#define APPS2PIN 39

int AppsError(){
    return -1;
}
int AppsRead() {
    int difference, Apps1deger, Apps2deger,Apps1,Apps2;
    float AppsOrtValue;
    
    Apps1deger= analogRead(APPS1PIN);
    Apps1 = Apps1deger*100/4095;
    Apps2deger= 2048-analogRead(APPS2PIN)*100/4095;         //  (Hangi pin olduğunu kontrol et)
    Apps2 = Apps2deger*100/4095;
    difference = abs(Apps1-Apps2);
    AppsOrtValue = ((Apps1deger+Apps2deger)/2) ;

    if (difference <= 10){
        return AppsOrtValue;
    }   else {
        return AppsError();
    }
}