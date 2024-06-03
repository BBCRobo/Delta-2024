#include "LS.h"

void LS::init() {
    pinMode(LS_CONTROL, OUTPUT);
    analogWrite(LS_CONTROL, 67);
    pinMode(LS_1, INPUT);
    pinMode(LS_2, INPUT);
    pinMode(LS_3, INPUT);
}

void LS::read() {
    Serial.printf("1: %d 2: %d 3: %d\n", analogRead(LS_1), analogRead(LS_2), analogRead(LS_3));
}

bool LS::isBlackTile() {
    int count = 0;
    double LSnow[LS_COUNT];

    for(int i = 0; i < LS_COUNT; i++) {
        LSnow[i] = analogRead(LSs[i]);
        if (LSinit[i] - LSnow[i] > LSdiff[i]) {
            count += 1;
        }
    }

    // for (int i = 0; i < LS_COUNT; ++i) {
    //     Serial.printf("%d - %d = %d > %d (%d)\t", (int)LSinit[i], (int)LSnow[i], (int)LSinit[i]-(int)LSnow[i], (int)LSdiff[i], ((int)LSinit[i] - (int)LSnow[i]) > (int)LSdiff[i]);
    // }
    // Serial.println();

    if(count == LS_COUNT) {return true;}
    return false;
}

bool LS::isSilverTile() {
    int count = 0;
    double LSnow[LS_COUNT];

    for(int i = 0; i < LS_COUNT; i++) {
        LSnow[i] = analogRead(LSs[i]);
        if (LSinit[i] - LSnow[i] > LSsilverdiff[i]) {
            count += 1;
        }
    }

    // for (int i = 0; i < LS_COUNT; ++i) {
        // Serial.printf("%d - %d = %d > %d (%d)\t", (int)LSinit[i], (int)LSnow[i], (int)LSinit[i]-(int)LSnow[i], (int)LSsilverdiff[i], ((int)LSinit[i] - (int)LSnow[i]) > (int)LSsilverdiff[i]);
    // }
    // Serial.println();

    if(count == LS_COUNT) {return true;}
    return false;
}
