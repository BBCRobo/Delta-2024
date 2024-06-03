
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Define.h>
#include <MotorArray.h>
#include <IMU.h>
#include <TEMP.h>

MotorArray legs(DXL_SERIAL, DXL_DIR_PIN);
IMU compass = IMU();
TEMP temp = TEMP();

void setup() {
    Serial.begin(9600);
    LATTE_SERIAL.begin(LATTE_BAUD);

    legs.init();
    compass.init();
    temp.init();
}

void loop() {
    std::vector<byte> message = {TRANSMIT_FIRST_BYTE, TRANSMIT_FIRST_BYTE};
    //compass.printQuat();
    compass.printOrient();
    std::vector<byte> compass_data = compass.convertOrient2Bytes();
    message.insert(message.end(), compass_data.begin(), compass_data.end());
    // temp.isheatVictim();
    //for (const auto& byte: message) {
    //	Serial.printf("%02x\t",byte);
    //}
    //Serial.println();
    temp.read();
    LATTE_SERIAL.write(message.data(), message.size());
}
