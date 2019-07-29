/**
 * WiFiManager.cpp
 * 
 * WiFiManager, a library for the ESP8266/Arduino platform
 * for configuration of WiFi credentials using a Captive Portal
 * 
 * @author Creator tzapu
 * @author tablatronix
 * @version 0.0.0
 * @license MIT
 */

#include "WiFiManager.h"

#if defined(ESP8266) || defined(ESP32)

#ifdef ESP32
uint8_t WiFiManager::_lastconxresulttmp = WL_IDLE_STATUS;
#endif

/**
 * --------------------------------------------------------------------------------
 *  WiFiManagerParameter
 * --------------------------------------------------------------------------------
**/

WiFiManagerParameter::WiFiManagerParameter() {
  WiFiManagerParameter("");
}

WiFiManagerParameter::WiFiManagerParameter(const char *custom) {
  _id             = NULL;
  _label          = NULL;
  _length         = 1;
  _value          = NULL;
  _labelPlacement = WFM_LABEL_BEFORE;
  _customHTML     = custom;
}

WiFiManagerParameter::WiFiManagerParameter(const char *id, const char *label) {
  init(id, label, "", 0, "", WFM_LABEL_BEFORE);
}

WiFiManagerParameter::WiFiManagerParameter(const char *id, const char *label, const char *defaultValue, int length) {
  init(id, label, defaultValue, length, "", WFM_LABEL_BEFORE);
}

WiFiManagerParameter::WiFiManagerParameter(const char *id, const char *label, const char *defaultValue, int length, const char *custom) {
  init(id, label, defaultValue, length, custom, WFM_LABEL_BEFORE);
}

WiFiManagerParameter::WiFiManagerParameter(const char *id, const char *label, const char *defaultValue, int length, const char *custom, int labelPlacement) {
  init(id, label, defaultValue, length, custom, labelPlacement);
}

void WiFiManagerParameter::init(const char *id, const char *label, const char *defaultValue, int length, const char *custom, int labelPlacement) {
  _id             = id;
  _label          = label;
  _labelPlacement = labelPlacement;
  _customHTML     = custom;
  setValue(defaultValue,length);
}

WiFiManagerParameter::~WiFiManagerParameter() {
  if (_value != NULL) {
    delete[] _value;
  }
  _length=0; // setting length 0, ideally the entire parameter should be removed, or added to wifimanager scope so it follows
}

// @note debug is not available in wmparameter class
void WiFiManagerParameter::setValue(const char *defaultValue, int length) {
  if(!_id){
    // Serial.println("cannot set value of this parameter");
    return;
  }
  
  // if(strlen(defaultValue) > length){
  //   // Serial.println("defaultValue length mismatch");
  //   // return false; //@todo bail 
  // }

  _length = length;
  _value  = new char[_length + 1]; 
  memset(_value, 0, _length + 1); // explicit null
  
  if (defaultValue != NULL) {
    strncpy(_value, defaultValue, _length);
  }
}
const char* WiFiManagerParameter::getValue() {
  return _value;
}
const char* WiFiManagerParameter::getID() {
  return _id;
}
const char* WiFiManagerParameter::getPlaceholder() {
  return _label;
}
const char* WiFiManagerParameter::getLabel() {
  return _label;
}
int WiFiManagerParameter::getValueLength() {
  return _length;
}
int WiFiManagerParameter::getLabelPlacement() {
  return _labelPlacement;
}
const char* WiFiManagerParameter::getCustomHTML() {
  return _customHTML;
}

/**
 * [addParameter description]
 * @access public
 * @param {[type]} WiFiManagerParameter *p [description]
 */
bool WiFiManager::addParameter(WiFiManagerParameter *p) {

  // check param id is valid, unless null
  if(p->getID()){
    for (size_t i = 0; i < strlen(p->getID()); i++){
       if(!(isAlphaNumeric(p->getID()[i])) && !(p->getID()[i]=='_')){
        DEBUG_WM(DEBUG_ERROR,"[ERROR] parameter IDs can only contain alpha numeric chars");
        return false;
       }
    }
  }

  // init params if never malloc
  if(_params == NULL){
    DEBUG_WM(DEBUG_DEV,"allocating params bytes:",_max_params * sizeof(WiFiManagerParameter*));        
    _params = (WiFiManagerParameter**)malloc(_max_params * sizeof(WiFiManagerParameter*));
  }

  // resize the params array by increment of WIFI_MANAGER_MAX_PARAMS
  if(_paramsCount == _max_params){
    _max_params += WIFI_MANAGER_MAX_PARAMS;
    DEBUG_WM(DEBUG_DEV,F("Updated _max_params:"),_max_params);
    DEBUG_WM(DEBUG_DEV,"re-allocating params bytes:",_max_params * sizeof(WiFiManagerParameter*));    
    WiFiManagerParameter** new_params = (WiFiManagerParameter**)realloc(_params, _max_params * sizeof(WiFiManagerParameter*));
    // DEBUG_WM(WIFI_MANAGER_MAX_PARAMS);
    // DEBUG_WM(_paramsCount);
    // DEBUG_WM(_max_params);
    if (new_params != NULL) {
      _params = new_params;
    } else {
      DEBUG_WM(DEBUG_ERROR,"[ERROR] failed to realloc params, size not increased!");
      return false;
    }
  }

  _params[_paramsCount] = p;
  _paramsCount++;
  
  DEBUG_WM(DEBUG_VERBOSE,"Added Parameter:",p->getID());
  return true;
}

/**
 * [getParameters description]
 * @access public
 */
WiFiManagerParameter** WiFiManager::getParameters() {
  return _params;
}

/**
 * [getParametersCount description]
 * @access public
 */
int WiFiManager::getParametersCount() {
  return _paramsCount;
}

/**
 * --------------------------------------------------------------------------------
 *  WiFiManager 
 * --------------------------------------------------------------------------------
**/

// constructors
WiFiManager::WiFiManager(Stream& consolePort):_debugPort(consolePort){
  WiFiManagerInit();
}

WiFiManager::WiFiManager():WiFiManager(Serial) {
}

void WiFiManager::WiFiManagerInit(){
  setMenu(_menuIdsDefault);
  if(_debug && _debugLevel > DEBUG_DEV) debugPlatformInfo();
  _max_params = WIFI_MANAGER_MAX_PARAMS;
}

// destructor
WiFiManager::~WiFiManager() {
  _end();
  // parameters
  // @todo below belongs to wifimanagerparameter
  if (_params != NULL){
    DEBUG_WM(DEBUG_DEV,F("freeing allocated params!"));
    free(_params);
    _params = NULL;
  }

  // @todo remove event
  // #ifdef ESP32
  // WiFi.removeEvent(std::bind(&WiFiManager::WiFiEvent,this));
  // #endif

  DEBUG_WM(DEBUG_DEV,F("unloading"));
}

void WiFiManager::_begin(){
  if(_hasBegun) return;
  _hasBegun = true;
  _usermode = WiFi.getMode();

  #ifndef ESP32
  WiFi.persistent(false); // disable persistent so scannetworks and mode switching do not cause overwrites
  #endif
}

void WiFiManager::_end(){
  _hasBegun = false;
  if(_userpersistent) WiFi.persistent(true); // reenable persistent, there is no getter we rely on _userpersistent
  // if(_usermode != WIFI_OFF) WiFi.mode(_usermode);
}

// AUTOCONNECT

boolean WiFiManager::autoConnect() {
  String ssid = _wifissidprefix + "_" + String(WIFI_getChipId(),HEX);
  return autoConnect(ssid.c_str(), NULL);
}

/**
 * [autoConnect description]
 * @access public
 * @param  {[type]} char const         *apName     [description]
 * @param  {[type]} char const         *apPassword [description]
 * @return {[type]}      [description]
 */
boolean WiFiManager::autoConnect(char const *apName, char const *apPassword) {
  DEBUG_WM(F("AutoConnect"));
  _begin();

  // attempt to connect using saved settings, on fail fallback to AP config portal
  if(!WiFi.enableSTA(true)){
    // handle failure mode Brownout detector etc.
    DEBUG_WM(DEBUG_ERROR,"[FATAL] Unable to enable wifi!");
    return false;
  }
  
  WiFiSetCountry();

  #ifdef ESP32
  if(esp32persistent) WiFi.persistent(false); // disable persistent for esp32 after esp_wifi_start or else saves wont work
  #endif

  _usermode = WIFI_STA;

  // no getter for autoreconnectpolicy before this
  // https://github.com/esp8266/Arduino/pull/4359
  // so we must force it on else, if not connectimeout then waitforconnectionresult gets stuck endless loop
  WiFi_autoReconnect();

  // set hostname before stating
  if((String)_hostname != ""){
    DEBUG_WM(DEBUG_VERBOSE,"Setting hostname:",_hostname);
    bool res = true;
    #ifdef ESP8266
      res = WiFi.hostname(_hostname);
      #ifdef ESP8266MDNS_H
        DEBUG_WM(DEBUG_VERBOSE,"Setting MDNS hostname");
        if(MDNS.begin(_hostname)){
          MDNS.addService("http", "tcp", 80);
        }
      #endif
    #elif defined(ESP32)
      // @note hostname must be set after STA_START
      delay(200); // do not remove, give time for STA_START
      res = WiFi.setHostname(_hostname);
      #ifdef ESP32MDNS_H
        DEBUG_WM(DEBUG_VERBOSE,"Setting MDNS hostname");
        if(MDNS.begin(_hostname)){
          MDNS.addService("http", "tcp", 80);
        }
      #endif
    #endif
    
    if(!res)DEBUG_WM(DEBUG_ERROR,F("[ERROR] hostname: set failed!"));
    
    if(WiFi.status() == WL_CONNECTED){
      DEBUG_WM(DEBUG_VERBOSE,F("reconnecting to set new hostname"));
      // WiFi.reconnect(); // This does not reset dhcp
      WiFi_Disconnect();
      delay(200); // do not remove, need a delay for disconnect to change status()
    }
  }

  // if already connected, or try stored connect 
  // @note @todo ESP32 has no autoconnect, so connectwifi will always be called unless user called begin etc before
  // @todo check if correct ssid == saved ssid when already connected
  bool connected = false;
  if (WiFi.status() == WL_CONNECTED){
    connected = true;
    DEBUG_WM(F("AutoConnect: ESP Already Connected"));
    setSTAConfig();
  }

  if(connected || connectWifi("", "") == WL_CONNECTED){
    //connected
    DEBUG_WM(F("AutoConnect: SUCCESS"));
    DEBUG_WM(F("STA IP Address:"),WiFi.localIP());
    _lastconxresult = WL_CONNECTED;

    if((String)_hostname != ""){
      #ifdef ESP8266
        DEBUG_WM(DEBUG_DEV,"hostname: STA",WiFi.hostname());
      #elif defined(ESP32)
        DEBUG_WM(DEBUG_DEV,"hostname: STA",WiFi.getHostname());
      #endif
    }
    return true;
  }

  // possibly skip the config portal
  if (!_enableConfigPortal) {
    return false;
  }

  DEBUG_WM(F("AutoConnect: FAILED"));

  // not connected start configportal
  return startConfigPortal(apName, apPassword);
}

// CONFIG PORTAL
bool WiFiManager::startAP(){
  bool ret = true;
  DEBUG_WM(F("StartAP with SSID: "),_apName);

  #ifdef ESP8266
    // @bug workaround for bug #4372 https://github.com/esp8266/Arduino/issues/4372
    if(!WiFi.enableAP(true)) {
      DEBUG_WM(DEBUG_ERROR,"[ERROR] enableAP failed!");
      return false;
    }
    delay(500); // workaround delay
  #endif

  // setup optional soft AP static ip config
  if (_ap_static_ip) {
    DEBUG_WM(F("Custom AP IP/GW/Subnet:"));
    if(!WiFi.softAPConfig(_ap_static_ip, _ap_static_gw, _ap_static_sn)){
      DEBUG_WM(DEBUG_ERROR,"[ERROR] softAPConfig failed!");
    }
  }

  //@todo add callback here if needed to modify ap but cannot use setAPStaticIPConfig

  if(_channelSync){
    DEBUG_WM(DEBUG_VERBOSE,"Starting AP on channel:",WiFi.channel());
  }

  // start soft AP with password or anonymous
  if (_apPassword != "") {
    if(_channelSync){
      ret = WiFi.softAP(_apName.c_str(), _apPassword.c_str(),WiFi.channel());
    }  
    else{
      ret = WiFi.softAP(_apName.c_str(), _apPassword.c_str());//password option
    }
  } else {
    DEBUG_WM(DEBUG_VERBOSE,F("AP has anonymous access!"));    
    if(_channelSync){
      ret = WiFi.softAP(_apName.c_str(),"",WiFi.channel());
    }  
    else{
      ret = WiFi.softAP(_apName.c_str());
    }  
  }

  if(_debugLevel > DEBUG_DEV) debugSoftAPConfig();

  if(!ret) DEBUG_WM(DEBUG_ERROR,"[ERROR] There was a problem starting the AP");
  // @todo add softAP retry here
  
  delay(500); // slight delay to make sure we get an AP IP
  DEBUG_WM(F("AP IP address:"),WiFi.softAPIP());

  // set ap hostname
  #ifdef ESP32
    if(ret && (String)_hostname != ""){
      DEBUG_WM(DEBUG_VERBOSE,"setting softAP Hostname:",_hostname);
      bool res =  WiFi.softAPsetHostname(_hostname);
      if(!res)DEBUG_WM(DEBUG_ERROR,F("[ERROR] hostname: AP set failed!"));
      DEBUG_WM(DEBUG_DEV,F("hostname: AP"),WiFi.softAPgetHostname());
   }
  #endif

  return ret;
}

