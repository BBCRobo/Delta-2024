#ifndef MOTOR_ARRAY_H_
#define MOTOR_ARRAY_H_

#include <Arduino.h>
#include <Define.h>
#include <Motor.h>

class MotorArray {
public:
    MotorArray(HardwareSerial& port, int dir_pin);

    void init();
    void setTargetVelocityRaw(int16_t left, int16_t right);
    void getCurrentVelocityRaw(int16_t& left_vel, int16_t& right_vel);

private:
    Motor backLeft;
    Motor backRight;
    Motor frontLeft;
    Motor frontRight;

    Dynamixel2Arduino dxl;
};

#endif // MOTOR_ARRAY_H_
