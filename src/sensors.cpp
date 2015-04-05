/*
 * sensors.cpp
 *
 *  Created on: Mar 16, 2015
 *      Author: daniel
 */

#include <unistd.h>

#include "beagleIO.h"
#include "robot.h"
#include "sensors.h"

long prevLeftTickCount;
long prevRightTickCount;

void update_sensor_values() {
	SensorMemory leftSensor;
	SensorMemory rightSensor;

	// Read sensor values over I2C
	read_sensor(&leftSensor, sizeof(leftSensor), LEFT_SENSOR_ADR);
//	read_sensor(&rightSensor, sizeof(rightSensor), RIGHT_SENSOR_ADR);

	// Update values in main program variables
	current_state.sensors.distance.left = leftSensor.avg_sensor[0];
	current_state.sensors.encoders.leftTick = leftSensor.tick_count - prevLeftTickCount;
	current_state.sensors.encoders.leftPeriod = leftSensor.tick_period;

	current_state.sensors.distance.right = rightSensor.avg_sensor[0];
	current_state.sensors.encoders.rightTick = rightSensor.tick_count - prevRightTickCount;
	current_state.sensors.encoders.rightPeriod = rightSensor.tick_period;

	current_state.sensors.distance.front = 0;

	// Update previous tick counts
	prevLeftTickCount = leftSensor.tick_count;
	prevRightTickCount = rightSensor.tick_count;
}

void read_sensor(void *memAdr, size_t size, const char i2cAddress) {
	char start_adr[1] = { 0x00 };
	char tempArray[size];

	// Get data from left sensors
	for (int i = 0; i < READ_FAIL_COUNT; i++) {
		SENSOR_I2C.writeTo(start_adr, i2cAddress, sizeof(start_adr));
		SENSOR_I2C.requestFrom(tempArray, i2cAddress, size, true);

		if (verify_crc(tempArray, sizeof(tempArray), 0x0000)) {
			// Copy data into sensor struct
			memcpy(memAdr, &tempArray, size);

			break;
		} else {
			// Clear memory if invalid data
			memset(memAdr, 0x00, size);
		}
	}
}

bool verify_crc(char *str, size_t size, const unsigned short crc) {
	unsigned short calc_crc = 0x0000;

	// Calculate the CRC of the data
	for (unsigned int i = 0; i < size; i++) {
		calc_crc = crc_xmodem_update(calc_crc, str[i]);

		printf("Byte %i: %X\t CRC: %X\n", i, str[i], calc_crc);
	}

	// Check if calculated CRC matches supplied
	if (calc_crc == crc) {
		return true;
	} else {
		return false;
	}
}

unsigned short crc_xmodem_update (unsigned short crc, char data) {
	crc = crc ^ ((unsigned short)data << 8);

	for (int i=0; i< 8; i++) {
		if (crc & 0x8000)
			crc = (crc << 1) ^ 0x1021;
		else
			crc <<= 1;
	}

	return crc;
}
