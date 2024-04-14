#ifndef DEFINE_H_
#define DEFINE_H_

#include <Arduino.h>

// ---------- Motors ---------- //

#define DXL_SERIAL Serial3
#define DXL_DIR_PIN 22

#define DXL_BAUD 57600
#define DXL_PROT_VER 2.0
#define DXL_MAX_VELOCITY 1023

#define FL_ID 3
#define FL_REVERSED false

#define BL_ID 1
#define BL_REVERSED false

#define BR_ID 4
#define BR_REVERSED true

#define FR_ID 2
#define FR_REVERSED true

#endif // DEFINE_H_
