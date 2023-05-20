#include "bamocar-can.h"

CANmsg::CANmsg(long id, uint8_t length) : _can_id(id), _datalength(length) {
    if (length > 8) _datalength = 0;
    id > 0x7FF ? _is_ext = true : _is_ext = false; 
}

uint8_t const CANmsg::get_length() {return _datalength;}
uint8_t const CANmsg::get_data(uint8_t pos) {return _data[pos];}
uint8_t* const CANmsg::get_data() {return _data;}

long const CANmsg::get_id() {return _can_id;}
bool const CANmsg::is_ext() {return _is_ext;}

void CANmsg::reset_data(){
    memset(_data,0,8);
}

int CANmsg::write_data(uint8_t dlc, uint8_t* buffer) {
    reset_data();
    if (dlc > 8) return 0;
     memcpy(_data, buffer, dlc);
     return 1;
}

int CANmsg::write_data(long data){
    reset_data();
    
    //a function to assign _data[8] of CANmsg directly via a single number value
    //to be implemented
    
    /*for (int i = 0; i < 8; i++){
            ???
            _data[i] = (data >> 8*i) & 0xFF;

    }*/
}

int CANmsg::write_to_pos(uint8_t data, uint8_t pos){
    reset_data();
    if (pos > 8) return 0;
    _data[pos-1] = data;
    return 1;
}


int CANmsg::set_length(uint8_t dlc){
    if (dlc>8) return 0;
    _datalength = dlc;
    return 1;
}
int CANmsg::set_id(long id){
    if (id < 0 || id > 0x1FFFFFFF) return 0;

    if (id > 0x7FF) _is_ext = true;
    _can_id = id;

    return 1;
}

int CANmsg_send(CANmsg& msgframe, bool rtr = false) {
    if(!msgframe.is_ext()) CAN.beginPacket(msgframe.get_id(),msgframe.get_length(), rtr);
    else CAN.beginExtendedPacket(msgframe.get_id(),msgframe.get_length(), rtr);

    CAN.write(msgframe.get_data(), msgframe.get_length());
    
    return CAN.endPacket();
}

void CANmsg_get(CANmsg& msgframe) {
    int packetSize = CAN.parsePacket();

    if (packetSize || CAN.packetId() != -1) {

    

    if (CAN.packetExtended()) {
        //extended packet arrived
    }

    if (CAN.packetRtr()) {
      // Remote transmission request, packet contains no data
      
    }

    else 
        while (CAN.available()) {
            int pos = 1;
            msgframe.write_to_pos(CAN.read(),pos++);
        }

    }
}

