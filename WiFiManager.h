/**
 * WiFiManager.h
 * 
 * WiFiManager, a library for the ESP8266/Arduino platform
 * for configuration of WiFi credentials using a Captive Portal
 * 
 * @author Creator tzapu
 * @author tablatronix
 * @version 0.0.0
 * @license MIT
 */

#ifndef WiFiManager_h
#define WiFiManager_h

#define WM_WEBSERVERSHIM // use webserver shim lib
// #define WM_MDNS       // use MDNS
// #define WM_FIXERASECONFIG // use erase flash fix

#ifdef ESP8266

    extern "C" {
      #include "user_interface.h"
    }
    #include <ESP8266WiFi.h>
    #include <ESP8266WebServer.h>

    #ifdef WM_MDNS
        #include <ESP8266mDNS.h>
    #endif

    #define WIFI_getChipId() ESP.getChipId()
    #define WM_WIFIOPEN   ENC_TYPE_NONE

#elif defined(ESP32)

    #include <WiFi.h>
    #include <esp_wifi.h>  
    
    #define WIFI_getChipId() (uint32_t)ESP.getEfuseMac()
    #define WM_WIFIOPEN   WIFI_AUTH_OPEN

    #ifndef WEBSERVER_H
        #warning "WEBSERVER not implemented in espressif/esp32, see readme notes"
        #ifdef WM_WEBSERVERSHIM
            #include <WebServer.h>
        #else
            #include <ESP8266WebServer.h>
            // Forthcoming official
            // https://github.com/esp8266/ESPWebServer
        #endif
    #endif

    #ifdef WM_MDNS
        #include <ESPmDNS.h>
    #endif
#else
#endif

#include <DNSServer.h>
#include <memory>
#include "strings_en.h"

#ifndef WIFI_MANAGER_MAX_PARAMS
    #define WIFI_MANAGER_MAX_PARAMS 5 // params will autoincrement and realloc by this amount when max is reached
#endif

#define WFM_LABEL_BEFORE 1
#define WFM_LABEL_AFTER 2
#define WFM_NO_LABEL 0

class WiFiManagerParameter {
  public:
    /** 
        Create custom parameters that can be added to the WiFiManager setup web page
        @id is used for HTTP queries and must not contain spaces nor other special characters
    */
    WiFiManagerParameter(const char *custom);
    WiFiManagerParameter(const char *id, const char *placeholder);
    WiFiManagerParameter(const char *id, const char *placeholder, const char *defaultValue, int length);
    WiFiManagerParameter(const char *id, const char *placeholder, const char *defaultValue, int length, const char *custom);
    WiFiManagerParameter(const char *id, const char *placeholder, const char *defaultValue, int length, const char *custom, int labelPlacement);
    ~WiFiManagerParameter();

    const char *getID();
    const char *getValue();
    const char *getPlaceholder();
    int         getValueLength();
    int         getLabelPlacement();
    const char *getCustomHTML();
    void        setValue(const char *defaultValue, int length);
  private:
    const char *_id;
    const char *_placeholder;
    char       *_value;
    int         _length;
    int         _labelPlacement;
    const char *_customHTML;

    void init(const char *id, const char *placeholder, const char *defaultValue, int length, const char *custom);
    void init(const char *id, const char *placeholder, const char *defaultValue, int length, const char *custom, int labelPlacement);

    friend class WiFiManager;
};


class WiFiManager
{
  public:
    WiFiManager(Stream& consolePort);
    WiFiManager();
    ~WiFiManager();
    void WiFiManagerInit();

    typedef enum {
        MENU_WIFI       = 0,
        MENU_WIFINOSCAN = 1,
        MENU_INFO       = 2,
        MENU_PARAM      = 3,
        MENU_CLOSE      = 4,
        MENU_RESTART    = 5,
        MENU_EXIT       = 6,
        MENU_ERASE      = 7,
        MENU_SEP        = 8
    } menu_page_t;

    // auto connect to saved wifi, or custom, and start config portal on failures
    boolean       autoConnect();
    boolean       autoConnect(char const *apName, char const *apPassword = NULL);

    //manually start the config portal, autoconnect does this automatically on connect failure
    boolean       startConfigPortal(); // auto generates apname
    boolean       startConfigPortal(char const *apName, char const *apPassword = NULL);
    
    //manually start the web portal, autoconnect does this automatically on connect failure    
    void          startWebPortal();
    //manually stop the web portal if started manually
    void          stopWebPortal();
    // Run webserver processing, if setConfigPortalBlocking(false)
    boolean       process();

    // get the AP name of the config portal, so it can be used in the callback
    String        getConfigPortalSSID();

    // erase wifi credentials
    void          resetSettings();
    // reboot esp
    void          reboot();
    // disconnect wifi, without persistent saving or erasing
    bool          disconnect();

