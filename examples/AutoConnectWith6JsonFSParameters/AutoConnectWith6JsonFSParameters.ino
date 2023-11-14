#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson


//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  /*
    //edit the following json values using https://arduinojson.org/v6/assistant/ and paste it below. No more edits are necessary
    // DON'T change json keys, only values
{
  "commentEditJson": "use https://bit.ly/343wNw1",
  "commentArduinoJson": "use https://bit.ly/3oH4F9S",
  "customParam1": {
    "name/id": "server",
    "placeholder": "mqtt server",
    "defaultVar": "mqtt",
    "defaultValue": "cloudmqtt.com",
    "commentCharLength": "use https://bit.ly/3gFQqiB",
    "maxCharLength": 40
  },
  "customParam2": {
    "name/id": "port",
    "placeholder": "mqtt port",
    "defaultVar": "mqtt_port",
    "defaultValue": "8080",
    "maxCharLength": 6
  },
  "customParam3": {
    "name/id": "blynk",
    "placeholder": "blynk token",
    "defaultVar": "blynk_token",
    "defaultValue": "YOUR_BLYNK_TOKEN",
    "maxCharLength": 34
  },
  "customParam4": {
    "name/id": "apiKey",
    "placeholder": "api key",
    "defaultVar": "apiKey",
    "defaultValue": "YOUR_API_KEY",
    "maxCharLength": 40
  },
  "customParam5": {
    "name/id": "tempThreshold",
    "placeholder": "temperature threshold",
    "defaultVar": "tempThres",
    "defaultValue": "30.0",
    "maxCharLength": 5
  },
  "customParam6": {
    "name/id": "sampleInterval",
    "placeholder": "sample interval seconds",
    "defaultVar": "sampleIntervalsec",
    "defaultValue": "300",
    "maxCharLength": 4
  }
}
  */

  //paste the result here
  const char* json = "{\"commentEditJson\":\"use https://bit.ly/343wNw1\",\"commentArduinoJson\":\"use https://bit.ly/3oH4F9S\",\"customParam1\":{\"name/id\":\"server\",\"placeholder\":\"mqtt server\",\"defaultVar\":\"mqtt\",\"defaultValue\":\"cloudmqtt.com\",\"commentCharLength\":\"use https://bit.ly/3gFQqiB\",\"maxCharLength\":40},\"customParam2\":{\"name/id\":\"port\",\"placeholder\":\"mqtt port\",\"defaultVar\":\"mqtt_port\",\"defaultValue\":\"8080\",\"maxCharLength\":6},\"customParam3\":{\"name/id\":\"blynk\",\"placeholder\":\"blynk token\",\"defaultVar\":\"blynk_token\",\"defaultValue\":\"YOUR_BLYNK_TOKEN\",\"maxCharLength\":34},\"customParam4\":{\"name/id\":\"apiKey\",\"placeholder\":\"api key\",\"defaultVar\":\"apiKey\",\"defaultValue\":\"YOUR_API_KEY\",\"maxCharLength\":40},\"customParam5\":{\"name/id\":\"tempThreshold\",\"placeholder\":\"temperature threshold\",\"defaultVar\":\"tempThres\",\"defaultValue\":\"30.0\",\"maxCharLength\":5},\"customParam6\":{\"name/id\":\"sampleInterval\",\"placeholder\":\"sample interval seconds\",\"defaultVar\":\"sampleIntervalsec\",\"defaultValue\":\"300\",\"maxCharLength\":5}}";

  DynamicJsonDocument doc(1536);
  deserializeJson(doc, json);

  const char* commentEditJson = doc["commentEditJson"]; // "use https://bit.ly/343wNw1"
  const char* commentArduinoJson = doc["commentArduinoJson"]; // "use https://bit.ly/3oH4F9S"

  JsonObject customParam1 = doc["customParam1"];
  const char* customParam1_name_id = customParam1["name/id"]; // "server"
  const char* customParam1_placeholder = customParam1["placeholder"]; // "mqtt server"
  const char* customParam1_defaultVar = customParam1["defaultVar"]; // "mqtt"
  const char* customParam1_commentCharLength = customParam1["commentCharLength"]; // "use https://bit.ly/3gFQqiB"
  int customParam1_maxCharLength = customParam1["maxCharLength"]; // 40
  char customParam1_defaultValue[customParam1_maxCharLength];
  strcpy(customParam1_defaultValue, customParam1["defaultValue"]); // "cloudmqtt.com"

  JsonObject customParam2 = doc["customParam2"];
  const char* customParam2_name_id = customParam2["name/id"]; // "port"
  const char* customParam2_placeholder = customParam2["placeholder"]; // "mqtt port"
  const char* customParam2_defaultVar = customParam2["defaultVar"]; // "mqtt_port"
  int customParam2_maxCharLength = customParam2["maxCharLength"]; // 6
  char customParam2_defaultValue[customParam2_maxCharLength];
  strcpy(customParam2_defaultValue, customParam2["defaultValue"]); // "8080"


  JsonObject customParam3 = doc["customParam3"];
  const char* customParam3_name_id = customParam3["name/id"]; // "blynk"
  const char* customParam3_placeholder = customParam3["placeholder"]; // "blynk token"
  const char* customParam3_defaultVar = customParam3["defaultVar"]; // "blynk_token"
  int customParam3_maxCharLength = customParam3["maxCharLength"]; // 34
  char customParam3_defaultValue[customParam3_maxCharLength];
  strcpy(customParam3_defaultValue, customParam3["defaultValue"]); // "YOUR_BLYNK_TOKEN"

  JsonObject customParam4 = doc["customParam4"];
  const char* customParam4_name_id = customParam4["name/id"]; // "apiKey"
  const char* customParam4_placeholder = customParam4["placeholder"]; // "api key"
  const char* customParam4_defaultVar = customParam4["defaultVar"]; // "apiKey"
  int customParam4_maxCharLength = customParam4["maxCharLength"]; // 40
  char customParam4_defaultValue[customParam4_maxCharLength];
  strcpy(customParam4_defaultValue, customParam4["defaultValue"]); // "YOUR_API_KEY"

  JsonObject customParam5 = doc["customParam5"];
  const char* customParam5_name_id = customParam5["name/id"]; // "tempThreshold"
  const char* customParam5_placeholder = customParam5["placeholder"]; // "temperature threshold"
  const char* customParam5_defaultVar = customParam5["defaultVar"]; // "tempThres"
  int customParam5_maxCharLength = customParam5["maxCharLength"]; // 5
  char customParam5_defaultValue[customParam5_maxCharLength];
  strcpy(customParam5_defaultValue, customParam5["defaultValue"]); // "30.0"

  JsonObject customParam6 = doc["customParam6"];
  const char* customParam6_name_id = customParam6["name/id"]; // "sampleInterval"
  const char* customParam6_placeholder = customParam6["placeholder"]; // "sample interval seconds"
  const char* customParam6_defaultVar = customParam6["defaultVar"]; // "sampleIntervalsec"
  int customParam6_maxCharLength = customParam6["maxCharLength"]; // 5
  char customParam6_defaultValue[customParam6_maxCharLength];
  strcpy(customParam6_defaultValue, customParam6["defaultValue"]); // "300"

  //clean FS, for testing
  //SPIFFS.format();

  //read configuration from FS json
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);