/**
 * [startWebPortal description]
 * @access public
 * @return {[type]} [description]
 */
void WiFiManager::startWebPortal() {
  if(configPortalActive || webPortalActive) return;
  setupConfigPortal();
  webPortalActive = true;
}

/**
 * [stopWebPortal description]
 * @access public
 * @return {[type]} [description]
 */
void WiFiManager::stopWebPortal() {
  if(!configPortalActive && !webPortalActive) return;
  DEBUG_WM(DEBUG_VERBOSE,F("Stopping Web Portal"));  
  webPortalActive = false;
  shutdownConfigPortal();
}

boolean WiFiManager::configPortalHasTimeout(){

    if(_configPortalTimeout == 0 || (_apClientCheck && (WiFi_softap_num_stations() > 0))){
      if(millis() - timer > 30000){
        timer = millis();
        DEBUG_WM(DEBUG_VERBOSE,"NUM CLIENTS: " + (String)WiFi_softap_num_stations());
      }
      _configPortalStart = millis(); // kludge, bump configportal start time to skew timeouts
      return false;
    }
    // handle timeout
    if(_webClientCheck && (_webPortalAccessed>_configPortalStart)>0) _configPortalStart = _webPortalAccessed;

    if(millis() > _configPortalStart + _configPortalTimeout){
      DEBUG_WM(F("config portal has timed out"));
      return true;
    } else if(_debugLevel > 0) {
      // log timeout
      if(_debug){
        uint16_t logintvl = 30000; // how often to emit timeing out counter logging
        if((millis() - timer) > logintvl){
          timer = millis();
          DEBUG_WM(DEBUG_VERBOSE,F("Portal Timeout In"),(String)((_configPortalStart + _configPortalTimeout-millis())/1000) + (String)F(" seconds"));
        }
      }
    }

    return false;
}

void WiFiManager::setupConfigPortal() {

  DEBUG_WM(F("Starting Web Portal"));

  // setup dns and web servers
  dnsServer.reset(new DNSServer());
  server.reset(new WM_WebServer(80));

  /* Setup the DNS server redirecting all the domains to the apIP */
  dnsServer->setErrorReplyCode(DNSReplyCode::NoError);
  // DEBUG_WM("dns server started port: ",DNS_PORT);
  DEBUG_WM(DEBUG_DEV,"dns server started with ip: ",WiFi.softAPIP());
  dnsServer->start(DNS_PORT, F("*"), WiFi.softAPIP());

  // @todo new callback, webserver started, callback cannot override handlers, but can grab them first

  if ( _webservercallback != NULL) {
    _webservercallback();
  }

  /* Setup httpd callbacks, web pages: root, wifi config pages, SO captive portal detectors and not found. */
  server->on(String(FPSTR(R_root)).c_str(),       std::bind(&WiFiManager::handleRoot, this));
  server->on(String(FPSTR(R_wifi)).c_str(),       std::bind(&WiFiManager::handleWifi, this, true));
  server->on(String(FPSTR(R_wifinoscan)).c_str(), std::bind(&WiFiManager::handleWifi, this, false));
  server->on(String(FPSTR(R_wifisave)).c_str(),   std::bind(&WiFiManager::handleWifiSave, this));
  server->on(String(FPSTR(R_info)).c_str(),       std::bind(&WiFiManager::handleInfo, this));
  server->on(String(FPSTR(R_param)).c_str(),      std::bind(&WiFiManager::handleParam, this));
  server->on(String(FPSTR(R_paramsave)).c_str(),  std::bind(&WiFiManager::handleParamSave, this));
  server->on(String(FPSTR(R_restart)).c_str(),    std::bind(&WiFiManager::handleReset, this));
  server->on(String(FPSTR(R_exit)).c_str(),       std::bind(&WiFiManager::handleExit, this));
  server->on(String(FPSTR(R_close)).c_str(),      std::bind(&WiFiManager::handleClose, this));
  server->on(String(FPSTR(R_erase)).c_str(),      std::bind(&WiFiManager::handleErase, this, false));
  server->on(String(FPSTR(R_status)).c_str(),     std::bind(&WiFiManager::handleWiFiStatus, this));
  server->onNotFound (std::bind(&WiFiManager::handleNotFound, this));
  
  server->begin(); // Web server start
  DEBUG_WM(DEBUG_VERBOSE,F("HTTP server started"));

  if(_preloadwifiscan) WiFi_scanNetworks(true,true); // preload wifiscan , async
}

boolean WiFiManager::startConfigPortal() {
  String ssid = _wifissidprefix + "_" + String(WIFI_getChipId(),HEX);  
  return startConfigPortal(ssid.c_str(), NULL);
}

/**
 * [startConfigPortal description]
 * @access public
 * @param  {[type]} char const         *apName     [description]
 * @param  {[type]} char const         *apPassword [description]
 * @return {[type]}      [description]
 */
boolean  WiFiManager::startConfigPortal(char const *apName, char const *apPassword) {
  _begin();

  //setup AP
  _apName     = apName; // @todo check valid apname ?
  _apPassword = apPassword;
  
  if(_apName == "") _apName = _wifissidprefix + "_" + String(WIFI_getChipId(),HEX);
  if(!validApPassword()) return false;
  
  // HANDLE issues with STA connections, shutdown sta if not connected, or else this will hang channel scanning and softap will not respond
  // @todo sometimes still cannot connect to AP for no known reason, no events in log either
  if(_disableSTA || (!WiFi.isConnected() && _disableSTAConn)){
    // this fixes most ap problems, however, simply doing mode(WIFI_AP) does not work if sta connection is hanging, must `wifi_station_disconnect` 
    WiFi_Disconnect();
    WiFi_enableSTA(false);
    DEBUG_WM(DEBUG_VERBOSE,F("Disabling STA"));
  }
  else {
    // @todo even if sta is connected, it is possible that softap connections will fail, IOS says "invalid password", windows says "cannot connect to this network" researching
    WiFi_enableSTA(true);
  }

  // init configportal globals to known states
  configPortalActive = true;
  bool result = connect = abort = false; // loop flags, connect true success, abort true break
  uint8_t state;

  _configPortalStart = millis();

  // start access point
  DEBUG_WM(DEBUG_VERBOSE,F("Enabling AP"));
  startAP();
  WiFiSetCountry();

  // do AP callback if set
  if ( _apcallback != NULL) {
    _apcallback(this);
  }

  // init configportal
  DEBUG_WM(DEBUG_DEV,F("setupConfigPortal"));
  setupConfigPortal();

  if(!_configPortalIsBlocking){
    DEBUG_WM(DEBUG_VERBOSE,F("Config Portal Running, non blocking/processing"));
    return result;
  }

  DEBUG_WM(DEBUG_VERBOSE,F("Config Portal Running, blocking, waiting for clients..."));
  // blocking loop waiting for config
  while(1){

    // if timed out or abort, break
    if(configPortalHasTimeout() || abort){
      DEBUG_WM(DEBUG_DEV,F("configportal abort"));
      shutdownConfigPortal();
      result = abort ? portalAbortResult : portalTimeoutResult; // false, false
      break;
    }

    state = processConfigPortal();

    // status change, break
    if(state != WL_IDLE_STATUS){
        result = (state == WL_CONNECTED); // true if connected
        break;
    }

    yield(); // watchdog
  }

  DEBUG_WM(DEBUG_NOTIFY,F("config portal exiting"));
  return result;
}

/**
 * [process description]
 * @access public
 * @return {[type]} [description]
 */
boolean WiFiManager::process(){
    if(webPortalActive || (configPortalActive && !_configPortalIsBlocking)){
        uint8_t state = processConfigPortal();
        return state == WL_CONNECTED;
    }
    return false;
}

//using esp enums returns for now, should be fine
uint8_t WiFiManager::processConfigPortal(){
    //DNS handler
    dnsServer->processNextRequest();
    //HTTP handler
    server->handleClient();

    // Waiting for save...
    if(connect) {
      connect = false;
      DEBUG_WM(DEBUG_VERBOSE,F("process connect"));
      if(_enableCaptivePortal) delay(_cpclosedelay); // keeps the captiveportal from closing to fast.

      // skip wifi if no ssid
      if(_ssid == ""){
        DEBUG_WM(DEBUG_VERBOSE,F("No ssid, skipping wifi"));
      }
      else{
        // attempt sta connection to submitted _ssid, _pass
        if (connectWifi(_ssid, _pass) == WL_CONNECTED) {
          
          DEBUG_WM(F("Connect to new AP [SUCCESS]"));
          DEBUG_WM(F("Got IP Address:"));
          DEBUG_WM(WiFi.localIP());

          if ( _savewificallback != NULL) {
            _savewificallback();
          }
          shutdownConfigPortal();
          return WL_CONNECTED; // CONNECT SUCCESS
        }
        DEBUG_WM(DEBUG_ERROR,F("[ERROR] Connect to new AP Failed"));
      }
 
      if (_shouldBreakAfterConfig) {
        // do save callback
        // @todo this is more of an exiting callback than a save, clarify when this should actually occur
        // confirm or verify data was saved to make this more accurate callback
        if ( _savewificallback != NULL) {
          _savewificallback();
        }
        shutdownConfigPortal();
        return WL_CONNECT_FAILED; // CONNECT FAIL
      }
      else{
        // clear save strings
        _ssid = "";
        _pass = "";
        // if connect fails, turn sta off to stabilize AP
        WiFi_Disconnect();
        WiFi_enableSTA(false);
        DEBUG_WM(DEBUG_VERBOSE,F("Disabling STA"));
      }
    }

    return WL_IDLE_STATUS;
}

/**
 * [shutdownConfigPortal description]
 * @access public
 * @return bool success (softapdisconnect)
 */
bool WiFiManager::shutdownConfigPortal(){
  if(webPortalActive) return false;

  //DNS handler
  dnsServer->processNextRequest();
  //HTTP handler
  server->handleClient();

  // @todo what is the proper way to shutdown and free the server up
  server->stop();
  server.reset();
  dnsServer->stop(); //  free heap ?
  dnsServer.reset();

  WiFi.scanDelete(); // free wifi scan results

  if(!configPortalActive) return false;

  // turn off AP
  // @todo bug workaround
  // https://github.com/esp8266/Arduino/issues/3793
  // [APdisconnect] set_config failed! *WM: disconnect configportal - softAPdisconnect failed
  // still no way to reproduce reliably
  DEBUG_WM(DEBUG_VERBOSE,F("disconnect configportal"));
  bool ret = false;
  ret = WiFi.softAPdisconnect(false);
  if(!ret)DEBUG_WM(DEBUG_ERROR,F("[ERROR] disconnect configportal - softAPdisconnect FAILED"));
  delay(1000);
  DEBUG_WM(DEBUG_VERBOSE,"restoring usermode",getModeString(_usermode));
  WiFi_Mode(_usermode); // restore users wifi mode, BUG https://github.com/esp8266/Arduino/issues/4372
  if(WiFi.status()==WL_IDLE_STATUS){
    WiFi.reconnect(); // restart wifi since we disconnected it in startconfigportal
    DEBUG_WM(DEBUG_VERBOSE,"WiFi Reconnect, was idle");
  }
  DEBUG_WM(DEBUG_VERBOSE,"wifi status:",getWLStatusString(WiFi.status()));
  DEBUG_WM(DEBUG_VERBOSE,"wifi mode:",getModeString(WiFi.getMode()));
  configPortalActive = false;
  _end();
  return ret;
}

