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
#include "crc.h"

void update_sensor_values() {
	SensorMemory leftSensor;
	SensorMemory rightSensor;

	// Read sensor values over I2C
	read_sensor(&leftSensor, sizeof(leftSensor), LEFT_SENSOR_ADR);
	read_sensor(&rightSensor, sizeof(rightSensor), RIGHT_SENSOR_ADR);

	// Update values in main program variables
	current_state.sensors.distance.left = leftSensor.avg_sensor[0];
	current_state.sensors.encoders.leftTick = leftSensor.tick_count;
	current_state.sensors.encoders.leftPeriod = leftSensor.tick_period;

	current_state.sensors.distance.right = rightSensor.avg_sensor[0];
	current_state.sensors.encoders.rightTick = rightSensor.tick_count;
	current_state.sensors.encoders.rightPeriod = rightSensor.tick_period;

	current_state.sensors.distance.front = 0;
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
