#include <cstdlib>
#include <cmath>

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

// Calculate distance between between points
float distance(const Point location, const Point destination) {
	float distance;

	distance = sqrt(pow(destination.x - location.x, 2) + pow(destination.y - location.y, 2));

	return distance;
}

// Calculate the cross track error
float cross_track_error(const Point location, const Point waypoint_a, const Point waypoint_b) {
	Point r;
	Point d;

	float cte;

	// Calculate values
	r.x = location.x - waypoint_a.x;
	r.y = location.y - waypoint_a.y;

	d.x = waypoint_b.x - waypoint_a.x;
	d.y = waypoint_b.y - waypoint_a.y;

	// Calculate cte
	cte = (r.y * d.x - r.x * d.y) / sqrt(d.x * d.x + d.y * d.y);

	return cte;
}

// Check if past current waypoint
bool past_waypoint(const Point location, const Point waypoint_a, const Point waypoint_b) {
	Point r;
	Point d;
	float u;

	// If past current point then move to next
	r.x = location.x - waypoint_a.x;
	r.y = location.y - waypoint_a.y;

	d.x = waypoint_b.x - waypoint_a.x;
	d.y = waypoint_b.y - waypoint_a.y;

	u = (r.x * d.x + r.y * d.y) / (d.x * d.x + d.y * d.y);

	return (u >= 0)?true:false;
}
