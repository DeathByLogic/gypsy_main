#include "classes/PID_v1.h"
//#include <PID_AutoTune_v0.h>

#include "pid.h"
#include "robot.h"

// Global Variables
PIDParam tuned_pid = {0.37, 0.02, 2.20};

PIDParam motor_20_pid = {7.00, 1.00, 5.00};
PIDParam motor_50_pid = {1.00, 0.00, 1.50};

PIDParam tuned_50s = {1.45, 0.11, 4.59};
PIDParam tuned_100s = {10.72, 0.90, 31.90};

//Specify the links and initial tuning parameters
PID direction_pid(&next_state.motors.direction.ctError, &next_state.motors.direction.command, &next_state.motors.direction.ctTarget, tuned_50s.Kp, tuned_50s.Ki, tuned_50s.Kd, DIRECT);
PID speed_pid(&next_state.speed, &next_state.motors.speed.command, &next_state.motors.speed.target, tuned_50s.Kp, tuned_50s.Ki, tuned_50s.Kd, DIRECT);

//PID_ATune aTune(&ct_error, &robot_dir);

void pid_init() {
	// Setup direction PID module
	direction_pid.SetMode(AUTOMATIC);
	direction_pid.SetSampleTime(SAMPLE_PERIOD);
	direction_pid.SetOutputLimits(OUTPUT_MIN, OUTPUT_MAX);

	// Setup direction PID module
	speed_pid.SetMode(AUTOMATIC);
	speed_pid.SetSampleTime(SAMPLE_PERIOD);
	speed_pid.SetOutputLimits(OUTPUT_MIN, OUTPUT_MAX);
}

void pid_control_loop() {
	// Run PID calculations
	direction_pid.Compute();
	speed_pid.Compute();
}
