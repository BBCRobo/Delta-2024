#ifndef DEFINE_H_
#define DEFINE_H_

#include <Arduino.h>
#include <Wire.h>
#include <imumaths.h>

// ---------- Motors ---------- //

#define DXL_SERIAL Serial2
#define DXL_DIR_PIN 11

#define DXL_BAUD 57600
#define DXL_PROT_VER 2.0
#define DXL_MAX_VELOCITY 450 // (106 / 0.229) - 20 for buffer

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

// ---------- IMU ---------- //

#define BNO_DEFAULT 55
#define BNO_ADDR 0x29
#define BNO_PORT Wire

#define ORIENT_SCALE_FACTOR 100.0f

// ---------- LattePanda ---------- //

#define LATTE_SERIAL Serial1
#define LATTE_BAUD 115200

#define TRANSMIT_FIRST_BYTE 0xFF

#endif // DEFINE_H_

