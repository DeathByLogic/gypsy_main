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

double remote_speed_command;
double remote_direction_command;

int listenfd = 0;
int connection_fd = 0;

//
// Start and stop the listening for remotes
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

	unsigned recvLength;
	unsigned sendLength;

	// Initialize the send buffer to all '0'
	memset(sendBuff, '0', sizeof(sendBuff));

	// Accept new connections
	accept_connection();

	// Read the incoming buffer
	recvLength = remote_receive(recvBuff, sizeof(recvBuff));

	// Verify Checksum
	if (recvBuff[recvLength - 1] == crc8(recvBuff, recvLength - 1)) {
		// Process commands
		printf(recvBuff);
	} else {
		printf("Remote CRC error.");
	}

	// Send the response
	remote_send(sendBuff, sizeof(sendBuff));
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

size_t remote_receive(char *buf, size_t len) {
	size_t n;

	if (connection_fd > 0) {
		while ((n = recv(connection_fd, buf, len - 1, MSG_DONTWAIT)) > 0) {
			buf[n] = 0;

			if(fputs(buf, stdout) == EOF) {
				printf("\n Error : Fputs error\n");
			}
		}
	}

	return n;
}

void remote_send(const char *buf, size_t len) {
	write(connection_fd, buf, len);
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