// @todo refactor this up into seperate functions
// one for connecting to flash , one for new client
// clean up, flow is convoluted, and causes bugs
uint8_t WiFiManager::connectWifi(String ssid, String pass) {
  DEBUG_WM(DEBUG_VERBOSE,F("Connecting as wifi client..."));
  
  uint8_t connRes = (uint8_t)WL_NO_SSID_AVAIL;

  setSTAConfig();
  //@todo catch failures in set_config
  
  // make sure sta is on before `begin` so it does not call enablesta->mode while persistent is ON ( which would save WM AP state to eeprom !)
  if(_cleanConnect) WiFi_Disconnect(); // disconnect before begin, in case anything is hung, this causes a 2 seconds delay for connect
  // @todo find out what status is when this is needed, can we detect it and handle it, say in between states or idle_status

  // if ssid argument provided connect to that
  if (ssid != "") {
    wifiConnectNew(ssid,pass);
    if(_saveTimeout > 0){
      connRes = waitForConnectResult(_saveTimeout); // use default save timeout for saves to prevent bugs in esp->waitforconnectresult loop
    }  
    else {
       connRes = waitForConnectResult(0);
    }
  }
  else {
    // connect using saved ssid if there is one
    if (WiFi_hasAutoConnect()) {
      wifiConnectDefault();
      connRes = waitForConnectResult();
    }
    else {
      DEBUG_WM(F("No saved credentials, skipping wifi"));
    }
  }

  DEBUG_WM(DEBUG_VERBOSE,F("Connection result:"),getWLStatusString(connRes));

// WPS enabled? https://github.com/esp8266/Arduino/pull/4889
#ifdef NO_EXTRA_4K_HEAP
  // do WPS, if WPS options enabled and not connected and no password was supplied
  // @todo this seems like wrong place for this, is it a fallback or option?
  if (_tryWPS && connRes != WL_CONNECTED && pass == "") {
    startWPS();
    // should be connected at the end of WPS
    connRes = waitForConnectResult();
  }
#endif

  if(connRes != WL_SCAN_COMPLETED){
    updateConxResult(connRes);
  }

  return connRes;
}

/**
 * connect to a new wifi ap
 * @since $dev
 * @param  String ssid 
 * @param  String pass 
 * @return bool success
 */
bool WiFiManager::wifiConnectNew(String ssid, String pass){
  bool ret = false;
  DEBUG_WM(F("Connecting to new AP:"),ssid);
  DEBUG_WM(DEBUG_DEV,F("Using Password:"),pass);
  WiFi_enableSTA(true,storeSTAmode); // storeSTAmode will also toggle STA on in default opmode (persistent) if true (default)
  WiFi.persistent(true);
  ret = WiFi.begin(ssid.c_str(), pass.c_str());
  WiFi.persistent(false);
  if(!ret) DEBUG_WM(DEBUG_ERROR,"[ERROR] wifi begin failed");
  return ret;
}

/**
 * connect to stored wifi
 * @since dev
 * @return bool success
 */
bool WiFiManager::wifiConnectDefault(){
  bool ret = false;
  DEBUG_WM(F("Connecting to saved AP:"),WiFi_SSID());
  DEBUG_WM(DEBUG_DEV,F("Using Password:"),WiFi.psk());
  ret = WiFi_enableSTA(true,storeSTAmode);
  if(!ret) DEBUG_WM(DEBUG_ERROR,"[ERROR] wifi enableSta failed");
  ret = WiFi.begin();
  if(!ret) DEBUG_WM(DEBUG_ERROR,"[ERROR] wifi begin failed");
  return ret;
}


/**
 * set sta config if set
 * @since $dev
 * @return bool success
 */
bool WiFiManager::setSTAConfig(){
  DEBUG_WM(F("STA static IP:"),_sta_static_ip);  
  bool ret = true;
  if (_sta_static_ip) {
      DEBUG_WM(DEBUG_VERBOSE,F("Custom static IP/GW/Subnet/DNS"));
    if(_sta_static_dns) {
      DEBUG_WM(DEBUG_VERBOSE,F("Custom static DNS"));
      ret = WiFi.config(_sta_static_ip, _sta_static_gw, _sta_static_sn, _sta_static_dns);
    }
    else {
      DEBUG_WM(DEBUG_VERBOSE,F("Custom STA IP/GW/Subnet"));
      ret = WiFi.config(_sta_static_ip, _sta_static_gw, _sta_static_sn);
    }

    if(!ret) DEBUG_WM(DEBUG_ERROR,"[ERROR] wifi config failed");
    else DEBUG_WM(F("STA IP set:"),WiFi.localIP());
  } else {
      DEBUG_WM(DEBUG_VERBOSE,F("setSTAConfig static ip not set"));
  }
  return ret;
}

// @todo change to getLastFailureReason and do not touch conxresult
void WiFiManager::updateConxResult(uint8_t status){
  // hack in wrong password detection
  _lastconxresult = status;
    #ifdef ESP8266
      if(_lastconxresult == WL_CONNECT_FAILED){
        if(wifi_station_get_connect_status() == STATION_WRONG_PASSWORD){
          _lastconxresult = WL_STATION_WRONG_PASSWORD;
        }
      }
    #elif defined(ESP32)
      // if(_lastconxresult == WL_CONNECT_FAILED){
      if(_lastconxresult == WL_CONNECT_FAILED || _lastconxresult == WL_DISCONNECTED){
        DEBUG_WM(DEBUG_DEV,"lastconxresulttmp:",getWLStatusString(_lastconxresulttmp));            
        if(_lastconxresulttmp != WL_IDLE_STATUS){
          _lastconxresult    = _lastconxresulttmp;
          // _lastconxresulttmp = WL_IDLE_STATUS;
        }
      }
    #endif
    DEBUG_WM(DEBUG_DEV,"lastconxresult:",getWLStatusString(_lastconxresult));
}

 
uint8_t WiFiManager::waitForConnectResult() {
  if(_connectTimeout > 0) DEBUG_WM(DEBUG_VERBOSE,_connectTimeout,F("ms connectTimeout set")); 
  return waitForConnectResult(_connectTimeout);
}

/**
 * waitForConnectResult
 * @param  uint16_t timeout  in seconds
 * @return uint8_t  WL Status
 */
uint8_t WiFiManager::waitForConnectResult(uint16_t timeout) {
  if (timeout == 0){
    DEBUG_WM(F("connectTimeout not set, ESP waitForConnectResult..."));
    return WiFi.waitForConnectResult();
  }

  unsigned long timeoutmillis = millis() + timeout;
  DEBUG_WM(DEBUG_VERBOSE,timeout,F("ms timeout, waiting for connect..."));
  uint8_t status = WiFi.status();
  
  while(millis() < timeoutmillis) {
    status = WiFi.status();
    // @todo detect additional states, connect happens, then dhcp then get ip, there is some delay here, make sure not to timeout if waiting on IP
    if (status == WL_CONNECTED || status == WL_CONNECT_FAILED) {
      return status;
    }
    DEBUG_WM (DEBUG_VERBOSE,F("."));
    delay(100);
  }
  return status;
}

// WPS enabled? https://github.com/esp8266/Arduino/pull/4889
#ifdef NO_EXTRA_4K_HEAP
void WiFiManager::startWPS() {
  DEBUG_WM(F("START WPS"));
  #ifdef ESP8266  
    WiFi.beginWPSConfig();
  #else
    // @todo
  #endif
  DEBUG_WM(F("END WPS"));
}
#endif

String WiFiManager::getHTTPHead(String title){
  String page;
  page += FPSTR(HTTP_HEAD_START);
  page.replace(FPSTR(T_v), title);
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_STYLE);
  page += _customHeadElement;

  if(_bodyClass != ""){
    String p = FPSTR(HTTP_HEAD_END);
    p.replace(FPSTR(T_c), _bodyClass); // add class str
    page += p;
  }
  else {
    page += FPSTR(HTTP_HEAD_END);
  } 

  return page;
}

/** 
 * HTTPD handler for page requests
 */
void WiFiManager::handleRequest() {
  _webPortalAccessed = millis();
}

/** 
 * HTTPD CALLBACK root or redirect to captive portal
 */
void WiFiManager::handleRoot() {
  DEBUG_WM(DEBUG_VERBOSE,F("<- HTTP Root"));
  if (captivePortal()) return; // If captive portal redirect instead of displaying the page
  handleRequest();
  String page = getHTTPHead(FPSTR(S_options)); // @token options
  String str  = FPSTR(HTTP_ROOT_MAIN);
  str.replace(FPSTR(T_v),configPortalActive ? _apName : WiFi.localIP().toString()); // use ip if ap is not active for heading
  page += str;
  page += FPSTR(HTTP_PORTAL_OPTIONS);
  page += getMenuOut();
  reportStatus(page);
  page += FPSTR(HTTP_END);

  server->sendHeader(FPSTR(HTTP_HEAD_CL), String(page.length()));
  server->send(200, FPSTR(HTTP_HEAD_CT), page);
  // server->close(); // testing reliability fix for content length mismatches during mutiple flood hits  WiFi_scanNetworks(); // preload wifiscan 
  if(_preloadwifiscan) WiFi_scanNetworks((unsigned)20000,true); // preload wifiscan throttled, async
  // @todo buggy, captive portals make a query on every page load, causing this to run every time in addition to the real page load
  // I dont understand why, when you are already in the captive portal, I guess they want to know that its still up and not done or gone
  // if we can detect these and ignore them that would be great, since they come from the captive portal redirect maybe there is a refferer
}

/**
 * HTTPD CALLBACK Wifi config page handler
 */
void WiFiManager::handleWifi(boolean scan) {
  DEBUG_WM(DEBUG_VERBOSE,F("<- HTTP Wifi"));
  handleRequest();
  String page = getHTTPHead(FPSTR(S_titlewifi)); // @token titlewifi
  if (scan) {
    // DEBUG_WM(DEBUG_DEV,"refresh flag:",server->hasArg(F("refresh")));
    WiFi_scanNetworks(server->hasArg(F("refresh")),false); //wifiscan, force if arg refresh
    page += getScanItemOut();
  }
  String pitem = "";

  pitem = FPSTR(HTTP_FORM_START);
  pitem.replace(FPSTR(T_v), F("wifisave")); // set form action
  page += pitem;

  pitem = FPSTR(HTTP_FORM_WIFI);
  pitem.replace(FPSTR(T_v), WiFi_SSID());
  page += pitem;

  page += getStaticOut();
  page += FPSTR(HTTP_FORM_WIFI_END);
  if(_paramsInWifi && _paramsCount>0){
    page += FPSTR(HTTP_FORM_PARAM_HEAD);
    page += getParamOut();
  }
  page += FPSTR(HTTP_FORM_END);
  page += FPSTR(HTTP_SCAN_LINK);
  reportStatus(page);
  page += FPSTR(HTTP_END);

  server->sendHeader(FPSTR(HTTP_HEAD_CL), String(page.length()));
  server->send(200, FPSTR(HTTP_HEAD_CT), page);
  // server->close(); // testing reliability fix for content length mismatches during mutiple flood hits

  DEBUG_WM(DEBUG_DEV,F("Sent config page"));
}

/**
 * HTTPD CALLBACK Wifi param page handler
 */
void WiFiManager::handleParam(){
  DEBUG_WM(DEBUG_VERBOSE,F("<- HTTP Param"));
  handleRequest();
  String page = getHTTPHead(FPSTR(S_titleparam)); // @token titlewifi

  String pitem = "";

  pitem = FPSTR(HTTP_FORM_START);
  pitem.replace(FPSTR(T_v), F("paramsave"));
  page += pitem;

  page += getParamOut();
  page += FPSTR(HTTP_FORM_END);
  reportStatus(page);
  page += FPSTR(HTTP_END);

  server->sendHeader(FPSTR(HTTP_HEAD_CL), String(page.length()));
  server->send(200, FPSTR(HTTP_HEAD_CT), page);

  DEBUG_WM(DEBUG_DEV,F("Sent param page"));
}


String WiFiManager::getMenuOut(){
  String page;  

  for(auto menuId :_menuIds ){
    if(((String)menuId == "param") && (_paramsCount == 0)) continue; // no params set, omit params
    page += HTTP_PORTAL_MENU[menuId];
  }

  return page;
}

