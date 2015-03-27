#ifndef _DEBUG_H
#define _DEBUG_H

// Includes
#include "navigation.h"

// Function Prototypes
void debug_init();
//void serial_write_float(float);
//void serial_write_location();
void debug_print_location(const Posistion);
void debug_print_path(DoubleList<Waypoint> *);
void debug_print_rev_path(DoubleList<Waypoint> *);
void debug_print_pointers(DoubleList<Waypoint> *);
void debug_print_waypoint(DoubleNode<Waypoint> *);

// Constant Definitions

#endif
