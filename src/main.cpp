
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Define.h>
#include <MotorArray.h>
#include <IMU.h>
#include <TEMP.h>
#include <LS.h>
#include <SparkFun_I2C_Mux_Arduino_Library.h>
#include <Servo.h>

QWIICMUX master_mux = QWIICMUX();
MotorArray legs(DXL_SERIAL, DXL_DIR_PIN);
IMU compass = IMU();
TEMP temp = TEMP(&master_mux);
LS ls = LS(&master_mux);
Servo dropper = Servo();

uint8_t buffer[4] = {0};
IntervalTimer latte_timer;

volatile std::vector<byte> global_message;
bool dropDropping = false;

void write_data() {
    LATTE_SERIAL.write(global_message.data(), global_message.size());
}

void printGlobalMsg() {
    for (const auto& byte: global_message) {
    	Serial.printf("%02x\t",byte);
    }
    Serial.println();
}

std::vector<byte> readData() {
    std::vector<byte> message = {TRANSMIT_FIRST_BYTE, TRANSMIT_FIRST_BYTE};
    
    // NOTE: LSB (not colour sensosr) is causing the code to halt and pause - will debug later
    std::vector<byte> compass_data = compass.convertOrient2Bytes();
    victim_type_t victim = victim_type_t::NO_VICTIM; // temp.isheatVictim();
    tile_colour_t tile = ls.getTileType();
    bool bumper_left = digitalRead(BUMPERL);
    bool bumper_right = digitalRead(BUMPERR);
    bool switch_state = digitalRead(START_PIN);
    uint8_t combined_byte = ((static_cast<uint8_t>(victim) & 0x03) << 6) | ((static_cast<uint8_t>(tile) & 0x03) << 4) | 
                            (dropDropping << 3) | (bumper_left << 2) | (bumper_right << 1) | switch_state;

    // Might also add wheel velocities too
    message.insert(message.end(), compass_data.begin(), compass_data.end());
    message.push_back(combined_byte);
    return message;
}

void listenData() {
    while(LATTE_SERIAL.available() >= 6) {
        uint8_t byte1 = LATTE_SERIAL.read();
        uint8_t byte2 = LATTE_SERIAL.peek();
        if(byte1 == TRANSMIT_FIRST_BYTE && byte2 == TRANSMIT_SECOND_BYTE) {
            LATTE_SERIAL.read();
            Serial.printf("1:%d 2:%d ", byte1, byte2);
            for(uint8_t i = 0; i < 4; i++) {
                buffer[i] = LATTE_SERIAL.read();
                Serial.print(buffer[i]); Serial.print(" ");
            }
            // Serial.println(millis() - lastTime);
            // lastTime = millis();
            Serial.println();
        }
    }
}

void dropVictims(uint8_t data) {
    uint8_t drop_cuml = 0;

    if((data >> 7) & 0x01) {
        uint8_t drop_multi = static_cast<uint8_t>((data >> 4) & 0x07);
        Serial.printf("Left:%u", drop_multi);
        for(uint8_t i = 0; i < drop_multi; i++) {
            digitalWrite(VICTIM_LED, HIGH);
            dropper.write(LEFT_MAX);
            delay(500);
            digitalWrite(VICTIM_LED, LOW);
            dropper.write(SERVO_MIDDLE);
            delay(500);
        }
        drop_cuml += drop_multi;
    }
    
    if ((data >> 3) & 0x01) {
        uint8_t drop_multi = static_cast<uint8_t>((data) & 0x07);
        for(uint8_t i = 0; i < drop_multi; i++) {
            digitalWrite(VICTIM_LED, HIGH);
            dropper.write(RIGHT_MAX);
            delay(500);
            digitalWrite(VICTIM_LED, LOW);
            dropper.write(SERVO_MIDDLE);
            delay(500);
        }
        drop_cuml += drop_multi;
    }

    for(uint8_t blinks_left = drop_cuml; blinks_left < 5; blinks_left++) {
        digitalWrite(VICTIM_LED, HIGH);
        delay(500);
        digitalWrite(VICTIM_LED, LOW);
        delay(500);
    }

    dropDropping = false;

    while(buffer[3] != 0) {
        std::vector<byte> temp_message = readData();
        noInterrupts();
        global_message = temp_message;
        interrupts();

        Serial.println("waiting for sending false again");
        listenData();
    }
}

void setup() {
    Serial.begin(9600);
    pinMode(START_PIN, INPUT);
    pinMode(BUMPERL, INPUT);
    pinMode(BUMPERR, INPUT);

    pinMode(VICTIM_LED, OUTPUT);
    digitalWrite(VICTIM_LED, LOW);

    LATTE_SERIAL.begin(LATTE_BAUD);

    MUX_PORT.begin();
    while(!master_mux.begin(MUX_ADDR, MUX_PORT)) {
        Serial.println("Mux did not begin");
    }

    legs.init();
    legs.setTargetVelocity(0, 0, 0);
    compass.init();
    // temp.init();
    ls.init();

    dropper.attach(SERVO_PIN);
    dropper.write(SERVO_MIDDLE);

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
    // printGlobalMsg();

    // ---------- Read Raw Data ---------- //
    // compass.printOrient();
    // temp.read();
    // ls.readColour();
    // ls.readLight();

    listenData();
    
    if(buffer[3] != 0) {
        dropDropping = true;
        Serial.println("got this");
        legs.setTargetVelocity(0, 0, 0);

        std::vector<byte> drop_msg = readData();
        noInterrupts();
        global_message = drop_msg;
        interrupts();

        printGlobalMsg();

        dropVictims(buffer[3]);
    } else {
        legs.setTargetVelocity(buffer[0], buffer[1], buffer[2]);
    }
}
