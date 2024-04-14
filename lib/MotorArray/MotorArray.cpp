#include "MotorArray.h"

MotorArray::MotorArray(HardwareSerial& port, int dir_pin) {
    dxl = Dynamixel2Arduino(port, dir_pin);

    backLeft = Motor(1, true, &dxl);
    //backRight = Motor(BR_ID, BR_REVERSED, &dxl);
    //frontLeft = Motor(FL_ID, FL_REVERSED, &dxl);
    //frontRight = Motor(FR_ID, FR_REVERSED, &dxl);
}

void MotorArray::init() {
    dxl.begin(DXL_BAUD);
    dxl.setPortProtocolVersion(DXL_PROT_VER);
    frontLeft.init();
    frontRight.init();
    backLeft.init();
    backRight.init();
}

void MotorArray::setTargetVelocityRaw(int16_t left, int16_t right) {
    frontLeft.setTargetVelocityRaw(left);
    frontRight.setTargetVelocityRaw(right);
    backLeft.setTargetVelocityRaw(left);
    backRight.setTargetVelocityRaw(right);
}

void MotorArray::getCurrentVelocityRaw(int16_t& left_vel, int16_t& right_vel) {
    left_vel = static_cast<int16_t>((frontLeft.getCurrentVelocityRaw() + backLeft.getCurrentVelocityRaw())/2);
    right_vel = static_cast<int16_t>((frontRight.getCurrentVelocityRaw() + backRight.getCurrentVelocityRaw())/2);
}
