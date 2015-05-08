#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "debug.h"
#include "global.h"

using namespace std;

// Init debug
void debug_init() {
  //Serial.begin(BAUD_RATE);
}

/*
// Write data as binary floats to serial port
void serial_write_float(float fVal) {
	byte * bVal = (byte *) &fVal;

	for (int i = 0; i < 4; i++)
		Serial.write(bVal[i]);
}

// Write current location 
void serial_write_location() {
  serial_write_float(sense_x);
  serial_write_float(sense_y);
  serial_write_float(sense_theta);
  
  //Serial.println("");
}
*/

// Print location to debug window
void debug_print_location(const Posistion posistion) {
	printf("Location: x: %'.4f\ty: %'.4f\to: %'.4f\n", posistion.location.x, posistion.location.y, posistion.heading * (180 / PI));
}

// Print path to debug
void debug_print_path(DoubleList<Waypoint> *list) {
	int i = 1;

	// Print Header
	printf("== Path Data ==\n");
  
	for (DoubleNode<Waypoint> *node = list->first(); node != NULL; node = node->next()) {
	  	  printf("%d) x: %'.4f\ty: %'.4f\tf: %d\n", i++, node->value.location.x, node->value.location.y, node->value.fixed);
	}
}

// Print path to debug
void debug_print_rev_path(DoubleList<Waypoint> *list) {
  int i = 1;

  // Print Header
  printf("== Path Data (Rev) ==\n");
  
  for (DoubleNode<Waypoint> *node = list->last(); node != NULL; node = node->prev()) {
	  printf("%d) x: %'.4f\ty: %'.4f\tf: %d\n", i++, node->value.location.x, node->value.location.y, node->value.fixed);
  }
}

void debug_print_waypoint(DoubleNode<Waypoint> *node) {
	printf("x: %'.4f\ty: %'.4f\tf: %d\n", node->value.location.x, node->value.location.y, node->value.fixed);
}

// Print path to debug
void debug_print_pointers(DoubleList<Waypoint> *list) {
	int i = 1;

	// Print Header
	printf("== List Pointers ==\n");
  
	for (DoubleNode<Waypoint> *node = list->first(); node != NULL; node = node->next()) {
		printf("%d) p: %p\tc: %p\tn: %p\n", i++, node->prev(), node, node->next());
	}
}
