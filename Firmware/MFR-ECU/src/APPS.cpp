#include "sensor.h"
#include <Arduino.h>

#define APPS1PIN 4
#define APPS2PIN 5

float AppsError(){

}
float AppsRead() {
    int difference, Apps1deger, Apps2deger;
    float AppsOrtValue;
    
    Apps1deger= analogRead(APPS1PIN)*100/255;
    Apps2deger= analogRead(APPS2PIN)*100/255;
    difference = abs(Apps1deger-Apps2deger);
    AppsOrtValue = ((Apps1deger+Apps2deger)/2) ;

    if (difference <= 10){
        return AppsOrtValue;
    }   else {
        return AppsError();
    }
}