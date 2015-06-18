/**************************************************************
 * WiFiManager is a library for the ESP8266/Arduino platform
 * (https://github.com/esp8266/Arduino) to enable easy 
 * configuration and reconfiguration of WiFi credentials and 
 * store them in EEPROM.
 * inspired by http://www.esp8266.com/viewtopic.php?f=29&t=2520
 * https://github.com/chriscook8/esp-arduino-apboot 
 * Built by AlexT https://github.com/tzapu
 * Licensed under MIT license
 **************************************************************/

#ifndef WiFiManager_h
#define WiFiManager_h

#include <ESP8266WiFi.h>

#include <EEPROM.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>

#define DEBUG //until arduino ide can include defines at compile time from main sketch

#ifdef DEBUG
#define DEBUG_PRINT(x)  Serial.println(x)
#else
#define DEBUG_PRINT(x)
#endif


class WiFiManager
{
public:
    WiFiManager(int eepromStart);
    void begin();
    void begin(char const *apName);
    
    boolean autoConnect();
    boolean autoConnect(char const *apName);
    
    boolean hasConnected();
    
    String beginConfigMode(void);
    void startWebConfig();
    
    String getSSID();
    String getPassword();
    //for conveniennce
    String urldecode(const char*);
private:
    const int WM_DONE = 0;
    const int WM_WAIT = 10;
    
    const String HTTP_404 = "HTTP/1.1 404 Not Found\r\n\r\n";
    const String HTTP_200 = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    const String HTTP_HEAD = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"/><title>{v}</title>";
    const String HTTP_STYLE = "<style>div,input {margin-bottom: 5px;}body{width:200px;display:block;margin-left:auto;margin-right:auto;}</style>";
    const String HTTP_SCRIPT = "<script>function c(l){document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();}</script>";
    const String HTTP_HEAD_END = "</head><body>";
    const String HTTP_ITEM = "<div><a href='#' onclick='c(this)'>{v}</a></div>";
    const String HTTP_FORM = "<form method='get' action='s'><input id='s' name='ssid' length=32 placeholder='SSID'><input id='p' name='pass' length=64 placeholder='password'><br/><input type='submit'></form>";
    const String HTTP_END = "</body></html>";
    
    int _eepromStart;
    const char* _apName = "no-net";
    String _ssid = "";
    String _pass = "";
    
    String getEEPROMString(int start, int len);
    void setEEPROMString(int start, int len, String string);
    int serverLoop();
    
    
    
    
    
};



#endif