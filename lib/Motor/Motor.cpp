#include "Motor.h"

Motor::Motor(uint8_t id, bool isReversed, Dynamixel2Arduino* motor) {
    this->id = id;
    this->isReversed = (isReversed ? -1 : 1);
    this->motor = motor;
}

void Motor::init() {
    if(motor->ping(id)) {motor->ledOn(id);}
    else {Serial.printf("init id%d failed\n", id);}
    motor->torqueOff(id);
    motor->setOperatingMode(id, OP_VELOCITY);
    motor->writeControlTableItem(ControlTableItem::VELOCITY_LIMIT, id, DXL_MAX_VELOCITY);
    motor->torqueOn(id);
    currMode = OP_VELOCITY;
}

void Motor::setTargetVelocity(int velocity) {
    int percentSpeed = constrain(velocity/static_cast<float>(MAX_SPEED) * DXL_MAX_VELOCITY, -DXL_MAX_VELOCITY, DXL_MAX_VELOCITY);
    // Serial.println(percentSpeed);
    motor->setGoalVelocity(id, percentSpeed * isReversed, UNIT_RAW);
}

float Motor::getCurrentVelocity() {
    float currVel = (motor->getPresentVelocity(id, UNIT_RPM)/60 * 2 * M_PI * isReversed);
    Serial.printf("id:%d Val: %f\n", id, currVel);
    return currVel;
}