#ifdef ARDUINOJSON_VERSION_MAJOR >= 6
        DynamicJsonDocument json(1536);
        auto deserializeError = deserializeJson(json, buf.get());
        serializeJson(json, Serial);
        if ( ! deserializeError ) {
#else
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
#endif
          Serial.println("\nparsed json");
          strcpy(customParam1_defaultValue, json[customParam1_defaultVar]);
          strcpy(customParam2_defaultValue, json[customParam2_defaultVar]);
          strcpy(customParam3_defaultValue, json[customParam3_defaultVar]);
          strcpy(customParam4_defaultValue, json[customParam4_defaultVar]);
          strcpy(customParam5_defaultValue, json[customParam5_defaultVar]);
          strcpy(customParam6_defaultValue, json[customParam6_defaultVar]);
        } else {
          Serial.println("failed to load json config");
        }
        configFile.close();
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read

  // The extra parameters to be configured (can be either global or just in the setup)
  // After connecting, parameter.getValue() will get you the configured value
  // id/name placeholder/prompt default length
  WiFiManagerParameter custom_1(customParam1_name_id, customParam1_placeholder, customParam1_defaultValue, customParam1_maxCharLength);
  WiFiManagerParameter custom_2(customParam2_name_id, customParam2_placeholder, customParam2_defaultValue, customParam2_maxCharLength);
  WiFiManagerParameter custom_3(customParam3_name_id, customParam3_placeholder, customParam3_defaultValue, customParam3_maxCharLength);
  WiFiManagerParameter custom_4(customParam4_name_id, customParam4_placeholder, customParam4_defaultValue, customParam4_maxCharLength);
  WiFiManagerParameter custom_5(customParam5_name_id, customParam5_placeholder, customParam5_defaultValue, customParam5_maxCharLength);
  WiFiManagerParameter custom_6(customParam6_name_id, customParam6_placeholder, customParam6_defaultValue, customParam6_maxCharLength);



  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  //set static ip
  wifiManager.setSTAStaticIPConfig(IPAddress(10, 0, 1, 99), IPAddress(10, 0, 1, 1), IPAddress(255, 255, 255, 0));

  //add all your parameters here
  wifiManager.addParameter(&custom_1);
  wifiManager.addParameter(&custom_2);
  wifiManager.addParameter(&custom_3);
  wifiManager.addParameter(&custom_4);
  wifiManager.addParameter(&custom_5);
  wifiManager.addParameter(&custom_6);

  //reset settings - for testing
  //wifiManager.resetSettings();

  //set minimu quality of signal so it ignores AP's under that quality
  //defaults to 8%
  //wifiManager.setMinimumSignalQuality();

  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  //wifiManager.setTimeout(120);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("AutoConnectAP", "password")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  //read updated parameters
  strcpy(customParam1_defaultValue, custom_1.getValue());
  strcpy(customParam2_defaultValue, custom_2.getValue());
  strcpy(customParam3_defaultValue, custom_3.getValue());
  strcpy(customParam4_defaultValue, custom_4.getValue());
  strcpy(customParam5_defaultValue, custom_5.getValue());
  strcpy(customParam6_defaultValue, custom_6.getValue());
  Serial.println("The values in the file are: ");
  Serial.println("\"" + (String)customParam1_defaultVar + " : " + String(customParam1_defaultValue));
  Serial.println("\"" + (String)customParam2_defaultVar + " : " + String(customParam2_defaultValue));
  Serial.println("\"" + (String)customParam3_defaultVar + " : " + String(customParam3_defaultValue));
  Serial.println("\"" + (String)customParam4_defaultVar + " : " + String(customParam4_defaultValue));
  Serial.println("\"" + (String)customParam5_defaultVar + " : " + String(customParam5_defaultValue));
  Serial.println("\"" + (String)customParam6_defaultVar + " : " + String(customParam6_defaultValue));



  //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("saving config");
#ifdef ARDUINOJSON_VERSION_MAJOR >= 6
    DynamicJsonDocument json(1536);
#else
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
#endif
    json[customParam1_defaultVar] = customParam1_defaultValue;
    json[customParam2_defaultVar] = customParam2_defaultValue;
    json[customParam3_defaultVar] = customParam3_defaultValue;
    json[customParam4_defaultVar] = customParam4_defaultValue;
    json[customParam5_defaultVar] = customParam5_defaultValue;
    json[customParam6_defaultVar] = customParam6_defaultValue;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

#ifdef ARDUINOJSON_VERSION_MAJOR >= 6
    serializeJson(json, Serial);
    serializeJson(json, configFile);
#else
    json.printTo(Serial);
    json.printTo(configFile);
#endif
    configFile.close();
    //end save
  }

  Serial.println("local ip");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:

}
