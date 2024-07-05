#ifndef IMU_H_
#define IMU_H_

#include <Arduino.h>
#include <Adafruit_BNO055.h>
#include <Define.h>

#include <vector>

class IMU {
public:
    IMU() {};

    void init();
    std::vector<byte> convertOrient2Bytes();
    imu::Vector<3> getOrientVector() {return bno.getVector(Adafruit_BNO055::VECTOR_EULER);}

    void printOrient();
    void printQuat();
private:

private:
    Adafruit_BNO055 bno = Adafruit_BNO055(BNO_DEFAULT, BNO_ADDR, &BNO_PORT);

};

#endif // IMU_H_ 
