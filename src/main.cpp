
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

    legs.init();
    compass.init();
    temp.init();
    ls.init();
}

void loop() {
    std::vector<byte> message = {TRANSMIT_FIRST_BYTE, TRANSMIT_FIRST_BYTE};
    
    std::vector<byte> compass_data = compass.convertOrient2Bytes();
    victim_type_t victim = victim_type_t::RIGHT; // temp.isheatVictim();
    tile_colour_t tile =  tile_colour_t::BLUE; // ls.getTileType();
    bool bumper_left = 0; //digitalRead(BUMPERL);
    bool bumper_right = 1; //digitalRead(BUMPERR);
    bool switch_state = 1; // digitalRead(START_PIN);
    
    uint8_t combined_byte = ((static_cast<uint8_t>(victim) & 0x03) << 6) | ((static_cast<uint8_t>(tile) & 0x07) << 3) | (bumper_left << 2) | (bumper_right << 1) | switch_state;

    message.insert(message.end(), compass_data.begin(), compass_data.end());
    message.push_back(combined_byte);
    // Might also add wheel velocities too
    LATTE_SERIAL.write(message.data(), message.size());

    // ---------- Read Data Message ---------- //
    // for (const auto& byte: message) {
    // 	Serial.printf("%02x\t",byte);
    // }
    // Serial.println();

    // ---------- Read Raw Data ---------- //
    // compass.printOrient();
    // temp.read();
    // ls.readColour();
    // ls.readLight();
    Serial.printf("Raw:%u Byte:%02x\n", combined_byte, combined_byte);
}
