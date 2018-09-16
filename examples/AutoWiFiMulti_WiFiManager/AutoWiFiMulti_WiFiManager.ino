#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ESP8266WiFiMulti.h>
#include <EEPROM.h>

#define MAX_SSID  10
#define MAX_SSID_SIZE  20
#define MAX_PASSWORD_SIZE  20
#define STA_DISCONNECT_TIMEOUT 30

WiFiManager wifiManager;
ESP8266WiFiMulti wifiMulti;

struct MySSID {
  char ssid[MAX_SSID_SIZE];
  char password[MAX_PASSWORD_SIZE];
};

int disconnect_timer = 0;
int eeAddress = 0;
int eeAddressBegin = 1024;
int ssidCount = 0;
int lastStatus = WL_CONNECTED;

bool sanitizeSSID(const char *ssid)
{
  if (ssid && strlen(ssid)) {
    return true;
  }
  
  return false;
}

unsigned int getSSIDCount(const char *_ssid)
{
  int ssidCount = 0;
  EEPROM.begin(eeAddressBegin );
  for (int i = 0; i < MAX_SSID; i++) {
    MySSID ssid;
    ssidCount = i;
    EEPROM.get(eeAddress + (i * sizeof(struct MySSID)), ssid);
    if (!sanitizeSSID(ssid.ssid)) {
      break;
    }
    if (strcmp(ssid.ssid, _ssid) == 0) {
      break;
    }
  }
  EEPROM.end();
  return ssidCount;
}

void getSSIDList()
{
  Serial.println("Get SSIDs from EEPROM:");
  EEPROM.begin(eeAddressBegin );
  for (int i = 0; i < MAX_SSID; i++) {
    MySSID ssid;
    EEPROM.get(eeAddress + (i * sizeof(struct MySSID)), ssid);
    if (sanitizeSSID(ssid.ssid)) {
      Serial.print("SSID ");
      Serial.print(i);
      Serial.printf(": ");
      Serial.print(ssid.ssid);
      Serial.print(", ");
      Serial.print("Password: ");
      Serial.print(ssid.password);
      Serial.println();
    }
  }
  Serial.println();
  EEPROM.end();
}

void addSSID(int num, const char *ssid, const char *password)
{
  struct MySSID _ssid;

  if (num >= MAX_SSID) {
    Serial.println("Max supported SSID " + MAX_SSID);
    return;
  }

  Serial.println(ssid);
  if (!sanitizeSSID(ssid)) {
    Serial.println("Invalid SSID");
    return;
  }
  
  strcpy(_ssid.ssid , ssid);
  strcpy(_ssid.password, password);
  
  EEPROM.begin(eeAddressBegin);
  EEPROM.put(eeAddress + (num * sizeof(struct MySSID)), _ssid);
  EEPROM.commit();
}

void addWifiMultiList()
{
  EEPROM.begin(eeAddressBegin);
  Serial.println("Restore EEPROM SSIDs to MultiWiFi: ");
  for (int i = 0; i < MAX_SSID; i++) {
    MySSID ssid;
    EEPROM.get(eeAddress + (i * sizeof(struct MySSID)), ssid);
    if (sanitizeSSID(ssid.ssid)) {
      Serial.print(" ");
      Serial.print(ssid.ssid);
      Serial.print(":");
      Serial.println(ssid.password);
      wifiMulti.addAP(ssid.ssid, ssid.password);
    }
  }
  EEPROM.end();
}

void saveConfig()
{
  addSSID(getSSIDCount(WiFi.SSID().c_str()), WiFi.SSID().c_str(), WiFi.psk().c_str());
  getSSIDList();
  Serial.println("Config saved!");
}

void cleanEEPROM()
{
  EEPROM.begin(eeAddressBegin);
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.end();
}

void resetSettings()
{
  cleanEEPROM();
  wifiManager.resetSettings();
}

void setup()
{
  Serial.begin(115200);
  pinMode(BUILTIN_LED, OUTPUT);
  delay(10);

  // Reset and clear EEPROM for the first time, Default EEPROM might contain garbage chars
  // resetSettings();
  wifiManager.setSaveConfigCallback(saveConfig);
  getSSIDList();
  addWifiMultiList();
}

void loop()
{
  int status = wifiMulti.run();
  delay(1000);

  if (status == WL_CONNECTED) {
    disconnect_timer = 0;
    if (lastStatus != status) {
      Serial.println("");
      Serial.print("Connected SSID: " + WiFi.SSID());
      Serial.print(", IP address: ");
      Serial.println(WiFi.localIP());
    }
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    disconnect_timer++;
    if (lastStatus != status) {
      if (disconnect_timer > STA_DISCONNECT_TIMEOUT) {
        Serial.println("Connecting SSID: " + WiFi.SSID());
        wifiManager.autoConnect(wifiMulti);
      }
    }
    digitalWrite(LED_BUILTIN, LOW);
  }

  lastStatus = status;
}
