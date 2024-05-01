#ifndef DEFINE_H_
#define DEFINE_H_

#include <Arduino.h>
#include <Wire.h>
#include <imumaths.h>
#include <array>

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

#define WHEEL_VEL_SCALE_FACTOR 100.0f

// ---------- IMU ---------- //

#define BNO_DEFAULT 55
#define BNO_ADDR 0x29
#define BNO_PORT Wire

#define ORIENT_SCALE_FACTOR 100.0f

// ---------- Functions ---------- //

// Not very scalable ik but good enough
inline std::vector<byte> convertData2Bytes(const imu::Vector<3>& orient, const std::array<float, 2>& wheel_vel) {
    std::vector<byte> messageArray;
    
    // Static casting data to uint16_t
    uint16_t orient_x_scaled = static_cast<uint16_t>(orient.x() * ORIENT_SCALE_FACTOR);
    uint16_t orient_y_scaled = static_cast<uint16_t>((orient.y() + 180.0f) * ORIENT_SCALE_FACTOR);
    uint16_t orient_z_scaled = static_cast<uint16_t>((orient.z() + 180.0f) * ORIENT_SCALE_FACTOR);
    uint16_t left_vel = static_cast<uint16_t>(wheel_vel[0] * WHEEL_VEL_SCALE_FACTOR);
    uint16_t right_vel = static_cast<uint16_t>(wheel_vel[1] * WHEEL_VEL_SCALE_FACTOR);

    // Bit shifting and appending to message
    messageArray.push_back(orient_x_scaled & 0xFF);
    messageArray.push_back((orient_x_scaled >> 8) & 0xFF);

    messageArray.push_back(orient_y_scaled & 0xFF);
    messageArray.push_back((orient_y_scaled >> 8) & 0xFF);

    messageArray.push_back(orient_z_scaled & 0xFF);
    messageArray.push_back((orient_z_scaled >> 8) & 0xFF);

    messageArray.push_back(left_vel & 0xFF);
    messageArray.push_back((left_vel >> 8) & 0xFF);

    messageArray.push_back(right_vel & 0xFF);
    messageArray.push_back((right_vel >> 8) & 0xFF);

    return messageArray;
}

#endif // DEFINE_H_
