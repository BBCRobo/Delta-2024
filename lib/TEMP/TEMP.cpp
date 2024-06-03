#include "TEMP.h"

void TEMP::init() {
    pinMode(TEMP_MUX_ENABLE, OUTPUT);
    digitalWrite(TEMP_MUX_ENABLE, HIGH);
    TempMux.begin(MUX_ADDR, TEMP_MUX);

    TempMux.setPort(TEMP_L);
    TMPS.begin();
    delay(20);
    TempMux.setPort(TEMP_R);
    TMPS.begin();
    delay(20);

    TempMux.setPort(TEMP_L);
    double a = TMPS.readObjectTempC();

    TempMux.setPort(TEMP_R);
    double b = TMPS.readObjectTempC();

    // Serial.printf("A:%fB:%f\n", a, b);
    initTemp = (a+b)/2.00;
}

victim_type_t TEMP::isheatVictim() {
    TempMux.setPort(TEMP_L);
    double a = TMPS.readObjectTempC();
    TempMux.setPort(TEMP_R);
    double b = TMPS.readObjectTempC();
    

    if(abs(initTemp-a)>TEMP_THRESHOLD) {return victim_type_t::LEFT;}
    else if(abs(initTemp-b)>TEMP_THRESHOLD) {return victim_type_t::RIGHT;}
    return victim_type_t::NONE;
}

void TEMP::read() {
    TempMux.setPort(TEMP_L);
    double a = TMPS.readObjectTempC();
    TempMux.setPort(TEMP_R);
    double b = TMPS.readObjectTempC();
    Serial.printf("L:%.2fR:%.2f\n",a,b);  
}