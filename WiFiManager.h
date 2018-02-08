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

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
#endif

#include <DNSServer.h>
#include <memory>

#if defined(ESP8266)
  extern "C" {
    #include "user_interface.h"
  }
#endif

#define WIFI_getChipId() ESP.getChipId()
#define WIFI_AUTH_OPEN   ENC_TYPE_NONE

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

    boolean       autoConnect();
    boolean       autoConnect(char const *apName, char const *apPassword = NULL);

    //if you want to always start the config portal, without trying to connect first
    boolean       startConfigPortal();
    boolean       startConfigPortal(char const *apName, char const *apPassword = NULL);
    boolean       process();
    void          startWebPortal();
    void          stopWebPortal();

    // get the AP name of the config portal, so it can be used in the callback
    String        getConfigPortalSSID();

    void          resetSettings();
    void          reboot();

    //sets timeout before AP,webserver loop ends and exits even if there has been no setup.
    //useful for devices that failed to connect at some point and got stuck in a webserver loop
    //in seconds setConfigPortalTimeout is a new name for setTimeout
    void          setConfigPortalTimeout(unsigned long seconds);
    void          setTimeout(unsigned long seconds); // @deprecated, alias

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
    //adds a custom parameter, returns false on failure
    bool          addParameter(WiFiManagerParameter *p);
    //if this is set, it will exit after config, even if connection is unsuccessful.
    void          setBreakAfterConfig(boolean shouldBreak);
    //if this is set, portal will be blocking and wait until save or exit, is false user must manually process to handle config portal
    void          setConfigPortalBlocking(boolean shouldBlock);


    //if this is set, customise style
    void          setCustomHeadElement(const char* element);
    //if this is true, remove duplicated Access Points - defaut true
    void          setRemoveDuplicateAPs(boolean removeDuplicates);
    //setter for ESP wifi.persistent so we can remember it and restore user preference, as WIFi._persistent is protected
    void          setRestorePersistent(boolean persistent);
    //if true, always show static net inputs, IP, subnet, gateway, if false, only show when already set.
    void          setShowStaticFields(boolean alwaysShow);
    //if true disable captive portal redirection
    void          setCaptivePortalEnable(boolean enabled);
    //if true, timeout captive portal even if a STA client connected (true), suggest disabling if captiveportal is open
    void          setCaptivePortalClientCheck(boolean enabled);
    //if true, reset timeout when webclient connects (true), suggest disabling if captiveportal is open    
    void          setWebPortalClientCheck(boolean enabled);

  private:
    std::unique_ptr<DNSServer>        dnsServer;
    std::unique_ptr<ESP8266WebServer> server;

    // ip configs
    IPAddress     _ap_static_ip;
    IPAddress     _ap_static_gw;
    IPAddress     _ap_static_sn;
    IPAddress     _sta_static_ip;
    IPAddress     _sta_static_gw;
    IPAddress     _sta_static_sn;

    // defaults
    const byte    DNS_PORT                = 53;
    String        _apName                 = "no-net";
    String        _apPassword             = "";
    String        _ssid                   = "";
    String        _pass                   = "";
    
    // options flags
    unsigned long _configPortalTimeout    = 0;
    unsigned long _connectTimeout         = 0;
    unsigned long _configPortalStart      = 0;
    unsigned long _webPortalAccessed      = 0;
    WiFiMode_t    _usermode               = WIFI_OFF;

    // option parameters
    int           _minimumQuality         = -1;
    boolean       _removeDuplicateAPs     = true;
    boolean       _shouldBreakAfterConfig = false;
    boolean       _tryWPS                 = false;
    boolean       _configPortalIsBlocking = true;
    boolean       _staShowStaticFields    = false;
    boolean       _enableCaptivePortal    = true;
    boolean       _userpersistent         = true;
    boolean       _cpClientCheck          = false; // keep cp alive if cp have station
    boolean       _webClientCheck         = true; // keep cp alive if web have client

    const char*   _customHeadElement      = "";

    void          setupConfigPortal();
    void          startWPS();
    bool          startAP();

    int           status = WL_IDLE_STATUS;
    int           connectWifi(String ssid, String pass);
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
    void          handleWiFiStatus();
    void          handleRequest();

    boolean       captivePortal();
    boolean       configPortalHasTimeout();
    boolean       stopConfigPortal();
    uint8_t       handleConfigPortal();

    // wifi platform abstractions
    bool          WiFi_Mode(WiFiMode_t m);
    bool          WiFi_Mode(WiFiMode_t m,bool persistent);
    bool          WiFi_Disconnect();
    bool          WiFi_enableSTA(bool enable);
    bool          WiFi_enableSTA(bool enable,bool persistent);
    bool          WiFi_eraseConfig();
    void          debugSoftAPConfig();

    // output helpers
    String        getParamOut();
    String        getScanItemOut();
    String        getStaticOut();
    String        getHTTPHead(String title);

    //helpers
    String        getWLStatusString(uint8_t status);    
    int           getRSSIasQuality(int RSSI);
    boolean       isIp(String str);
    String        toStringIp(IPAddress ip);
    boolean       validApPassword();
    String        encryptionTypeStr(uint8_t authmode);
    void          reportStatus(String &page);
    void          debugPlatformInfo();
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
      DEBUG_WM("NO fromString METHOD ON IPAddress, you need ESP8266 core 2.1.0 or newer for Custom IP configuration to work.");
      return false;
    }
};

#endif
