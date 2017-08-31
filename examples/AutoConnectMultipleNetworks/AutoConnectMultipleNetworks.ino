#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "WiFiManager.h"          //https://github.com/tzapu/WiFiManager


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //reset settings - for testing
  wifiManager.resetSettings();

  // add known access point credentials
  // these settings will be used if the connection to the last connected network fails
  wifiManager.addAP("myAP", "myPassword");
  wifiManager.addAP("guest"); // or without password


  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if(!wifiManager.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  } 

  //if you get here you have connected to the WiFi
  Serial.print("connected to: "); Serial.println(WiFi.SSID());
  Serial.print("local ip: "); Serial.println(WiFi.localIP());

  // display all known access points
  Serial.println("saved access points:");
  for (int i = 0; auto ap = wifiManager.getAP(i); i++ ) {
    Serial.println("  " + ap->ssid + ", " + ap->pass);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
