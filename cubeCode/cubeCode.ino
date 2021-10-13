#include "ModbusRtu.h"

// data array for modbus network sharing
uint16_t modbusBuffer[16] = {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int ledPin[4] = {13, 2, 3, 4};

/**
 *  Modbus object declaration
 *  u8id : node id = 0 for master, = 1..247 for slave
 *  port : serial port
 *  u8txenpin : 0 for RS-232 and USB-FTDI 
 *               or any pin number > 1 for RS-485
 */
Modbus slave(1,Serial,0); // this is slave @1 and RS-232 or USB-FTDI

void setup() 
{
  for (int ii = 0; ii < 4; ++ii) pinMode(ledPin[ii], OUTPUT);

  Serial.begin( 19200 ); // baud-rate at 19200
  slave.start();
}

void loop() 
{
  slave.poll( modbusBuffer, 16 );
  for (int ii = 0; ii < 4; ++ii) digitalWrite(ledPin[ii], modbusBuffer[ii]);
}
