#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

// select which pin will trigger the configuration portal when set to LOW
// ESP-01 users please note: the only pins available (0 and 2), are shared 
// with the bootloader, so always set them HIGH at power-up
#define TRIGGER_PIN 0
const char* modes[] = { "NULL", "STA", "AP", "STA+AP" };

WiFiManager wifiManager;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(3000);
  Serial.println("\n Starting");

  Serial.setDebugOutput(true);  
  Serial.println(modes[WiFi.getMode()]);
  WiFi.printDiag(Serial);

  //Local intialization. Once its business is done, there is no need to keep it around
  //reset settings - for testing
  //wifiManager.resetSettings();

  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  wifiManager.setConfigPortalTimeout(180);
  // uint8_t menu[] = {wifiManager.MENU_WIFI,wifiManager.MENU_INFO,wifiManager.MENU_PARAM,wifiManager.MENU_CLOSE};
  // wifiManager.setMenu(menu);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if(!wifiManager.autoConnect("AutoConnectAP")) {
    Serial.println("failed to connect and hit timeout");
  }

  pinMode(TRIGGER_PIN, INPUT);
}


void loop() {
  // is configuration portal requested?
  if ( digitalRead(TRIGGER_PIN) == LOW ) {
    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around

    //reset settings - for testing
    //wifiManager.resetSettings();

    //sets timeout until configuration portal gets turned off
    //useful to make it all retry or go to sleep
    //in seconds
    wifiManager.setConfigPortalTimeout(120);

    //it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration

    //WITHOUT THIS THE AP DOES NOT SEEM TO WORK PROPERLY WITH SDK 1.5 , update to at least 1.5.1
    //WiFi.mode(WIFI_STA);
    
    // disable captive portal redirection
    // wifiManager.setCaptivePortalEnable(false);
    
    if (!wifiManager.startConfigPortal("OnDemandAP")) {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
    } else {
      //if you get here you have connected to the WiFi
      Serial.println("connected...yeey :)");
    }  
  }

  // put your main code here, to run repeatedly:
}
