
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
    legs.setTargetVelocity(0, 0, 0);
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
    // Serial.printf("Raw:%u Byte:%02x\n", combined_byte, combined_byte);
    
    if(LATTE_SERIAL.available() > 0) {
        uint8_t temp = LATTE_SERIAL.peek();
        // Serial.printf("Temp:%d", temp);
        if(temp == TRANSMIT_FIRST_BYTE) {
            if(LATTE_SERIAL.available() >= LATTE_MSG_SIZE) {
                uint8_t header = LATTE_SERIAL.read();
                uint8_t status = LATTE_SERIAL.read();
                uint8_t left_speed = LATTE_SERIAL.read();
                uint8_t right_speed = LATTE_SERIAL.read();
                Serial.printf("Motors: Status:%d L:%d R:%d\n", status, left_speed, right_speed);
                legs.setTargetVelocity(status, left_speed, right_speed);
            }
        } else {
            LATTE_SERIAL.read();
        }
    }

    // if(LATTE_SERIAL.available() >= LATTE_MSG_SIZE) {
    //     uint8_t temp = LATTE_SERIAL.peek();
    //     // Serial.printf("Temp:%d", temp);
    //     if(temp == TRANSMIT_FIRST_BYTE) {
    //         LATTE_SERIAL.read();
    //         uint8_t status = LATTE_SERIAL.read();
    //         uint8_t left_speed = LATTE_SERIAL.read();
    //         uint8_t right_speed = LATTE_SERIAL.read();
    //         Serial.printf("Motors: Status:%d L:%d R:%d\n", status, left_speed, right_speed);
    //         legs.setTargetVelocity(status, left_speed, right_speed);
    //     } else {
    //         LATTE_SERIAL.read();
    //     }
    // } 
}
