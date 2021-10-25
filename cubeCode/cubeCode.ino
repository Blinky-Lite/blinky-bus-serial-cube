#include "ModbusRtu.h"
#define commLEDPin    13

boolean commLED = false;
uint16_t msgCnt = 0;
/**
 *  Modbus object declaration
 *  u8id : node id = 0 for master, = 1..247 for slave
 *  port : serial port
 *  u8txenpin : 0 for RS-232 and USB-FTDI 
 *               or any pin number > 1 for RS-485
 */
Modbus slave(1,Serial,0); // this is slave @1 and RS-232 or USB-FTDI
#define MODBUSBUFSIZE  4

union ModbusUnion
{
  struct
  {
    uint16_t initCube;
    uint16_t led1;
    uint16_t led2;
    uint16_t led3;
  };
  uint16_t modbusBuffer[MODBUSBUFSIZE];
} mb;

int led1Pin = 2;
int led2Pin = 3;
int led3Pin = 4;

void setLeds()
{
  digitalWrite(led1Pin, mb.led1);    
  digitalWrite(led2Pin, mb.led2);    
  digitalWrite(led3Pin, mb.led3);    
}

void setup() 
{
  pinMode(commLEDPin,    OUTPUT);
  digitalWrite(commLEDPin, commLED);    

  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);

  mb.initCube = 1; //tell tray we need an init
  mb.led1 = 0;
  mb.led2 = 0;
  mb.led3 = 0;
  setLeds();
  for (int ii = 0; ii < 10; ++ii)
  {
    commLED = !commLED;
    digitalWrite(commLEDPin, commLED);
    delay(300);    
  }
  commLED = true;
  digitalWrite(commLEDPin, commLED);
 
  Serial.begin( 19200 ); // baud-rate at 19200
  slave.start();
}

void loop() 
{
  slave.poll( mb.modbusBuffer, MODBUSBUFSIZE );
  checkComm();
  setLeds();
}
void checkComm()
{
  uint16_t numMessages;
  numMessages = slave.getInCnt();
  if (numMessages != msgCnt)
  {
    msgCnt = numMessages;
    commLED = !commLED;
    digitalWrite(commLEDPin, commLED);    
  }
  
}
