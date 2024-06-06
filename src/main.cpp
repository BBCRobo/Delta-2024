
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Define.h>
#include <MotorArray.h>
#include <IMU.h>
#include <TEMP.h>
#include <LS.h>
#include <SparkFun_I2C_Mux_Arduino_Library.h>

QWIICMUX master_mux = QWIICMUX();
MotorArray legs(DXL_SERIAL, DXL_DIR_PIN);
IMU compass = IMU();
TEMP temp = TEMP(&master_mux);
LS ls = LS(&master_mux);

void setup() {
    Serial.begin(9600);
    LATTE_SERIAL.begin(LATTE_BAUD);

    MUX_PORT.begin();
    while(!master_mux.begin(MUX_ADDR, MUX_PORT)) {
        Serial.println("Mux did not begin");
    }

    // legs.init();
    // compass.init();
    temp.init();
    ls.init();
}

void loop() {
    // std::vector<byte> message = {TRANSMIT_FIRST_BYTE, TRANSMIT_FIRST_BYTE};

    // tile_colour_t tile = ls.getTileType();
    // victim_type_t victim = temp.isheatVictim();
    // std::vector<byte> compass_data = compass.convertOrient2Bytes();

    // message.push_back(((static_cast<uint8_t>(victim) & 0x0F) << 4) | (static_cast<uint8_t>(tile) & 0x0F));
    // message.insert(message.end(), compass_data.begin(), compass_data.end());

    //for (const auto& byte: message) {
    //	Serial.printf("%02x\t",byte);
    //}
    //Serial.println();
    // compass.printOrient();
    // temp.read();
    ls.readColour();
    // LATTE_SERIAL.write(message.data(), message.size());
}
