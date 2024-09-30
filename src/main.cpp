
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

volatile byte global_message[SEND_MSG_SIZE]; 
bool dropDropping = false;
unsigned long lastTime;

void write_data() {
    byte temp_message[SEND_MSG_SIZE];
    memcpy(temp_message, (void*)global_message, SEND_MSG_SIZE);
    LATTE_SERIAL.write(temp_message, sizeof(temp_message));
}

void printGlobalMsg() {
    for (const auto& byte: global_message) {
    	Serial.printf("%02x\t",byte);
    }
    Serial.println();
}

void dropLeft() {
    for(int i = 79; i >= 64; i--) { // 450ms
        dropper.write(i);
        delay(30); // 450ms
        if((i * 30) % 150 == 0) {
            digitalWrite(VICTIM_LED, digitalRead(VICTIM_LED) == HIGH ? LOW : HIGH);
        }
    }
    delay(250);
    for(int i = 82; i <= 94; i++){ // left load, left chute: 89; left load, right chute: 105
        dropper.write(i);
        delay(25); // 300ms
        if((i * 25) % 150 == 0) {
            digitalWrite(VICTIM_LED, digitalRead(VICTIM_LED) == HIGH ? LOW : HIGH);
        }
    }
    digitalWrite(VICTIM_LED, LOW);
}

void dropRight() {
    for(int i = 101; i<=116; i++){
        dropper.write(i);
        delay(30);
        if((i * 30) % 150 == 0) {
            digitalWrite(VICTIM_LED, digitalRead(VICTIM_LED) == HIGH ? LOW : HIGH);
        }
    }
    delay(250);
    for(int i = 90; i>=78; i--){ // right load, right chute: 82; right laod, left chute: 66
        dropper.write(i);
        delay(25); // 300ms
        if((i * 25) % 150 == 0) {
            digitalWrite(VICTIM_LED, digitalRead(VICTIM_LED) == HIGH ? LOW : HIGH);
        }
    }
    digitalWrite(VICTIM_LED, LOW);
}

void readData(byte* message) {
    message[0] = TRANSMIT_FIRST_BYTE;
    message[1] = TRANSMIT_FIRST_BYTE;
    
    std::vector<byte> compass_data = compass.convertOrient2Bytes();

    victim_type_t victim = victim_type_t::NO_VICTIM; // temp.isheatVictim();
    tile_colour_t tile = ls.getTileType();

    bool bumper_left = digitalRead(BUMPERL);
    bool bumper_right = digitalRead(BUMPERR);
    bool switch_state = digitalRead(START_PIN);

    uint8_t combined_byte = ((static_cast<uint8_t>(victim) & 0x03) << 6) | ((static_cast<uint8_t>(tile) & 0x03) << 4) | 
                            (dropDropping << 3) | (bumper_left << 2) | (bumper_right << 1) | switch_state;

    size_t compass_data_size = compass_data.size();
    for(size_t i = 0; i < compass_data_size; ++i) {
        message[2 + i] = compass_data[i];
    }

    message[2 + compass_data_size] = combined_byte;

    uint16_t encoderDist = legs.getEncoderDist(compass.getOrientVector().x(), lastTime);

    message[2 + compass_data_size + 1] = static_cast<uint8_t>((encoderDist >> 8) & 0xFF);
    message[2 + compass_data_size + 2] = static_cast<uint8_t>(encoderDist);
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
            dropLeft();
        }
        drop_cuml += drop_multi;
    }
    
    if ((data >> 3) & 0x01) {
        uint8_t drop_multi = static_cast<uint8_t>((data) & 0x07);
        for(uint8_t i = 0; i < drop_multi; i++) {
            dropRight();
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
        byte temp_message[SEND_MSG_SIZE];
        readData(temp_message);
        noInterrupts();
        memcpy((void*)global_message, temp_message, SEND_MSG_SIZE);
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

    pinMode(LIGHT_BOARD, OUTPUT);
    analogWrite(LIGHT_BOARD, LIGHT_BOARD_PWM);

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

    lastTime = millis();
    byte temp_message[SEND_MSG_SIZE];
    readData(temp_message);
    noInterrupts();
    memcpy((void*)global_message, temp_message, SEND_MSG_SIZE);
    interrupts();

    latte_timer.begin(write_data, 15000); // 15ms
}

void loop() {
    byte temp_message[SEND_MSG_SIZE];
    readData(temp_message);
    noInterrupts();
    memcpy((void*)global_message, temp_message, SEND_MSG_SIZE);
    interrupts();

    listenData();

    // ---------- Read Data Message ---------- //
    // printGlobalMsg();

    // ---------- Read Raw Data ---------- //
    // compass.printOrient();
    // temp.read();
    // ls.readColour();
    // ls.readLight();
    // Serial.printf("Tile type :%d \n", ls.getTileType());
    
    if(buffer[3] != 0) {
        dropDropping = true;
        Serial.println("got this");
        legs.setTargetVelocity(0, 0, 0);

        byte temp_message[SEND_MSG_SIZE];
        readData(temp_message);
        noInterrupts();
        memcpy((void*)global_message, temp_message, SEND_MSG_SIZE);
        interrupts();

        dropVictims(buffer[3]);
    } else {
        legs.setTargetVelocity(buffer[0], buffer[1], buffer[2]);
    }
}
