/**
 * Basic example using LittleFS to store data
 */

#include <Arduino.h>
#include <LittleFS.h>
#include <FS.h>

String readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if(!file || file.isDirectory()){
    Serial.println("- empty file or failed to open file");
    return String();
  }
  Serial.println("- read from file:");
  String fileContent;
  while(file.available()){
    fileContent+=String((char)file.read());
  }
  file.close();
  Serial.println(fileContent);
  return fileContent;
}
void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.close();
}

int data = 4; 

#include <WiFiManager.h>
#define TRIGGER_PIN 2
int timeout = 120; // seconds to run for

void setup() {
if (!LittleFS.begin()) { //to start littlefs
Serial.println("LittleFS mount failed");
return;
}
data = readFile(LittleFS, "/data.txt").toInt();
WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP  
  // put your setup code here, to run once:
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  WiFiManager wm;
  //wm.resetSettings();
  bool res;
  res = wm.autoConnect("Setup");
  if(!res) {
     Serial.println("Failed to connect");
     // ESP.restart();
  } 

}

void loop() {
if ( digitalRead(TRIGGER_PIN) == LOW) {
    WiFiManager wm;    
    //wm.resetSettings();
    wm.setConfigPortalTimeout(timeout);
    if (!wm.startConfigPortal("Sharmander")) {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
      ESP.restart();
      delay(5000);
    }
    Serial.println("connected...yeey :)");
}
}