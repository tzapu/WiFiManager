/**
 * This is a kind of unit test for DEV for now
 * It contains many of the public methods
 * 
 */
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <time.h>
#include <stdio.h>

#define USEOTA
// enable OTA
#ifdef USEOTA
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#endif

const char* modes[] = { "NULL", "STA", "AP", "STA+AP" };

unsigned long mtime = 0;


WiFiManager wm;


// TEST OPTION FLAGS
bool TEST_CP         = false; // always start the configportal, even if ap found
int  TESP_CP_TIMEOUT = 90; // test cp timeout

bool TEST_NET        = true; // do a network test after connect, (gets ntp time)
bool ALLOWONDEMAND   = true; // enable on demand
int  ONDDEMANDPIN    = 0; // gpio for button

// char ssid[] = "*************";  //  your network SSID (name)
// char pass[] = "********";       // your network password

void saveWifiCallback(){
  Serial.println("[CALLBACK] saveCallback fired");
}

//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("[CALLBACK] configModeCallback fired");
  // myWiFiManager->setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0)); 
  // Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  // Serial.println(myWiFiManager->getConfigPortalSSID());
  // 
  // esp_wifi_set_bandwidth(WIFI_IF_AP, WIFI_BW_HT20);
}

void saveParamCallback(){
  Serial.println("[CALLBACK] saveParamCallback fired");
  // wm.stopConfigPortal();
}

void bindServerCallback(){
  wm.server->on("/custom",handleRoute);
  // wm.server->on("/info",handleRoute); // you can override wm!
}

void handleRoute(){
  Serial.println("[HTTP] handle route");
  wm.server->send(200, "text/plain", "hello from user code");
}

void setup() {
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  
  // put your setup code here, to run once:
  Serial.begin(115200);
  // Serial1.begin(115200);

  // Serial.setDebugOutput(true);  
  delay(1000);

  Serial.println("\n Starting");
  // WiFi.setSleepMode(WIFI_NONE_SLEEP); // disable sleep, can improve ap stability

  Serial.println("Error - TEST");
  Serial.println("Information- - TEST");

  Serial.println("[ERROR]  TEST");
  Serial.println("[INFORMATION] TEST");  

  wm.debugPlatformInfo();

  //reset settings - for testing
  // wm.resetSettings();
  // wm.erase();  

  // setup some parameters
  WiFiManagerParameter custom_html("<p>This Is Custom HTML</p>"); // only custom html
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", "", 40);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", "", 6);
  WiFiManagerParameter custom_token("api_token", "api token", "", 16);
  WiFiManagerParameter custom_tokenb("invalid token", "invalid token", "", 0); // id is invalid, cannot contain spaces
  WiFiManagerParameter custom_ipaddress("input_ip", "input IP", "", 15,"pattern='\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}'"); // custom input attrs (ip mask)

  const char _customHtml_checkbox[] = "type=\"checkbox\""; 
  WiFiManagerParameter custom_checkbox("checkbox", "my checkbox", "T", 2, _customHtml_checkbox, WFM_LABEL_AFTER);

  // callbacks
  wm.setAPCallback(configModeCallback);
  wm.setWebServerCallback(bindServerCallback);
  wm.setSaveConfigCallback(saveWifiCallback);
  wm.setSaveParamsCallback(saveParamCallback);

  // add all your parameters here
  wm.addParameter(&custom_html);
  wm.addParameter(&custom_mqtt_server);
  wm.addParameter(&custom_mqtt_port);
  wm.addParameter(&custom_token);
  wm.addParameter(&custom_tokenb);
  wm.addParameter(&custom_ipaddress);
  wm.addParameter(&custom_checkbox);

  // set values later if you want
  custom_html.setValue("test",4);
  custom_token.setValue("test",4);

  // invert theme, dark
  wm.setDarkMode(true);

  // show scan RSSI as percentage, instead of signal stength graphic
  // wm.setScanDispPerc(true);

