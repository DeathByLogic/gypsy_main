#ifndef _NAVIGATION_H
#define _NAVIGATION_H

// Includes
#include "global.h"
#include "double_list.h"

// Structure for each node
typedef struct {
  Point		location;	// Waypoint location
  bool 		fixed;		// Is this a fixed waypoint
} Waypoint;

// Constant Definitions

// Function Prototypes
void smooth_path(DoubleList<Waypoint> *, float, float, float);
void fill_route(DoubleList<Waypoint> *, const Waypoint *, const int);

// External Variables
extern DoubleList<Waypoint> route;
extern DoubleNode<Waypoint> *current_waypoint;

extern Waypoint box_path[];
extern Waypoint fig8_path[];

#endif
