#include <Arduino.h>

class musicPlayer{
public:
    musicPlayer(HardwareSerial *serial);
    void playFirst();
    void musicPause();
    void play();
    void playNext();
    void playPrevious();
    void setVolume(int volume);

    void execute_CMD(uint8_t CMD, uint8_t Par1, uint8_t Par2);

private:
    HardwareSerial *_serial;
};