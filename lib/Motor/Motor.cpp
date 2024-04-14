#include "Motor.h"

Motor::Motor(uint8_t id, bool isReversed, Dynamixel2Arduino* motor) {
    this->id = id;
    this->isReversed = (isReversed ? -1 : 1);
    this->motor = motor;
}

void Motor::init() {
    if(motor->ping(id)) {motor->ledOn(id);}
    motor->torqueOff(id);
    motor->setOperatingMode(id, OP_VELOCITY);
    motor->writeControlTableItem(ControlTableItem::VELOCITY_LIMIT, id, DXL_MAX_VELOCITY);
    motor->torqueOn(id);
    currMode = OP_VELOCITY;
}

void Motor::setTargetVelocityRaw(int16_t velocity) {
    motor->setGoalVelocity(id, velocity, UNIT_RAW);
}

int16_t Motor::getCurrentVelocityRaw() {
    return motor->getPresentVelocity(id, UNIT_RAW);
}
