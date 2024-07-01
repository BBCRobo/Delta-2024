#include "LS.h"

void LS::init() {
    pinMode(LS_CONTROL, OUTPUT);
    analogWrite(LS_CONTROL, LS_BOARD_BRIGHTNESS);
    pinMode(LS_1, INPUT);
    pinMode(LS_2, INPUT);
    pinMode(LS_3, INPUT);

    LSMux->setPort(COLOUR_MUX_PORT);
    while(!colour.begin(10, APDS9960_AGAIN_4X, APDS9960_ADDRESS, &MUX_PORT)) {
        Serial.println("Did not begin colour sensor");
    }
    colour.enableColor(true);
    delay(10);
    updateColourReadings();
}

void LS::readLight() {
    Serial.printf("1: %d 2: %d 3: %d\n", analogRead(LS_1), analogRead(LS_2), analogRead(LS_3));
}

void LS::readColour() {
    uint16_t r, g, b, c;
    r = g = b = c = 0;
    LSMux->setPort(COLOUR_MUX_PORT);
    if(colour.colorDataReady()) {
        colour.getColorData(&r, &g, &b, &c);
        Serial.printf("R:%uG:%uB:%uC:%u\n", r, g, b, c);
    }
}

void LS::updateColourReadings() {
    uint16_t r, g, b, c = 0;
    LSMux->setPort(COLOUR_MUX_PORT);
    if(!colour.colorDataReady()) {return;} // TODO: Keep previous values and send them
    
    colour.getColorData(&r, &g, &b, &c);
    // Serial.printf("R:%uG:%uB:%uC:%u\n", r, g, b, c);
    colourReadings[0] = r;
    colourReadings[1] = g;
    colourReadings[2] = b;
    colourReadings[3] = c;
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

    bool colour = (colourReadings[0] < BLACK_R && colourReadings[1] < BLACK_G && colourReadings[2] < BLACK_B && colourReadings[3] < BLACK_C);

    if(count == LS_COUNT && colour) {return true;}
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
    // 	Serial.printf("%d - %d = %d > %d (%d)\t", (int)LSinit[i], (int)LSnow[i], (int)LSinit[i]-(int)LSnow[i], (int)LSsilverdiff[i], ((int)LSinit[i] - (int)LSnow[i]) > (int)LSsilverdiff[i]);
    // }
    // Serial.println();

    if(count == LS_COUNT) {return true;}
    return false;
}

bool LS::isBlueTile() {
    return (colourReadings[0] < BLUE_R && colourReadings[1] < BLUE_G && colourReadings[2] >  BLUE_B && colourReadings[3] > BLUE_C);
}

tile_colour_t LS::getTileType() {
    // Might need to put blue first depending on how dark the blue is or if the black thereshold is blue as well
    updateColourReadings();
    if (isBlueTile()) {return tile_colour_t::BLUE;}
    if (isBlackTile()) {return tile_colour_t::BLACK;}
    if (isSilverTile()) {return tile_colour_t::SILVER;}
    
    return tile_colour_t::OTHER;    
}
