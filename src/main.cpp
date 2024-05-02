
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
    LATTE_SERIAL.begin(LATTE_BAUD);

    legs.init();
    compass.init();
}

void loop() {
    std::vector<byte> message = compass.convertData2Bytes();
    for (const auto& byte: message) {
	Serial.printf("%02x\t",byte);
    }
    Serial.println();
    LATTE_SERIAL.write(message.data(), message.size());
    compass.printOrient();
    delay(500);
}