    //sets timeout before AP,webserver loop ends and exits even if there has been no setup.
    //useful for devices that failed to connect at some point and got stuck in a webserver loop
    //in seconds setConfigPortalTimeout is a new name for setTimeout
    void          setConfigPortalTimeout(unsigned long seconds);
    void          setTimeout(unsigned long seconds); // @deprecated, alias

    //sets timeout for which to attempt connecting, useful if you get a lot of failed connects
    void          setConnectTimeout(unsigned long seconds);
    // toggle debug output
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
    //adds a custom parameter, returns false on failure
    bool          addParameter(WiFiManagerParameter *p);
    //if this is set, it will exit after config, even if connection is unsuccessful.
    void          setBreakAfterConfig(boolean shouldBreak);
    //if this is set, portal will be blocking and wait until save or exit, is false user must manually `process()` to handle config portal
    void          setConfigPortalBlocking(boolean shouldBlock);
    //if this is set, customise style
    void          setCustomHeadElement(const char* element);
    //if this is true, remove duplicated Access Points - defaut true
    void          setRemoveDuplicateAPs(boolean removeDuplicates);
    //setter for ESP wifi.persistent so we can remember it and restore user preference, as WIFi._persistent is protected
    void          setRestorePersistent(boolean persistent);
    //if true, always show static net inputs, IP, subnet, gateway, if false, only show when already set.
    void          setShowStaticFields(boolean alwaysShow);
    //if false, disable captive portal redirection
    void          setCaptivePortalEnable(boolean enabled);
    //if false, timeout captive portal even if a STA client connected (false), suggest disabling if captiveportal is open
    void          setCaptivePortalClientCheck(boolean enabled);
    //if true, reset timeout when webclient connects (true), suggest disabling if captiveportal is open    
    void          setWebPortalClientCheck(boolean enabled);
    // if true, enable autoreconnecting
    void          setWiFiAutoReconnect(boolean enabled);
    // if true, wifiscan will show percentage instead of quality icons, until we have better templating
    void          setScanDispPerc(boolean enabled);
    // set a custom hostname, sets sta and ap dhcp client id for esp32, and sta for esp8266
    bool          setHostname(const char * hostname);
    // set custom menu
    void          setMenu(uint8_t menu[]);

    // get last connection result, includes autoconnect and wifisave
    uint8_t       getLastConxResult();
    // get a status as string
    String        getWLStatusString(uint8_t status);    
    // check if the module has a saved ap to connect to
    bool          getWiFiIsSaved();

    // debug output the softap config
    void          debugSoftAPConfig();
    // debug output platform info and versioning
    void          debugPlatformInfo();
  private:
    std::unique_ptr<DNSServer>        dnsServer;

    #if defined(ESP32) && defined(WM_WEBSERVERSHIM)
        using WM_WebServer = WebServer;
    #else
        using WM_WebServer = ESP8266WebServer;
    #endif
        std::unique_ptr<WM_WebServer> server;

    // std:vector<uint8_t> _menuids;
    uint8_t _menuIds[10] = {MENU_WIFI,MENU_INFO,MENU_EXIT};

    // ip configs
    IPAddress     _ap_static_ip;
    IPAddress     _ap_static_gw;
    IPAddress     _ap_static_sn;
    IPAddress     _sta_static_ip;
    IPAddress     _sta_static_gw;
    IPAddress     _sta_static_sn;

    // defaults
    const byte    DNS_PORT                = 53;
    const byte    HTTP_PORT               = 80;
    String        _apName                 = "no-net";
    String        _apPassword             = "";
    String        _ssid                   = "";
    String        _pass                   = "";
    
    // options flags
    unsigned long _configPortalTimeout    = 0; // ms close config portal loop if set (depending on  _cp/webClientCheck options)
    unsigned long _connectTimeout         = 0; // ms stop trying to connect to ap if set
    unsigned long _configPortalStart      = 0; // ms config portal start time (updated for timeouts)
    unsigned long _webPortalAccessed      = 0; // ms last web access time
    WiFiMode_t    _usermode               = WIFI_OFF;
    String        _wifissidprefix         = FPSTR(S_ssidpre); // auto apname prefix prefix+chipid
    uint8_t       _lastconxresult         = WL_IDLE_STATUS;
    int           _numNetworks            = 0;
    unsigned long _lastscan               = 0; // ms

    #ifdef ESP32
    static uint8_t _lastconxresulttmp; // tmp var for esp32 callback
    #endif

    #ifndef WL_STATION_WRONG_PASSWORD
    uint8_t WL_STATION_WRONG_PASSWORD     = 7; // @kludge define a WL status for wrong password
    #endif

