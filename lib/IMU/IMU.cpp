#include "IMU.h"

void IMU::init() {
    bno.begin();
    bno.setExtCrystalUse(true);
}

void IMU::printOrient() {
    auto orient = getOrientVector();

    Serial.printf("Euler X:%f\tY:%f\tZ:%f\n", orient.x(), orient.y(), orient.z());
}

std::vector<byte> IMU::convertData2Bytes() {
    std::vector<byte> messageArray;
    
    auto orient = getOrientVector();
    
    // Static casting data to uint16_t
    uint16_t orient_x_scaled = static_cast<uint16_t>(orient.x() * ORIENT_SCALE_FACTOR);
    uint16_t orient_y_scaled = static_cast<uint16_t>((orient.y() + 180.0f) * ORIENT_SCALE_FACTOR);
    uint16_t orient_z_scaled = static_cast<uint16_t>((orient.z() + 180.0f) * ORIENT_SCALE_FACTOR);

    // Bit shifting and appending to message
    messageArray.push_back(orient_x_scaled & 0xFF);
    messageArray.push_back((orient_x_scaled >> 8) & 0xFF);

    messageArray.push_back(orient_y_scaled & 0xFF);
    messageArray.push_back((orient_y_scaled >> 8) & 0xFF);

    messageArray.push_back(orient_z_scaled & 0xFF);
    messageArray.push_back((orient_z_scaled >> 8) & 0xFF);
    
    return messageArray;
}

