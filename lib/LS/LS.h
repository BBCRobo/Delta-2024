#ifndef LS_H_
#define LS_H_

#include <Arduino.h>
#include <Define.h>
#include <Adafruit_APDS9960.h>
#include <SparkFun_I2C_Mux_Arduino_Library.h>

class LS
{
public:
    LS(QWIICMUX* mux) : LSMux(mux) {};

    void init();
    void readLight();
    void readColour();

    tile_colour_t getTileType();

private:
    bool isBlackTile();
    bool isSilverTile();
    bool isBlueTile();

private:
    QWIICMUX* LSMux;
    Adafruit_APDS9960 colour;

    int LSs[LS_COUNT] = {LS_1, LS_2, LS_3};
    double LSinit[LS_COUNT] = {910, 920, 910};
    double LSdiff[LS_COUNT] = {500, 500, 500};
    double LSsilverdiff[LS_COUNT] = {200, 200, 200};
};

#endif