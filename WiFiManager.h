/**************************************************************
   WiFiManager is a library for the ESP8266/Arduino platform
   (https://github.com/esp8266/Arduino) to enable easy
   configuration and reconfiguration of WiFi credentials using a Captive Portal
   inspired by:
   http://www.esp8266.com/viewtopic.php?f=29&t=2520
   https://github.com/chriscook8/esp-arduino-apboot
   https://github.com/esp8266/Arduino/tree/master/libraries/DNSServer/examples/CaptivePortalAdvanced
   Built by AlexT https://github.com/tzapu
   Licensed under MIT license
 **************************************************************/

#ifndef WiFiManager_h
#define WiFiManager_h

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <memory>

#include <webgets.h>

extern "C" {
  #include "user_interface.h"
}

const char HTTP_HEAD[] PROGMEM            = "<!DOCTYPE html><html lang=\"en\"><head><meta charset='UTF-8' name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/><title>{v}</title>";
const char HTTP_STYLE[] PROGMEM           = "<style>.c{text-align:center;} div,input{padding:5px;font-size:1em;} input{width:95%;} body{text-align:center;font-family:verdana;} button{border:0;border-radius:0.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;} .q{float:right;width:64px;text-align:right;} .l{background:url(\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAALVBMVEX///8EBwfBwsLw8PAzNjaCg4NTVVUjJiZDRUUUFxdiZGSho6OSk5Pg4eFydHTCjaf3AAAAZElEQVQ4je2NSw7AIAhEBamKn97/uMXEGBvozkWb9C2Zx4xzWykBhFAeYp9gkLyZE0zIMno9n4g19hmdY39scwqVkOXaxph0ZCXQcqxSpgQpONa59wkRDOL93eAXvimwlbPbwwVAegLS1HGfZAAAAABJRU5ErkJggg==\") no-repeat left center;background-size:1em;} input[type=checkbox]{float:right;width:5%;cursor:pointer;} label{width:90%;float:left;cursor:pointer;} input[type=radio]{float:right;width:5%;cursor:pointer;} fieldset{width:95%}</style>";
const char HTTP_SCRIPT[] PROGMEM          = "<script>ep='data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIyNCIgaGVpZ2h0PSIyNCIgdmlld0JveD0iMCAwIDI0IDI0Ij48cGF0aCBkPSJNMTUgMTJjMCAxLjY1NC0xLjM0NiAzLTMgM3MtMy0xLjM0Ni0zLTMgMS4zNDYtMyAzLTMgMyAxLjM0NiAzIDN6bTktLjQ0OXMtNC4yNTIgOC40NDktMTEuOTg1IDguNDQ5Yy03LjE4IDAtMTIuMDE1LTguNDQ5LTEyLjAxNS04LjQ0OXM0LjQ0Ni03LjU1MSAxMi4wMTUtNy41NTFjNy42OTQgMCAxMS45ODUgNy41NTEgMTEuOTg1IDcuNTUxem0tNyAuNDQ5YzAtMi43NTctMi4yNDMtNS01LTVzLTUgMi4yNDMtNSA1IDIuMjQzIDUgNSA1IDUtMi4yNDMgNS01eiIvPjwvc3ZnPg==';esp='data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIyNCIgaGVpZ2h0PSIyNCIgdmlld0JveD0iMCAwIDI0IDI0Ij48cGF0aCBkPSJNMTEuODg1IDE0Ljk4OGwzLjEwNC0zLjA5OC4wMTEuMTFjMCAxLjY1NC0xLjM0NiAzLTMgM2wtLjExNS0uMDEyem04LjA0OC04LjAzMmwtMy4yNzQgMy4yNjhjLjIxMi41NTQuMzQxIDEuMTQ5LjM0MSAxLjc3NiAwIDIuNzU3LTIuMjQzIDUtNSA1LS42MzEgMC0xLjIyOS0uMTMtMS43ODUtLjM0NGwtMi4zNzcgMi4zNzJjMS4yNzYuNTg4IDIuNjcxLjk3MiA0LjE3Ny45NzIgNy43MzMgMCAxMS45ODUtOC40NDkgMTEuOTg1LTguNDQ5cy0xLjQxNS0yLjQ3OC00LjA2Ny00LjU5NXptMS40MzEtMy41MzZsLTE4LjYxOSAxOC41OC0xLjM4Mi0xLjQyMiAzLjQ1NS0zLjQ0N2MtMy4wMjItMi40NS00LjgxOC01LjU4LTQuODE4LTUuNThzNC40NDYtNy41NTEgMTIuMDE1LTcuNTUxYzEuODI1IDAgMy40NTYuNDI2IDQuODg2IDEuMDc1bDMuMDgxLTMuMDc1IDEuMzgyIDEuNDJ6bS0xMy43NTEgMTAuOTIybDEuNTE5LTEuNTE1Yy0uMDc3LS4yNjQtLjEzMi0uNTM4LS4xMzItLjgyNyAwLTEuNjU0IDEuMzQ2LTMgMy0zIC4yOTEgMCAuNTY3LjA1NS44MzMuMTM0bDEuNTE4LTEuNTE1Yy0uNzA0LS4zODItMS40OTYtLjYxOS0yLjM1MS0uNjE5LTIuNzU3IDAtNSAyLjI0My01IDUgMCAuODUyLjIzNSAxLjY0MS42MTMgMi4zNDJ6Ii8+PC9zdmc+';function sh(p,n){var pE=document.getElementById(n);if(pE.type=='password'){pE.type='text';document.getElementById(p).src=esp;}else{pE.type='password';document.getElementById(p).src=ep;}}function c(l){document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();}</script>";
const char HTTP_HEAD_END[] PROGMEM        = "</head><body><div style='text-align:left;display:inline-block;min-width:260px;'>";
const char HTTP_PORTAL_OPTIONS[] PROGMEM  = "<form action=\"/wifi\" method=\"get\"><button>Configure WiFi</button></form><br/><form action=\"/0wifi\" method=\"get\"><button>Configure WiFi (No Scan)</button></form><br/><form action=\"/i\" method=\"get\"><button>Info</button></form><br/><form action=\"/r\" method=\"post\"><button>Reset</button></form>";
const char HTTP_ITEM[] PROGMEM            = "<div><a href='#p' onclick='c(this)'>{v}</a>&nbsp;<span class='q {i}'>{r}%</span></div>";
const char HTTP_FORM_START[] PROGMEM      = "<form method='get' action='wifisave'><input id='s' name='s' length=32 placeholder='SSID'><br/><input style='width:80%' type='password' length=64 id='p' name='p' placeholder='password'><img style='position:relative;top:8px;left:8px;' id='eye_p' onClick=\"sh('eye_p','p')\" src=''><script>document.getElementById('eye_p').src=ep;</script><br/>";
const char HTTP_FORM_PARAM[] PROGMEM      = "<br/><input id='{i}' name='{n}' maxlength={l} placeholder='{p}' value='{v}' {c}>";
const char HTTP_FORM_END[] PROGMEM        = "<br/><button type='submit'>save</button></form>";
const char HTTP_SCAN_LINK[] PROGMEM       = "<br/><div class=\"c\"><a href=\"/wifi\">Scan</a></div>";
const char HTTP_SAVED[] PROGMEM           = "<div>Credentials Saved<br />Trying to connect ESP to network.<br />If it fails reconnect to AP to try again</div>";
const char HTTP_END[] PROGMEM             = "</div></body></html>";

