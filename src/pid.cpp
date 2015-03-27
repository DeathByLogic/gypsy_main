#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <cmath>

#include "global.h"
#include "debug.h"

#include "PID_v1.h"
//#include <PID_AutoTune_v0.h>
#include "pid.h"
#include "robot.h"
#include "global.h"
#include "motor.h"

using namespace std;

// Global Variables
PIDParam tuned_pid = {0.37, 0.02, 2.20};

PIDParam motor_20_pid = {7.00, 1.00, 5.00};
PIDParam motor_50_pid = {1.00, 0.00, 1.50};

PIDParam tuned_50s = {1.45, 0.11, 4.59};
PIDParam tuned_100s = {10.72, 0.90, 31.90};

// Current Robot Speed & Direction
double robot_speed = 0.0;
double robot_dir = 0.0;

// Current Error and Target
double ct_target = 0.0;
double ct_error = 0.0;

//Specify the links and initial tuning parameters
PID myPID(&ct_error, &current_state.direction_command, &ct_target, tuned_50s.Kp, tuned_50s.Ki, tuned_50s.Kd, DIRECT);
//PID myPID(&ct_error, &robot_dir, &ct_target, tuned_50s.Kp, tuned_50s.Ki, tuned_50s.Kd, DIRECT);
//PID_ATune aTune(&ct_error, &robot_dir);

void pid_init() {
  // setup PID module
  myPID.SetMode(AUTOMATIC);
  myPID.SetSampleTime(SAMPLE_PERIOD); 
  myPID.SetOutputLimits(OUTPUT_MIN, OUTPUT_MAX);
}

//void pid_control_loop(union sigval)
void pid_control_loop() {
	// Run PID calculations
	myPID.Compute();
}

// Calculate the cross track error
float calculate_cte(DoubleNode<Waypoint> *&node, const Point location) {
	Point current;
	Point next;
	Point r;
	Point d;
	float u;
	float cte;

	// If not the last node
	if (node->next() != NULL) {
		// Store current point and next point locations
		current.x = node->value.location.x;
		current.y = node->value.location.y;

		next.x = node->next()->value.location.x;
		next.y = node->next()->value.location.y;

		// Calculate values
		r.x = location.x - current.x;
		r.y = location.y - current.y;

		d.x = next.x - current.x;
		d.y = next.y - current.y;

		u = (r.x * d.x + r.y * d.y) / (d.x * d.x + d.y * d.y);

		// If past current point then move to next
		if (u > 1.0) {
			node = node->next();

			printf("Reached Waypoint: ");
			debug_print_waypoint(node);
		}

		// Calculate cte
		cte = (r.y * d.x - r.x * d.y) / sqrt(d.x * d.x + d.y * d.y);
	} else {
		cte = 0.0;
	}

	return cte;
}

//
// Old Code
//

/*
// Global Variables
//PIDParam Theta_Param = {26.3663, 4.0130, 1.1822};
PIDParam Theta_PID = {-0.35, 0.0, 0.0};
PIDParam Loc_PID = {0.0, 0.0, 0.0};

// Calculate location based PID
float calc_location_PID(PIDParam PID_Param, float error) {
  static float int_error = 0.0;
  static float pre_error = 0.0;
  
  float drv_error = 0.0;
  float per_error = 0.0;
  
  per_error = error;
  int_error += per_error * SAMPLE_PERIOD;
  drv_error = (per_error - pre_error) / SAMPLE_PERIOD;
  
  pre_error = per_error; 
  
  return PID_Param.Kp * per_error + PID_Param.Ki * int_error + PID_Param.Kd * drv_error;
}

// Calculate theta based PID
float calc_theta_PID(PIDParam PID_Param, float error) {
  static float int_error = 0.0;
  static float pre_error = 0.0;
  
  float drv_error = 0.0;
  float per_error = 0.0;
  
  per_error = error;
  int_error += per_error * SAMPLE_PERIOD;
  drv_error = (per_error - pre_error) / SAMPLE_PERIOD;
  
  pre_error = per_error; 
  
  return PID_Param.Kp * per_error + PID_Param.Ki * int_error + PID_Param.Kd * drv_error;
}
*/
