/*
 * sonar.cpp
 *
 *  Created on: May 12, 2013
 *      Author: daniel
 */

#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <poll.h>
#include <unistd.h>

#include "global.h"
//#include "sonar.h"
#include "beagleIO.h"

// Sonar timer ID


/*void sonar_init() {
	// Configure the I/O pins
	P9.pinExport(SONAR_1_PW_PIN);
	P9.pinExport(SONAR_1_RX_PIN);

	// Change encoder pins to input
	P9.digitalSetDirection(SONAR_1_PW_PIN, INPUT_PIN);
	P9.digitalSetDirection(SONAR_1_RX_PIN, OUTPUT_PIN);

	// Attach interrupt pins to encoder functions
	P9.digitalSetEdge(SONAR_1_PW_PIN, BOTH_EDGES);

	// Set Rx pins to 0 to stop ranging
	P9.digitalSetValue(SONAR_1_RX_PIN, false);

	// Init the timer loop for the sonar sensors
	sonar_timer_init();
}*/
