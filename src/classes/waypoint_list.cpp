#include <cstdio>
#include <cmath>

#include "waypoint_list.h"

WaypointList::~WaypointList() {
	clear();
}

void WaypointList::add(const Waypoint *route_points, const int length) {
  for (int i = 0; i < length; i++) {
     addLast(route_points[i]);
  }
}

void WaypointList::smooth(float tolerance, float weight_data, float weight_smooth) {
  float change = tolerance;
  Point aux;

  // Create a temporary list
  DoubleList<Waypoint> org_list;

  // Pointer to current nodes
  DoubleNode<Waypoint> *sth_node;
  DoubleNode<Waypoint> *org_node;

  // Create a duplicate list
  for (DoubleNode<Waypoint> *node = first(); node != NULL; node = node->next()) {
    org_list.addLast(node->value);
  }

  // Smooth path
  while (change >= tolerance) {
    change = 0.0;

    // Assign second node to pointers
    sth_node = first();
    org_node = org_list.first();

    // Loop until second to last node
    while (sth_node != NULL) {
      // Smooth current point only if not a waypoint
      if ((sth_node != first()) && (sth_node != last()) && (!sth_node->value.fixed)) {
        // Save previous values
        aux.x = sth_node->value.location.x;
        aux.y = sth_node->value.location.y;

        // Smooth x
        sth_node->value.location.x += weight_data * (org_node->value.location.x - sth_node->value.location.x);
        sth_node->value.location.x += weight_smooth * (sth_node->prev()->value.location.x + sth_node->next()->value.location.x - (2.0 * sth_node->value.location.x));

        // Smooth y
        sth_node->value.location.y += weight_data * (org_node->value.location.y - sth_node->value.location.y);
        sth_node->value.location.y += weight_smooth * (sth_node->prev()->value.location.y + sth_node->next()->value.location.y - (2.0 * sth_node->value.location.y));

        // Calculate change
        change += std::abs(aux.x - sth_node->value.location.x);
        change += std::abs(aux.y - sth_node->value.location.y);
      }

      // Move to next node
      sth_node = sth_node->next();
      org_node = org_node->next();
    }
  }
}

void WaypointList::print() {
  int i = 1;

  for (DoubleNode<Waypoint> *node = first(); node != NULL; node = node->next()) {
    printf("%d) x: %'.4f\ty: %'.4f\tf: %d\n", i++, node->value.location.x, node->value.location.y, node->value.fixed);
  }
}
