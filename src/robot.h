#ifndef _ROBOT_H
#define _ROBOT_H

#include <signal.h>
#include <time.h>

#include "pid.h"
#include "navigation.h"
#include "beagleGPIO.h"


// Constants
#define START_BUTTON_PIN			24
#define PAUSE_BUTTON_PIN			42

#define LED1_PIN					39
#define LED2_PIN					45

#define DEFAULT_ROBOT_SPEED			25

#define DEFAULT_START_DELAY_SEC		1
#define DEFAULT_START_DELAY_NSEC	0

#define DEFAULT_BKG_PERIOD_SEC		0
#define DEFAULT_BKG_PERIOD_NSEC		500E6

#define DEFAULT_PID_PERIOD_SEC		0
#define DEFAULT_PID_PERIOD_NSEC		100E6

// Function Prototypes
void init_vars();
void init_sys();
void uninit_sys();

void background();

void load_path();

void robot_init();
void robot_run(union sigval arg);
void robot_fsm();

void timer_init();
void timer_uninit();

// Type Definitions
typedef struct {
	bool					disableStartButton;			// Disable waiting for the start button to be pushed
	timespec				startDelayPeriod;			// Wait for n number of ms before starting movement
	timespec				locationMsgPeriod;			// n number of ms between printing of location messages
	float					defaultSpeed;				// The default speed of the robot
	Posistion				startPosistion;				// The starting position of the robot
	PIDParam				PIDValues;					// The Kp, Ki and Kd values
	timespec				PIDLoopPeriod;				// The PID loop period
} ProgramVariables;

typedef enum {
	RBS_WAIT_FOR_START_CMD,
	RBS_START_DELAY,
	RBS_RUNNING,
	RBS_PAUSED,
	RBS_FINISHED,
	RBS_EXIT
} RobotStates;

typedef struct {
	Posistion				posistion;
	double					speed_command;
	double					direction_command;
	RobotStates				state;
} RobotVariables;

// External variables
extern ProgramVariables		prgm_vars;
extern RobotVariables		current_state;

#endif
