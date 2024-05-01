#include "IMU.h"

void IMU::init() {
    bno.begin();
    bno.setExtCrystalUse(true);
}

void IMU::getOrientation(imu::Vector<3>& orientation) {
    auto orient = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    // Deadband filter
    orientation = orient;
}

void IMU::getAngAccel(imu::Vector<3>& acceleration) {
    auto accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    // Deadband filter
    acceleration = accel;
}

void IMU::getAngVel(imu::Vector<3>& velocity) {
    auto vel = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    // Deadband filter
    velocity = vel;
}
