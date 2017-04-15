#ifndef _POINT_H
#define _POINT_H

#include <cstdio>

class Point {
  public:
    // X & Y location
	float x;
    float y;
	
	// Print location
	void print() {
	  printf("x: %'.4f\ty: %'.4f", x, y);
	};
};

#endif
