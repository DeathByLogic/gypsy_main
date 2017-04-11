/*
 * sensors.h
 *
 *  Created on: Mar 16, 2015
 *      Author: daniel
 */

#ifndef SENSORS_H_
#define SENSORS_H_

// Includes
#include "beagleIO.h"

// Constant Definitions
#define SENSOR_I2C			P9_20
#define LEFT_SENSOR_ADR		0x73
#define RIGHT_SENSOR_ADR	0x72
//#define READ_FAIL_COUNT		3

#define ENCODER_READ_PERIOD_SEC		0
#define ENCODER_READ_PERIOD_NSEC	10000000

#define SONAR_READ_PERIOD_SEC		0
#define SONAR_READ_PERIOD_NSEC		50000000

#define NUM_SENSORS	7

typedef struct {
	beagleI2C		*port;
	char			address;
	char		 	location;
	char			length;
	void			*value;
	timespec		read_period;
	timespec		prev_read_time;
	bool			enabled;
} SensorInterface;

// Type Definitions
struct SensorMemory {
	float			raw_sensor[NUM_SENSORS];
	float			avg_sensor[NUM_SENSORS];
	unsigned long	tick_period;
	long			tick_count;
	unsigned short	crc;
} __attribute__((packed));

// External Variables
extern SensorMemory LeftSensor;
extern SensorMemory RightSensor;

// Functions
void sensor_update(timespec current_time);
void read_sensor(beagleI2C *port, const char i2cAddress, const char startAddress, size_t size, void *memAdr);

#endif /* SENSORS_H_ */
