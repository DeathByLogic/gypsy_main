#ifndef _NAVIGATION_H
#define _NAVIGATION_H

// Includes
#include "classes/waypoint.h"
#include "classes/waypoint_list.h"
#include "classes/double_node.h"
#include "global.h"


// Constant Definitions

// Function Prototypes

// External Variables
extern WaypointList route;
extern DoubleNode<Waypoint> *current_waypoint;

extern Waypoint box_path[];
extern Waypoint fig8_path[];

#endif
