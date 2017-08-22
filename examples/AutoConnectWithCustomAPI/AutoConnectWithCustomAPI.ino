#include <ESP8266WiFi.h>       //https://github.com/esp8266/Arduino        

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>       //https://github.com/tzapu/WiFiManager

#include <ArduinoJson.h>       //https://github.com/bblanchon/ArduinoJson

#define LED D0

void callback(WiFiManager *wifiManager) {
  //do the work
  digitalWrite(LED, !digitalRead(LED));

  //make the json response
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["pin_state"] = digitalRead(LED);
  char buffer[json.measureLength() + 1];
  json.printTo(buffer, sizeof(buffer));

  //send the response back
  wifiManager->sendResponse(200, "application/json", buffer);
}

void setup() {
    // put your setup code here, to run once
    Serial.begin(115200);
    pinMode(LED, OUTPUT);

    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //reset saved settings
    wifiManager.resetSettings();

    //register the API endpoint, 
    //so when the configuration portal is active 
    //this will also be registered
    wifiManager.addAPIEndpoint("/pin/1", callback);

    //do the usual auto connection procedure
    wifiManager.autoConnect();
    
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
}

void loop() {
}