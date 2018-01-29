#include <FS.h>          // this needs to be first, or it all crashes and burns...
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

/**************************************************************************************
 * this example shows how to set a static IP configuration for the ESP
 * although the IP shows in the config portal, the changes will revert 
 * to the IP set in the source file.
 * if you want the ability to configure and persist the new IP configuration
 * look at the FS examples, which save the config to file
 *************************************************************************************/
 
//default custom static IP
//char static_ip[16] = "10.0.1.59";
//char static_gw[16] = "10.0.1.1";
//char static_sn[16] = "255.255.255.0";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //reset settings - for testing
  //wifiManager.resetSettings();

  //set static ip
  //block1 should be used for ESP8266 core 2.1.0 or newer, otherwise use block2

  //start-block1
  //IPAddress _ip,_gw,_sn;
  //_ip.fromString(static_ip);
  //_gw.fromString(static_gw);
  //_sn.fromString(static_sn);
  //end-block1

  //start-block2
  IPAddress _ip = IPAddress(10, 0, 1, 78);
  IPAddress _gw = IPAddress(10, 0, 1, 1);
  IPAddress _sn = IPAddress(255, 255, 255, 0);
  //end-block2
  
  wifiManager.setSTAStaticIPConfig(_ip, _gw, _sn);


  //tries to connect to last known settings
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP" with password "password"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("AutoConnectAP", "password")) {
    Serial.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.restart();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");


  Serial.println("local ip");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:


}
