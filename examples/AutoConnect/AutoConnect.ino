#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

//this intitilises the library, parameter is eeprom start
WiFiManager wifi;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    
    
    //WiFiManager
    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
    wifi.autoConnect("AutoConnectAP");
    
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
}

void loop() {
    // put your main code here, to run repeatedly:
    
}