// // is it possible in softap mode to detect aps without scanning
// bool WiFiManager::WiFi_scanNetworksForAP(bool force){
//   WiFi_scanNetworks(force);
// }

void WiFiManager::WiFi_scanComplete(int networksFound){
  _lastscan = millis();
  _numNetworks = networksFound;
  DEBUG_WM(DEBUG_VERBOSE,F("WiFi Scan ASYNC completed"), "in "+(String)(_lastscan - _startscan)+" ms");  
  DEBUG_WM(DEBUG_VERBOSE,F("WiFi Scan ASYNC found:"),_numNetworks);
}

bool WiFiManager::WiFi_scanNetworks(){
  return WiFi_scanNetworks(false,false);
}

bool WiFiManager::WiFi_scanNetworks(unsigned int cachetime,bool async){
    return WiFi_scanNetworks(millis()-_lastscan > cachetime,async);
}
bool WiFiManager::WiFi_scanNetworks(unsigned int cachetime){
    return WiFi_scanNetworks(millis()-_lastscan > cachetime,false);
}
bool WiFiManager::WiFi_scanNetworks(bool force,bool async){
    // DEBUG_WM(DEBUG_DEV,"scanNetworks async:",async == true);
    // DEBUG_WM(DEBUG_DEV,_numNetworks,(millis()-_lastscan ));
    // DEBUG_WM(DEBUG_DEV,"scanNetworks force:",force == true);
    if(force || _numNetworks == 0 || (millis()-_lastscan > 60000)){
      int8_t res;
      _startscan = millis();
      if(async){
        #ifdef ESP8266
          #ifndef WM_NOASYNC // no async available < 2.4.0
          DEBUG_WM(DEBUG_VERBOSE,F("WiFi Scan ASYNC started"));
          using namespace std::placeholders; // for `_1`
          WiFi.scanNetworksAsync(std::bind(&WiFiManager::WiFi_scanComplete,this,_1));
          #else
          res = WiFi.scanNetworks();
          #endif
        #else
          DEBUG_WM(DEBUG_VERBOSE,F("WiFi Scan ASYNC started"));
          res = WiFi.scanNetworks(true);
        #endif
        return false;
      }
      else{
        res = WiFi.scanNetworks();
      }
      if(res == WIFI_SCAN_FAILED) DEBUG_WM(DEBUG_ERROR,"[ERROR] scan failed");
      else if(res == WIFI_SCAN_RUNNING){
        DEBUG_WM(DEBUG_ERROR,"[ERROR] scan waiting");
        while(WiFi.scanComplete() == WIFI_SCAN_RUNNING){
          DEBUG_WM(DEBUG_ERROR,".");
          delay(100);
        }
        _numNetworks = WiFi.scanComplete();
      }
      else if(res >=0 ) _numNetworks = res;
      _lastscan = millis();
      DEBUG_WM(DEBUG_VERBOSE,F("WiFi Scan completed"), "in "+(String)(_lastscan - _startscan)+" ms");
      return true;
    } else DEBUG_WM(DEBUG_VERBOSE,"Scan is cached",(String)(millis()-_lastscan )+" ms ago");
    return false;
}

String WiFiManager::WiFiManager::getScanItemOut(){
    String page;

    if(!_numNetworks) WiFi_scanNetworks(); // scan in case this gets called before any scans

    int n = _numNetworks;
    if (n == 0) {
      DEBUG_WM(F("No networks found"));
      page += FPSTR(S_nonetworks); // @token nonetworks
    } 
    else {
      DEBUG_WM(n,F("networks found"));
      //sort networks
      int indices[n];
      for (int i = 0; i < n; i++) {
        indices[i] = i;
      }

      // RSSI SORT
      for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
          if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
            std::swap(indices[i], indices[j]);
          }
        }
      }

      /* test std:sort
        std::sort(indices, indices + n, [](const int & a, const int & b) -> bool
        {
        return WiFi.RSSI(a) > WiFi.RSSI(b);
        });
       */

      // remove duplicates ( must be RSSI sorted )
      if (_removeDuplicateAPs) {
        String cssid;
        for (int i = 0; i < n; i++) {
          if (indices[i] == -1) continue;
          cssid = WiFi.SSID(indices[i]);
          for (int j = i + 1; j < n; j++) {
            if (cssid == WiFi.SSID(indices[j])) {
              DEBUG_WM(DEBUG_VERBOSE,F("DUP AP:"),WiFi.SSID(indices[j]));
              indices[j] = -1; // set dup aps to index -1
            }
          }
        }
      }

      // token precheck, to speed up replacements on large ap lists
      String HTTP_ITEM_STR = FPSTR(HTTP_ITEM);

      // toggle icons with percentage
      HTTP_ITEM_STR.replace("{qp}", FPSTR(HTTP_ITEM_QP));
      HTTP_ITEM_STR.replace("{h}",_scanDispOptions ? "" : "h");
      HTTP_ITEM_STR.replace("{qi}", FPSTR(HTTP_ITEM_QI));
      HTTP_ITEM_STR.replace("{h}",_scanDispOptions ? "h" : "");
 
      // set token precheck flags
      bool tok_r = HTTP_ITEM_STR.indexOf(FPSTR(T_r)) > 0;
      bool tok_R = HTTP_ITEM_STR.indexOf(FPSTR(T_R)) > 0;
      bool tok_e = HTTP_ITEM_STR.indexOf(FPSTR(T_e)) > 0;
      bool tok_q = HTTP_ITEM_STR.indexOf(FPSTR(T_q)) > 0;
      bool tok_i = HTTP_ITEM_STR.indexOf(FPSTR(T_i)) > 0;
      
      //display networks in page
      for (int i = 0; i < n; i++) {
        if (indices[i] == -1) continue; // skip dups

        DEBUG_WM(DEBUG_VERBOSE,F("AP: "),(String)WiFi.RSSI(indices[i]) + " " + (String)WiFi.SSID(indices[i]));

        int rssiperc = getRSSIasQuality(WiFi.RSSI(indices[i]));
        uint8_t enc_type = WiFi.encryptionType(indices[i]);

        if (_minimumQuality == -1 || _minimumQuality < rssiperc) {
          String item = HTTP_ITEM_STR;
          item.replace(FPSTR(T_v), htmlEntities(WiFi.SSID(indices[i]))); // ssid no encoding
          if(tok_e) item.replace(FPSTR(T_e), encryptionTypeStr(enc_type));
          if(tok_r) item.replace(FPSTR(T_r), (String)rssiperc); // rssi percentage 0-100
          if(tok_R) item.replace(FPSTR(T_R), (String)WiFi.RSSI(indices[i])); // rssi db
          if(tok_q) item.replace(FPSTR(T_q), (String)int(round(map(rssiperc,0,100,1,4)))); //quality icon 1-4
          if(tok_i){
            if (enc_type != WM_WIFIOPEN) {
              item.replace(FPSTR(T_i), F("l"));
            } else {
              item.replace(FPSTR(T_i), "");
            }
          }
          //DEBUG_WM(item);
          page += item;
          delay(0);
        } else {
          DEBUG_WM(DEBUG_VERBOSE,F("Skipping , does not meet _minimumQuality"));
        }

      }
      page += FPSTR(HTTP_BR);
    }

    return page;
}

String WiFiManager::getIpForm(String id, String title, String value){
    String item = FPSTR(HTTP_FORM_LABEL);
    item += FPSTR(HTTP_FORM_PARAM);
    item.replace(FPSTR(T_i), id);
    item.replace(FPSTR(T_n), id);
    item.replace(FPSTR(T_p), FPSTR(T_t));
    // item.replace(FPSTR(T_p), default);
    item.replace(FPSTR(T_t), title);
    item.replace(FPSTR(T_l), F("15"));
    item.replace(FPSTR(T_v), value);
    item.replace(FPSTR(T_c), "");
    return item;  
}

String WiFiManager::getStaticOut(){
  String page;
  if ((_staShowStaticFields || _sta_static_ip) && _staShowStaticFields>=0) {
    DEBUG_WM(DEBUG_DEV,"_staShowStaticFields");
    page += FPSTR(HTTP_FORM_STATIC_HEAD);
    // @todo how can we get these accurate settings from memory , wifi_get_ip_info does not seem to reveal if struct ip_info is static or not
    page += getIpForm(FPSTR(S_ip),FPSTR(S_staticip),(_sta_static_ip ? _sta_static_ip.toString() : "")); // @token staticip
    // WiFi.localIP().toString();
    page += getIpForm(FPSTR(S_gw),FPSTR(S_staticgw),(_sta_static_gw ? _sta_static_gw.toString() : "")); // @token staticgw
    // WiFi.gatewayIP().toString();
    page += getIpForm(FPSTR(S_sn),FPSTR(S_subnet),(_sta_static_sn ? _sta_static_sn.toString() : "")); // @token subnet
    // WiFi.subnetMask().toString();
  }

  if((_staShowDns || _sta_static_dns) && _staShowDns>=0){
    page += getIpForm(FPSTR(S_dns),FPSTR(S_staticdns),(_sta_static_dns ? _sta_static_dns.toString() : "")); // @token dns
  }

  if(page!="") page += FPSTR(HTTP_BR); // @todo remove these, use css

  return page;
}

String WiFiManager::getParamOut(){
  String page;

  if(_paramsCount > 0){

    String HTTP_PARAM_temp = FPSTR(HTTP_FORM_LABEL);
    HTTP_PARAM_temp += FPSTR(HTTP_FORM_PARAM);
    bool tok_I = HTTP_PARAM_temp.indexOf(FPSTR(T_I)) > 0;
    bool tok_i = HTTP_PARAM_temp.indexOf(FPSTR(T_i)) > 0;
    bool tok_n = HTTP_PARAM_temp.indexOf(FPSTR(T_n)) > 0;
    bool tok_p = HTTP_PARAM_temp.indexOf(FPSTR(T_p)) > 0;
    bool tok_t = HTTP_PARAM_temp.indexOf(FPSTR(T_t)) > 0;
    bool tok_l = HTTP_PARAM_temp.indexOf(FPSTR(T_l)) > 0;
    bool tok_v = HTTP_PARAM_temp.indexOf(FPSTR(T_v)) > 0;
    bool tok_c = HTTP_PARAM_temp.indexOf(FPSTR(T_c)) > 0;

    char valLength[5];
    // add the extra parameters to the form
    for (int i = 0; i < _paramsCount; i++) {
      if (_params[i] == NULL || _params[i]->_length == 0) {
        DEBUG_WM(DEBUG_ERROR,"[ERROR] WiFiManagerParameter is out of scope");
        break;
      }

     // label before or after, @todo this could be done via floats or CSS and eliminated
     String pitem;
      switch (_params[i]->getLabelPlacement()) {
        case WFM_LABEL_BEFORE:
          pitem = FPSTR(HTTP_FORM_LABEL);
          pitem += FPSTR(HTTP_FORM_PARAM);
          break;
        case WFM_LABEL_AFTER:
          pitem = FPSTR(HTTP_FORM_PARAM);
          pitem += FPSTR(HTTP_FORM_LABEL);
          break;
        default:
          // WFM_NO_LABEL
          pitem = FPSTR(HTTP_FORM_PARAM);
          break;
      }

      // Input templating
      // "<br/><input id='{i}' name='{n}' maxlength='{l}' value='{v}' {c}>";
      // if no ID use customhtml for item, else generate from param string
      if (_params[i]->getID() != NULL) {
        if(tok_I)pitem.replace(FPSTR(T_I), (String)FPSTR(S_parampre)+(String)i); // T_I id number
        if(tok_i)pitem.replace(FPSTR(T_i), _params[i]->getID()); // T_i id name
        if(tok_n)pitem.replace(FPSTR(T_n), _params[i]->getID()); // T_n id name alias
        if(tok_p)pitem.replace(FPSTR(T_p), FPSTR(T_t)); // T_p replace legacy placeholder token
        if(tok_t)pitem.replace(FPSTR(T_t), _params[i]->getLabel()); // T_t title/label
        snprintf(valLength, 5, "%d", _params[i]->getValueLength());
        if(tok_l)pitem.replace(FPSTR(T_l), valLength); // T_l value length
        if(tok_v)pitem.replace(FPSTR(T_v), _params[i]->getValue()); // T_v value
        if(tok_c)pitem.replace(FPSTR(T_c), _params[i]->getCustomHTML()); // T_c meant for additional attributes, not html, but can stuff
      } else {
        pitem = _params[i]->getCustomHTML();
      }

      page += pitem;
    }
  }

  return page;
}

