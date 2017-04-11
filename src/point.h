#ifndef _POINT_H
#define _POINT_H

#include <stdio>

class Point {
  public:
    // X & Y location
	float x;
    float y;
	
	// Print location
	void print();
}

void Waypoint::print() {
	printf("x: %'.4f\ty: %'.4f", x, y);
}

#endif