/*
 * crc.cpp
 *
 *  Created on: Apr 11, 2015
 *      Author: daniel
 */

#include <unistd.h>

#include "crc.h"

bool verify_crc(char *str, size_t size, const unsigned short crc) {
	unsigned short calc_crc = 0x0000;

	// Calculate the CRC of the data
	for (unsigned int i = 0; i < size; i++) {
		calc_crc = crc_xmodem_update(calc_crc, str[i]);
	}

	// Check if calculated CRC matches supplied
	if (calc_crc == crc) {
		return true;
	} else {
		return false;
	}
}

unsigned short crc_xmodem_update (unsigned short crc, char data) {
	crc = crc ^ ((unsigned short)data << 8);

	for (int i=0; i< 8; i++) {
		if (crc & 0x8000)
			crc = (crc << 1) ^ 0x1021;
		else
			crc <<= 1;
	}

	return crc;
}