void WiFiManager::handleWiFiStatus(){
  DEBUG_WM(DEBUG_VERBOSE,F("<- HTTP WiFi status "));
  handleRequest();
  String page;
  // String page = "{\"result\":true,\"count\":1}";
  #ifdef JSTEST
    page = FPSTR(HTTP_JS);
  #endif
  server->sendHeader(FPSTR(HTTP_HEAD_CL), String(page.length()));
  server->send(200, FPSTR(HTTP_HEAD_CT), page);
}

/** 
 * HTTPD CALLBACK save form and redirect to WLAN config page again
 */
void WiFiManager::handleWifiSave() {
  DEBUG_WM(DEBUG_VERBOSE,F("<- HTTP WiFi save "));
  DEBUG_WM(DEBUG_DEV,F("Method:"),server->method() == HTTP_GET  ? (String)FPSTR(S_GET) : (String)FPSTR(S_POST));
  handleRequest();

 // @todo use new callback for before paramsaves
  if ( _presavecallback != NULL) {
    _presavecallback();
  }

  //SAVE/connect here
  _ssid = server->arg(F("s")).c_str();
  _pass = server->arg(F("p")).c_str();

  if(_paramsInWifi) doParamSave();

  if (server->arg(FPSTR(S_ip)) != "") {
    //_sta_static_ip.fromString(server->arg(FPSTR(S_ip));
    String ip = server->arg(FPSTR(S_ip));
    optionalIPFromString(&_sta_static_ip, ip.c_str());
    DEBUG_WM(DEBUG_DEV,F("static ip:"),ip);
  }
  if (server->arg(FPSTR(S_gw)) != "") {
    String gw = server->arg(FPSTR(S_gw));
    optionalIPFromString(&_sta_static_gw, gw.c_str());
    DEBUG_WM(DEBUG_DEV,F("static gateway:"),gw);
  }
  if (server->arg(FPSTR(S_sn)) != "") {
    String sn = server->arg(FPSTR(S_sn));
    optionalIPFromString(&_sta_static_sn, sn.c_str());
    DEBUG_WM(DEBUG_DEV,F("static netmask:"),sn);
  }
  if (server->arg(FPSTR(S_dns)) != "") {
    String dns = server->arg(FPSTR(S_dns));
    optionalIPFromString(&_sta_static_dns, dns.c_str());
    DEBUG_WM(DEBUG_DEV,F("static DNS:"),dns);
  }

  String page = getHTTPHead(FPSTR(S_titlewifisaved)); // @token titlewifisaved
  page += FPSTR(HTTP_SAVED);
  page += FPSTR(HTTP_END);

  server->sendHeader(FPSTR(HTTP_HEAD_CL), String(page.length()));
  server->sendHeader(FPSTR(HTTP_HEAD_CORS), FPSTR(HTTP_HEAD_CORS_ALLOW_ALL));
  server->send(200, FPSTR(HTTP_HEAD_CT), page);

  DEBUG_WM(DEBUG_DEV,F("Sent wifi save page"));

  connect = true; //signal ready to connect/reset process in processConfigPortal
}

void WiFiManager::handleParamSave() {

  DEBUG_WM(DEBUG_VERBOSE,F("<- HTTP WiFi save "));
  DEBUG_WM(DEBUG_DEV,F("Method:"),server->method() == HTTP_GET  ? (String)FPSTR(S_GET) : (String)FPSTR(S_POST));
  handleRequest();

  doParamSave();

  String page = getHTTPHead(FPSTR(S_titleparamsaved)); // @token titleparamsaved
  page += FPSTR(HTTP_PARAMSAVED);
  page += FPSTR(HTTP_END);

  server->sendHeader(FPSTR(HTTP_HEAD_CL), String(page.length()));
  server->send(200, FPSTR(HTTP_HEAD_CT), page);

  DEBUG_WM(DEBUG_DEV,F("Sent param save page"));
}

void WiFiManager::doParamSave(){
   // @todo use new callback for before paramsaves, is this really needed?
  if ( _presavecallback != NULL) {
    _presavecallback();
  }

  //parameters
  if(_paramsCount > 0){
    DEBUG_WM(DEBUG_VERBOSE,F("Parameters"));
    DEBUG_WM(DEBUG_VERBOSE,FPSTR(D_HR));
    for (int i = 0; i < _paramsCount; i++) {
      if (_params[i] == NULL) {
        break; // @todo might not be needed anymore
      }
      //read parameter from server
      String name = (String)FPSTR(S_parampre)+(String)i;
      String value;
      if(server->hasArg(name)) {
        value = server->arg(name);
      } else {
        value = server->arg(_params[i]->getID());
      }

      //store it in params array
      value.toCharArray(_params[i]->_value, _params[i]->_length+1); // length+1 null terminated
      DEBUG_WM(DEBUG_VERBOSE,(String)_params[i]->getID() + ":",value);
    }
    DEBUG_WM(DEBUG_VERBOSE,FPSTR(D_HR));
  }

   if ( _saveparamscallback != NULL) {
    _saveparamscallback();
  }
   
}

/** 
 * HTTPD CALLBACK info page
 */
void WiFiManager::handleInfo() {
  DEBUG_WM(DEBUG_VERBOSE,F("<- HTTP Info"));
  handleRequest();
  String page = getHTTPHead(FPSTR(S_titleinfo)); // @token titleinfo
  reportStatus(page);

  uint16_t infos = 0;

  //@todo convert to enum or refactor to strings
  #ifdef ESP8266
    infos = 27;
    String infoids[] = {
      F("esphead"),
      F("uptime"),
      F("chipid"),
      F("fchipid"),
      F("idesize"),
      F("flashsize"),
      F("sdkver"),
      F("corever"),
      F("bootver"),
      F("cpufreq"),
      F("freeheap"),
      F("memsketch"),
      F("memsmeter"),
      F("lastreset"),
      F("wifihead"),
      F("apip"),
      F("apmac"),
      F("apssid"),
      F("apbssid"),
      F("staip"),
      F("stagw"),
      F("stasub"),
      F("dnss"),
      F("host"),
      F("stamac"),
      F("conx"),
      F("autoconx")
    };

  #elif defined(ESP32)
    infos = 22;
    String infoids[] = {
      F("esphead"),
      F("uptime"),
      F("chipid"),
      F("chiprev"),
      F("idesize"),
      F("sdkver"),
      F("cpufreq"),
      F("freeheap"),
      F("lastreset"),
      // F("temp"),
      F("wifihead"),
      F("apip"),
      F("apmac"),
      F("aphost"),
      F("apssid"),
      F("apbssid"),
      F("staip"),
      F("stagw"),
      F("stasub"),
      F("dnss"),
      F("host"),
      F("stamac"),
      F("conx")
    };
  #endif

  for(size_t i=0; i<infos;i++){
    if(infoids[i] != NULL) page += getInfoData(infoids[i]);
  }
  page += F("</dl>");
  if(_showInfoErase) page += FPSTR(HTTP_ERASEBTN);
  page += FPSTR(HTTP_HELP);
  page += FPSTR(HTTP_END);

  server->sendHeader(FPSTR(HTTP_HEAD_CL), String(page.length()));
  server->send(200, FPSTR(HTTP_HEAD_CT), page);

  DEBUG_WM(DEBUG_DEV,F("Sent info page"));
}

String WiFiManager::getInfoData(String id){

  String p;
  // @todo add versioning
  if(id==F("esphead"))p = FPSTR(HTTP_INFO_esphead);
  else if(id==F("wifihead"))p = FPSTR(HTTP_INFO_wifihead);
  else if(id==F("uptime")){
    // subject to rollover!
    p = FPSTR(HTTP_INFO_uptime);
    p.replace(FPSTR(T_1),(String)(millis() / 1000 / 60));
    p.replace(FPSTR(T_2),(String)((millis() / 1000) % 60));
  }
  else if(id==F("chipid")){
    p = FPSTR(HTTP_INFO_chipid);
    p.replace(FPSTR(T_1),String(WIFI_getChipId(),HEX));
  }
  #ifdef ESP32
  else if(id==F("chiprev")){
      p = FPSTR(HTTP_INFO_chiprev);
      String rev = (String)ESP.getChipRevision();
      #ifdef _SOC_EFUSE_REG_H_
        String revb = (String)(REG_READ(EFUSE_BLK0_RDATA3_REG) >> (EFUSE_RD_CHIP_VER_RESERVE_S)&&EFUSE_RD_CHIP_VER_RESERVE_V);
        p.replace(FPSTR(T_1),rev+"<br/>"+revb);
      #else
        p.replace(FPSTR(T_1),rev);
      #endif
  }
  #endif
  #ifdef ESP8266
  else if(id==F("fchipid")){
      p = FPSTR(HTTP_INFO_fchipid);
      p.replace(FPSTR(T_1),(String)ESP.getFlashChipId());
  }
  #endif
  else if(id==F("idesize")){
    p = FPSTR(HTTP_INFO_idesize);
    p.replace(FPSTR(T_1),(String)ESP.getFlashChipSize());
  }
  else if(id==F("flashsize")){
    #ifdef ESP8266
      p = FPSTR(HTTP_INFO_flashsize);
      p.replace(FPSTR(T_1),(String)ESP.getFlashChipRealSize());
    #endif
  }
  else if(id==F("sdkver")){
    p = FPSTR(HTTP_INFO_sdkver);
    #ifdef ESP32
      p.replace(FPSTR(T_1),(String)esp_get_idf_version());
    #else
    p.replace(FPSTR(T_1),(String)system_get_sdk_version());
    #endif
  }
  else if(id==F("corever")){
    #ifdef ESP8266
      p = FPSTR(HTTP_INFO_corever);
      p.replace(FPSTR(T_1),(String)ESP.getCoreVersion());
    #endif      
  }
  #ifdef ESP8266
  else if(id==F("bootver")){
      p = FPSTR(HTTP_INFO_bootver);
      p.replace(FPSTR(T_1),(String)system_get_boot_version());
  }
  #endif  
  else if(id==F("cpufreq")){
    p = FPSTR(HTTP_INFO_cpufreq);
    p.replace(FPSTR(T_1),(String)ESP.getCpuFreqMHz());
  }
  else if(id==F("freeheap")){
    p = FPSTR(HTTP_INFO_freeheap);
    p.replace(FPSTR(T_1),(String)ESP.getFreeHeap());
  }
  #ifdef ESP8266
  else if(id==F("memsketch")){
    p = FPSTR(HTTP_INFO_memsketch);
    p.replace(FPSTR(T_1),(String)(ESP.getSketchSize()));
    p.replace(FPSTR(T_2),(String)(ESP.getSketchSize()+ESP.getFreeSketchSpace()));
  }
  #endif  
  #ifdef ESP8266
  else if(id==F("memsmeter")){
    p = FPSTR(HTTP_INFO_memsmeter);
    p.replace(FPSTR(T_1),(String)(ESP.getSketchSize()));
    p.replace(FPSTR(T_2),(String)(ESP.getSketchSize()+ESP.getFreeSketchSpace()));
  }
  #endif 
  else if(id==F("lastreset")){
    #ifdef ESP8266
      p = FPSTR(HTTP_INFO_lastreset);
      p.replace(FPSTR(T_1),(String)ESP.getResetReason());
    #elif defined(ESP32) && defined(_ROM_RTC_H_)
      // requires #include <rom/rtc.h>
      p = FPSTR(HTTP_INFO_lastreset);
      for(int i=0;i<2;i++){
        int reason = rtc_get_reset_reason(i);
        String tok = (String)T_ss+(String)(i+1)+(String)T_es;
        switch (reason)
        {
          //@todo move to array
          case 1  : p.replace(tok,F("Vbat power on reset"));break;
          case 3  : p.replace(tok,F("Software reset digital core"));break;
          case 4  : p.replace(tok,F("Legacy watch dog reset digital core"));break;
          case 5  : p.replace(tok,F("Deep Sleep reset digital core"));break;
          case 6  : p.replace(tok,F("Reset by SLC module, reset digital core"));break;
          case 7  : p.replace(tok,F("Timer Group0 Watch dog reset digital core"));break;
          case 8  : p.replace(tok,F("Timer Group1 Watch dog reset digital core"));break;
          case 9  : p.replace(tok,F("RTC Watch dog Reset digital core"));break;
          case 10 : p.replace(tok,F("Instrusion tested to reset CPU"));break;
          case 11 : p.replace(tok,F("Time Group reset CPU"));break;
          case 12 : p.replace(tok,F("Software reset CPU"));break;
          case 13 : p.replace(tok,F("RTC Watch dog Reset CPU"));break;
          case 14 : p.replace(tok,F("for APP CPU, reseted by PRO CPU"));break;
          case 15 : p.replace(tok,F("Reset when the vdd voltage is not stable"));break;
          case 16 : p.replace(tok,F("RTC Watch dog reset digital core and rtc module"));break;
          default : p.replace(tok,F("NO_MEAN"));
        }
      }
    #endif
  }
  else if(id==F("apip")){
    p = FPSTR(HTTP_INFO_apip);
    p.replace(FPSTR(T_1),WiFi.softAPIP().toString());
  }
  else if(id==F("apmac")){
    p = FPSTR(HTTP_INFO_apmac);
    p.replace(FPSTR(T_1),(String)WiFi.softAPmacAddress());
  }
  #ifdef ESP32
  else if(id==F("aphost")){
      p = FPSTR(HTTP_INFO_aphost);
      p.replace(FPSTR(T_1),WiFi.softAPgetHostname());
  }
  #endif
  else if(id==F("apssid")){
    p = FPSTR(HTTP_INFO_apssid);
    p.replace(FPSTR(T_1),htmlEntities((String)WiFi_SSID()));
  }
  else if(id==F("apbssid")){
    p = FPSTR(HTTP_INFO_apbssid);
    p.replace(FPSTR(T_1),(String)WiFi.BSSIDstr());
  }
  else if(id==F("staip")){
    p = FPSTR(HTTP_INFO_staip);
    p.replace(FPSTR(T_1),WiFi.localIP().toString());
  }
  else if(id==F("stagw")){
    p = FPSTR(HTTP_INFO_stagw);
    p.replace(FPSTR(T_1),WiFi.gatewayIP().toString());
  }
  else if(id==F("stasub")){
    p = FPSTR(HTTP_INFO_stasub);
    p.replace(FPSTR(T_1),WiFi.subnetMask().toString());
  }
  else if(id==F("dnss")){
    p = FPSTR(HTTP_INFO_dnss);
    p.replace(FPSTR(T_1),WiFi.dnsIP().toString());
  }
  else if(id==F("host")){
    p = FPSTR(HTTP_INFO_host);
    #ifdef ESP32
      p.replace(FPSTR(T_1),WiFi.getHostname());
    #else
    p.replace(FPSTR(T_1),WiFi.hostname());
    #endif
  }
  else if(id==F("stamac")){
    p = FPSTR(HTTP_INFO_stamac);
    p.replace(FPSTR(T_1),WiFi.macAddress());
  }
  else if(id==F("conx")){
    p = FPSTR(HTTP_INFO_conx);
    p.replace(FPSTR(T_1),WiFi.isConnected() ? FPSTR(S_y) : FPSTR(S_n));
  }
  #ifdef ESP8266
  else if(id==F("autoconx")){
    p = FPSTR(HTTP_INFO_autoconx);
    p.replace(FPSTR(T_1),WiFi.getAutoConnect() ? FPSTR(S_enable) : FPSTR(S_disable));
  }
  #endif
  #ifdef ESP32
  else if(id==F("temp")){
    // temperature is not calibrated, varying large offsets are present, use for relative temp changes only
    p = FPSTR(HTTP_INFO_temp);
    p.replace(FPSTR(T_1),(String)temperatureRead());
    p.replace(FPSTR(T_2),(String)((temperatureRead()+32)*1.8));
  }
  #endif
  return p;
}

