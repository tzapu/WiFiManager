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

//#include <EEPROM.h>
//#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <memory>


const char HTTP_200[] PROGMEM = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
//const char HTTP_HEAD[] PROGMEM = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"/><title>{v}</title>";
const char HTTP_STYLE[] PROGMEM = "<style>div,input{padding:5px;font-size:1em;} input{width:95%;} body{text-align: center;} button{border:0;border-radius:0.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;}</style>";
const char HTTP_SCRIPT[] PROGMEM = "<script>function c(l){document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();}</script>";
const char HTTP_HEAD_END[] PROGMEM = "</head><body><div style='text-align: left; display: inline-block;'>";
const char HTTP_PORTAL_OPTIONS[] PROGMEM = "<form action=\"/wifi\" method=\"get\"><button>Configure WiFi</button></form><br/><form action=\"/0wifi\" method=\"get\"><button>Configure WiFi (No Scan)</button></form>";
const char HTTP_ITEM[] PROGMEM = "<div><a href='#' onclick='c(this)'>{v}</a> {r}% {i}</div>";
const char HTTP_ITEM_PADLOCK[] PROGMEM = "<img src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAgAAAAKCAMAAAC+Ge+yAAAACVBMVEUAAAC7u7v///9etNz6AAAAI0lEQVQI133GsQ3AMAAEoXv2H9qNJacKFbEaWTQJ+uQB/nMdEaYAXeJkZ9AAAAAASUVORK5CYII='/>";
const char HTTP_FORM[] PROGMEM = "<form method='get' action='wifisave'><input id='s' name='s' length=32 placeholder='SSID'><br/><input id='p' name='p' length=64 placeholder='password'><br/><br/><button type='submit'>save</button></form>";
const char HTTP_SAVED[] PROGMEM = "<div>Credentials Saved<br />Trying to connect ESP to network.<br />If it fails reconnect to AP to try again</div>";
const char HTTP_END[] PROGMEM = "</div></body></html>";


class WiFiManager
{
public:
    WiFiManager();
        
    boolean autoConnect();
    boolean autoConnect(char const *apName);
    boolean autoConnect(char const *apName, char const *apPasswd);

    String  getSSID();
    String  getPassword();

    void    resetSettings();
    //for convenience
    String  urldecode(const char*);

    //sets timeout before webserver loop ends and exits even if there has been no setup. 
    //usefully for devices that failed to connect at some point and got stuck in a webserver loop
    //in seconds
    void    setTimeout(unsigned long seconds);
    void    setDebugOutput(boolean debug);

    //sets a custom ip /gateway /subnet configuration
    void    setAPConfig(IPAddress ip, IPAddress gw, IPAddress sn);
    void    setAPCallback( void (*func)(void) );    
 
private:
    std::unique_ptr<DNSServer> dnsServer;
    std::unique_ptr<ESP8266WebServer> server;

    const int WM_DONE = 0;
    const int WM_WAIT = 10;
    
    //const char PROGMEM HTTP_404[]  = "HTTP/1.1 404 Not Found\r\n\r\n";
    //const String HTTP_200 = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    const String HTTP_HEAD = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"/><title>{v}</title>";
    //const String HTTP_STYLE = "<style>div,input {margin-bottom: 5px;} body{width:200px;display:block;margin-left:auto;margin-right:auto;} button{padding:0.75rem 1rem;border:0;border-radius:0.317rem;background-color:#1fa3ec;color:#fff;line-height:1.5;cursor:pointer;}</style>";
    //const String HTTP_SCRIPT = "<script>function c(l){document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();}</script>";
    //const String HTTP_HEAD_END = "</head><body>";
    //const String HTTP_ITEM = "<div><a href='#' onclick='c(this)'>{v}</a></div>";
    //const String HTTP_FORM = "<form method='get' action='wifisave'><input id='s' name='s' length=32 placeholder='SSID'><input id='p' name='p' length=64 placeholder='password'><br/><button type='submit'>save</button></form>";
    //const String HTTP_SAVED = "<div>Credentials Saved<br />Trying to connect ESP to network.<br />If it fails reconnect to AP to try again</div>";
    //const String HTTP_END = "</body></html>";
    //const char HTTP_END[] PROGMEM = R"=====(
    //</body></html>
    //)=====";

    void    begin();
    void    begin(char const *apName);
    void    begin(char const *apName, char const *apPasswd);
    
    int         _eepromStart;
    const char* _apName = "no-net";
    const char* _apPasswd = NULL;
    String      _ssid = "";
    String      _pass = "";
    unsigned long timeout = 0;
    unsigned long start = 0;
    IPAddress   _ip;
    IPAddress   _gw;
    IPAddress   _sn;
    
    String getEEPROMString(int start, int len);
    void setEEPROMString(int start, int len, String string);

    bool keepLooping = true;
    int status = WL_IDLE_STATUS;
    void connectWifi(String ssid, String pass);

    void handleRoot();
    void handleWifi(bool scan);
    void handleWifiSave();
    void handleNotFound();
    void handle204();
    boolean captivePortal();
    
    // DNS server
    const byte DNS_PORT = 53;

    //helpers
    int getRSSIasQuality(int RSSI);
    boolean isIp(String str);
    String toStringIp(IPAddress ip);

    boolean connect;
    boolean _debug = true;

    void (*_apcallback)(void) = NULL;

    template <typename Generic>
    void DEBUG_PRINT(Generic text);
};



#endif
