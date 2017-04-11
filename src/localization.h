/*
 * localization.h
 *
 *  Created on: May 14, 2013
 *      Author: daniel
 */

#ifndef LOCALIZATION_H_
#define LOCALIZATION_H_

#include <gsl/gsl_matrix.h>

#include "global.h"

#define MAP_SIZE			50
#define MAP_CENTER_X		25
#define MAP_CENTER_Y		25
#define MAP_INITAL_VALUE	0.5
#define MAP_CELL_SIZE		10

#define RIGHT_SENSOR_OFFSET	5.25
#define LEFT_SENSOR_OFFSET	5.25
#define FRONT_SENSOR_OFFSET	5.00

#define RIGHT_SENSOR_ANGLE	-PI/8.0
#define LEFT_SENSOR_ANGLE	PI/8.0
#define FRONT_SENSOR_ANGLE	0.0

// Function Prototypes
void localize_init();
void localize();
Point localize_sensor(float distance, float angle);
void decay_map();
void add_object(Point object);
void print_map();

/*
class robotmap {
public:

	struct weightedPoint {
		float x;
		float y;
		float weight;
	};

	void clearMap();

	void setLocation();
	void getLocation();
	void moveLocation();

	void addObsticle();

private:
	gsl_matrix *_map;

	int _xSize;
	int _ySize;

	int _xCenter;
	int _yCenter;
};

class localization {
public:
	localization();
	~localization();

	robotmap map;

	gsl_matrix *_location;
};

*/
#endif /* LOCALIZATION_H_ */