/** 
 * HTTPD CALLBACK root or redirect to captive portal
 */
void WiFiManager::handleExit() {
  DEBUG_WM(DEBUG_VERBOSE,F("<- HTTP Exit"));
  handleRequest();
  String page = getHTTPHead(FPSTR(S_titleexit)); // @token titleexit
  page += FPSTR(S_exiting); // @token exiting
  server->sendHeader(FPSTR(HTTP_HEAD_CL), String(page.length()));
  server->send(200, FPSTR(HTTP_HEAD_CT), page);
  abort = true;
}

/** 
 * HTTPD CALLBACK reset page
 */
void WiFiManager::handleReset() {
  DEBUG_WM(DEBUG_VERBOSE,F("<- HTTP Reset"));
  handleRequest();
  String page = getHTTPHead(FPSTR(S_titlereset)); //@token titlereset
  page += FPSTR(S_resetting); //@token resetting
  page += FPSTR(HTTP_END);

  server->sendHeader(FPSTR(HTTP_HEAD_CL), String(page.length()));
  server->send(200, FPSTR(HTTP_HEAD_CT), page);

  DEBUG_WM(F("RESETTING ESP"));
  delay(1000);
  reboot();
}

/** 
 * HTTPD CALLBACK erase page
 */

// void WiFiManager::handleErase() {
//   handleErase(false);
// }
void WiFiManager::handleErase(boolean opt) {
  DEBUG_WM(DEBUG_VERBOSE,F("<- HTTP Erase"));
  handleRequest();
  String page = getHTTPHead(FPSTR(S_titleerase)); // @token titleerase

  bool ret = erase(opt);

  if(ret) page += FPSTR(S_resetting); // @token resetting
  else {
    page += FPSTR(S_error); // @token erroroccur
    DEBUG_WM(DEBUG_ERROR,F("[ERROR] WiFi EraseConfig failed"));
  }

  page += FPSTR(HTTP_END);
  server->sendHeader(FPSTR(HTTP_HEAD_CL), String(page.length()));
  server->send(200, FPSTR(HTTP_HEAD_CT), page);

  if(ret){
    delay(2000);
  	DEBUG_WM(F("RESETTING ESP"));
  	reboot();
  }	
}

/** 
 * HTTPD CALLBACK 404
 */
void WiFiManager::handleNotFound() {
  if (captivePortal()) return; // If captive portal redirect instead of displaying the page
  handleRequest();
  String message = FPSTR(S_notfound); // @token notfound
  message += FPSTR(S_uri); // @token uri
  message += server->uri();
  message += FPSTR(S_method); // @token method
  message += ( server->method() == HTTP_GET ) ? FPSTR(S_GET) : FPSTR(S_POST);
  message += FPSTR(S_args); // @token args
  message += server->args();
  message += F("\n");

  for ( uint8_t i = 0; i < server->args(); i++ ) {
    message += " " + server->argName ( i ) + ": " + server->arg ( i ) + "\n";
  }
  server->sendHeader(F("Cache-Control"), F("no-cache, no-store, must-revalidate"));
  server->sendHeader(F("Pragma"), F("no-cache"));
  server->sendHeader(F("Expires"), F("-1"));
  server->sendHeader(FPSTR(HTTP_HEAD_CL), String(message.length()));
  server->send ( 404, FPSTR(HTTP_HEAD_CT2), message );
}

/**
 * HTTPD redirector
 * Redirect to captive portal if we got a request for another domain. 
 * Return true in that case so the page handler do not try to handle the request again. 
 */
