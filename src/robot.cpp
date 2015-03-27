#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <signal.h>
#include <time.h>
#include <string.h>

#include "global.h"
#include "pid.h"
#include "motor.h"
#include "encoders.h"
#include "robot.h"
#include "navigation.h"
#include "debug.h"
#include "beagleIO.h"
#include "commandlineparsing.h"
//#include "sonar.h"

using namespace std;

// Current Robot State
RobotVariables		current_state;

// Global program Variables
ProgramVariables	prgm_vars;

// PID Loop timer ID
timer_t pid_loop_id;

//SonarSystem *sonar;
//SonarSystem::SonarSensor *sensors;

// Main function
int main(int argc, char *argv[]) {
	// Initialize variables
	init_vars();

	// Process command line arguments
	process_arg(argc, argv);

	// Initialize the robot subsystems
	init_sys();

	// Load a path to follow
	load_path();

	// Run background while robot not finished
	while (current_state.state != RBS_EXIT) {
		// Run the background loop
		background();
	}

	// Uninitialize system
	uninit_sys();
}

void init_vars() {
	// Set robot initial state
	current_state.speed_command = 0.0;
	current_state.direction_command = 0.0;
	current_state.state = RBS_WAIT_FOR_START_CMD;

	// Init program variables
	prgm_vars.PIDLoopPeriod.tv_sec = DEFAULT_PID_PERIOD_SEC;
	prgm_vars.PIDLoopPeriod.tv_nsec = DEFAULT_PID_PERIOD_NSEC;
	prgm_vars.PIDValues = {1.45, 0.11, 4.59};
	prgm_vars.defaultSpeed = DEFAULT_ROBOT_SPEED;
	prgm_vars.disableStartButton = false;
	prgm_vars.locationMsgPeriod.tv_sec = DEFAULT_BKG_PERIOD_SEC;
	prgm_vars.locationMsgPeriod.tv_nsec = DEFAULT_BKG_PERIOD_NSEC;
	prgm_vars.startDelayPeriod.tv_sec = DEFAULT_START_DELAY_SEC;
	prgm_vars.startDelayPeriod.tv_nsec = DEFAULT_START_DELAY_NSEC;
	prgm_vars.startPosistion.location = {0.0, 0.0};
	prgm_vars.startPosistion.heading = 0.0;
}

// Program Setup Function
void init_sys() {
	robot_init();		// Init robot pins and setting
	pid_init();			// Init PID control loop
	motor_init();		// Init motor control
	encoders_init();	// Init wheel encoders
	debug_init();		// Init debug features
	timer_init();		// Init robot loop timer
	//sonar_init();

	//sensors = new SonarSystem::SonarSensor[1];

	//sensors->fileDesc = 0;
}

void uninit_sys() {
	timer_uninit();		// Uninit PID control loop
	encoders_uninit();	// Uninit wheel encoders

	// Turn off LEDs
	LED1_PIN.writePin(true);
	LED2_PIN.writePin(true);
}

// Init the robot variables
void robot_init() {
	// Change start button pin to input
	START_BUTTON_PIN.exportPin();
	PAUSE_BUTTON_PIN.exportPin();

	START_BUTTON_PIN.setDirection(INPUT_PIN);
	PAUSE_BUTTON_PIN.setDirection(INPUT_PIN);

	// Change LEDs pins to output
	LED1_PIN.exportPin();
	LED2_PIN.exportPin();

	LED1_PIN.setDirection(OUTPUT_PIN);
	LED2_PIN.setDirection(OUTPUT_PIN);

	LED1_PIN.writePin(true);
	LED2_PIN.writePin(true);
}

// Init timer for the main loop
void timer_init() {
	int rc;
	struct sigevent svnt;
	struct itimerspec  its;

	// Set timer settings
	svnt.sigev_notify = SIGEV_THREAD;
	svnt.sigev_notify_function = &robot_run;
	svnt.sigev_notify_attributes = NULL;

	if ((rc = timer_create(CLOCK_MONOTONIC, &svnt, &pid_loop_id)) < 0) {
		perror("Create Timer");
	}

	its.it_interval = prgm_vars.PIDLoopPeriod;
	its.it_value = prgm_vars.PIDLoopPeriod;

	if ((rc = timer_settime(pid_loop_id, 0, &its, NULL)) < 0) {
		perror("Set Timer");
	}
}

void timer_uninit() {
	timer_delete(pid_loop_id);
}

