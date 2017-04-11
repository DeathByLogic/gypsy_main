/*
 * localization.cpp
 *
 *  Created on: May 10, 2015
 *      Author: daniel
 */

#include <stdio.h>
#include <math.h>

#include "global.h"
#include "robot.h"
#include "localization.h"

void localize_init() {
	// Allocate memory for map
	//current_state.objectMap = gsl_matrix_alloc(MAP_SIZE, MAP_SIZE);

	// Initialize all cells on map to 0.5
	//gsl_matrix_set_all(current_state.objectMap, MAP_INITAL_VALUE);
}

void localize() {
	Point right_sensor = localize_sensor(current_state.sensors.distance.right + RIGHT_SENSOR_OFFSET, current_state.posistion.heading);//RIGHT_SENSOR_ANGLE);

	//decay_map();

	//add_object(right_sensor);

	//print_map();
}

Point localize_sensor(float distance, float angle) {
	Point objLocation;

	objLocation.x = distance * cos(angle);
	objLocation.y = distance * sin(angle);

	return objLocation;
}

/*
void decay_map() {
	for (int x = 0; x < MAP_SIZE; x++) {
		for (int y = 0; y < MAP_SIZE; y++) {
			if (map[x][y] > 0) {
				map[x][y] -= MAP_OBJ_DECAY;
			}
		}
	}
}

void add_object(Point object) {
	int x = (int)(object.x / MAP_CELL_SIZE);
	int y = (int)(object.y / MAP_CELL_SIZE) + 5;

	if (map[x][y] + MAP_OBJ_INCREMENT > 255) {
		map[x][y] = 255;
	} else {
		map[x][y] += MAP_OBJ_INCREMENT;
	}
}
*/

void print_map() {
	printf("---------------------------------------------------\n");

	for (int x = 0; x < MAP_SIZE; x++) {
		for (int y = 0; y < MAP_SIZE; y++) {
			//printf("|0x%02x", map[x][y]);
		}

		printf("|\n");
		printf("---------------------------------------------------\n");
	}
}