/*
  Set cutom menu via menu[] or vector
  const char* menu[] = {"wifi","wifinoscan","info","param","close","sep","erase","restart","exit"};
  wm.setMenu(menu,9); // custom menu array must provide length
*/

  std::vector<const char *> menu = {"wifi","wifinoscan","info","param","close","sep","erase","update","restart","exit"};
  wm.setMenu(menu); // custom menu, pass vector
  
  // wm.setParamsPage(true); // move params to seperate page, not wifi, do not combine with setmenu!

  // set STA static ip
  // wm.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
  // wm.setShowStaticFields(false);
  // wm.setShowDnsFields(false);

  // set AP static ip
  // wm.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
  // wm.setAPStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0)); 

  // set country
  // setting wifi country seems to improve OSX soft ap connectivity, 
  // may help others as well, default is CN which has different channels
  wm.setCountry("US"); 

  // set Hostname
  wm.setHostname("WIFIMANAGERTESTING");

  // set custom channel
  // wm.setWiFiAPChannel(13);
  
  // set AP hidden
  // wm.setAPHidden(true);

  // show password publicly in form
  // wm.setShowPassword(true);

  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep in seconds
  wm.setConfigPortalTimeout(120);
  
  // set connection timeout
  // wm.setConnectTimeout(20);

  // set wifi connect retries
  // wm.setConnectRetries(2);

  // connect after portal save toggle
  wm.setSaveConnect(false); // do not connect, only save

  // show static ip fields
  // wm.setShowStaticFields(true);
  
  // wm.startConfigPortal("AutoConnectAP", "password");
  
  // This is sometimes necessary, it is still unknown when and why this is needed but it may solve some race condition or bug in esp SDK/lib
  // wm.setCleanConnect(true); // disconnect before connect, clean connect
  
  wm.setBreakAfterConfig(true); // needed to use saveWifiCallback

  // set custom webserver port, automatic captive portal does not work with custom ports!
  // wm.setHttpPort(8080);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  
  wifiInfo();

  if(!wm.autoConnect("WM_AutoConnectAP","12345678")) {
    Serial.println("failed to connect and hit timeout");
  }
  else if(TEST_CP) {
    // start configportal always
    delay(1000);
    Serial.println("TEST_CP ENABLED");
    wm.setConfigPortalTimeout(TESP_CP_TIMEOUT);
    wm.startConfigPortal("WM_ConnectAP");
  }
  else {
    //if you get here you have connected to the WiFi
     Serial.println("connected...yeey :)");
  }
  
  wifiInfo();
  pinMode(ONDDEMANDPIN, INPUT_PULLUP);

  #ifdef USEOTA
    ArduinoOTA.begin();
  #endif

}

void wifiInfo(){
  WiFi.printDiag(Serial);
  Serial.println("SAVED: " + (String)wm.getWiFiIsSaved() ? "YES" : "NO");
  Serial.println("SSID: " + (String)wm.getWiFiSSID());
  Serial.println("PASS: " + (String)wm.getWiFiPass());
}

void loop() {

  #ifdef USEOTA
  ArduinoOTA.handle();
  #endif
  // is configuration portal requested?
  if (ALLOWONDEMAND && digitalRead(ONDDEMANDPIN) == LOW ) {
    delay(100);
    if ( digitalRead(ONDDEMANDPIN) == LOW ){
      Serial.println("BUTTON PRESSED");
      wm.setConfigPortalTimeout(140);
      wm.setParamsPage(false); // move params to seperate page, not wifi, do not combine with setmenu!

      // disable captive portal redirection
      // wm.setCaptivePortalEnable(false);
      
      if (!wm.startConfigPortal("OnDemandAP","12345678")) {
        Serial.println("failed to connect and hit timeout");
        delay(3000);
      }
    }
    else {
      //if you get here you have connected to the WiFi
      Serial.println("connected...yeey :)");
      getTime();
    }
  }

  // every 10 seconds
  if(millis()-mtime > 10000 ){
    if(WiFi.status() == WL_CONNECTED){
      getTime();
    }
    else Serial.println("No Wifi");  
    mtime = millis();
  }
  // put your main code here, to run repeatedly:
  delay(100);
}

void getTime() {
  int tz           = -5;
  int dst          = 0;
  time_t now       = time(nullptr);
  unsigned timeout = 5000; // try for timeout
  unsigned start   = millis();
  configTime(tz * 3600, dst * 3600, "pool.ntp.org", "time.nist.gov");
  Serial.print("Waiting for NTP time sync: ");
  while (now < 8 * 3600 * 2 ) { // what is this ?
    delay(100);
    Serial.print(".");
    now = time(nullptr);
    if((millis() - start) > timeout){
      Serial.println("\n[ERROR] Failed to get NTP time.");
      return;
    }
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo); // @NOTE doesnt work in esp2.3.0
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

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
