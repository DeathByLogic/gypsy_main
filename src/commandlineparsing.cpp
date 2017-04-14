/*
 * commandlineparsing.cpp
 *
 *  Created on: Apr 26, 2013
 *      Author: daniel
 */

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <unistd.h>

#include "commandlineparsing.h"
#include "robot.h"

using namespace std;

void print_help() {
	cout << "Usage: Robot [OPTIONS] ROUTE_FILE" << endl;
	cout << endl;
	cout << "Options                Meaning" << endl;
	cout << " -h, -?                Show this message" << endl;
	cout << " -v <velocity>         Sets the robots velocity" << endl;
	cout << " -p <x> <y>            Sets the robots starting x and y location" << endl;
	cout << " -h <theta>            Sets the robots starting heading in -pi <= radians <= pi" << endl;
	cout << " -l <sec> <msec>		Period between location messages in seconds and milliseconds, 0 to disable" << endl;
	cout << " -s                    Disables waiting for start button before moving" << endl;
	cout << " -d <sec> <msec>		Period to wait before moving in seconds and milliseconds " << endl;
	cout << " -P <Kp> <Ki> <Kd>     PID parameters" << endl;
	cout << " -T <sec> <msec>       PID loop time in seconds and milliseconds" << endl;
}

int process_arg(int argc, char *argv[]) {
	for (int count = 1; count < argc; count++) {
		if ((strcmp(argv[count], "-?") == 0) or (strcmp(argv[count], "-h") == 0) or (strcmp(argv[count], "--help") == 0)) {
			// Print options
			print_help();

			// Exit program
			exit (EXIT_SUCCESS);
		} else if (strcmp(argv[count], "-v") == 0) {
			// Robot starting velocity
			if (count + 1 <= argc) {
				prgm_vars.defaultSpeed = strtof(argv[++count], NULL);
			}
		} else if (strcmp(argv[count], "-p") == 0) {
			// Robot starting position
			if (count + 2 <= argc) {
				prgm_vars.startPosition.location.x = strtof(argv[++count], NULL);
				prgm_vars.startPosition.location.y = strtof(argv[++count], NULL);
			}
		} else if (strcmp(argv[count], "-h") == 0) {
			// Robot starting heading
			if (count + 1 <= argc) {
				prgm_vars.startPosition.heading = strtof(argv[++count], NULL);
			}
		} else if (strcmp(argv[count], "-l") == 0) {
			// Location message period in milliseconds
			if (count + 2 <= argc) {
				prgm_vars.locationMsgPeriod.tv_sec = strtoul(argv[++count], NULL, 10);
				prgm_vars.locationMsgPeriod.tv_nsec = strtoul(argv[++count], NULL, 10) * 1E6;
			}
		} else if (strcmp(argv[count], "-s") == 0) {
			// Disable wait for start
			prgm_vars.disableStartButton = true;
		} else if (strcmp(argv[count], "-d") == 0) {
			// Supply the startup delay period
			if (count + 2 <= argc) {
				prgm_vars.startDelayPeriod.tv_sec = strtoul(argv[++count], NULL, 10);
				prgm_vars.startDelayPeriod.tv_nsec = strtoul(argv[++count], NULL, 10) * 1E6;
			}
		} else if (strcmp(argv[count], "-P") == 0) {
			// Supply PID values
			if (count + 3 <= argc) {
				prgm_vars.PIDValues.Kp = strtof(argv[++count], NULL);
				prgm_vars.PIDValues.Ki = strtof(argv[++count], NULL);
				prgm_vars.PIDValues.Kd = strtof(argv[++count], NULL);
			}
		} else if (strcmp(argv[count], "-T") == 0) {
			// Supply PID loop period
			if (count + 2 <= argc) {
				prgm_vars.PIDLoopPeriod.tv_sec = strtoul(argv[++count], NULL, 10);
				prgm_vars.PIDLoopPeriod.tv_nsec = strtoul(argv[++count], NULL, 10) * 1E6;
			}
		} else {
			// Load route
		}
	}

	return 0;
}
