#include <cstdlib>

#include "classes/waypoint.h"
#include "classes/waypoint_list.h"
#include "classes/double_node.h"
#include "navigation.h"

// Robot Route
WaypointList route;

// Current waypoint
DoubleNode<Waypoint> *current_waypoint;

// Box
Waypoint box_path[16] = {{{0, 0}, true}, {{25, 0}, false}, {{50, 0}, false}, {{50, 25}, false}, {{50, 50}, false}, {{50, 75}, false}, {{50, 100}, false}, {{25, 100}, false},
                         {{0, 100}, false}, {{-25, 100}, false}, {{-50, 100}, false}, {{-50, 75}, false}, {{-50, 50}, false}, {{-50, 25}, false}, {{-50, 0}, false}, {{-25, 0}, false}};

// Figure 8
Waypoint fig8_path[16] = {{{0, 0}, true}, {{25, 0}, false}, {{50, 0}, false}, {{50, 25}, false}, {{50, 50}, false}, {{25, 50}, false}, {{0, 50}, false}, {{0, 25}, false},
                          {{0, 0}, false}, {{0, -25}, false}, {{0, -50}, false}, {{-25, -50}, false}, {{-50, -50}, false}, {{-50, -25}, false}, {{-50, 0}, false}, {{-25, 0}, false}};
