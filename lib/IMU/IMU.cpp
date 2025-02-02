#include "IMU.h"

void IMU::init() {
    bno.begin(adafruit_bno055_opmode_t::OPERATION_MODE_IMUPLUS);
    bno.setExtCrystalUse(true);
}

void IMU::printOrient() {
    auto orient = getOrientVector();

    Serial.printf("Euler X:%f\tY:%f\tZ:%f\n", orient.x(), orient.y(), orient.z());
}

void IMU::printQuat() {
    sensors_event_t event;
    bno.getEvent(&event);
    auto quat = bno.getQuat();

    Serial.printf("Quat: X:%f Y:%f Z:%f W:%f\n", quat.x(), quat.y(), quat.z(), quat.w());
}

std::vector<byte> IMU::convertOrient2Bytes() {
    std::vector<byte> messageArray;
    
    auto orient = getOrientVector();
    
    // Static casting data to uint16_t
    uint16_t orient_x_scaled = static_cast<uint16_t>(orient.x() * ORIENT_SCALE_FACTOR);
    uint16_t orient_y_scaled = static_cast<uint16_t>((orient.y() + 180.0f) * ORIENT_SCALE_FACTOR);
    uint16_t orient_z_scaled = static_cast<uint16_t>((orient.z() + 180.0f) * ORIENT_SCALE_FACTOR);

    messageArray.push_back(orient_x_scaled & 0xFF);
    messageArray.push_back((orient_x_scaled >> 8) & 0xFF);

    messageArray.push_back(orient_y_scaled & 0xFF);
    messageArray.push_back((orient_y_scaled >> 8) & 0xFF);

    messageArray.push_back(orient_z_scaled & 0xFF);
    messageArray.push_back((orient_z_scaled >> 8) & 0xFF);
    
    return messageArray;
}

