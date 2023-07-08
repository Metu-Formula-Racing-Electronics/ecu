#include <DFPlayer.h>

# define Start_Byte 0x7E
# define Version_Byte 0xFF
# define Command_Length 0x06
# define End_Byte 0xEF
# define Acknowledge 0x00
# define ACTIVATED LOW

musicPlayer::musicPlayer(HardwareSerial *serial = &Serial2){
    _serial = serial;
}

void musicPlayer::playFirst() // İlk mp3 sırası kodu
{
    execute_CMD(0x3F, 0, 0);
    setVolume(20);
    execute_CMD(0x11,0,1);
}

void musicPlayer::musicPause() // durdurma kodu
{
    execute_CMD(0x0E,0,0);
}

void musicPlayer::play() // çalma kodu
{
    execute_CMD(0x0D,0,1);
}

void musicPlayer::playNext() // sonraki mp3 kodu
{
    execute_CMD(0x01,0,1);
}
void musicPlayer::playPrevious() // önceki mp3 kodu
{
    execute_CMD(0x02,0,1);
}

void musicPlayer::setVolume(int volume)
{
    execute_CMD(0x06, 0, volume); // ses şiddeti
}

void musicPlayer::execute_CMD(uint8_t CMD, uint8_t Par1, uint8_t Par2){
    // doğrulama byte hesabı (2 bytes)
    uint16_t checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
    // Build the command line
    uint8_t Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,\
    Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte};
    //Send the command line to the module
    for (uint8_t k=0; k<10; k++){
        _serial->write( Command_line[k]);
    }
}