#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "beagleIO.h"
#include "global.h"
#include "encoders.h"

using namespace std;

//
// Global variables
//

// Encoder Counts
int encoder_left_count;
int encoder_right_count;

// Calculated Position & Heading
//float sense_speed;
//float sense_x = 0.0;
//float sense_y = 0.0;
//float sense_theta = 0.0;

// Thread handle
pthread_t enc_thread_id;

// Init encoder pins and configure interrupts
void encoders_init() {
	int rc;

	ENC_RIGHT_A_PIN.exportPin();
	ENC_RIGHT_B_PIN.exportPin();
	ENC_LEFT_A_PIN.exportPin();
	ENC_LEFT_B_PIN.exportPin();

	// Change encoder pins to input
	ENC_RIGHT_A_PIN.setDirection(INPUT_PIN);
	ENC_RIGHT_B_PIN.setDirection(INPUT_PIN);
	ENC_LEFT_A_PIN.setDirection(INPUT_PIN);
	ENC_LEFT_B_PIN.setDirection(INPUT_PIN);

	// Attach interrupt pins to encoder functions
	ENC_RIGHT_A_PIN.setEdge(BOTH_EDGES);
	ENC_RIGHT_B_PIN.setEdge(BOTH_EDGES);
	ENC_LEFT_A_PIN.setEdge(BOTH_EDGES);
	ENC_LEFT_B_PIN.setEdge(BOTH_EDGES);

	// Create thread for encoder interrupts
	if ((rc = pthread_create(&enc_thread_id, NULL, encoder_thread, (void *)&enc_thread_id)) < 0 ) {
		perror("Encoders");
	}
}

// Uninit encoders
void encoders_uninit() {
	pthread_cancel(enc_thread_id);
}

void *encoder_thread(void *arg) {
	int rc;
	bool enc_value[ENC_POLL_COUNT];
	struct pollfd enc_int[ENC_POLL_COUNT];

	EncoderState enc_left_state = STATE_FW_A;
	EncoderState enc_right_state = STATE_FW_A;

	enc_int[0].fd = ENC_RIGHT_A_PIN.openPin(O_RDONLY | O_NONBLOCK);
	enc_int[1].fd = ENC_RIGHT_B_PIN.openPin(O_RDONLY | O_NONBLOCK);
	enc_int[2].fd = ENC_LEFT_A_PIN.openPin(O_RDONLY | O_NONBLOCK);
	enc_int[3].fd = ENC_LEFT_B_PIN.openPin(O_RDONLY | O_NONBLOCK);

	for (int i = 0; i < ENC_POLL_COUNT; i++) {
		enc_int[i].events = POLLPRI;
	}

	while (true) {
		//P8.digitalSetValue(ENC_DEBUG_R, false);

		if ((rc = poll(enc_int, ENC_POLL_COUNT, ENC_POLL_TIMEOUT)) < 0) {
			perror("Encoder ISR");
		} else {
			// Check if right encoders event occurred
			if ((enc_int[0].revents & POLLPRI) or (enc_int[1].revents & POLLPRI)) {
				//P8.digitalSetValue(ENC_DEBUG_R, true);

				// Read in encoder values
				ENC_RIGHT_A_PIN.readPin(&enc_value[0]);
				ENC_RIGHT_B_PIN.readPin(&enc_value[1]);

				//printf("Right Enc Event L:%d r:%d\n", enc_value[0], enc_value[1]);

				// Run encoder fsm
				encoder_fsm(&enc_right_state, &encoder_right_count, enc_value[0], enc_value[1]);
			}

			// Check if left encoders event occurred
			if ((enc_int[2].revents & POLLPRI) or (enc_int[3].revents & POLLPRI)) {
				// Read in encoder values
				ENC_LEFT_A_PIN.readPin(&enc_value[2]);
				ENC_LEFT_B_PIN.readPin(&enc_value[3]);

				//printf("Left Enc Event L:%d r:%d\n", enc_value[2], enc_value[3]);

				// Run encoder fsm
				encoder_fsm(&enc_left_state, &encoder_left_count, enc_value[2], enc_value[3]);
			}

			// Check for hangups
			for (int j = 0; j < ENC_POLL_COUNT; j++) {
				if (enc_int[j].revents & POLLHUP) {
					// A hangup has occurred

				}
			}
		}
	}

	return NULL;
}

// Interrupt service routine for right wheel encoder
void encoder_fsm(EncoderState *current_state, int *count, const bool A, const bool B) {
	EncoderState next_state;

	// update state based on new wheel sensor reading
	switch (*current_state) {
	case STATE_FW_A:
	case STATE_RV_A:
		if (!A & B) {
			next_state = STATE_FW_B;
		} else if (A & !B) {
			next_state = STATE_RV_D;
		} else {
			next_state = *current_state;
		}

		break;
	case STATE_FW_B:
	case STATE_RV_B:
		if (!A & !B) {
			next_state = STATE_FW_C;
		} else if (A & B) {
			next_state = STATE_RV_A;
		} else {
			next_state = *current_state;
		}

		break;
	case STATE_FW_C:
	case STATE_RV_C:
		if (A & !B) {
			next_state = STATE_FW_D;
		} else if (!A & B) {
			next_state = STATE_RV_B;
		} else {
			next_state = *current_state;
		}

		break;
	case STATE_FW_D:
	case STATE_RV_D:
		if (A & B) {
			next_state = STATE_FW_A;
		} else if (!A & !B) {
			next_state = STATE_RV_C;
		} else {
			next_state = *current_state;
		}

		break;
	default:
		next_state = STATE_FW_A;

		break;
	}

	// determine if state has changed and if moving forward or reverse
	if (next_state != *current_state) {
		if ((next_state == STATE_FW_A) || (next_state == STATE_FW_B) || (next_state == STATE_FW_C) || (next_state == STATE_FW_D)) {
			*count += 1;
		} else {
			*count -= 1;
		}
	} else {
		*count += 0;
	}

	// Change States
	*current_state = next_state;
}

// Update the calculated position with new data
void update_location(Posistion *posistion) {
	float theta_l;
	float theta_r;
	float theta_sum;

	float delta_x = 0.0;
	float delta_y = 0.0;

	// Left Wheel
	theta_l = -((float)encoder_left_count * WHEEL_DIM * PI / ENC_COUNT) / WHEEL_BASE;
	encoder_left_count = 0;

	delta_x += 0.5 * WHEEL_BASE * (sin(posistion->heading) - sin(posistion->heading + theta_l));
	delta_y += 0.5 * WHEEL_BASE * (cos(posistion->heading + theta_l) - cos(posistion->heading));

	// Right Wheel
	theta_r = ((float)encoder_right_count * WHEEL_DIM * PI / ENC_COUNT) / WHEEL_BASE;
	encoder_right_count = 0;

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
