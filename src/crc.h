/*
 * crc.h
 *
 *  Created on: Apr 11, 2015
 *      Author: daniel
 */

#ifndef CRC_H_
#define CRC_H_

bool verify_crc(char *str, size_t size, unsigned short crc);
unsigned short crc_xmodem_update (unsigned short crc, char data);

#endif /* CRC_H_ */
