/**
 * This is a kind of unit test for DEV for now
 */
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

#define TRIGGER_PIN 0
const char* modes[] = { "NULL", "STA", "AP", "STA+AP" };

WiFiManager wm;

char ssid[] = "*************";  //  your network SSID (name)
char pass[] = "********";       // your network password

void debugchipid(){
  // WiFi.mode(WIFI_STA);
  // WiFi.printDiag(Serial);
  // Serial.println(modes[WiFi.getMode()]);
  
  // ESP.eraseConfig();
  // wm.resetSettings();
  // wm.erase(true);
  WiFi.mode(WIFI_AP);
  // WiFi.softAP();
  WiFi.enableAP(true);
  delay(500);
  // esp_wifi_start();
  delay(1000);
  WiFi.printDiag(Serial);
  delay(60000);
  ESP.restart();

  // AP esp_267751
  // 507726A4AE30
  // ESP32 Chip ID = 507726A4AE30
}

void saveCallback(){
  Serial.println("saveCallback fired");
}


//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("configModeCallback fired");
  // myWiFiManager->setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0)); 
  // Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  // Serial.println(myWiFiManager->getConfigPortalSSID());
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setDebugOutput(true);  
  // delay(3000);
  Serial.println("\n Starting");
  // WiFi.setSleepMode(WIFI_NONE_SLEEP);
  
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  // WiFiManager wm;

  wm.debugPlatformInfo();

  //Local intialization. Once its business is done, there is no need to keep it around
  //reset settings - for testing
  // wm.resetSettings();
  // wm.erase();
  
  wm.setClass("invert");
  wm.setAPCallback(configModeCallback);

  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  // wm.setConfigPortalTimeout(600);
  // wm.setConnectTimeout(5);
  // wm.setShowStaticFields(true);

  // uint8_t menu[] = {wm.MENU_WIFI,wm.MENU_INFO,wm.MENU_PARAM,wm.MENU_CLOSE};
  // wm.setMenu(menu);

  // std::vector<WiFiManager::menu_page_t> menu = {wm.MENU_WIFI,wm.MENU_INFO,wm.MENU_PARAM,wm.MENU_CLOSE,wm.MENU_SEP,wm.MENU_ERASE,wm.MENU_EXIT};
  // wm.setMenu(menu);

// std::vector<WiFiManager::menu_page_t> menu = {wm.MENU_WIFI,wm.MENU_INFO,wm.MENU_PARAM,wm.MENU_CLOSE,wm.MENU_SEP,wm.MENU_ERASE,wm.MENU_EXIT};
  // wm.setMenu(menu);


  WiFiManagerParameter custom_html("<p>This Is Custom HTML</p>");
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", "", 40);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", "", 6);
  WiFiManagerParameter custom_token("api_token", "api token", "", 16);
  WiFiManagerParameter custom_tokenb("invalid token", "invalid token", "", 0); // id is invalid, cannot contain spaces
  WiFiManagerParameter custom_ipaddress("input_ip", "input IP", "", 15,"pattern='\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}'");

  // callbacks
  wm.setSaveConfigCallback(saveCallback);

  //add all your parameters here
  wm.addParameter(&custom_html);
  wm.addParameter(&custom_mqtt_server);
  wm.addParameter(&custom_mqtt_port);
  wm.addParameter(&custom_token);
  wm.addParameter(&custom_tokenb);
  wm.addParameter(&custom_ipaddress);

  custom_html.setValue("test",4);
  custom_token.setValue("test",4);

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
  // wm.setMenu(menu,9);

  std::vector<const char *> menu = {"wifi","wifinoscan","info","param","close","sep","erase","restart","exit"};
  wm.setMenu(menu);
  
  // set static sta ip
  // wm.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
  // wm.setShowStaticFields(false);
  // wm.setShowDnsFields(false);

  // set static ip
  // wm.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
  // wm.setAPStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0)); 

  // WiFi.mode(WIFI_STA);
  // const wifi_country_t COUNTRY_US{"US",1,11,WIFI_COUNTRY_POLICY_AUTO};
  // const wifi_country_t COUNTRY_CN{"CN",1,13,WIFI_COUNTRY_POLICY_AUTO};
  // const wifi_country_t COUNTRY_JP{"JP",1,14,WIFI_COUNTRY_POLICY_AUTO};
  // esp_wifi_set_country(&COUNTRY_US);

  // wm.setCountry("US");
  
  wm.setConfigPortalTimeout(120);
  // wm.startConfigPortal("AutoConnectAP", "password");

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  
  if(!wm.autoConnect("AutoConnectAP")) {
    Serial.println("failed to connect and hit timeout");
  }
  else {
    // start configportal always
    // wm.setConfigPortalTimeout(60);
    // wm.startConfigPortal();
  }

  pinMode(TRIGGER_PIN, INPUT);
}

void loop() {
  // is configuration portal requested?
  if ( digitalRead(TRIGGER_PIN) == LOW ) {
    Serial.println("BUTTON PRESSED");
    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    //reset settings - for testing
    //wm.resetSettings();

    //sets timeout until configuration portal gets turned off
    //useful to make it all retry or go to sleep, in seconds
    wm.setConfigPortalTimeout(120);

    //it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration

    //WITHOUT THIS THE AP DOES NOT SEEM TO WORK PROPERLY WITH SDK 1.5 , update to at least 1.5.1
    //WiFi.mode(WIFI_STA);
    
    // disable captive portal redirection
    // wm.setCaptivePortalEnable(false);
    
    if (!wm.startConfigPortal("OnDemandAP")) {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
    } else {
      //if you get here you have connected to the WiFi
      Serial.println("connected...yeey :)");
    }  
  }

  // put your main code here, to run repeatedly:
  delay(5000);
}
