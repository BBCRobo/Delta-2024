
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Define.h>
#include <MotorArray.h>
#include <IMU.h>

MotorArray legs(DXL_SERIAL, DXL_DIR_PIN);
IMU comp();

void setup() {
    Serial.begin(9600);

    legs.init();
    comp.init();
}

void loop() {
    imu::Vector<3> orient, accel, vel;
    comp.getOrientation(orient);
    comp.getAngAccel(accel);
    comp.getAngVel(vel);
    Serial.printf("X:%f\tY:%f\tZ:%f\tVelX:%f\tVelY:%f\tVelZ:%f\n", orient.x(), orient.y(), orient.z(),
    		vel.x(), vel.y(), vel.z());
    Serial.printf("AccelX:%f\tAccelY:%f\tAccelZ:%f\n", accel.x(), accel.y(), accel.z());
}