#define WIFI_MANAGER_MAX_PARAMS 15

class WiFiManager
{
  public:
    WiFiManager();

    boolean       autoConnect();
    boolean       autoConnect(char const *apName, char const *apPassword = NULL);

    //if you want to always start the config portal, without trying to connect first
    boolean       startConfigPortal();
    boolean       startConfigPortal(char const *apName, char const *apPassword = NULL);

    // get the AP name of the config portal, so it can be used in the callback
    String        getConfigPortalSSID();

    void          resetSettings();

    //sets timeout before webserver loop ends and exits even if there has been no setup.
    //useful for devices that failed to connect at some point and got stuck in a webserver loop
    //in seconds setConfigPortalTimeout is a new name for setTimeout
    void          setConfigPortalTimeout(unsigned long seconds);
    void          setTimeout(unsigned long seconds);

    //sets timeout for which to attempt connecting, useful if you get a lot of failed connects
    void          setConnectTimeout(unsigned long seconds);


    void          setDebugOutput(boolean debug);
    //defaults to not showing anything under 8% signal quality if called
    void          setMinimumSignalQuality(int quality = 8);
    //sets a custom ip /gateway /subnet configuration
    void          setAPStaticIPConfig(IPAddress ip, IPAddress gw, IPAddress sn);
    //sets config for a static IP
    void          setSTAStaticIPConfig(IPAddress ip, IPAddress gw, IPAddress sn);
    //called when AP mode and config portal is started
    void          setAPCallback( void (*func)(WiFiManager*) );
    //called when settings have been changed and connection was successful
    void          setSaveConfigCallback( void (*func)(void) );
    //called as long as the config portal loop
    void          setUserJobCallback( void (*func)(void) );
    //adds a custom parameter
    void          addParameter(web_parameter& p);
    //if this is set, it will exit after config, even if connection is unsuccessful.
    void          setBreakAfterConfig(boolean shouldBreak);
    //if this is set, try WPS setup when starting (this will delay config portal for up to 2 mins)
    //TODO
    //if this is set, customise style
    void          setCustomHeadElement(const char* element);
    //if this is true, remove duplicated Access Points - defaut true
    void          setRemoveDuplicateAPs(boolean removeDuplicates);

