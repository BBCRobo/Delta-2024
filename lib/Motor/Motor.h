#ifndef MOTOR_H_
#define MOTOR_H_

#include <Arduino.h>
#include <Define.h>
#include <Dynamixel2Arduino.h>

// NOTE: I overload the constructor so I can privately declare the instances of
//	 the motors in MotorArray.h, and can redefine them with input variables
//	 in the constructor of MotorArray later on

class Motor {
public:
    Motor() {}

    Motor(uint8_t id, bool isReversed, Dynamixel2Arduino* motor);

    void init();
    void setTargetVelocity(int velocity);
    uint8_t getCurrentVelocity();

private:
    uint8_t id;
    int isReversed;
    Dynamixel2Arduino* motor;
    
    OperatingMode currMode;
};

#endif // MOTOR_H_
