#ifndef _WAYPOINT_LIST_H
#define _WAYPOINT_LIST_H

#include "double_list.h"
#include "waypoint.h"

class WaypointList : public DoubleList<Waypoint> {
  public:
	~WaypointList();

	// Bulk add waypoints
	void add(const Waypoint *route_points, const int length);
	
	// Perform optimizations on list
	void smooth(float tolerance, float weight_data, float weight_smooth);
	
	// Print List
	void print();

  private:
};

#endif
