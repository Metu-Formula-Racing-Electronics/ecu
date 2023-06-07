#include "sensor.h"
#include <Arduino.h>
/* changing bits to the ASCII values         gelen mesaj 11bit*/
#include <iostream>
#include <string>
#include <bitset>
#include <bits/stdc++.h>
using namespace std;

#include "FS.h"
#include "SD.h"
#include "SPI.h"

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if(!root){
    Serial.println("Failed to open directory");
    return;
  }
  if(!root.isDirectory()){
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if(levels){
        listDir(fs, file.name(), levels -1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
     file = root.openNextFile();
  }
}
void createDir(fs::FS &fs, const char * path){
  Serial.printf("Creating Dir: %s\n", path);
  if(fs.mkdir(path)){
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}
void removeDir(fs::FS &fs, const char * path){
  Serial.printf("Removing Dir: %s\n", path);
  if(fs.rmdir(path)){
    Serial.println("Dir removed");
  } else {
    Serial.println("rmdir failed");
  }
}
void readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
}
void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}
void appendFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)){
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}
void renameFile(fs::FS &fs, const char * path1, const char * path2){
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2)) {
    Serial.println("File renamed");
  } else {
    Serial.println("Rename failed");
  }
}
void deleteFile(fs::FS &fs, const char * path){
  Serial.printf("Deleting file: %s\n", path);
  if(fs.remove(path)){
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}

// TO INITIALIZE SD CARD

void SD_setup(){
  Serial.begin(115200);
  if(!SD.begin()){
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }
  createDir(SD, "/deneme");
  readFile(SD,"/deneme.txt");
  writeFile(SD,"/deneme.txt","HELLO WORLD");
  appendFile(SD,"/deneme.txt","sdazaa\n");
}  
void SD_loop(){
  appendFile(SD,"/deneme.txt", "zaa appendledim");
  delay(1000);
}














/*
 std::string asciiOutput;
  for (size_t i = 0; i < binaryInput.length(); i += 8) {
    std::string byteString = binaryInput.substr(i, 8);
    char asciiChar = static_cast<char>(std::bitset<8>(byteString).to_ulong());
    asciiOutput.push_back(asciiChar);
  }
  return asciiOutput;
}

int main() {
  std::string binaryInput = "00000100101"; // 11-bit binary message
  std::bitset<11> bitsetInput(binaryInput);
  unsigned long identifier = bitsetInput.to_ulong() >> 3; // Extract the 11-bit identifier
  std::string dataBytes = binaryInput.substr(3); // Extract the data bytes
  std::string asciiOutput = binaryToASCII(dataBytes);
  std::cout << "Identifier: " << std::hex << identifier << std::endl;
  std::cout << "ASCII Output: " << asciiOutput << std::endl;
  return 0;
}
*/