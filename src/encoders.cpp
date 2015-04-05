#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "beagleIO.h"
#include "robot.h"
#include "global.h"
#include "encoders.h"

//
// Global variables
//

// Update the calculated position with new data
void update_location(Posistion *posistion, long left_encoder_count, long right_encoder_count) {
	float theta_l;
	float theta_r;
	float theta_sum;

	float delta_x = 0.0;
	float delta_y = 0.0;

	// Left Wheel
	theta_l = -((float)left_encoder_count * WHEEL_DIM * PI / ENC_COUNT) / WHEEL_BASE;
//	encoder_left_count = 0;

	delta_x += 0.5 * WHEEL_BASE * (sin(posistion->heading) - sin(posistion->heading + theta_l));
	delta_y += 0.5 * WHEEL_BASE * (cos(posistion->heading + theta_l) - cos(posistion->heading));

	// Right Wheel
	theta_r = ((float)right_encoder_count * WHEEL_DIM * PI / ENC_COUNT) / WHEEL_BASE;
//	encoder_right_count = 0;

	delta_x += 0.5 * WHEEL_BASE * (sin(posistion->heading + theta_r) - sin(posistion->heading));
	delta_y += 0.5 * WHEEL_BASE * (cos(posistion->heading) - cos(posistion->heading + theta_r));

	// Sum up current theta and delta
	theta_sum = posistion->heading + (theta_r + theta_l);

	// Keep theta within +- PI
	if (theta_sum > PI) {
		theta_sum -= 2 * PI;
	}

	if (theta_sum < -PI) {
		theta_sum += 2 * PI;
	}

	// Update external variables
	posistion->location.x += delta_x;
	posistion->location.y += delta_y;
	posistion->heading = theta_sum;
}

void update_speed(float *speed, unsigned long left_period, unsigned long right_period) {
	float rightSpeed;
	float leftSpeed;

	leftSpeed = (WHEEL_DIM * PI / ENC_COUNT) / (left_period / PERIOD_DIVIDER);
	rightSpeed = (WHEEL_DIM * PI / ENC_COUNT) / (right_period / PERIOD_DIVIDER);

	*speed = (leftSpeed + rightSpeed) / 2.0;
}
