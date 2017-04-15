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
void update_location(Position *position, long left_encoder_count, long right_encoder_count) {
	float theta_l;
	float theta_r;
	float theta_sum;

	float delta_x = 0.0;
	float delta_y = 0.0;

	// Left Wheel
	theta_l = -((float)left_encoder_count * WHEEL_DIM * PI / ENC_COUNT) / WHEEL_BASE;
//	encoder_left_count = 0;

	delta_x += 0.5 * WHEEL_BASE * (sin(position->heading) - sin(position->heading + theta_l));
	delta_y += 0.5 * WHEEL_BASE * (cos(position->heading + theta_l) - cos(position->heading));

	// Right Wheel
	theta_r = ((float)right_encoder_count * WHEEL_DIM * PI / ENC_COUNT) / WHEEL_BASE;
//	encoder_right_count = 0;

	delta_x += 0.5 * WHEEL_BASE * (sin(position->heading + theta_r) - sin(position->heading));
	delta_y += 0.5 * WHEEL_BASE * (cos(position->heading) - cos(position->heading + theta_r));

	// Sum up current theta and delta
	theta_sum = position->heading + (theta_r + theta_l);

	// Keep theta within +- PI
	if (theta_sum > PI) {
		theta_sum -= 2 * PI;
	}

	if (theta_sum < -PI) {
		theta_sum += 2 * PI;
	}

	// Update external variables
	position->location.x += delta_x;
	position->location.y += delta_y;
	position->heading = theta_sum;
}

void update_speed(float *speed, unsigned long left_period, unsigned long right_period) {
	float rightSpeed;
	float leftSpeed;

	leftSpeed = (WHEEL_DIM * PI / ENC_COUNT) / (left_period / PERIOD_DIVIDER);
	rightSpeed = (WHEEL_DIM * PI / ENC_COUNT) / (right_period / PERIOD_DIVIDER);

	*speed = (leftSpeed + rightSpeed) / 2.0;
}
