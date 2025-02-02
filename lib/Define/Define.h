#ifndef DEFINE_H_
#define DEFINE_H_

#include <Arduino.h>
#include <Wire.h>
#include <imumaths.h>

#define START_PIN 12

// ---------- Victims ---------- //

#define VICTIM_LED 13
#define LIGHT_BOARD 10
#define LIGHT_BOARD_PWM 150

// ---------- SERVO ---------- //

#define SERVO_PIN 2

#define SERVO_MIDDLE 90

// ---------- Types ---------- //

typedef enum {
    NO_VICTIM = 0,
    LEFT = 1,
    RIGHT = 2
} victim_type_t;


typedef enum {
    OTHER = 0,
    BLACK = 1,
    SILVER = 2,
    BLUE = 3
} tile_colour_t;

// ---------- Motors ---------- //

#define DXL_SERIAL Serial2
#define DXL_DIR_PIN 11

#define DXL_BAUD 57600
#define DXL_PROT_VER 2.0
#define DXL_MAX_VELOCITY 1023
#define MAX_SPEED 255

// Max RPM of XC430-W150-T is 106 @ 12V no load
// 0.229rpm for each UNIT_RAW (int16_t)
// https://emanual.robotis.com/docs/en/dxl/x/xc430-w150/

#define FL_ID 3
#define FL_REVERSED false

#define BL_ID 2
#define BL_REVERSED false

#define BR_ID 1
#define BR_REVERSED true

#define FR_ID 4
#define FR_REVERSED true

#define WHEEL_DIAMETER_MM 70 // MM not considering squishing
#define ENCODER_OFFSET 1000

// ---------- IMU ---------- //

#define BNO_DEFAULT 55
#define BNO_ADDR 0x29
#define BNO_PORT Wire

#define ORIENT_SCALE_FACTOR 100.0f

// ---------- LattePanda ---------- //

#define LATTE_SERIAL Serial1
#define LATTE_BAUD 9600

#define TRANSMIT_FIRST_BYTE 0xFF
#define TRANSMIT_SECOND_BYTE 0xFE
#define LATTE_MSG_SIZE 5
#define SEND_MSG_SIZE 11

// --------- LS ---------- //

#define LS_CONTROL 4
#define COLOUR_MUX_PORT 3

#define LS_1 22
#define LS_2 21
#define LS_3 20

#define LS_COUNT 3

#define LS_BOARD_BRIGHTNESS 150 // PWM


// all the others are less than
#define BLUE_R 5
#define BLUE_G 12
#define BLUE_B 15 // greater than
#define BLUE_C 25 // greater than

#define BLACK_R 5
#define BLACK_G 5
#define BLACK_B 5
#define BLACK_C 10

// ---------- TEMP ---------- //

#define TEMP_L 5
#define TEMP_R 2
#define TEMP_ADDR 0x5A

#define TEMP_THRESHOLD 4.0f

// ---------- MUX ---------- //

#define MUX_PORT Wire1
#define MUX_ADDR 0x70

// ---------- BUMPER ---------- //

#define BUMPERL 5
#define BUMPERR 6

// ---------- I2C Scanner ---------- //

inline void I2CScanner(TwoWire& the_wire)
{
    byte error, address;
    int nDevices;

    Serial.println("Scanning...");

    nDevices = 0;
    for(address = 1; address < 127; address++ )
    {
        the_wire.beginTransmission(address);
        error = the_wire.endTransmission();

        the_wire.beginTransmission(address+1);

    if (error == 0) // Special flag for SAMD Series
    {
        Serial.print("I2C device found at address 0x");
        if (address<16)
            Serial.print("0");
        Serial.print(address,HEX);
        Serial.println("!");

        nDevices++;
    }
    else if (error==4) 
    {
        Serial.print("Unknown error at address 0x");
        if (address<16) 
            Serial.print("0");
        Serial.println(address,HEX);
    }
    }
    if (nDevices == 0)
        Serial.println("No I2C devices found\n");
    else
        Serial.println("done\n");

    delay(5000);           // wait 5 seconds for next scan
}
#endif // DEFINE_H_

