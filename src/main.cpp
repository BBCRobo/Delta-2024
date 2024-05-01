
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Define.h>
#include <MotorArray.h>
#include <IMU.h>

MotorArray legs(DXL_SERIAL, DXL_DIR_PIN);
IMU compass = IMU();

void setup() {
    Serial.begin(9600);

    legs.init();
    compass.init();
}

void loop() {
    imu::Vector<3> orient;

    std::vector<byte> message = compass.convertData2Bytes();
    for (const auto& byte: message) {
	    Serial.printf("%02x\t",byte);
    }
    Serial.println();
}
