#ifndef _ENCODERS_H
#define _ENCODERS_H

// Constant Definitions
#define WHEEL_BASE			13.25		// Distance between wheels
#define WHEEL_DIM			4.875		// Diameter of wheels
#define ENC_COUNT			100			// Number of encoder counts per revolution

#define PERIOD_DIVIDER		1E6			// Microseconds per second

//Function Prototypes
void encoders_init();
void encoders_uninit();
void update_location(Posistion *, const long, const long);
void update_speed(float *speed, unsigned long left_period, unsigned long right_period);

// External Variables

#endif
