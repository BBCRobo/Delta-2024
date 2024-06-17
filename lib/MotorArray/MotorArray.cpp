#include "MotorArray.h"

MotorArray::MotorArray(HardwareSerial& port, int dir_pin) {
    dxl = Dynamixel2Arduino(port, dir_pin);

    backLeft = Motor(BL_ID, BL_REVERSED, &dxl);
    backRight = Motor(BR_ID, BR_REVERSED, &dxl);
    frontLeft = Motor(FL_ID, FL_REVERSED, &dxl);
    frontRight = Motor(FR_ID, FR_REVERSED, &dxl);
}

void MotorArray::init() {
    dxl.begin(DXL_BAUD);
    dxl.setPortProtocolVersion(DXL_PROT_VER);
    frontLeft.init();
    frontRight.init();
    backLeft.init();
    backRight.init();
}

void MotorArray::setTargetVelocity(uint8_t status, uint8_t left, uint8_t right) {
    int left_vel = (left > MAX_SPEED ? MAX_SPEED : left);
    int right_vel = (right > MAX_SPEED ? MAX_SPEED : right);

    left_vel *= ((status >> 1) & 0x01) == 1 ? -1 : 1;
    right_vel *= (status & 0x01) == 1 ? -1 : 1;

    // Serial.printf("L:%d R:%d\n", left_vel, right_vel);

    frontLeft.setTargetVelocity(left_vel);
    frontRight.setTargetVelocity(right_vel);
    backLeft.setTargetVelocity(left_vel);
    backRight.setTargetVelocity(right_vel);
}

std::array<float, 2> MotorArray::getCurrentVelocity() {
    float left_vel = static_cast<float>((frontLeft.getCurrentVelocity() + backLeft.getCurrentVelocity())/2);
    float right_vel = static_cast<float>((frontRight.getCurrentVelocity() + backRight.getCurrentVelocity())/2);
    return {left_vel, right_vel};
}
