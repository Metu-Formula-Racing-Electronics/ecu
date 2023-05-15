#include <Arduino.h>
#if defined(ARDUINO_ARCH_RP2040)

#else

#ifdef ESP32
  #include <WiFi.h>
  #include "SPIFFS.h"

#endif
#include "AudioFileSourceSPIFFS.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"

// To run, set your ESP8266 build to 160MHz, and include a SPIFFS of 512KB or greater.
// Use the "Tools->ESP8266/ESP32 Sketch Data Upload" menu to write the MP3 to SPIFFS
// Then upload the sketch normally.  

// pno_cs from https://ccrma.stanford.edu/~jos/pasp/Sound_Examples.html


AudioGeneratorMP3 *mp3;
AudioFileSourceSPIFFS *file;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;


// Called when a metadata event occurs (i.e. an ID3 tag, an ICY block, etc.
void mp3_MDCallback(void *cbData, const char *type, bool isUnicode, const char *string)
{
  (void)cbData;
  Serial.printf("ID3 callback for: %s = '", type);

  if (isUnicode) {
    string += 2;
  }
  
  while (*string) {
    char a = *(string++);
    if (isUnicode) {
      string++;
    }
    Serial.printf("%c", a);
  }
  Serial.printf("'\n");
  Serial.flush();
}


void mp3_setup()
{
  WiFi.mode(WIFI_OFF); 
  Serial.begin(115200);
  SPIFFS.begin();
  Serial.printf("Sample MP3 playback begins...\n");

  audioLogger = &Serial;
  file = new AudioFileSourceSPIFFS("soundfile.mp3");
  id3 = new AudioFileSourceID3(file);
  id3->RegisterMetadataCB(mp3_MDCallback, (void*)"ID3TAG");
  out = new AudioOutputI2S(0,1);
  mp3 = new AudioGeneratorMP3();
  mp3->begin(id3, out);
}

void mp3_loop()
{
  if (mp3->isRunning()) {
    if (mp3->loop()) mp3->stop();
  } else {
    Serial.printf("MP3 done\n");
  }
}
#endif
