
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Define.h>
#include <MotorArray.h>
#include <Adafruit_BNO055.h>

MotorArray legs(DXL_SERIAL, DXL_DIR_PIN);
Adafruit_BNO055 bno(BNO_DEFAULT, BNO_ADDR, &BNO_PORT);

void setup() {
    Serial.begin(9600);

    legs.init();

    while(!bno.begin()) {
        Serial.println("Failed to initialise BNO055");
        delay(100);
    }

    bno.setExtCrystalUse(true);
    legs.setTargetVelocityRaw(0, 0);
}

void loop() {
    sensors_event_t orientationData, angVelocityData, angAccelData;
    bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
    bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
    bno.getEvent(&angAccelData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
    Serial.printf("X:%f\tY:%f\tZ:%f\tVelX:%f\tVelY:%f\tVelZ:%f\n", orientationData.orientation.x, orientationData.orientation.y, orientationData.orientation.z,
    		angVelocityData.gyro.x, angVelocityData.gyro.y, angVelocityData.gyro.z);
    // Serial.printf("AccelX:%f\tAccelY:%f\tAccelZ:%f\n", angAccelData.acceleration.x, angAccelData.acceleration.y, angAccelData.acceleration.z);
    int16_t left, right;
    legs.getCurrentVelocityRaw(left, right);
    Serial.printf("LWheel:%dRWheel:%d\n", left, right);
    // highByte: ((num >> 8) & 0xFF) lowByte: (num & 0xFF)
}
