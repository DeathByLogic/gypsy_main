#ifndef _WAYPOINT_H
#define _WAYPOINT_H

#include <stdio>
#include "point.h"

class Waypoint {
  public:
	Point location;	// Waypoint location
    bool fixed;		// Is this a fixed waypoint
	
	// Print location
	void print();
}

void Waypoint::print() {
	printf("x: %'.4f\ty: %'.4f\tf: %d\n", location.x, location.y, fixed);
}

#endif