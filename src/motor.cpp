#include "beagleIO.h"
#include "Sabertooth.h"
#include "global.h"
#include "motor.h"

// Global variables
Sabertooth ST(HBRIDGE_ADDRESS, &HBRIDGE_SERIAL_TX_PIN);

void motor_init() {
	// Set pin modes
	//P8.pinSetMux(HBRIDGE_SERIAL_TX_PIN, MUX_4, PULLUP_DISABLED, PULLDOWN, OUTPUT_PIN, FAST_SLEW);

	//P8.pinSetMux(HBRIDGE_SERIAL_TX_DUMMY, MUX_7, PULLUP_DISABLED, PULLDOWN, INPUT_PIN);

	//P8.pinExport(HBRIDGE_SERIAL_TX_DUMMY);

	//P8.digitalSetDirection(HBRIDGE_SERIAL_TX_DUMMY, INPUT_PIN);

	// Init Serial port for H-Bridge
	HBRIDGE_SERIAL_TX_PIN.openPort(BR_38400, PB_NONE);

	// Init H-Bridge shutdown pin
	HBRIDGE_SHUTDOWN.exportPin();
	HBRIDGE_SHUTDOWN.setDirection(OUTPUT_PIN);
	HBRIDGE_SHUTDOWN.writePin(true);
  
	ST.setDeadband(HBRIDGE_DEADBAND);       // Set deadband width
	ST.setMinVoltage(HBRIDGE_MIN_VOLTAGE);  // Set battery cutoff
	ST.setTimeout(HBRIDGE_TIMEOUT);			// Set serial timeout
}

void motor_update(int speed_command, int direction_command) {
  int motor_speed;
  int motor_direction;
  
  // Constrain speed and direction
  speed_command = CONSTRAIN(speed_command, SPEED_MIN, SPEED_MAX);
  direction_command = CONSTRAIN(direction_command, DIR_MIN, DIR_MAX);
  
  // Scale the values to be used with the motors
  motor_speed = MAP(speed_command, SPEED_MIN, SPEED_MAX, HBRIDGE_CMD_MIN, HBRIDGE_CMD_MAX);
  motor_direction = MAP(direction_command, DIR_MIN, DIR_MAX, -motor_speed, motor_speed);
//  motor_direction = MAP(direction_command, DIR_MIN, DIR_MAX, HBRIDGE_CMD_MIN, HBRIDGE_CMD_MAX);
  
  ST.drive(motor_speed);
  ST.turn(motor_direction);
}

void motor_paused(bool paused) {
	HBRIDGE_SHUTDOWN.writePin(!paused);
}
