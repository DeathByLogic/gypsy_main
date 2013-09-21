/*
 * localization.h
 *
 *  Created on: May 14, 2013
 *      Author: daniel
 */

#ifndef LOCALIZATION_H_
#define LOCALIZATION_H_

#include <gsl/gsl_matrix.h>

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
};

class localization {
public:
	localization();
	~localization();

	robotmap map;

	gsl_matrix *_location;
};


#endif /* LOCALIZATION_H_ */
