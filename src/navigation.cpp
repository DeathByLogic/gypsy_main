#include <stdlib.h>

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

// Smooth the path
void smooth_path(DoubleList<Waypoint> *sth_list, float tolerance, float weight_data, float weight_smooth) {
  float change = tolerance;
  Point aux;
  
  // Create a temporary list
  DoubleList<Waypoint> org_list;
 
  // Pointer to current nodes
  DoubleNode<Waypoint> *sth_node;
  DoubleNode<Waypoint> *org_node;

  // Create a duplicate list
  for (DoubleNode<Waypoint> *node = sth_list->first(); node != NULL; node = node->next()) {
    org_list.addLast(node->value);
  }

  // Smooth path
  while (change >= tolerance) {
    change = 0.0;
    
    // Assign second node to pointers
    sth_node = sth_list->first();
    org_node = org_list.first();
    
    // Loop until second to last node
    while (sth_node != NULL) {
      // Smooth current point only if not a waypoint
      if ((sth_node != sth_list->first()) && (sth_node != sth_list->last()) && (!sth_node->value.fixed)) {
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
        change += abs(aux.x - sth_node->value.location.x);
        change += abs(aux.y - sth_node->value.location.y);
      }
      
      // Move to next node
      sth_node = sth_node->next();
      org_node = org_node->next();
    }
  }
}

void fill_route(DoubleList<Waypoint> *list, const Waypoint *route_points, const int length) {
  for (int i = 0; i < length; i++) {
     list->addLast(route_points[i]);
  }
}
