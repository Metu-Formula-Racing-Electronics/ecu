#include <Arduino.h>
#include <NEXTlcd.h>

void nextlcd::writeSensor(int rpm, float currentVal){

    int angle_rpm = (rpm * 3 / 10 + 330) % 360;
    int angle_current = (int(currentVal) *3 + 330) % 360;

    Serial.print("surus.textRPMValue.txt=\"");
    Serial.print(rpm);
    Serial.print("\"\xFF\xFF\xFF");
    Serial.print("surus.textCurrValue.txt=\"");
    Serial.print(currentVal);
    Serial.print("\"\xFF\xFF\xFF");
    Serial.print("surus.gaugeRPM.val=");
    Serial.print(angle_rpm);
    Serial.print("\xFF\xFF\xFF");
    Serial.print("surus.gaugeCurrent.val=");
    Serial.print(angle_current);
    Serial.print("\xFF\xFF\xFF");
}

void nextlcd::changeMode(int mod){
    if(mod == 1) { //mod 1 = surus modu;
        Serial.print("page surus");
    }
    else if(mod == 2){ // mod 2 = otonom;
        Serial.print("page otonom");
    }
    Serial.print("\xFF\xFF\xFF");
}

void nextlcd::changeStatus(String status){
    Serial.print("otonom.textStatus.txt=\"");
    Serial.print(status);
    Serial.print("\"\xFF\xFF\xFF");
}