
String at = "AT";
String deviceName = "AT+NAMEBB01";
String password = "AT+PIN5822";
String baudRate = "AT+BAUD4";
// Command Reference
// http://www.martyncurrey.com/hc-06-hc01-comv2-0/
/*
1——— 1200
2——— 2400
3——— 4800
4——— 9600
5——— 19200
6——— 38400
7——— 57600
8——— 115200
 */
char commandChar[32];


void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  sendATCommand(at); 
  sendATCommand(deviceName);
  sendATCommand(password);
  sendATCommand(baudRate);
}

void loop()
{
}
void sendATCommand(String ATcommand)
{
  int commandLength = ATcommand.length();
  for (int ii = 0; ii < commandLength; ++ii)
  {
    Serial1.write(ATcommand.charAt(ii)); delay(50);
  }
  delay(500);
  Serial.print("AT response: ");
  while (Serial1.available())
  { 
    Serial.write(Serial1.read());
  }
  Serial.println();
}
