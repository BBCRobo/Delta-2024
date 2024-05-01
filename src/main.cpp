
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
    compass.getOrientation(orient);
    Serial.printf("X:%f\tY:%f\tZ:%f\n", orient.x(), orient.y(), orient.z());

    std::vector<byte> message = convertData2Bytes(orient, legs.getCurrentVelocityRaw());
    for (const auto& byte: message) {
	    Serial.printf("%02x\t",byte);
    }
    Serial.println();
}
