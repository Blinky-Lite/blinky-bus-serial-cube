#include "BlinkyBus.h"
#define BAUD_RATE  115200
#define commLEDPin    13
#define INCRPERIOD  1000

#define BLINKYBUSBUFSIZE  32
#define EXTRABUFSIZE      27

union BlinkyBusUnion
{
  struct
  {
    int16_t state;
    int16_t watchdog;
    int16_t led1;
    int16_t led2;
    int16_t led3;
    int16_t extra[EXTRABUFSIZE];
  };
  int16_t buffer[BLINKYBUSBUFSIZE];
} bb;

usb_serial_class port = Serial;
BlinkyBus blinkyBus(bb.buffer, BLINKYBUSBUFSIZE, port, commLEDPin);

const int led1Pin = 2;
const int led2Pin = 3;
const int led3Pin = 4;
unsigned long nowTime = 0;
unsigned long lastTime = 0;

void setLeds()
{
  digitalWrite(led1Pin, bb.led1);    
  digitalWrite(led2Pin, bb.led2);    
  digitalWrite(led3Pin, bb.led3);    
}

void setup() 
{
  
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);

  bb.state = 1; //init
  bb.watchdog = 0;
  bb.led1 = 0;
  bb.led2 = 0;
  bb.led3 = 0;
  setLeds();

  port.begin(BAUD_RATE);
  blinkyBus.start();
  nowTime = millis();
  lastTime = nowTime;
  for (int ii = 0; ii < EXTRABUFSIZE; ++ii) bb.extra[ii] = 0;
}

void loop() 
{
  blinkyBus.poll();
  setLeds();
  nowTime = millis();
  if (((nowTime - lastTime) > INCRPERIOD) || (nowTime < lastTime))
  {
    lastTime = nowTime;
    bb.watchdog = bb.watchdog + 1;
    if (bb.watchdog > 32760) bb.watchdog = 0;
  }
}
