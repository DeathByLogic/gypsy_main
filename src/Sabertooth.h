/*
Arduino Library for SyRen/Sabertooth Packet Serial
Copyright (c) 2012 Dimension Engineering LLC
http://www.dimensionengineering.com/arduino

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE
USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef Sabertooth_h
#define Sabertooth_h   

#include <stdint.h>
#include "beagleIO.h"

class Sabertooth
{
public:
  Sabertooth(uint8_t address);
  Sabertooth(uint8_t address, beagleSerial *port);

public:
  inline uint8_t		address() const { return _address; }
  inline beagleSerial  *port   () const { return _port;    }

  void autobaud(bool dontWait = false) const;
  void command (uint8_t command, uint8_t value) const;

  static void autobaud(beagleSerial *port, bool dontWait = false);
  
public:
  void motor(int power) const;
  void motor(uint8_t motor, int power) const;
  void drive(int power) const;
  void turn (int power) const;
  void stop () const;
  
public:
  void setMinVoltage(uint8_t value) const;
  void setMaxVoltage(uint8_t value) const;
  void setBaudRate  (long baudRate) const;
  void setDeadband  (uint8_t value) const;
  void setRamping   (uint8_t value) const;
  void setTimeout   (int  milliseconds) const;
  
private:
  const uint8_t		_address;
  beagleSerial	   *_port;
};

class SabertoothPacket
{
public:
  SabertoothPacket(uint8_t command, uint8_t value);

public:
  inline uint8_t command () const { return _command; }
  inline uint8_t value   () const { return _value;   }
  
  uint8_t checksum(uint8_t address) const;
  void getBytes(uint8_t address, uint8_t bytes[4]) const;

public:
  void send(uint8_t address) const;
  void send(uint8_t address, beagleSerial *port) const;
  void send(const Sabertooth& sabertooth) const;
  
public:
  static SabertoothPacket motor(            int power);
  static SabertoothPacket motor(uint8_t motor, int power);
  static SabertoothPacket drive            (int power);
  static SabertoothPacket turn             (int power);

public:
  static SabertoothPacket setMinVoltage(uint8_t value);
  static SabertoothPacket setMaxVoltage(uint8_t value);
  static SabertoothPacket setBaudRate  (long baudRate);
  static SabertoothPacket setDeadband  (uint8_t value);
  static SabertoothPacket setRamping   (uint8_t value);
  static SabertoothPacket setTimeout   (int  milliseconds);

private:
  static SabertoothPacket throttleType(uint8_t command, int power);
  
private:
  uint8_t _command, _value;
};

#endif