boolean WiFiManager::captivePortal() {
  DEBUG_WM(DEBUG_DEV,"-> " + server->hostHeader());
  
  if(!_enableCaptivePortal) return false; // skip redirections

  if (!isIp(server->hostHeader())) {
    DEBUG_WM(DEBUG_VERBOSE,F("<- Request redirected to captive portal"));
    server->sendHeader(F("Location"), (String)F("http://") + toStringIp(server->client().localIP()), true);
    server->send ( 302, FPSTR(HTTP_HEAD_CT2), ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server->client().stop(); // Stop is needed because we sent no content length
    return true;
  }
  return false;
}

void WiFiManager::stopCaptivePortal(){
  _enableCaptivePortal= false;
  // @todo maybe disable configportaltimeout(optional), or just provide callback for user
}

void WiFiManager::handleClose(){
  stopCaptivePortal();
  DEBUG_WM(DEBUG_VERBOSE,F("<- HTTP close"));
  handleRequest();
  String page = getHTTPHead(FPSTR(S_titleclose)); // @token titleclose
  page += FPSTR(S_closing); // @token closing
  server->sendHeader(FPSTR(HTTP_HEAD_CL), String(page.length()));
  server->send(200, FPSTR(HTTP_HEAD_CT), page);
}

void WiFiManager::reportStatus(String &page){
  updateConxResult(WiFi.status());
  String str;
  if (WiFi_SSID() != ""){
    if (WiFi.status()==WL_CONNECTED){
      str = FPSTR(HTTP_STATUS_ON);
      str.replace(FPSTR(T_i),WiFi.localIP().toString());
      str.replace(FPSTR(T_v),htmlEntities(WiFi_SSID()));
    }
    else {
      str = FPSTR(HTTP_STATUS_OFF);
      str.replace(FPSTR(T_v),htmlEntities(WiFi_SSID()));
      if(_lastconxresult == WL_STATION_WRONG_PASSWORD){
        // wrong password
        str.replace(FPSTR(T_c),"D"); // class
        str.replace(FPSTR(T_r),FPSTR(HTTP_STATUS_OFFPW));
      }
      else if(_lastconxresult == WL_NO_SSID_AVAIL){
        // connect failed, or ap not found
        str.replace(FPSTR(T_c),"D");
        str.replace(FPSTR(T_r),FPSTR(HTTP_STATUS_OFFNOAP));
      }
      else if(_lastconxresult == WL_CONNECT_FAILED){
        // connect failed
        str.replace(FPSTR(T_c),"D");
        str.replace(FPSTR(T_r),FPSTR(HTTP_STATUS_OFFFAIL));
      }
      else{
        str.replace(FPSTR(T_c),"");
        str.replace(FPSTR(T_r),"");
      } 
    }
  }
  else {
    str = FPSTR(HTTP_STATUS_NONE);
  }
  page += str;
}

// PUBLIC

// METHODS

/**
 * reset wifi settings, clean stored ap password
 */

/**
 * [stopConfigPortal description]
 * @return {[type]} [description]
 */
bool WiFiManager::stopConfigPortal(){
  if(_configPortalIsBlocking){
    abort = true;
    return true;
  }
  return shutdownConfigPortal();  
}

/**
 * disconnect
 * @access public
 * @since $dev
 * @return bool success
 */
bool WiFiManager::disconnect(){
  if(WiFi.status() != WL_CONNECTED){
    DEBUG_WM(DEBUG_VERBOSE,"Disconnecting: Not connected");
    return false;
  }  
  DEBUG_WM("Disconnecting");
  return WiFi_Disconnect();
}

/**
 * reboot the device
 * @access public
 */
void WiFiManager::reboot(){
  DEBUG_WM("Restarting");
  ESP.restart();
}

/**
 * reboot the device
 * @access public
 */
bool WiFiManager::erase(){
  return erase(false);
}

bool WiFiManager::erase(bool opt){
  DEBUG_WM("Erasing");

  #if defined(ESP32) && ((defined(WM_ERASE_NVS) || defined(nvs_flash_h)))
    // if opt true, do nvs erase
    if(opt){
      DEBUG_WM("Erasing NVS");
      int err;
      esp_err_t err;
      err = nvs_flash_init();
      DEBUG_WM(DEBUG_VERBOSE,"nvs_flash_init: ",err!=ESP_OK ? (String)err : "Success");
      err = nvs_flash_erase();
      DEBUG_WM(DEBUG_VERBOSE,"nvs_flash_erase: ", err!=ESP_OK ? (String)err : "Success");
      return err == ESP_OK;
    }
  #else
    (void)opt;
  #endif

  return WiFi_eraseConfig();
}

/**
 * [resetSettings description]
 * ERASES STA CREDENTIALS
 * @access public
 */
void WiFiManager::resetSettings() {
  DEBUG_WM(F("SETTINGS ERASED"));
  WiFi_enableSTA(true,true); // must be sta to disconnect erase
  
  if (_resetcallback != NULL)
      _resetcallback();
  
  #ifdef ESP32
    WiFi.disconnect(true,true);
  #else
    WiFi.persistent(true);
    WiFi.disconnect(true);
    WiFi.persistent(false);
  #endif  
}

// SETTERS

/**
 * [setTimeout description]
 * @access public
 * @param {[type]} unsigned long seconds [description]
 */
void WiFiManager::setTimeout(unsigned long seconds) {
  setConfigPortalTimeout(seconds);
}

/**
 * [setConfigPortalTimeout description]
 * @access public
 * @param {[type]} unsigned long seconds [description]
 */
void WiFiManager::setConfigPortalTimeout(unsigned long seconds) {
  _configPortalTimeout = seconds * 1000;
}

/**
 * [setConnectTimeout description]
 * @access public
 * @param {[type]} unsigned long seconds [description]
 */
void WiFiManager::setConnectTimeout(unsigned long seconds) {
  _connectTimeout = seconds * 1000;
}

/**
 * [setConnectTimeout description
 * @access public
 * @param {[type]} unsigned long seconds [description]
 */
void WiFiManager::setSaveConnectTimeout(unsigned long seconds) {
  _saveTimeout = seconds * 1000;
}

/**
 * [setDebugOutput description]
 * @access public
 * @param {[type]} boolean debug [description]
 */
void WiFiManager::setDebugOutput(boolean debug) {
  _debug = debug;
  if(_debug && _debugLevel == DEBUG_DEV) debugPlatformInfo();
}

/**
 * [setAPStaticIPConfig description]
 * @access public
 * @param {[type]} IPAddress ip [description]
 * @param {[type]} IPAddress gw [description]
 * @param {[type]} IPAddress sn [description]
 */
void WiFiManager::setAPStaticIPConfig(IPAddress ip, IPAddress gw, IPAddress sn) {
  _ap_static_ip = ip;
  _ap_static_gw = gw;
  _ap_static_sn = sn;
}

/**
 * [setSTAStaticIPConfig description]
 * @access public
 * @param {[type]} IPAddress ip [description]
 * @param {[type]} IPAddress gw [description]
 * @param {[type]} IPAddress sn [description]
 */
void WiFiManager::setSTAStaticIPConfig(IPAddress ip, IPAddress gw, IPAddress sn) {
  _sta_static_ip = ip;
  _sta_static_gw = gw;
  _sta_static_sn = sn;
}

/**
 * [setSTAStaticIPConfig description]
 * @since $dev
 * @access public
 * @param {[type]} IPAddress ip [description]
 * @param {[type]} IPAddress gw [description]
 * @param {[type]} IPAddress sn [description]
 * @param {[type]} IPAddress dns [description]
 */
void WiFiManager::setSTAStaticIPConfig(IPAddress ip, IPAddress gw, IPAddress sn, IPAddress dns) {
  setSTAStaticIPConfig(ip,gw,sn);
  _sta_static_dns = dns;
}

/**
 * [setMinimumSignalQuality description]
 * @access public
 * @param {[type]} int quality [description]
 */
void WiFiManager::setMinimumSignalQuality(int quality) {
  _minimumQuality = quality;
}

/**
 * [setBreakAfterConfig description]
 * @access public
 * @param {[type]} boolean shouldBreak [description]
 */
void WiFiManager::setBreakAfterConfig(boolean shouldBreak) {
  _shouldBreakAfterConfig = shouldBreak;
}

/**
 * setAPCallback, set a callback when softap is started
 * @access public 
 * @param {[type]} void (*func)(WiFiManager* wminstance)
 */
void WiFiManager::setAPCallback( std::function<void(WiFiManager*)> func ) {
  _apcallback = func;
}

/**
 * setWebServerCallback, set a callback after webserver is reset, and before routes are setup
 * if we set webserver handlers before wm, they are used and wm is not by esp webserver
 * on events cannot be overrided once set, and are not mutiples
 * @access public 
 * @param {[type]} void (*func)(void)
 */
void WiFiManager::setWebServerCallback( std::function<void()> func ) {
  _webservercallback = func;
}

/**
 * setSaveConfigCallback, set a save config callback after closing configportal
 * @note calls only if wifi is saved or changed, or setBreakAfterConfig(true)
 * @access public
 * @param {[type]} void (*func)(void)
 */
void WiFiManager::setSaveConfigCallback( std::function<void()> func ) {
  _savewificallback = func;
}

/**
 * setConfigResetCallback, set a callback to occur when a resetSettings() occurs
 * @access public
 * @param {[type]} void(*func)(void)
 */
void WiFiManager::setConfigResetCallback( std::function<void()> func ) {
    _resetcallback = func;
}

/**
 * setSaveParamsCallback, set a save params callback on params save in wifi or params pages
 * @access public
 * @param {[type]} void (*func)(void)
 */
void WiFiManager::setSaveParamsCallback( std::function<void()> func ) {
  _saveparamscallback = func;
}

/**
 * setPreSaveConfigCallback, set a callback to fire before saving wifi or params
 * @access public
 * @param {[type]} void (*func)(void)
 */
void WiFiManager::setPreSaveConfigCallback( std::function<void()> func ) {
  _presavecallback = func;
}

/**
 * set custom head html
 * custom element will be added to head, eg. new style tag etc.
 * @access public
 * @param char element
 */
void WiFiManager::setCustomHeadElement(const char* element) {
  _customHeadElement = element;
}

/**
 * toggle wifiscan hiding of duplicate ssid names
 * if this is false, wifiscan will remove duplicat Access Points - defaut true
 * @access public
 * @param boolean removeDuplicates [true]
 */
void WiFiManager::setRemoveDuplicateAPs(boolean removeDuplicates) {
  _removeDuplicateAPs = removeDuplicates;
}

/**
 * toggle configportal blocking loop
 * if enabled, then the configportal will enter a blocking loop and wait for configuration
 * if disabled use with process() to manually process webserver
 * @since $dev
 * @access public
 * @param boolean shoudlBlock [false]
 */
void WiFiManager::setConfigPortalBlocking(boolean shoudlBlock) {
  _configPortalIsBlocking = shoudlBlock;
}

/**
 * toggle restore persistent, track internally
 * sets ESP wifi.persistent so we can remember it and restore user preference on destruct
 * there is no getter in esp8266 platform prior to https://github.com/esp8266/Arduino/pull/3857
 * @since $dev
 * @access public
 * @param boolean persistent [true]
 */
void WiFiManager::setRestorePersistent(boolean persistent) {
  _userpersistent = persistent;
  if(!persistent) DEBUG_WM(F("persistent is off"));
}

/**
 * toggle showing static ip form fields
 * if enabled, then the static ip, gateway, subnet fields will be visible, even if not set in code
 * @since $dev
 * @access public
 * @param boolean alwaysShow [false]
 */
void WiFiManager::setShowStaticFields(boolean alwaysShow){
  if(_disableIpFields) _staShowStaticFields = alwaysShow ? 1 : -1;
  else _staShowStaticFields = alwaysShow ? 1 : 0;
}

/**
 * toggle showing dns fields
 * if enabled, then the dns1 field will be visible, even if not set in code
 * @since $dev
 * @access public
 * @param boolean alwaysShow [false]
 */
void WiFiManager::setShowDnsFields(boolean alwaysShow){
  if(_disableIpFields) _staShowDns = alwaysShow ? 1 : -1;
  _staShowDns = alwaysShow ? 1 : 0;
}

/**
 * toggle captive portal
 * if enabled, then devices that use captive portal checks will be redirected to root
 * if not you will automatically have to navigate to ip [192.168.4.1]
 * @since $dev
 * @access public
 * @param boolean enabled [true]
 */
void WiFiManager::setCaptivePortalEnable(boolean enabled){
  _enableCaptivePortal = enabled;
}

/**
 * toggle wifi autoreconnect policy
 * if enabled, then wifi will autoreconnect automatically always
 * On esp8266 we force this on when autoconnect is called, see notes
 * On esp32 this is handled on SYSTEM_EVENT_STA_DISCONNECTED since it does not exist in core yet
 * @since $dev
 * @access public
 * @param boolean enabled [true]
 */
void WiFiManager::setWiFiAutoReconnect(boolean enabled){
  _wifiAutoReconnect = enabled;
}

/**
 * toggle configportal timeout wait for station client
 * if enabled, then the configportal will start timeout when no stations are connected to softAP
 * disabled by default as rogue stations can keep it open if there is no auth
 * @since $dev
 * @access public
 * @param boolean enabled [false]
 */
void WiFiManager::setAPClientCheck(boolean enabled){
  _apClientCheck = enabled;
}

/**
 * toggle configportal timeout wait for web client
 * if enabled, then the configportal will restart timeout when client requests come in
 * @since $dev
 * @access public
 * @param boolean enabled [true]
 */
void WiFiManager::setWebPortalClientCheck(boolean enabled){
  _webClientCheck = enabled;
}

/**
 * toggle wifiscan percentages or quality icons
 * @since $dev
 * @access public
 * @param boolean enabled [false]
 */
void WiFiManager::setScanDispPerc(boolean enabled){
  _scanDispOptions = enabled;
}

/**
 * toggle configportal if autoconnect failed
 * if enabled, then the configportal will be activated on autoconnect failure
 * @since $dev
 * @access public
 * @param boolean enabled [true]
 */
void WiFiManager::setEnableConfigPortal(boolean enable)
{
    _enableConfigPortal = enable;
}


/**
 * set the hostname (dhcp client id)
 * @since $dev
 * @access public
 * @param  char* hostname 32 character hostname to use for sta+ap in esp32, sta in esp8266
 * @return bool false if hostname is not valid
 */
bool  WiFiManager::setHostname(const char * hostname){
  //@todo max length 32
  _hostname = hostname;
  return true;
}

/**
 * toggle showing erase wifi config button on info page
 * @param boolean enabled
 */
void WiFiManager::setShowInfoErase(boolean enabled){
  _showInfoErase = enabled;
}

/**
 * set menu items and order
 * if param is present in menu , params will be removed from wifi page automatically
 * eg.
 *  const char * menu[] = {"wifi","setup","sep","info","exit"};
 *  WiFiManager.setMenu(menu);
 * @since $dev
 * @param uint8_t menu[] array of menu ids
 */
void WiFiManager::setMenu(const char * menu[], uint8_t size){
  // DEBUG_WM(DEBUG_VERBOSE,"setmenu array");
  _menuIds.clear();
  for(size_t i = 0; i < size; i++){
    for(size_t j = 0; j < _nummenutokens; j++){
      if(menu[i] == _menutokens[j]){
        if((String)menu[i] == "param") _paramsInWifi = false; // param auto flag
        _menuIds.push_back(j);
      }
    }
  }
  // DEBUG_WM(getMenuOut());
}

/**
 * setMenu with vector
 * eg.
 * std::vector<const char *> menu = {"wifi","setup","sep","info","exit"};
 * WiFiManager.setMenu(menu);
 * tokens can be found in _menutokens array in strings_en.h
 * @shiftIncrement $dev
 * @param {[type]} std::vector<const char *>& menu [description]
 */
void WiFiManager::setMenu(std::vector<const char *>& menu){
  // DEBUG_WM(DEBUG_VERBOSE,"setmenu vector");
  _menuIds.clear();
  for(auto menuitem : menu ){
    for(size_t j = 0; j < _nummenutokens; j++){
      if(menuitem == _menutokens[j]){
        if((String)menuitem == "param") _paramsInWifi = false; // param auto flag
        _menuIds.push_back(j);
      }
    }
  }
  // DEBUG_WM(getMenuOut());
}


// GETTERS

/**
 * get config portal AP SSID
 * @since 0.0.1
 * @access public
 * @return String the configportal ap name
 */
String WiFiManager::getConfigPortalSSID() {
  return _apName;
}

/**
 * return the last known connection result
 * logged on autoconnect and wifisave, can be used to check why failed
 * get as readable string with getWLStatusString(getLastConxResult);
 * @since $dev
 * @access public
 * @return bool return wl_status codes
 */
uint8_t WiFiManager::getLastConxResult(){
  return _lastconxresult;
}

/**
 * check if wifi has a saved ap or not
 * @since $dev
 * @access public
 * @return bool true if a saved ap config exists
 */
bool WiFiManager::getWiFiIsSaved(){
  return WiFi_hasAutoConnect();
}

/**
 * setCountry
 * @since $dev
 * @param String cc country code, must be defined in WiFiSetCountry, US, JP, CN
 */
void WiFiManager::setCountry(String cc){
  _wificountry = cc;
}

/**
 * setClass
 * @param String str body class string
 */
void WiFiManager::setClass(String str){
  _bodyClass = str;
}
  
// HELPERS

// DEBUG
// @todo fix DEBUG_WM(0,0);
template <typename Generic>
void WiFiManager::DEBUG_WM(Generic text) {
  DEBUG_WM(DEBUG_NOTIFY,text,"");
}

template <typename Generic>
void WiFiManager::DEBUG_WM(wm_debuglevel_t level,Generic text) {
  if(_debugLevel >= level) DEBUG_WM(level,text,"");
}

template <typename Generic, typename Genericb>
void WiFiManager::DEBUG_WM(Generic text,Genericb textb) {
  DEBUG_WM(DEBUG_NOTIFY,text,textb);
}

template <typename Generic, typename Genericb>
void WiFiManager::DEBUG_WM(wm_debuglevel_t level,Generic text,Genericb textb) {
  if(!_debug || _debugLevel < level) return;

  if(_debugLevel >= DEBUG_MAX){
    _debugPort.print("MEM: ");
    _debugPort.println((String)ESP.getFreeHeap());
  }
  _debugPort.print("*WM: ");
  if(_debugLevel == DEBUG_DEV) _debugPort.print("["+(String)level+"] ");
  _debugPort.print(text);
  if(textb){
    _debugPort.print(" ");
    _debugPort.print(textb);
  }
  _debugPort.println();
}

/**
 * [debugSoftAPConfig description]
 * @access public
 * @return {[type]} [description]
 */
void WiFiManager::debugSoftAPConfig(){
    #ifdef ESP8266
      softap_config config;
      wifi_softap_get_config(&config);
    #elif defined(ESP32)
      wifi_config_t conf_config;
      esp_wifi_get_config(WIFI_IF_AP, &conf_config); // == ESP_OK
      wifi_ap_config_t config = conf_config.ap;
    #endif

    DEBUG_WM(F("SoftAP Configuration"));
    DEBUG_WM(FPSTR(D_HR));
    DEBUG_WM(F("ssid:            "),(char *) config.ssid);
    DEBUG_WM(F("password:        "),(char *) config.password);
    DEBUG_WM(F("ssid_len:        "),config.ssid_len);
    DEBUG_WM(F("channel:         "),config.channel);
    DEBUG_WM(F("authmode:        "),config.authmode);
    DEBUG_WM(F("ssid_hidden:     "),config.ssid_hidden);
    DEBUG_WM(F("max_connection:  "),config.max_connection);
    DEBUG_WM(F("beacon_interval: "),(String)config.beacon_interval + "(ms)");
    DEBUG_WM(FPSTR(D_HR));
}

/**
 * [debugPlatformInfo description]
 * @access public
 * @return {[type]} [description]
 */
void WiFiManager::debugPlatformInfo(){
  #ifdef ESP8266
    system_print_meminfo();
    DEBUG_WM(F("getCoreVersion():         "),ESP.getCoreVersion());
    DEBUG_WM(F("system_get_sdk_version(): "),system_get_sdk_version());
    DEBUG_WM(F("system_get_boot_version():"),system_get_boot_version());
    DEBUG_WM(F("getFreeHeap():            "),(String)ESP.getFreeHeap());
  #elif defined(ESP32)
    size_t freeHeap = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    DEBUG_WM("Free heap:       ", freeHeap);
    DEBUG_WM("ESP-IDF version: ", esp_get_idf_version());
  #endif
}

int WiFiManager::getRSSIasQuality(int RSSI) {
  int quality = 0;

  if (RSSI <= -100) {
    quality = 0;
  } else if (RSSI >= -50) {
    quality = 100;
  } else {
    quality = 2 * (RSSI + 100);
  }
  return quality;
}

/** Is this an IP? */
boolean WiFiManager::isIp(String str) {
  for (size_t i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) {
      return false;
    }
  }
  return true;
}

