#ifndef _ROBOT_H
#define _ROBOT_H

#include <signal.h>
#include <time.h>
#include <gsl/gsl_matrix.h>

#include "pid.h"
#include "navigation.h"
#include "beagleGPIO.h"

// Constants
#define START_BUTTON_PIN			P9_24
#define PAUSE_BUTTON_PIN			P9_42

#define LED1_PIN					P8_39
#define LED2_PIN					P8_45

#define LED_SLOW_PULSE_SEC			0
#define LED_SLOW_PULSE_NSEC			500E6

#define LED_FAST_PULSE_SEC			0
#define LED_FAST_PULSE_NSEC			100E6

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
	short					remotePort;					// Port for the remote
} ProgramVariables;

typedef struct {
	int						leftTickCount;
	int						rightTickCount;
	float					leftTickPeriod;
	float					rightTickPeriod;
	float					frontSonarDistance;
	float					leftSonarDistance;
	float					rightSonarDistance;
} SensorData;

typedef enum {
	RBS_WAIT_FOR_START_CMD,
	RBS_REMOTE,
	RBS_START_DELAY,
	RBS_RUNNING,
	RBS_PAUSED,
	RBS_FINISHED,
	RBS_EXIT
} RobotStates;

typedef struct {
	long					leftTick;
	long					rightTick;
	unsigned long			leftPeriod;
	unsigned long			rightPeriod;
} WheelEncoders;

typedef struct {
	long					quality;
	long					distance;
} LidarSensor;

typedef struct {
	float					front;
	float					left;
	float					right;
	LidarSensor				lidar[360];
} DistanceSensors;

typedef struct {
	WheelEncoders			encoders;
	DistanceSensors			distance;
} RobotSensors;

typedef struct {
	Posistion				posistion;
	float					speed;
	double					speedCommand;
	double					directionCommand;
	bool					remote_enabled;
	RobotStates				state;
	RobotSensors			sensors;
	gsl_matrix				*objectMap;
	gsl_matrix				*locationMap;
} RobotVariables;

// External variables
extern ProgramVariables		prgm_vars;
extern RobotVariables		current_state;

#endif
