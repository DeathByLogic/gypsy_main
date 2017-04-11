/*
 * remote.cpp
 *
 *  Created on: Jan 30, 2015
 *      Author: daniel
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

#include "remote.h"
#include "robot.h"
#include "crc.h"

double remote_speed_command;
double remote_direction_command;

int listenfd = 0;
int connection_fd = 0;

//
// Start and stop the listening for remote
//

void remote_init() {
	start_remote_server(prgm_vars.remotePort);
}

void remote_uninit() {
	stop_remote_server();
}

// Open port and listen for remote connections
void start_remote_server(short port) {
    struct sockaddr_in serv_addr;

    listenfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd, 10);
}

// Close port
void stop_remote_server() {
	close(connection_fd);
}

//
// Processing of remote commands
//

void process_remote() {
	char sendBuff[255];
	char recvBuff[255];

	int recvLength;

	// Initialize the send buffer to all '0'
	memset(sendBuff, '0', sizeof(sendBuff));

	// Accept new connections
	accept_connection();

	// Read and process incoming buffer
	if ((recvLength = remote_receive(recvBuff, sizeof(recvBuff))) > 0) {
		while (recvLength > 0) {
			recvLength = process_command(recvBuff, recvLength);
		}
	}
}

int process_command(char *buf, size_t len) {
	CommandMessages rmt_cmd = (CommandMessages) buf[0];
	unsigned int cmd_len = CommandSize(rmt_cmd);

/*
	for (unsigned int i = 0; i < cmd_len; i++) {
		printf("Byte %i: %x\n", i, buf[i]);
	}
*/

	// Check if enough bytes have been received
	if (len >= cmd_len) {
		// Verify checksum
		if (verify_crc(buf, cmd_len, 0x0000)) {
			// Process command
			switch (rmt_cmd) {
			// General Get Commands
			case RMT_GET_STATUS:
				break;
			case RMT_GET_CONFIGURATION:
				break;
			// Get Motion Commands
			case RMT_GET_SPEED_COMMAND:
				break;
			case RMT_GET_DIR_COMMAND:
				break;
			case RMT_GET_POSITION:
				break;
			case RMT_GET_HEADING:
				break;
			case RMT_GET_SPEED:
				break;
			// Get Waypoint Commands
			case RMT_GET_CRNT_WAYPOINT:
				break;
			case RMT_GET_NEXT_WAYPOINT:
				break;
			case RMT_GET_PREV_WAYPOINT:
				break;
			case RMT_GET_FIRST_WAYPOINT:
				break;
			case RMT_GET_LAST_WAYPOINT:
				break;
			case RMT_GET_WAYPOINT_COUNT:
				break;
			case RMT_GET_WAYPOINT_N:
				break;
			// Get Set Commands
			case RMT_SET_CONFIGUTATION:
				current_state.remote_enabled = buf[1] & RMT_CONFIG_ENABLED;

				break;
			// Set Motion Commands
			case RMT_SET_SPEED_COMMAND:
				current_state.speedCommand = (signed char) buf[1];

				break;
			case RMT_SET_DIR_COMMAND:
				current_state.directionCommand = (signed char)buf[1];

				break;
			case RMT_SET_POSITION:
				break;
			case RMT_SET_HEADING:
				break;
			// Set Waypoint Commands
			case RMT_SET_CRNT_WAYPOINT:
				break;
			case RMT_APPEND_WAYPOINT:
				break;
			case RMT_INSERT_WAYPOINT:
				break;
			case RMT_DELETE_WAYPOINT:
				break;
			default:
				// Invalid command, flush buffer
				len = 0;

				break;
			}

			// Calc new length
			len -= cmd_len;

			// Shift buffer
			memmove(buf, buf + cmd_len, len);
		} else {
			printf("Remote: Checksum Error");

			// Flush buffer on error
			len = 0;
		}
	} else {
		printf("Remote: Length Error");

		// Flush buffer on error
		len = 0;
	}

	// Return length of buffer after command is processed
	return len;
}

void accept_connection() {
	int new_connection_fd;

	if ((new_connection_fd = accept(listenfd, (struct sockaddr*)NULL, NULL)) > 0) {
		// Close existing connection
		close(connection_fd);

		// Set new connection as current
		connection_fd = new_connection_fd;
	}
}

int remote_receive(char *buf, size_t len) {
	int n;

	if (connection_fd > 0) {
		n = recv(connection_fd, buf, len, MSG_DONTWAIT);
	}

	return n;
}

void remote_send(const char *buf, size_t len) {
	write(connection_fd, buf, len);
}

unsigned int CommandSize(CommandMessages rmt_cmd) {
	unsigned int rtn = 0;

	switch (rmt_cmd) {
	// General Get Commands
	case RMT_GET_STATUS:
		break;
	case RMT_GET_CONFIGURATION:
		break;
	// Get Motion Commands
	case RMT_GET_SPEED_COMMAND:
		break;
	case RMT_GET_DIR_COMMAND:
		break;
	case RMT_GET_POSITION:
		break;
	case RMT_GET_HEADING:
		break;
	case RMT_GET_SPEED:
		break;
	// Get Waypoint Commands
	case RMT_GET_CRNT_WAYPOINT:
		break;
	case RMT_GET_NEXT_WAYPOINT:
		break;
	case RMT_GET_PREV_WAYPOINT:
		break;
	case RMT_GET_FIRST_WAYPOINT:
		break;
	case RMT_GET_LAST_WAYPOINT:
		break;
	case RMT_GET_WAYPOINT_COUNT:
		break;
	case RMT_GET_WAYPOINT_N:
		break;
	// Get Set Commands
	case RMT_SET_CONFIGUTATION:
	case RMT_SET_SPEED_COMMAND:
	case RMT_SET_DIR_COMMAND:
		rtn = 4;

		break;
	case RMT_SET_POSITION:
		break;
	case RMT_SET_HEADING:
		break;
	// Set Waypoint Commands
	case RMT_SET_CRNT_WAYPOINT:
		break;
	case RMT_APPEND_WAYPOINT:
		break;
	case RMT_INSERT_WAYPOINT:
		break;
	case RMT_DELETE_WAYPOINT:
		break;
	}

	return rtn;
}

char crc8(const char *buf, size_t len) {
	const char *data = buf;
	unsigned crc = 0;

	for (int j = len; j; j--, data++) {
		crc ^= (*data << 8);

		for (int i = 8; i; i--) {
			if (crc & 0x8000) {
				crc ^= (0x1070 << 3);
			}

			crc <<= 1;
		}
	}

	return (char)(crc >> 8);
}
