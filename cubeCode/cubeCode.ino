#include "BlinkyBus.h"
#define BAUD_RATE  19200
#define commLEDPin    13

#define BLINKYBUSBUFSIZE  4
union BlinkyBusUnion
{
  struct
  {
    int16_t state;
    int16_t led1;
    int16_t led2;
    int16_t led3;
  };
  int16_t buffer[BLINKYBUSBUFSIZE];
} bb;
BlinkyBus blinkyBus(bb.buffer, BLINKYBUSBUFSIZE, Serial1, commLEDPin);

int led1Pin = 2;
int led2Pin = 3;
int led3Pin = 4;

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
  bb.led1 = 0;
  bb.led2 = 0;
  bb.led3 = 0;
  setLeds();

  Serial1.begin(BAUD_RATE);
  blinkyBus.start();

}

void loop() 
{
  blinkyBus.poll();
  setLeds();
}
