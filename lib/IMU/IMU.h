#ifndef IMU_H_
#define IMU_H_

#include <Arduino.h>
#include <Adafruit_BNO055.h>
#include <Define.h>

class IMU {
public:
    IMU() {};

    void init();

    void getOrientation(imu::Vector<3>& orientation);
    void getAngAccel(imu::Vector<3>& acceleration);
    void getAngVel(imu::Vector<3>& velocity);

    void getDeadBand();
private:
    Adafruit_BNO055 bno = Adafruit_BNO055(BNO_DEFAULT, BNO_ADDR, &BNO_PORT);

};

#endif // IMU_H_ 
