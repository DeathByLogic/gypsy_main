/*
 * sensors.cpp
 *
 *  Created on: Mar 16, 2015
 *      Author: daniel
 */

#include <unistd.h>
#include <time.h>

#include "beagleIO.h"
#include "robot.h"
#include "sensors.h"
#include "crc.h"

// Global variables
const struct timespec SONAR_READ_PERIOD = {SONAR_READ_PERIOD_SEC, SONAR_READ_PERIOD_NSEC};
const struct timespec ENCODERS_READ_PERIOD = {ENCODER_READ_PERIOD_SEC, ENCODER_READ_PERIOD_NSEC};

SensorInterface sensors[NUM_SENSORS] = {
	{&SENSOR_I2C, 0x73, 0x00, 1, &current_state.sensors.encoders.leftTick,		ENCODERS_READ_PERIOD, 	{0, 0}, true},	// Left wheel encoder tick count
	{&SENSOR_I2C, 0x74, 0x00, 1, &current_state.sensors.encoders.rightTick, 	ENCODERS_READ_PERIOD, 	{0, 0}, true},	// Right wheel encoder tick count
	{&SENSOR_I2C, 0x73, 0x01, 1, &current_state.sensors.encoders.leftPeriod,	ENCODERS_READ_PERIOD, 	{0, 0}, true},	// Left wheel period
	{&SENSOR_I2C, 0x74, 0x01, 1, &current_state.sensors.encoders.rightPeriod,	ENCODERS_READ_PERIOD, 	{0, 0}, true},	// Right wheel period
	{&SENSOR_I2C, 0x73, 0x08, 2, &current_state.sensors.distance.left, 			SONAR_READ_PERIOD, 		{0, 0}, true},	// Left sonar sensor
	{&SENSOR_I2C, 0x73, 0x10, 2, &current_state.sensors.distance.front, 		SONAR_READ_PERIOD, 		{0, 0}, true},	// Center sonar sensor
	{&SENSOR_I2C, 0x73, 0x18, 2, &current_state.sensors.distance.right, 		SONAR_READ_PERIOD, 		{0, 0}, true}};	// Right sonar sensor

void sensor_update(timespec current_time) {
	// Determine what sensors to read based on previous read time
	for (int i; i < NUM_SENSORS; i++) {
		if (sensors[i].enabled == true) {
			if (current_time > (sensors[i].prev_read_time + sensors[i].read_period)) {
				// Read the sensor
				read_sensor(sensors[i].port, sensors[i].address, sensors[i].location, sensors[i].length, sensors[i].value);

				// Update the previous read time
				sensors[i].prev_read_time = current_time;
			}
		}
	}
}

/*
void update_sensor_values() {
	SensorMemory leftSensor;
	SensorMemory rightSensor;

	// Read sensor values over I2C
	//read_sensor(&leftSensor, sizeof(leftSensor), LEFT_SENSOR_ADR);
	//read_sensor(&rightSensor, sizeof(rightSensor), RIGHT_SENSOR_ADR);

	// Update values in main program variables
	current_state.sensors.distance.left = leftSensor.avg_sensor[0];
	current_state.sensors.encoders.leftTick = leftSensor.tick_count;
	current_state.sensors.encoders.leftPeriod = leftSensor.tick_period;

	current_state.sensors.distance.right = rightSensor.avg_sensor[0];
	current_state.sensors.encoders.rightTick = rightSensor.tick_count;
	current_state.sensors.encoders.rightPeriod = rightSensor.tick_period;

	current_state.sensors.distance.front = 0;

	// Perform sensor fusion
	//sensor_fusion();
}
*/

void read_sensor(beagleI2C *port, const char i2cAddress, const char startAddress, const size_t size, void *memAdr) {
	char tempArray[size];

	// Get data from sensor
//	for (int i = 0; i < READ_FAIL_COUNT; i++) {
		port->writeTo(&startAddress, i2cAddress, sizeof(startAddress));
		port->requestFrom(tempArray, i2cAddress, size, true);

		// Copy data into sensor struct
		memcpy(memAdr, &tempArray, size);

//		if (verify_crc(tempArray, sizeof(tempArray), 0x0000)) {
//			// Copy data into sensor struct
//			memcpy(memAdr, &tempArray, size);
//
//			break;
//		} else {
//			// Clear memory if invalid data
//			memset(memAdr, 0x00, size);
//		}
//	}
}

void sensor_fusion() {

}
