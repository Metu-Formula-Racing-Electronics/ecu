#ifndef bamocar_can_h
#define bamocar_can_h
#include "bamocar-can.h"
#include "bamocar-registers.h"
#include <mcp_can.h>
#include <SPI.h>
class Bamocar
{
public:
  Bamocar();

  void startCAN();

  void getSpeed(char interval);
  void setSpeed(unsigned int speed);
  void setAccel(unsigned int period);
  void setDecel(unsigned int period);
  void setTorque(unsigned int torque);
  void getCurrent(char interval);
  void getCurrentDevice(char interval);
  void getMotorTemp(char interval);
  void getStatus(char interval);
  void setSoftEnable(bool enable);
  void getHardEnable(char interval);
  void setRxID (unsigned short rxID);

  void StatusRead();

private:
long unsigned int rxId = 0X181;
byte len = 3;
byte rxBuf[3];
char msgString[128]; 
 
  unsigned short m_rxID;// ID of bamocar
  unsigned short m_txID;

 
  char m_baudrate;

  byte m_data[3];
  

  void sendCAN(byte stmp[]);
};

#endif