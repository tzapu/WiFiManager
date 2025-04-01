#include <FS.h>                   //this needs to be first, or it all crashes and burns...

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson

//define your default values here, if there are different values in config.json, they are overwritten.

char apiKey[20] = "";
WiFiClient client;
char defaultHost[100] = "184.106.153.149";  //Thing Speak IP address (sometime the web address causes issues with ESP's :/
long itt = 0;

const byte wifiResetPin = 13;   //D7 on Node MCU
int interruptPinDebounce = 0;
long debouncing_time = 1000; //Debouncing Time in Milliseconds
volatile unsigned long wifiResetLastMillis = 0;

//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback ()
{
    Serial.println("Should save config");
    shouldSaveConfig = true;
}

void handleWifiReset() 
{
    if(millis() < wifiResetLastMillis)
    {
        wifiResetLastMillis = millis();    //To handle error caused by the 49 day rollover in millis
    }
    if( (millis() - wifiResetLastMillis) >= debouncing_time)
    {
        Serial.println("Clearing WiFi data resetting");
        WiFiManager wifiManager;
        wifiManager.resetSettings();
        SPIFFS.format();    //If you change the number of wifi prams, you will need to call this or everything breaks :(
        ESP.reset();
        delay(1000);
    }
    wifiResetLastMillis = millis();
}

void setup()
{
    WiFiManager wifiManager;
    
    // put your setup code here, to run once:
    Serial.begin(115200);
        
    pinMode(wifiResetPin, INPUT_PULLUP);    //Reset pin setup
    attachInterrupt(digitalPinToInterrupt(wifiResetPin), handleWifiReset, FALLING);

    //clean FS, for testing
    //SPIFFS.format();

    //read configuration from FS json
    Serial.println("mounting FS...");

    if (SPIFFS.begin())    
    {
        Serial.println("mounted file system");
        if (SPIFFS.exists("/config.json"))        
        {
            //file exists, reading and loading
            Serial.println("reading config file");
            File configFile = SPIFFS.open("/config.json", "r");
            if (configFile)
            {
                Serial.println("opened config file");
                size_t size = configFile.size();
                // Allocate a buffer to store contents of the file.
                std::unique_ptr<char[]> buf(new char[size]);

                configFile.readBytes(buf.get(), size);
                DynamicJsonBuffer jsonBuffer;
                JsonObject& json = jsonBuffer.parseObject(buf.get());
                json.printTo(Serial);
                if (json.success())
                {
                    Serial.println("\nparsed json");

                    strcpy(defaultHost, json["defaultHost"]);
                    strcpy(apiKey, json["apiKey"]);
                }
                else
                {
                    Serial.println("failed to load json config");
                }
            }
        }
    }
    else    
    {
        Serial.println("failed to mount FS");
    }
    //end read



    // The extra parameters to be configured (can be either global or just in the setup)
    // After connecting, parameter.getValue() will get you the configured value
    // id/name placeholder/prompt default length
    WiFiManagerParameter customHostServer("defaultHost", "Host Server", defaultHost, 100);
    WiFiManagerParameter customAPIKey("apiKey", "Host API Key", apiKey, 20);

    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    //WiFiManager wifiManager;

    //set config save notify callback
    wifiManager.setSaveConfigCallback(saveConfigCallback);

    //add all your parameters here
    wifiManager.addParameter(&customHostServer);
    wifiManager.addParameter(&customAPIKey);

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
    if (!wifiManager.autoConnect("AutoConnectAP", "password"))
    {
        Serial.println("failed to connect and hit timeout");
        delay(3000);
        //reset and try again, or maybe put it to deep sleep
        ESP.reset();
        delay(5000);
    }

    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");

    //read updated parameters
    strcpy(defaultHost, customHostServer.getValue());
    strcpy(apiKey, customAPIKey.getValue());

    //save the custom parameters to FS
    if (shouldSaveConfig)
    {
        Serial.println("saving config");
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.createObject();
        json["defaultHost"] = defaultHost;
        json["apiKey"] = apiKey;

        File configFile = SPIFFS.open("/config.json", "w");
        if (!configFile)
        {
            Serial.println("failed to open config file for writing");
        }

        json.printTo(Serial);
        json.printTo(configFile);
        configFile.close();
        //end save
    }

    Serial.println("local ip");
    Serial.println(WiFi.localIP());
}

//Loop code taken from ThingSpeak's example of HTTP Post
void loop()
{
    if (client.connect(defaultHost,80))
    { // "184.106.153.149" or api.thingspeak.com
        itt++;  //Replace with a sensor reading or something useful
        String postStr = apiKey;
        postStr +="&field1=";
        postStr += String(itt);
        postStr += "\r\n\r\n";

        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: "+String(apiKey)+"\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(postStr.length());
        client.print("\n\n");
        client.print(postStr);

        Serial.println("% send to Thingspeak");
    }

    client.stop();

    Serial.println("Waiting…");
    // thingspeak needs minimum 15 sec delay between updates
    delay(20000);
}
