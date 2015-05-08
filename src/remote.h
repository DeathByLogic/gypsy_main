/*
 * remote.h
 *
 *  Created on: Jan 30, 2015
 *      Author: daniel
 */

#ifndef REMOTE_H_
#define REMOTE_H_

// Constants
#define RMT_COMMAND				0x00
#define RMT_RESPONSE			0x80

#define RMT_CONFIG_ENABLED		0x01

// Type Definitions
typedef enum {
	RMT_GET_STATUS			= 0x01,
	RMT_GET_CONFIGURATION	= 0x02,

	RMT_GET_SPEED_COMMAND	= 0x11,
	RMT_GET_DIR_COMMAND		= 0x12,
	RMT_GET_POSITION		= 0x13,
	RMT_GET_HEADING			= 0x14,
	RMT_GET_SPEED			= 0x15,

	RMT_GET_CRNT_WAYPOINT	= 0x21,
	RMT_GET_NEXT_WAYPOINT	= 0x22,
	RMT_GET_PREV_WAYPOINT	= 0x23,
	RMT_GET_FIRST_WAYPOINT	= 0x24,
	RMT_GET_LAST_WAYPOINT	= 0x25,
	RMT_GET_WAYPOINT_COUNT	= 0x26,
	RMT_GET_WAYPOINT_N		= 0x27,

	RMT_SET_CONFIGUTATION	= 0x42,

	RMT_SET_SPEED_COMMAND	= 0x51,
	RMT_SET_DIR_COMMAND		= 0x52,
	RMT_SET_POSITION		= 0x53,
	RMT_SET_HEADING			= 0x54,

	RMT_SET_CRNT_WAYPOINT	= 0x61,
	RMT_APPEND_WAYPOINT		= 0x62,
	RMT_INSERT_WAYPOINT		= 0x63,
	RMT_DELETE_WAYPOINT		= 0x64
} CommandMessages;

// Function Prototypes
void remote_init();
void remote_uninit();

void start_remote_server(short port);
void stop_remote_server();
void accept_connection();
void process_remote();
int remote_receive(char *buf, size_t len);
void remote_send(const char *buf, size_t len);
int process_command(char *buf, size_t len);
unsigned int CommandSize(CommandMessages rmt_cmd);
char crc8(const char *buf, size_t len) ;

// External variables
extern double remote_speed_command;
extern double remote_direction_command;

#endif /* REMOTE_H_ */
