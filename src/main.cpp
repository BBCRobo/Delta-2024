
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Define.h>
#include <MotorArray.h>

MotorArray legs(DXL_SERIAL, DXL_DIR_PIN);

void setup() {
	Serial.begin(9600);
	
	delay(6000);
	Serial.println("starting");

	legs.init();
	legs.setTargetVelocityRaw(1023, -1023);
}

void loop() {
	int16_t left = 0;
	int16_t right = 0;
	legs.getCurrentVelocityRaw(left, right);
	Serial.printf("Left:%d Right:%d\n", left, right);
	// highByte: ((num >> 8) & 0xFF) lowByte: (num & 0xFF)
}