    // option parameters
    int           _minimumQuality         = -1;    // filter wifiscan ap by this rssi
    boolean       _removeDuplicateAPs     = true;  // remove dup aps from wifiscan
    boolean       _shouldBreakAfterConfig = false; // stop configportal on save failure
    boolean       _tryWPS                 = false; // try WPS on save failure, unsupported
    boolean       _configPortalIsBlocking = true;  // configportal enters blocking loop 
    boolean       _staShowStaticFields    = false; // always show static ip fields, even if not set in code
    boolean       _enableCaptivePortal    = true;  // enable captive portal redirection
    boolean       _userpersistent         = true;  // users preffered persistence to restore
    boolean       _wifiAutoReconnect      = true;  // there is no platform getter for this, we must assume its true and make it so
    boolean       _cpClientCheck          = false; // keep cp alive if cp have station
    boolean       _webClientCheck         = true;  // keep cp alive if web have client
    boolean       _scanDispOptions        = false; // show percentage in scans not icons
    boolean       _paramsInWifi           = true;  // show custom parameters on wifi page
    boolean       _preloadwifiscan        = true;  // preload wifiscan
    const char *  _hostname               = "";

    const char*   _customHeadElement      = ""; // store custom head element html from user

    void          setupConfigPortal();
    void          startWPS();
    bool          startAP();

    uint8_t       connectWifi(String ssid, String pass);
    uint8_t       waitForConnectResult();
    uint8_t       waitForConnectResult(uint16_t timeout);

    // webserver handlers
    void          handleRoot();
    void          handleWifi(boolean scan);
    void          handleWifiSave();
    void          handleInfo();
    void          handleReset();
    void          handleNotFound();
    void          handleExit();
    void          handleErase();
    void          handleParam();
    void          handleWiFiStatus();
    void          handleRequest();
    void          handleParamSave();
    void          doParamSave();

    boolean       captivePortal();
    boolean       configPortalHasTimeout();
    boolean       stopConfigPortal();
    uint8_t       processConfigPortal();
    void          stopCaptivePortal();

    // wifi platform abstractions
    bool          WiFi_Mode(WiFiMode_t m);
    bool          WiFi_Mode(WiFiMode_t m,bool persistent);
    bool          WiFi_Disconnect();
    bool          WiFi_enableSTA(bool enable);
    bool          WiFi_enableSTA(bool enable,bool persistent);
    bool          WiFi_eraseConfig();
    uint8_t       WiFi_softap_num_stations();
    bool          WiFi_hasAutoConnect();
    void          WiFi_autoReconnect();
    String        WiFi_SSID();
    bool          WiFi_scanNetworks();
    bool          WiFi_scanNetworks(bool force);
    bool          WiFi_scanNetworks(int cachetime);

    #ifdef ESP32
    static void   WiFiEvent(WiFiEvent_t event, system_event_info_t info);
    #endif

    // output helpers
    String        getParamOut();
    String        getIpForm(String id, String title, String value);
    String        getScanItemOut();
    String        getStaticOut();
    String        getHTTPHead(String title);
    String        getMenuOut();
    //helpers
    int           getRSSIasQuality(int RSSI);
    boolean       isIp(String str);
    String        toStringIp(IPAddress ip);
    boolean       validApPassword();
    String        encryptionTypeStr(uint8_t authmode);
    void          reportStatus(String &page);
    String        getInfoData(String id);

    // flags
    boolean       connect;
    boolean       abort;
    boolean       reset               = false;
    boolean       configPortalActive  = false;
    boolean       webPortalActive     = false;
    boolean       portalTimeoutResult = false;
    boolean       portalAbortResult   = false;
    boolean       storeSTAmode        = true; // option store persistent STA mode in connectwifi 
    int           timer               = 0;
    
    // WiFiManagerParameter
    int         _paramsCount          = 0;
    int         _max_params;
    WiFiManagerParameter** _params;

    // debugging
    boolean       _debug              = true;
    uint8_t       _debugLevel         = 2;
    Stream&     _debugPort; // debug output stream ref
    template <typename Generic>
    void        DEBUG_WM(Generic text);
    template <typename Generic, typename Genericb>
    void        DEBUG_WM(Generic text,Genericb textb);

    // callbacks
    void (*_apcallback)(WiFiManager*) = NULL;
    void (*_savecallback)(void)       = NULL;

    template <class T>
    auto optionalIPFromString(T *obj, const char *s) -> decltype(  obj->fromString(s)  ) {
      return  obj->fromString(s);
    }
    auto optionalIPFromString(...) -> bool {
      // DEBUG_WM("NO fromString METHOD ON IPAddress, you need ESP8266 core 2.1.0 or newer for Custom IP configuration to work.");
      return false;
    }
};

#endif
