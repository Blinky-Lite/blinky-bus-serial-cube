#define MAXBUFFERSIZE 32
struct ReceiveData
{
    uint8_t command;
    uint8_t address;
    int16_t value;
};

class BlinkyBus
{
    private:
        Stream *port; //!< Pointer to Stream class object (Either HardwareSerial or SoftwareSerial)
        ReceiveData receiveData;
        int16_t sendBufferData[MAXBUFFERSIZE];
        void sendBuffer(int16_t *regs, uint8_t u8size);
        boolean commLED;
        int commLEDPin = -1;


    public:
        BlinkyBus(Stream& port, int commLEDPin);
        void poll(int16_t *regs, uint8_t u8size);
        void start();

};

BlinkyBus::BlinkyBus(Stream& port, int commLEDPin)
{
    this->port = &port;
    this->commLEDPin = commLEDPin;
    this->commLED = false;
}

void BlinkyBus::start()
{
  if (commLEDPin > 1)
  {
    pinMode(commLEDPin,    OUTPUT);
    digitalWrite(commLEDPin, commLED);    
    for (int ii = 0; ii < 10; ++ii)
    {
      commLED = !commLED;
      digitalWrite(commLEDPin, commLED);
      delay(100);    
    }
    commLED = false;
    digitalWrite(commLEDPin, commLED);
  }

}

void BlinkyBus::poll(int16_t *regs, uint8_t u8size)
{

    if(port->available() > 0)
    { 
        port->readBytes((uint8_t*)&receiveData, 4);
/*
        Serial.print(receiveData.command);
        Serial.print(", ");
        Serial.print(receiveData.address);
        Serial.print(", ");
        Serial.println(receiveData.value);
*/
        if (receiveData.command == 1)
        {
            if ((receiveData.address >= 0) && (receiveData.address < u8size))
            {
                regs[receiveData.address] = receiveData.value;
            }
        }
        sendBuffer(regs,u8size);
        if (commLEDPin > 1)
        {
          commLED = true;
          digitalWrite(commLEDPin, commLED); 
          delay(50);   
          commLED = false;
          digitalWrite(commLEDPin, commLED);    
        }
        
    }
}
void BlinkyBus::sendBuffer(int16_t *regs, uint8_t u8size)
{
    for (int ii = 0; ii < MAXBUFFERSIZE; ++ii) sendBufferData[ii] = 0;
    for (int ii = 0; ii < u8size; ++ii)        sendBufferData[ii] = regs[ii];
    sendBufferData[MAXBUFFERSIZE - 1] = (int16_t) u8size;
    Serial1.write((uint8_t*)sendBufferData, MAXBUFFERSIZE * 2);
    Serial1.flush();
}
