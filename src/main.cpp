
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

uint8_t buffer[3] = {0};
IntervalTimer latte_timer;

volatile std::vector<byte> global_message;

void write_data() {
    LATTE_SERIAL.write(global_message.data(), global_message.size());
}

std::vector<byte> readData() {
    std::vector<byte> message = {TRANSMIT_FIRST_BYTE, TRANSMIT_FIRST_BYTE};
    
    std::vector<byte> compass_data = compass.convertOrient2Bytes();
    victim_type_t victim = victim_type_t::RIGHT; // temp.isheatVictim();
    tile_colour_t tile =  tile_colour_t::BLUE; // ls.getTileType();
    bool bumper_left = 0; //digitalRead(BUMPERL);
    bool bumper_right = 1; //digitalRead(BUMPERR);
    bool switch_state = 1; // digitalRead(START_PIN);
    
    uint8_t combined_byte = ((static_cast<uint8_t>(victim) & 0x03) << 6) | ((static_cast<uint8_t>(tile) & 0x07) << 3) | (bumper_left << 2) | (bumper_right << 1) | switch_state;
    
    // Might also add wheel velocities too
    message.insert(message.end(), compass_data.begin(), compass_data.end());
    message.push_back(combined_byte);
    return message;
}

void setup() {
    Serial.begin(9600);
    LATTE_SERIAL.begin(LATTE_BAUD);

    MUX_PORT.begin();
    while(!master_mux.begin(MUX_ADDR, MUX_PORT)) {
        Serial.println("Mux did not begin");
    }

    delay(3000);

    legs.init();
    legs.setTargetVelocity(0, 0, 0);
    compass.init();
    temp.init();
    ls.init();

    global_message = readData();

    latte_timer.begin(write_data, 10000); // 10ms
}

unsigned long lastTime = millis();

void loop() {
    std::vector<byte> temp_message = readData();
    noInterrupts();
    global_message = temp_message;
    interrupts();

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

    // while(LATTE_SERIAL.available() >= 5) {
    //     uint8_t temp = LATTE_SERIAL.read();
    //     Serial.println(temp);
    // }
    // Serial.println("p");

    while(LATTE_SERIAL.available() >= 5) {
        uint8_t byte1 = LATTE_SERIAL.read();
        uint8_t byte2 = LATTE_SERIAL.peek();
        if(byte1 == TRANSMIT_FIRST_BYTE && byte2 == TRANSMIT_SECOND_BYTE) {
            LATTE_SERIAL.read();
            // Serial.printf("1:%d 2:%d ", byte1, byte2);
            for(uint8_t i = 0; i < 3; i++) {
                buffer[i] = LATTE_SERIAL.read();
                // Serial.print(buffer[i]); Serial.print(" ");
            }
            Serial.println(millis() - lastTime);
            lastTime = millis();
            // Serial.println();
        }
    }
    
    legs.setTargetVelocity(buffer[0], buffer[1], buffer[2]); // -this function is causing the issue - randomly takes 200ms to run
}
