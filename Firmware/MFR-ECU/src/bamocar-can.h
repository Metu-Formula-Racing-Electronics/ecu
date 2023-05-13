#ifndef bamocar_can_h
#define baomcar_can_h

#include "bamocar-register.h"
#include <Arduino.h>
#include <ESP32SJA1000.h>

/*class M_data {
    public:
        M_data(uint8_t address, uint16_t m_data16) {
            data[0] = address;
            data[1] = m_data16 & 0xFF;
            data[2] = m_data16 >> 8;
            dataLength = 3;
        }

        M_data(uint8_t address, int16_t m_data16) {
            data[0] = address;
            data[1] = m_data16 & 0xFF;
            data[2] = m_data16 >> 8;
            dataLength = 3;
        }

        M_data(uint8_t address, uint32_t m_data32) {
            data[0] = address;
            data[1] = m_data32 & 0xFF;
            data[2] = (m_data32 >> 8) & 0xFF;
            data[3] = (m_data32 >> 16) & 0xFF;
            data[4] = (m_data32 >> 24) & 0xFF;
            dataLength = 5;
        }

        M_data(uint8_t address, int32_t m_data32) {
            data[0] = address;
            data[1] = m_data32 & 0xFF;
            data[2] = (m_data32 >> 8) & 0xFF;
            data[3] = (m_data32 >> 16) & 0xFF;
            data[4] = (m_data32 >> 24) & 0xFF;
            dataLength = 5;
        }

        M_data(uint8_t address, uint8_t requestedAddress, uint8_t interval) {
            data[0] = address;
            data[1] = requestedAddress;
            data[2] = interval;
            dataLength = 3;
        }

        uint8_t get_data(uint8_t position) {
            if(position >= dataLength) return 0;
            return data[position];
        }

        uint8_t length() {
            return dataLength;
        }

    protected:
        uint8_t data[5];
        uint8_t dataLength;
};*/


struct CANmsg{
    
    CANmsg(long id, uint8_t length);

    //getters
    long const get_id();
    bool const is_ext();
    uint8_t const get_length();
    uint8_t const get_data(uint8_t pos);
    uint8_t* const get_data();

    //setters
    void reset_data();
    int write_data(uint8_t dlc, uint8_t* buffer);
    int write_data(long data);
    
    int write_to_pos(uint8_t data, uint8_t pos);

    void set_ext(bool is_ext);
    int set_length(uint8_t dlc);
    int set_id(long id);

    protected:
        long _can_id;
        uint8_t _datalength;
        bool _is_ext;
        uint8_t _data[8];
};

int CANmsg_set(CANmsg& msgframe, uint8_t* buffer);

int CANmsg_send(CANmsg& msgframe);

void CANmsg_get(CANmsg& msgframe);


/*struct Bamocar_CANmsg{
        
        protected:
        Bamocar_CANmsg(uint16_t rxID, uint16_t _txID, int16_t data16);
        Bamocar_CANmsg(uint16_t rxID, uint16_t _txID, int32_t data16);
        uint16_t _rxID;
        uint16_t _txID;
        uint8_t _data[5];
        uint8_t _dataLength;

    };
*/
#endif