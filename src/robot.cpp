#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <csignal>
#include <cstring>
#include <thread>

#include "global.h"
#include "pid.h"
#include "motor.h"
#include "encoders.h"
#include "robot.h"
#include "navigation.h"
#include "beagleIO.h"
#include "commandlineparsing.h"
#include "sensors.h"

using namespace std;

// Current Robot State
RobotVariables		current_state;
RobotVariables		next_state;

// Global program Variables
ProgramVariables	prgm_vars;

// PID Loop timer ID
timer_t pid_loop_id;

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

	// Uninitialized system
	uninit_sys();
}

void init_vars() {
	// Set robot initial state
	next_state.motors.speed.target = 0.0;
	next_state.motors.direction.ctTarget = 0.0;
	next_state.state = RBS_WAIT_FOR_START_CMD;

	current_state = next_state;

	// Initialize program variables
	prgm_vars.PIDLoopPeriod.tv_sec = DEFAULT_PID_PERIOD_SEC;
	prgm_vars.PIDLoopPeriod.tv_nsec = DEFAULT_PID_PERIOD_NSEC;
	prgm_vars.PIDValues = {1.45, 0.11, 4.59};
	prgm_vars.defaultSpeed = DEFAULT_ROBOT_SPEED;
	prgm_vars.disableStartButton = false;
	prgm_vars.locationMsgPeriod.tv_sec = DEFAULT_BKG_PERIOD_SEC;
	prgm_vars.locationMsgPeriod.tv_nsec = DEFAULT_BKG_PERIOD_NSEC;
	prgm_vars.startDelayPeriod.tv_sec = DEFAULT_START_DELAY_SEC;
	prgm_vars.startDelayPeriod.tv_nsec = DEFAULT_START_DELAY_NSEC;
	prgm_vars.startPosition.location = {0.0, 0.0};
	prgm_vars.startPosition.heading = 0.0;
}

// Program Setup Function
void init_sys() {
	robot_init();		// Init robot pins and setting
	pid_init();			// Init PID control loop
	motor_init();		// Init motor control
//	encoders_init();	// Init wheel encoders
	timer_init();		// Init robot loop timer
}

void uninit_sys() {
	timer_uninit();		// Uninit PID control loop
//	encoders_uninit();	// Uninit wheel encoders

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
				current_state.position.print();

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
	}
}

// Main robot timer interrupt
void robot_run(union sigval arg) {
	// Update the current state
	current_state = next_state;

	// Update motor output with previous calculations
	motor_update(current_state.motors.speed.command, current_state.motors.direction.command);

	// Read buttons for next state
	read_buttons();

	// Get sensor data for next state
	update_sensor_values();

	// Run the robot state machine
	robot_state();
}

// Read button states
void read_buttons() {
	// Buttons are low when pushed so invert the state of the buttons
	next_state.buttons.startButton = not START_BUTTON_PIN.readPin();
	next_state.buttons.pauseButton = not PAUSE_BUTTON_PIN.readPin();
}

// Robot State Machine
void robot_state() {
	static timespec start_delay_time;
	timespec current_time;

	// Get the current time
	clock_gettime(CLOCK_BOOTTIME, &current_time);

	switch (current_state.state) {
	case RBS_WAIT_FOR_START_CMD:
		// Wait for start button
		if ((current_state.buttons.startButton == true) or (prgm_vars.disableStartButton == true)) {
			// Set starting point
			current_waypoint = route.first();

			// Set robot initial position
			next_state.position = prgm_vars.startPosition;

			// Update start delay time to new value
			start_delay_time = current_time + prgm_vars.startDelayPeriod;

			// Set robot to delay mode
			next_state.state = RBS_START_DELAY;
		}

		break;
	case RBS_START_DELAY:
		if (current_time > start_delay_time) {
			// Print the starting waypoint
			printf("==Starting Movement==\n");

			// Set robot to movement mode
			next_state.state = RBS_RUNNING;
		}

		break;
	case RBS_RUNNING:
		// Update the current location
		next_state.position = update_location(current_state.position, current_state.sensors.encoders.leftTick, current_state.sensors.encoders.rightTick);

		// Update the current speed
		next_state.speed = update_speed(current_state.sensors.encoders.leftPeriod, current_state.sensors.encoders.rightPeriod);

		// If Emergency button is pushed, pause robot
		if (current_state.buttons.pauseButton == true) {
			// Stop the robot
			next_state.motors.speed.target = 0.0;

			printf("== Pausing Movement ==\n");

			// Set robot to paused state;
			next_state.state = RBS_PAUSED;
		} else {
			// Check if at or past current waypoint
			if (past_waypoint(next_state.position.location, current_waypoint->value.location, current_waypoint->next()->value.location)) {
				// Move to next waypoint
				current_waypoint = current_waypoint->next();

				printf("Reached Waypoint: ");
				current_waypoint->value.print();
			}

			// Check if at last waypoint
			if (current_waypoint == route.last()) {
				// If at last waypoint stop
				next_state.motors.speed.target = 0.0;

				printf("== Stopping Movement ==\n");

				// Robot has finished the route
				next_state.state = RBS_FINISHED;
			} else {
				// Maintain default speed
				next_state.motors.speed.target = prgm_vars.defaultSpeed;

				// Calculate the cross track error value
				next_state.motors.direction.ctError = cross_track_error(next_state.position.location, current_waypoint->value.location, current_waypoint->next()->value.location);

				// Run the control loop
				pid_control_loop();

				// Constrain speed and direction commands
				next_state.motors.speed.command = CONSTRAIN(next_state.motors.speed.command , SPEED_MIN, SPEED_MAX);
				next_state.motors.direction.command = CONSTRAIN(next_state.motors.direction.command, DIR_MIN, DIR_MAX);
			}
		}

		break;
	case RBS_PAUSED:
		// Stop the robot
		next_state.motors.speed.target = 0.0;

		// Update the current location
		next_state.position = update_location(current_state.position, current_state.sensors.encoders.leftTick, current_state.sensors.encoders.rightTick);

		// If Emergency button is released, resume
		if (current_state.buttons.pauseButton == false) {

			// Set robot to paused state;
			next_state.state = RBS_RUNNING;

			printf("== Resuming Movement ==\n");
		}

		break;
	case RBS_FINISHED:
		// Stop the robot
		next_state.motors.speed.target = 0.0;

		if (current_state.buttons.startButton == true) {
			// Update start delay time to new value
			start_delay_time = current_time + prgm_vars.startDelayPeriod;

			// Set robot to delay mode
			next_state.state = RBS_START_DELAY;
		} else if (current_state.buttons.pauseButton == true) {
			next_state.state = RBS_EXIT;
		}

		break;
	case RBS_EXIT:
		// Do nothing program exiting
		break;
	default:
		next_state.state = RBS_FINISHED;

		break;
	}
}

// Load a path into the route
void load_path() {
	// Add waypoints to route
	route.add(box_path, 16);

	// Print out path
	printf("== Original Path Data ==\n");
	route.print();

	// Smooth route
	//route.smooth(0.00001, 0.5, 0.25);

	// Print smoothed route
	printf("== Smoothed Path Data ==\n");
	route.print();
}
