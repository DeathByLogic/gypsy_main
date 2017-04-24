#ifndef _MOTOR_H
#define _MOTOR_H

// Constant Definitions
#define HBRIDGE_ADDRESS     	128
#define HBRIDGE_BAUD_RATE   	9600

#define HBRIDGE_MIN_VOLTAGE 	34  // Cutoff voltage of roughly 10V
#define HBRIDGE_DEADBAND    	3   // Deadband cutoff of 3 counts

#define HBRIDGE_CMD_MIN    	   -127
#define HBRIDGE_CMD_MAX     	127

#define HBRIDGE_SERIAL_TX_PIN	P8_37
#define HBRIDGE_SERIAL_TX_DUMMY	P8_43

// Motion Constants
#define SPEED_MIN          	   -100
#define SPEED_MAX           	100

#define DIR_MIN            	   -100
#define DIR_MAX             	100

// Function Prototypes
void motor_init();
void motor_update(int, int);
void update_motor();

#endif
