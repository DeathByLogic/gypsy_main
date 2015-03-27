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

#define RMT_GET_STATUS			0x01
#define RMT_GET_CONFIGURATION	0x02

#define RMT_GET_SPEED_COMMAND	0x11
#define RMT_GET_DIR_COMMAND		0x12
#define RMT_GET_POSITION		0x13
#define RMT_GET_HEADING			0x14
#define RMT_GET_SPEED			0x15

#define RMT_GET_CRNT_WAYPOINT	0x21
#define RMT_GET_NEXT_WAYPOINT	0x22
#define RMT_GET_PREV_WAYPOINT	0x23
#define RMT_GET_FIRST_WAYPOINT	0x24
#define RMT_GET_LAST_WAYPOINT	0x25
#define RMT_GET_WAYPOINT_COUNT	0x26
#define RMT_GET_WAYPOINT_N		0x27

#define RMT_SET_CONFIGUTATION	0x42

#define RMT_SET_SPEED_COMMAND	0x51
#define RMT_SET_DIR_COMMAND		0x52
#define RMT_SET_POSITION		0x53
#define RMT_SET_HEADING			0x54

#define RMT_SET_CRNT_WAYPOINT	0x61
#define RMT_APPEND_WAYPOINT		0x62
#define RMT_INSERT_WAYPOINT		0x63
#define RMT_DELETE_WAYPOINT		0x64

// Function Prototypes
void remote_init();
void remote_uninit();

void start_remote_server(short port);
void stop_remote_server();
void accept_connection();
void process_remote();

size_t remote_receive(char *buf, size_t len);
void remote_send(const char *buf, size_t len);
char crc8(const char *buf, size_t len) ;

// Type Definitions

// External variables
extern double remote_speed_command;
extern double remote_direction_command;

#endif /* REMOTE_H_ */
