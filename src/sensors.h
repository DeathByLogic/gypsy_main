/*
 * sensors.h
 *
 *  Created on: Mar 16, 2015
 *      Author: daniel
 */

#ifndef SENSORS_H_
#define SENSORS_H_

// Constant Definitions
#define SENSOR_I2C			P9_20
#define LEFT_SENSOR_ADR		0x72
#define RIGHT_SENSOR_ADR	0x71
#define READ_FAIL_COUNT		3

#define NUM_SENSORS	2

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
void update_sensor_values();
void read_sensor(void *memAdr, size_t size, const char i2cAddress);
bool verify_crc(char *str, size_t size, unsigned short crc);
unsigned short crc_xmodem_update (unsigned short crc, char data);

#endif /* SENSORS_H_ */