// Background program loop
void background() {
	static timespec prev_time;
	static bool	led = true;

	timespec current_time;
	timespec delta_time;

	// Get the current time
	clock_gettime(CLOCK_BOOTTIME, &current_time);

	// Print the current location
	if (current_time > prev_time) {
		switch (current_state.state) {
		case RBS_WAIT_FOR_START_CMD:
			// Update time to new value
			delta_time.tv_sec = 0;
			delta_time.tv_nsec = 500E6;

			prev_time = current_time + delta_time;

			led = !led;

			LED1_PIN.writePin(led);
			LED2_PIN.writePin(true);

			break;
		case RBS_START_DELAY:
		case RBS_PAUSED:
			// Update time to new value
			delta_time.tv_sec = 0;
			delta_time.tv_nsec = 100E6;

			prev_time = current_time + delta_time;

			led = !led;

			LED1_PIN.writePin(led);
			LED2_PIN.writePin(true);

			break;
		case RBS_RUNNING:
			// Adjust LEDs
			LED1_PIN.writePin(false);
			LED2_PIN.writePin(true);

			if ((prgm_vars.locationMsgPeriod.tv_sec > 0) or (prgm_vars.locationMsgPeriod.tv_nsec > 0)) {
				// Print location
				debug_print_location(current_state.posistion);

				// Update time to new value
				prev_time = current_time + prgm_vars.locationMsgPeriod;
			}

			break;
		case RBS_FINISHED:
			// Update time to new value
			delta_time.tv_sec = 0;
			delta_time.tv_nsec = 500E6;

			prev_time = current_time + delta_time;

			led = !led;

			LED1_PIN.writePin(led);
			LED2_PIN.writePin(!led);

			break;
		default:

			break;
		}

		// Update to new time

	}
}

// Main robot loop
void robot_run(union sigval arg) {
	static timespec start_delay_time;
	timespec current_time;

	// Get the current time
	clock_gettime(CLOCK_BOOTTIME, &current_time);

	switch (current_state.state) {
	case RBS_WAIT_FOR_START_CMD:
		// Wait for start button
		if ((START_BUTTON_PIN.readPin() == false) or (prgm_vars.disableStartButton == true)) {
			// Set starting point
			current_waypoint = route.first();

			// Set robot initial position
			current_state.posistion = prgm_vars.startPosistion;

			// Update start delay time to new value
			start_delay_time = current_time + prgm_vars.startDelayPeriod;

			// Set robot to delay mode
			current_state.state = RBS_START_DELAY;
		}

		break;
	case RBS_START_DELAY:
		if (current_time > start_delay_time) {
			// Print the starting waypoint
			printf("==Starting Movement==\n");

			// Set robot to movement mode
			current_state.state = RBS_RUNNING;
		}

		break;
	case RBS_RUNNING:
		if (current_waypoint == route.last()) {
			// If at last waypoint stop
			current_state.speed_command = 0.0;

			printf("== Stopping Movement ==\n");

			// Robot has finished the route
			current_state.state = RBS_FINISHED;
		} else {
			// Maintain current speed
			current_state.speed_command = prgm_vars.defaultSpeed;

			 // Update the current location
			update_location(&current_state.posistion);

			// Calculate the error value
			ct_error = calculate_cte(current_waypoint, current_state.posistion.location);

			// Run the control loop
			pid_control_loop();

			// If Emergency button is pushed, pause robot
			if (PAUSE_BUTTON_PIN.readPin() == false) {
				// Set robot to paused state;
				current_state.state = RBS_PAUSED;

				// Stop the robot
				current_state.speed_command = 0.0;

				printf("== Pausing Movement ==\n");
			}
		}

		break;
	case RBS_PAUSED:
		// Stop the robot
		current_state.speed_command = 0.0;

		// Update the current location
		update_location(&current_state.posistion);

		// If Emergency button is released, resume
		if (PAUSE_BUTTON_PIN.readPin() == true) {

			// Set robot to paused state;
			current_state.state = RBS_RUNNING;

			printf("== Resuming Movement ==\n");
		}

		break;
	case RBS_FINISHED:
		// Stop the robot
		current_state.speed_command = 0.0;

		if (START_BUTTON_PIN.readPin() == false) {
			// Update start delay time to new value
			start_delay_time = current_time + prgm_vars.startDelayPeriod;

			// Set robot to delay mode
			current_state.state = RBS_START_DELAY;
		} else if (PAUSE_BUTTON_PIN.readPin() == false) {
			current_state.state = RBS_EXIT;
		}

		break;
	case RBS_EXIT:
		// Do nothing program exiting
		break;
	default:
		current_state.state = RBS_FINISHED;

		break;
	}

	// Update h-bridge output
	motor_update(current_state.speed_command, current_state.direction_command);
}

// Load a path into the route
void load_path() {
	fill_route(&route, box_path, 16);

	debug_print_path(&route);

	//smooth_path(&route, 0.00001, 0.5, 0.25);

	//debug_print_path(&route);
}
