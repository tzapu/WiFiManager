#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "WiFiManager.h"          //https://github.com/tzapu/WiFiManager


void load(WiFiManager &wifiManager) {
  //clean FS, for testing
//  SPIFFS.format();

  //read configuration from FS json
  Serial.println("mounting FS...");
  if (SPIFFS.begin()) {
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file...");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial); // debug output to console
        if (json.success()) {
          Serial.println("\nparsed json");
          if ( json["Networks"].is<JsonArray>() ) {
            for (int i=0; i < json["Networks"].size(); i++) {
              auto obj = json["Networks"][i];

              // add existing networks and credentials
              wifiManager.addAP(obj["SSID"], obj["Password"]);
            }
          }
        } else {
          Serial.println("failed to load json config");
          return;
        }
      }
    }
    SPIFFS.end();
  } else {
    Serial.println("failed to mount FS -> format");
    SPIFFS.format();
  }
}

void save(WiFiManager &wifiManager) {
  //save the custom parameters to FS
  if (SPIFFS.begin()) {
    Serial.println("saving config...");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    JsonArray& networks = json.createNestedArray("Networks");

    // encode known networks to JSON
    for (int i = 0; auto ap = wifiManager.getAP(i); i++ ) {
      JsonObject& obj = jsonBuffer.createObject();
      obj["SSID"] = ap->ssid;
      obj["Password"] = ap->pass;
      networks.add(obj);
    }

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
      return;
    }

    json.printTo(Serial); Serial.println(); // debug output to console
    json.printTo(configFile);
    configFile.close();
    SPIFFS.end();
  }
}

//flag for saving data
bool shouldSaveConfig = false;
//callback notifying us of the need to save config
void saveConfigCallback () {
  shouldSaveConfig = true;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //reset settings - for testing
  wifiManager.resetSettings();

  // load known access points
  load(wifiManager);

  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  
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

  // save known access points
  if ( shouldSaveConfig )
    save(wifiManager);

  //if you get here you have connected to the WiFi
  Serial.println();
  Serial.print("connected to: "); Serial.println(WiFi.SSID());
  Serial.print("local ip: "); Serial.println(WiFi.localIP());

}

void loop() {
  // put your main code here, to run repeatedly:

}