  private:
    std::unique_ptr<DNSServer>        dnsServer;
    std::unique_ptr<ESP8266WebServer> server;

    //const int     WM_DONE                 = 0;
    //const int     WM_WAIT                 = 10;

    //const String  HTTP_HEAD = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"/><title>{v}</title>";

    void          setupConfigPortal();
    void          startWPS();

    const char*   _apName                 = "no-net";
    const char*   _apPassword             = NULL;
    String        _ssid                   = "";
    String        _pass                   = "";
    unsigned long _configPortalTimeout    = 0;
    unsigned long _connectTimeout         = 0;
    unsigned long _configPortalStart      = 0;

    IPAddress     _ap_static_ip;
    IPAddress     _ap_static_gw;
    IPAddress     _ap_static_sn;
    IPAddress     _sta_static_ip;
    IPAddress     _sta_static_gw;
    IPAddress     _sta_static_sn;

    int           _minimumQuality         = -1;
    boolean       _removeDuplicateAPs     = true;
    boolean       _shouldBreakAfterConfig = false;
    boolean       _tryWPS                 = false;

    const char*   _customHeadElement      = "";

    //String        getEEPROMString(int start, int len);
    //void          setEEPROMString(int start, int len, String string);

    int           status = WL_IDLE_STATUS;
    int           connectWifi(String ssid, String pass);
    uint8_t       waitForConnectResult();

    void          handleRoot();
    void          handleWifi(boolean scan);
    void          handleWifiSave();
    void          handleInfo();
    void          handleReset();
    void          handleNotFound();
    void          handle204();
    boolean       captivePortal();
    boolean       configPortalHasTimeout();

    // DNS server
    const byte    DNS_PORT = 53;

    //helpers
    int           getRSSIasQuality(int RSSI);
    boolean       isIp(String str);
    String        toStringIp(IPAddress ip);

    boolean       connect;
    boolean       _debug = true;

    void (*_apcallback)(WiFiManager*) = NULL;
    void (*_user_job)(void) = NULL;
    void (*_savecallback)(void) = NULL;

    std::list<std::reference_wrapper<web_parameter>> _params;

    template <typename Generic>
    void          DEBUG_WM(Generic text);

    template <class T>
    auto optionalIPFromString(T *obj, const char *s) -> decltype(  obj->fromString(s)  ) {
      return  obj->fromString(s);
    }
    auto optionalIPFromString(...) -> bool {
      DEBUG_WM("NO fromString METHOD ON IPAddress, you need ESP8266 core 2.1.0 or newer for Custom IP configuration to work.");
      return false;
    }
};

#endif
