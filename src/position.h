#ifndef _POSITION_H
#define _POSITION_H

#include <stdio>
#include "point.h"

class Position {
  public:
    // X & Y location & heading
	Point location
	float heading
	
	// Print location
	void print();
}

void Waypoint::print() {
		printf("Location: x: %'.4f\ty: %'.4f\to: %'.4f\n", location.x, location.y, heading);
}

#endif