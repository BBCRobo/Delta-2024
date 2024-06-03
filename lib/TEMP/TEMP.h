#ifndef TEMP_H_
#define TEMP_H_

#include <Arduino.h>
#include <Define.h>
#include <SparkFun_I2C_Mux_Arduino_Library.h>
#include <Adafruit_MLX90614.h>

class TEMP
{
public:
    TEMP(){};
    
    void init();
    void read();
    
    victim_type_t isheatVictim();

private:
    QWIICMUX TempMux;
    Adafruit_MLX90614 TMPS;

    float initTemp;
};

#endif