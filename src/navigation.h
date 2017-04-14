#ifndef _NAVIGATION_H
#define _NAVIGATION_H

// Includes
#include "waypoint_list.h"
#include "global.h"
#include "double_list.h"

// Constant Definitions

// Function Prototypes
void smooth_path(DoubleList<Waypoint> *, float, float, float);
void fill_route(DoubleList<Waypoint> *, const Waypoint *, const int);

// External Variables
extern WaypointList route;
extern DoubleNode<Waypoint> *current_waypoint;

extern Waypoint box_path[];
extern Waypoint fig8_path[];

#endif
