/*
 * global.h
 *
 *  Created on: Feb 20, 2013
 *      Author: daniel
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

// Includes
#include <ctime>

// Constant Definitions
#define PI				(float)3.14159

// Macro Definitions
#define CONSTRAIN(amt, low, high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define MAP(value, in_min, in_max, out_min, out_max) ((value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)

// Type Definitions
/*
typedef struct {
	float		x;
	float		y;
} Point;

typedef struct {
	Point		location;
	float		heading;
} Position;
*/

// Functions
timespec operator+(const timespec &a, const timespec &b);
timespec operator-(const timespec &a, const timespec &b);
bool operator>(const timespec &a, const timespec &b);

#endif /* GLOBAL_H_ */
