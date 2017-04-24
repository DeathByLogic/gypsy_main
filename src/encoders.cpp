#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#include "classes/position.h"
#include "beagleIO.h"
#include "robot.h"
#include "global.h"
#include "encoders.h"

//
// Global variables
//

// Update the calculated position with new data
Position update_location(const Position current_position, const long left_delta, const long right_delta) {
	Position new_position;

	float theta_l;
	float theta_r;
	float theta_sum;

	float delta_x = 0.0;
	float delta_y = 0.0;

	// Left Wheel
	theta_l = -((float)left_delta * WHEEL_DIM * PI / ENC_COUNT) / WHEEL_BASE;

	delta_x += 0.5 * WHEEL_BASE * (sin(current_position.heading) - sin(current_position.heading + theta_l));
	delta_y += 0.5 * WHEEL_BASE * (cos(current_position.heading + theta_l) - cos(current_position.heading));

	// Right Wheel
	theta_r = ((float)right_delta * WHEEL_DIM * PI / ENC_COUNT) / WHEEL_BASE;

	delta_x += 0.5 * WHEEL_BASE * (sin(current_position.heading + theta_r) - sin(current_position.heading));
	delta_y += 0.5 * WHEEL_BASE * (cos(current_position.heading) - cos(current_position.heading + theta_r));

	// Sum up current theta and delta
	theta_sum = current_position.heading + (theta_r + theta_l);

	// Keep theta within +- PI
	if (theta_sum > PI) {
		theta_sum -= 2 * PI;
	}

	if (theta_sum < -PI) {
		theta_sum += 2 * PI;
	}

	// Update variables
	new_position.location.x = current_position.location.x + delta_x;
	new_position.location.y = current_position.location.y + delta_y;
	new_position.heading = theta_sum;

	return new_position;
}

float update_speed(const unsigned long left_period, const unsigned long right_period) {
	float rightSpeed;
	float leftSpeed;

	leftSpeed = (WHEEL_DIM * PI / ENC_COUNT) / (left_period / PERIOD_DIVIDER);
	rightSpeed = (WHEEL_DIM * PI / ENC_COUNT) / (right_period / PERIOD_DIVIDER);

	return (leftSpeed + rightSpeed) / 2.0;
}
