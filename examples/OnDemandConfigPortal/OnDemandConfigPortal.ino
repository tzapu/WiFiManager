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
  Serial.setDebugOutput(true);  
  delay(3000);
  Serial.println("\n Starting");

  // Serial.println(modes[WiFi.getMode()]);
  // WiFi.printDiag(Serial);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //Local intialization. Once its business is done, there is no need to keep it around
  //reset settings - for testing
  //wifiManager.resetSettings();

  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  wifiManager.setConfigPortalTimeout(20);
  wifiManager.setConnectTimeout(10);
  // wifiManager.setShowStaticFields(true);

  // uint8_t menu[] = {wifiManager.MENU_WIFI,wifiManager.MENU_INFO,wifiManager.MENU_PARAM,wifiManager.MENU_CLOSE};
  // wifiManager.setMenu(menu);

  // std::vector<WiFiManager::menu_page_t> menu = {wifiManager.MENU_WIFI,wifiManager.MENU_INFO,wifiManager.MENU_PARAM,wifiManager.MENU_CLOSE,wifiManager.MENU_SEP,wifiManager.MENU_ERASE,wifiManager.MENU_EXIT};
  // wifiManager.setMenu(menu);

// std::vector<WiFiManager::menu_page_t> menu = {wifiManager.MENU_WIFI,wifiManager.MENU_INFO,wifiManager.MENU_PARAM,wifiManager.MENU_CLOSE,wifiManager.MENU_SEP,wifiManager.MENU_ERASE,wifiManager.MENU_EXIT};
  // wifiManager.setMenu(menu);


  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", "", 40);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", "", 6);
  WiFiManagerParameter custom_token("api_token", "api token", "", 0);
  WiFiManagerParameter custom_tokenb("invalid token", "invalid token", "", 0);

  //add all your parameters here
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_token);
  wifiManager.addParameter(&custom_tokenb);

        // MENU_WIFI       = 0,
        // MENU_WIFINOSCAN = 1,
        // MENU_INFO       = 2,
        // MENU_PARAM      = 3,
        // MENU_CLOSE      = 4,
        // MENU_RESTART    = 5,
        // MENU_EXIT       = 6,
        // MENU_ERASE      = 7,
        // MENU_SEP        = 8

  // const char* menu[] = {"wifi","wifinoscan","info","param","close","sep","erase","restart","exit"};
  // wifiManager.setMenu(menu,9);

  std::vector<const char *> menu = {"wifi","wifinoscan","info","param","close","sep","erase","restart","exit"};
  wifiManager.setMenu(menu);
  
  //set static ip
  wifiManager.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
  // wifiManager.setShowStaticFields(false);
  // wifiManager.setShowDnsFields(false);

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
