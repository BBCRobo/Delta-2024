#include "IMU.h"

void IMU::init() {
    compass.begin();
    compass.setExtCrystalUse(true);
}

void IMU::getOrientationData(imu::Vector<3>& orientation) {
    orient = compass.getVector(Adafruit_BNO055::VECTOR_EULER);
    // Deadband filter
    orientation = orient;
}

void IMU::getAngAccel(imu::Vector<3>& acceleration) {
    accel = compass.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    // Deadband filter
    acceleration = accel
}

void IMU::getAngVel(imu::Vector<3>& velocity) {
    vel = compass.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    // Deadband filter
    velocity = vel;
}
