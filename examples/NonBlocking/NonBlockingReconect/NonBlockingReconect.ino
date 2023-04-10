#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

const unsigned long connectionTimeout = 60; // 1 minute timeout

// reconect interval every 2 minutes try to reconect if the connection is lost
const int reconnectInterval = 120*1000; 

// flash button on GPIO0
const int resetButtonPin = 0; 

struct WiFiStatusDescription {
  int8_t code;
  const char* description;
};

// Descriptions of Wi-Fi status codes
const WiFiStatusDescription wifiStatusDescriptions[] = {
  {WL_IDLE_STATUS, "WL_IDLE_STATUS: Wi-Fi module is idle"},
  {WL_NO_SSID_AVAIL, "WL_NO_SSID_AVAIL: No SSID available"},
  {WL_SCAN_COMPLETED, "WL_SCAN_COMPLETED: Wi-Fi scan completed"},
  {WL_CONNECTED, "WL_CONNECTED: Connected to Wi-Fi network"},
  {WL_CONNECT_FAILED, "WL_CONNECT_FAILED: Connection failed"},
  {WL_CONNECTION_LOST, "WL_CONNECTION_LOST: Connection lost"},
  {WL_DISCONNECTED, "WL_DISCONNECTED: Wi-Fi disconnected"},
};

// Return a description of the given Wi-Fi status code
const char* getWiFiStatusDescription(int8_t code) {
  for (size_t i = 0; i < sizeof(wifiStatusDescriptions) / sizeof(wifiStatusDescriptions[0]); ++i) {
    if (wifiStatusDescriptions[i].code == code) {
      return wifiStatusDescriptions[i].description;
    }
  }
  return "Unknown Wi-Fi status";
}

int i=0;

unsigned long lastReconnectAttempt = 0;

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);

  // ledul onboard 
  pinMode(LED_BUILTIN, OUTPUT);    // LED pin as output.
  
  // flash button - to erase the wifi data
  pinMode(resetButtonPin, INPUT_PULLUP);

  reconnect();
}

void eraseEEPROM() {
  WiFiManager wifiManager;
  wifiManager.resetSettings();
  Serial.println("EEPROM erased.");
  delay(500); // Add a delay to allow the EEPROM write to complete
  ESP.restart();
}

void reconnect() {
  WiFiManager wifiManager;
  
  wifiManager.setDebugOutput(4);

  // Set the timeout for the autoConnect function
  //Timeout for the config portal in seconds
  wifiManager.setConfigPortalTimeout(connectionTimeout); 
  
  //Timeout for the stored connection in seconds
  wifiManager.setConnectTimeout(connectionTimeout/3); 

  if (WiFi.SSID() == "") { // No stored network, start AP mode
    Serial.println("No stored network, starting AP mode...");
    if (!wifiManager.startConfigPortal("AP_ESP8266", "parola1234")) { // Replace with your desired AP name and password (at least 8 characters)
      Serial.println("Failed to connect to AP.");
      return; // Exit function without waiting for a timeout
    }
  } else { // Stored network available
    Serial.print("Connecting to stored network: ");
    Serial.println(WiFi.SSID());
    if (!wifiManager.autoConnect()) {
      Serial.println("Failed to connect to stored network.");
      return; // Exit function without waiting for a timeout
    }
  }

  Serial.println("Connected to Wi-Fi network.");
}

void loop() {
  
  // if you press the flash button, erase the EEPROM
  if (digitalRead(resetButtonPin) == LOW) {
    Serial.println("Erasing EEPROM...");
    eraseEEPROM();
  }

  // if the WiFi connection is lost, try to reconnect
  if (WiFi.status() != WL_CONNECTED && (millis() - lastReconnectAttempt > reconnectInterval)) {
    Serial.println(" ");
    Serial.println("WiFi connection lost. Attempting to reconnect...");
    lastReconnectAttempt = millis();
    Serial.println("Attempting to reconnect...");
    reconnect();
  }

  delay(500);

  // blink the LED to show we're still alive
  if(i++%2==0){
    digitalWrite(LED_BUILTIN, HIGH);
  }else{
    digitalWrite(LED_BUILTIN, LOW); 
  }
  
    // print the WiFi status every 50 loops
  if(i%50==0){
    Serial.println(".");
    Serial.print("WiFi Status: ");
    Serial.print(WiFi.status());
    Serial.print(" - ");
    Serial.println(getWiFiStatusDescription(WiFi.status()));
    Serial.println(millis() - lastReconnectAttempt);
  }else{
    Serial.print(".");
  }

}
