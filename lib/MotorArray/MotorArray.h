#ifndef MOTOR_ARRAY_H_
#define MOTOR_ARRAY_H_

#include <Arduino.h>
#include <Define.h>
#include <Motor.h>
#include <array>

class MotorArray {
public:
    MotorArray(HardwareSerial& port, int dir_pin);

    void init();
    void setTargetVelocity(uint8_t status, uint8_t left, uint8_t right);
    uint16_t getEncoderDist(float currPitch, unsigned long &lastTime);

private:
    std::array<float, 2> getCurrentVelocity();

    double tile_distx = ENCODER_OFFSET;

    Motor backLeft;
    Motor backRight;
    Motor frontLeft;
    Motor frontRight;

    Dynamixel2Arduino dxl;
};

#endif // MOTOR_ARRAY_H_
