#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager


// The extra parameters to be configured (can be either global or just in the setup)
WiFiManagerParameter mqtt_server("server", "mqtt server", NULL, 40);
WiFiManagerParameter mqtt_port("port", "mqtt port", "8080", 8);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //add all your parameters here
  wifiManager.addParameter(&mqtt_server);
  wifiManager.addParameter(&mqtt_port);
  
  //reset settings - for testing
  wifiManager.resetSettings();

  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  wifiManager.setTimeout(180);
  
  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if(!wifiManager.autoConnect("AutoConnectAP")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  } 

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  // use your parameter values as needed
  Serial.print(mqtt_server.getPlaceholder());
  Serial.print(": ");
  Serial.println(mqtt_server.getValue());

  Serial.print(mqtt_port.getPlaceholder());
  Serial.print(": ");
  Serial.println(mqtt_port.getValue());
}

void loop() {
  // put your main code here, to run repeatedly:


}
