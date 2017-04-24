#ifndef _ENCODERS_H
#define _ENCODERS_H

// Constant Definitions
#define WHEEL_BASE			13.25		// Distance between wheels
#define WHEEL_DIM			4.875		// Diameter of wheels
#define ENC_COUNT			100			// Number of encoder counts per revolution

#define PERIOD_DIVIDER		1E6			// Microseconds per second

#include "classes/position.h"

//Function Prototypes
void encoders_init();
void encoders_uninit();

Position update_location(const Position current_position, const long left_delta, const long right_delta);
float update_speed(const unsigned long left_period, const unsigned long right_period);

// External Variables

#endif
