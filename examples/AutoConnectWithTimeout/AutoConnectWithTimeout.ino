#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <EEPROM.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager


//this intitilises the library, parameter is eeprom start



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifi;

  //reset settings - for testing
  //wifi.resetSettings();

  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  wifi.setTimeout(180);
  
  //fetches ssid and pass from eeprom and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if(!wifi.autoConnect("AutoConnectAP")) {
    Serial.println("failed to connect and hit timeout");
    delay(5000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  } 

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
 
}

void loop() {
  // put your main code here, to run repeatedly:

}