/** IP to String? */
String WiFiManager::toStringIp(IPAddress ip) {
  String res = "";
  for (int i = 0; i < 3; i++) {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}

boolean WiFiManager::validApPassword(){
  // check that ap password is valid, return false
  if (_apPassword == NULL) _apPassword = "";
  if (_apPassword != "") {
    if (_apPassword.length() < 8 || _apPassword.length() > 63) {
      DEBUG_WM(F("AccessPoint set password is INVALID or <8 chars"));
      _apPassword = "";
      return false; // @todo FATAL or fallback to empty ?
    }
    DEBUG_WM(DEBUG_VERBOSE,F("AccessPoint set password is VALID"));
    DEBUG_WM(_apPassword);
  }
  return true;
}

/**
 * encode htmlentities
 * @since $dev
 * @param  string str  string to replace entities
 * @return string      encoded string
 */
String WiFiManager::htmlEntities(String str) {
  str.replace("&","&amp;");
  str.replace("<","&lt;");
  str.replace(">","&gt;");
  // str.replace("'","&#39;");
  // str.replace("\"","&quot;");
  // str.replace("/": "&#x2F;");
  // str.replace("`": "&#x60;");
  // str.replace("=": "&#x3D;");
return str;
}

/**
 * [getWLStatusString description]
 * @access public
 * @param  {[type]} uint8_t status        [description]
 * @return {[type]}         [description]
 */
String WiFiManager::getWLStatusString(uint8_t status){
  if(status <= 7) return WIFI_STA_STATUS[status];
  return FPSTR(S_NA);
}

String WiFiManager::encryptionTypeStr(uint8_t authmode) {
  // DEBUG_WM("enc_tye: ",authmode);
  return AUTH_MODE_NAMES[authmode];
}

String WiFiManager::getModeString(uint8_t mode){
  if(mode <= 3) return WIFI_MODES[mode];
  return FPSTR(S_NA);
}

bool WiFiManager::WiFiSetCountry(){
  bool ret = false;
  #ifdef ESP32
  // @todo check if wifi is init, no idea how, doesnt seem to be exposed
  if(_wificountry == "") return ret; // skip not set
  else if(WiFi.getMode() == WIFI_MODE_NULL); // exception if wifi not init!
  else if(_wificountry == "US") ret = esp_wifi_set_country(&WM_COUNTRY_US) == ESP_OK;
  else if(_wificountry == "JP") ret = esp_wifi_set_country(&WM_COUNTRY_JP) == ESP_OK;
  else if(_wificountry == "CN") ret = esp_wifi_set_country(&WM_COUNTRY_CN) == ESP_OK;
  else DEBUG_WM(DEBUG_ERROR,"[ERROR] country code not found");
  
  if(ret) DEBUG_WM(DEBUG_VERBOSE,"esp_wifi_set_country: " + _wificountry);
  else DEBUG_WM(DEBUG_ERROR,"[ERROR] esp_wifi_set_country failed");
  #endif
  return ret;
}


// set mode ignores WiFi.persistent 
bool WiFiManager::WiFi_Mode(WiFiMode_t m,bool persistent) {
    bool ret;
    #ifdef ESP8266
      if((wifi_get_opmode() == (uint8) m ) && !persistent) {
          return true;
      }
      ETS_UART_INTR_DISABLE();
      if(persistent) ret = wifi_set_opmode(m);
      else ret = wifi_set_opmode_current(m);
      ETS_UART_INTR_ENABLE();
    return ret;
    #elif defined(ESP32)
      if(persistent && esp32persistent) WiFi.persistent(true);
      ret = WiFi.mode(m); // @todo persistent check persistant mode , NI
      if(persistent && esp32persistent) WiFi.persistent(false);
      return ret;
    #endif
}
bool WiFiManager::WiFi_Mode(WiFiMode_t m) {
	return WiFi_Mode(m,false);
}

// sta disconnect without persistent
bool WiFiManager::WiFi_Disconnect() {
    #ifdef ESP8266
      if((WiFi.getMode() & WIFI_STA) != 0) {
          bool ret;
          DEBUG_WM(DEBUG_DEV,F("WIFI station disconnect"));
          ETS_UART_INTR_DISABLE(); // @todo probably not needed
          ret = wifi_station_disconnect();
          ETS_UART_INTR_ENABLE();        
          return ret;
      }
    #elif defined(ESP32)
      DEBUG_WM(DEBUG_DEV,F("WIFI station disconnect"));
      return WiFi.disconnect(); // not persistent atm
    #endif
    return false;
}

// toggle STA without persistent
bool WiFiManager::WiFi_enableSTA(bool enable,bool persistent) {
    DEBUG_WM(DEBUG_DEV,F("WiFi station enable"));
    #ifdef ESP8266
      WiFiMode_t newMode;
      WiFiMode_t currentMode = WiFi.getMode();
      bool isEnabled         = (currentMode & WIFI_STA) != 0;
      if(enable) newMode     = (WiFiMode_t)(currentMode | WIFI_STA);
      else newMode           = (WiFiMode_t)(currentMode & (~WIFI_STA));

      if((isEnabled != enable) || persistent) {
          if(enable) {
          	if(persistent) DEBUG_WM(DEBUG_DEV,F("enableSTA PERSISTENT ON"));
              return WiFi_Mode(newMode,persistent);
          } else {
              return WiFi_Mode(newMode,persistent);
          }
      } else {
          return true;
      }
    #elif defined(ESP32)
      bool ret;
      if(persistent && esp32persistent) WiFi.persistent(true);
      ret =  WiFi.enableSTA(enable); // @todo handle persistent when it is implemented in platform
      if(persistent && esp32persistent) WiFi.persistent(false);
      return ret;
    #endif
}
bool WiFiManager::WiFi_enableSTA(bool enable) {
	return WiFi_enableSTA(enable,false);
}

bool WiFiManager::WiFi_eraseConfig() {
    #ifdef ESP8266
      #ifndef WM_FIXERASECONFIG 
        return ESP.eraseConfig();
      #else
        // erase config BUG replacement
        // https://github.com/esp8266/Arduino/pull/3635
        const size_t cfgSize = 0x4000;
        size_t cfgAddr = ESP.getFlashChipSize() - cfgSize;

        for (size_t offset = 0; offset < cfgSize; offset += SPI_FLASH_SEC_SIZE) {
            if (!ESP.flashEraseSector((cfgAddr + offset) / SPI_FLASH_SEC_SIZE)) {
                return false;
            }
        }
        return true;
      #endif
    #elif defined(ESP32)
      bool ret;
      WiFi.mode(WIFI_AP_STA); // cannot erase if not in STA mode !
      WiFi.persistent(true);
      ret = WiFi.disconnect(true,true);
      WiFi.persistent(false);
      return ret;
    #endif
}

uint8_t WiFiManager::WiFi_softap_num_stations(){
  #ifdef ESP8266
    return wifi_softap_get_station_num();
  #elif defined(ESP32)
    return WiFi.softAPgetStationNum();
  #endif
}

bool WiFiManager::WiFi_hasAutoConnect(){
  return WiFi_SSID() != "";
}

String WiFiManager::WiFi_SSID(){
  #ifdef ESP8266
    return WiFi.SSID();
  #elif defined(ESP32)
    //@todo , workaround only when not connected
    wifi_config_t conf;
    esp_wifi_get_config(WIFI_IF_STA, &conf);
    return String(reinterpret_cast<const char*>(conf.sta.ssid));
  #endif
}

#ifdef ESP32
void WiFiManager::WiFiEvent(WiFiEvent_t event,system_event_info_t info){
    if(!_hasBegun){
      // DEBUG_WM(DEBUG_VERBOSE,"[ERROR] WiFiEvent, not ready");
      return;
    } 
    // DEBUG_WM(DEBUG_VERBOSE,"[EVENT]",event);
    if(event == SYSTEM_EVENT_STA_DISCONNECTED){
      DEBUG_WM(DEBUG_VERBOSE,"[EVENT] WIFI_REASON:",info.disconnected.reason);
      if(info.disconnected.reason == WIFI_REASON_AUTH_EXPIRE || info.disconnected.reason == WIFI_REASON_AUTH_FAIL){
        _lastconxresulttmp = 7; // hack in wrong password internally, sdk emit WIFI_REASON_AUTH_EXPIRE on some routers on auth_fail
      } else _lastconxresulttmp = WiFi.status();
      if(info.disconnected.reason == WIFI_REASON_NO_AP_FOUND) DEBUG_WM(DEBUG_VERBOSE,"[EVENT] WIFI_REASON: NO_AP_FOUND");
      #ifdef esp32autoreconnect
        DEBUG_WM(DEBUG_VERBOSE,"[Event] SYSTEM_EVENT_STA_DISCONNECTED, reconnecting");
        WiFi.reconnect();
      #endif
  }
  else if(event == SYSTEM_EVENT_SCAN_DONE){
    uint16_t scans = WiFi.scanComplete();
    WiFi_scanComplete(scans);
  }
}
#endif

void WiFiManager::WiFi_autoReconnect(){
  #ifdef ESP8266
    WiFi.setAutoReconnect(_wifiAutoReconnect);
  #elif defined(ESP32)
    // if(_wifiAutoReconnect){
      // @todo move to seperate method, used for event listener now
      DEBUG_WM(DEBUG_VERBOSE,"ESP32 event handler enabled");
      using namespace std::placeholders;
      WiFi.onEvent(std::bind(&WiFiManager::WiFiEvent,this,_1,_2));
    // }
  #endif
}

#endif
