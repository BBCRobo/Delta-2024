#include "TEMP.h"

void TEMP::init() {
    pinMode(TEMP_MUX_ENABLE, OUTPUT);
    digitalWrite(TEMP_MUX_ENABLE, HIGH);
    TempMux.begin(MUX_ADDR, TEMP_MUX);

    TempMux.setPort(TEMPL);
    TMPS.begin();
    delay(20);
    TempMux.setPort(TEMPR);
    TMPS.begin();
    delay(20);

    TempMux.setPort(TEMPL);
    double a = TMPS.readObjectTempC();

    TempMux.setPort(TEMPR);
    double b = TMPS.readObjectTempC();

    // Serial.printf("A:%fB:%f\n", a, b);
    initTemp = (a+b)/2.00;
}

Direction TEMP::isheatVictim(LRF* lrf) {
    TempMux.setPort(TEMPL);
    double a = TMPS.readObjectTempC();
    TempMux.setPort(TEMPR);
    double b = TMPS.readObjectTempC();
    

    if(abs(initTemp-a)>TEMP_THRESHOLD && lrf->victimValid(l)) {return l;}
    else if(abs(initTemp-b)>TEMP_THRESHOLD && lrf->victimValid(r)) {return r;}
    return n;
}

void TEMP::read() {
    TempMux.setPort(TEMPL);
    double a = TMPS.readObjectTempC();
    TempMux.setPort(TEMPR);
    double b = TMPS.readObjectTempC();
    Serial.printf("L:%.2fR:%.2f\n",a,b);  
}