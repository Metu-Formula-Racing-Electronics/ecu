#include "bamocar-can.h"
#include "bamocar-registers.h"
#include "Arduino.h"

//selamlar

Bamocar::Bamocar()
{
    //sets rx address to bamocar controller default
    m_rxID = 0x201;
    m_txID = 0X181;
}

MCP_CAN CAN0(10);//SET CS PIN to 10



void Bamocar::startCAN()
{
    if(CAN0.begin(MCP_ANY,CAN_500KBPS,MCP_16MHZ)==CAN_OK)Serial.println("MCP2515 is working");//Change the transmission rate(500 in Bamocar interface) if It is not working.
    else Serial.println("Error MCP is not working");
    CAN0.setMode(MCP_NORMAL);
}

/**
 * ==========
 * Bamocar::setRxID
 * ==========
 */
void Bamocar::setRxID(unsigned short rxID){
    m_rxID = rxID;
}

/**
 * ==========
 * Bamocar::sendCAN
 * ==========
 */
void Bamocar::sendCAN(byte stmp[])
{
    
    m_data[0] = stmp[0];
    m_data[1] = stmp[1];
    m_data[2] = stmp[2]; 
    INT8U len = 0x03;
    
    byte sndStat=CAN0.sendMsgBuf(m_rxID,0,len,m_data);//change to unsigned short if it fails in mcp_2515 lib.
    
    if (sndStat ==CAN_OK)
    {
        Serial.println("Message Sent to Bamocar");
        
    }
}

/**
 * ==========
 * Bamocar::listenCAN
 * ==========
 */
void Bamocar::StatusRead()

{
    

    CAN0.readMsgBuf(&rxId,0,&len,rxBuf);
    for (byte i=0;i<len;i++){
        sprintf(msgString, " 0x%.2X", rxBuf[i]);
        Serial.print(msgString);
        }


   
}

void Bamocar::getSpeed(char interval)
{
    m_data[0] = REG_REQUEST;
    m_data[1] = REG_N_ACTUAL;
    m_data[2] = interval;
    sendCAN(m_data);
}
void Bamocar::setSpeed(unsigned int speed)
{
    m_data[0] = REG_N_CMD;
    m_data[1] = speed & 0xFF; 
    m_data[2] = speed >> 8;
    sendCAN(m_data);
}
void Bamocar::setAccel(unsigned int period)
{
    m_data[0] = REG_RAMP_ACC;
    m_data[1] = period & 0xFF; 
    m_data[2] = period >> 8;
    sendCAN(m_data);
}
void Bamocar::setDecel(unsigned int period)
{
    m_data[0] = REG_RAMP_DEC;
    m_data[1] = period & 0xFF; 
    m_data[2] = period >> 8;
    sendCAN(m_data);
}
void Bamocar::setTorque(unsigned int torque)
{
    m_data[0] = REG_TORQUE_CMD;
    m_data[1] = torque & 0xFF; 
    m_data[2] = torque >> 8;
    sendCAN(m_data);
}
void Bamocar::getCurrent(char interval)
{
    m_data[0] = REG_REQUEST;
    m_data[1] = REG_I_ACTUAL;
    m_data[2] = interval;
    sendCAN(m_data);
}
void Bamocar::getCurrentDevice(char interval)
{
    m_data[0] = REG_REQUEST;
    m_data[1] = REG_I_DEVICE;
    m_data[2] = interval;
    sendCAN(m_data);
}
void Bamocar::getMotorTemp(char interval)
{
    m_data[0] = REG_REQUEST;
    m_data[1] = REG_TEMP_MOTOR;
    m_data[2] = interval;
    sendCAN(m_data);
}
void Bamocar::getStatus(char interval)
{
    m_data[0] = REG_REQUEST;
    m_data[1] = REG_STATUS;
    m_data[2] = interval;
    sendCAN(m_data);
}

void Bamocar::setSoftEnable(bool enable)
{
    m_data[0] = REG_ENABLE;
    if (enable){
        m_data[1] = 0x00;
    }
    else{
        m_data[1] = 0x04;
    }
    m_data[2] = 0x00;
    sendCAN(m_data);
}
void Bamocar::getHardEnable(char interval)
{
    m_data[0] = REG_REQUEST;
    m_data[1] = 0xE8; //consider defining a register
    m_data[2] = interval;
    sendCAN(m_data);
}
