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

#include "WiFiManager.h"
#include "strings_en.h"

// PARAMETERS

WiFiManagerParameter::WiFiManagerParameter(const char *custom) {
  _id             = NULL;
  _placeholder    = NULL;
  _length         = 0;
  _value          = NULL;
  _labelPlacement = WFM_LABEL_BEFORE;
  _customHTML     = custom;
}

WiFiManagerParameter::WiFiManagerParameter(const char *id, const char *placeholder, const char *defaultValue, int length) {
  init(id, placeholder, defaultValue, length, "", WFM_LABEL_BEFORE);
}

WiFiManagerParameter::WiFiManagerParameter(const char *id, const char *placeholder, const char *defaultValue, int length, const char *custom) {
  init(id, placeholder, defaultValue, length, custom, WFM_LABEL_BEFORE);
}

WiFiManagerParameter::WiFiManagerParameter(const char *id, const char *placeholder, const char *defaultValue, int length, const char *custom, int labelPlacement) {
  init(id, placeholder, defaultValue, length, custom, labelPlacement);
}

void WiFiManagerParameter::init(const char *id, const char *placeholder, const char *defaultValue, int length, const char *custom, int labelPlacement) {
  _id             = id;
  _placeholder    = placeholder;
  _length         = length;
  _labelPlacement = labelPlacement;
  _value          = new char[length + 1];
  for (int i = 0; i < length + 1; i++) {
    _value[i] = 0;
  }
  if (defaultValue != NULL) {
    strncpy(_value, defaultValue, length + 1); // length+1 due to null terminated string
  }

  _customHTML = custom;
}

WiFiManagerParameter::~WiFiManagerParameter() {
  if (_value != NULL) {
    delete[] _value;
  }
}

const char* WiFiManagerParameter::getValue() {
  return _value;
}
const char* WiFiManagerParameter::getID() {
  return _id;
}
const char* WiFiManagerParameter::getPlaceholder() {
  return _placeholder;
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

bool WiFiManager::addParameter(WiFiManagerParameter *p) {

  // check param id is valid
  for (int i = 0; i < strlen(p->getID()); i++){
     if(!isAlphaNumeric(p->getID()[i])){
      DEBUG_WM("[ERROR] parameter IDs can only contain alpha numeric chars");
      return false;
     }
  }

  if(_paramsCount == _max_params){
    // resize the params array by increment of WIFI_MANAGER_MAX_PARAMS
    _max_params += WIFI_MANAGER_MAX_PARAMS;
    DEBUG_WM(F("Updated _max_params:"),_max_params);
    WiFiManagerParameter** new_params = (WiFiManagerParameter**)realloc(_params, _max_params * sizeof(WiFiManagerParameter*));
    DEBUG_WM(WIFI_MANAGER_MAX_PARAMS);
    DEBUG_WM(_paramsCount);
    DEBUG_WM(_max_params);
    if (new_params != NULL) {
      _params = new_params;
    } else {
      DEBUG_WM("[ERROR] failed to realloc params, size not increased!");
      return false;
    }
  }
  _params[_paramsCount] = p;
  _paramsCount++;
  DEBUG_WM("Added Parameter:",p->getID());
  return true;
}

// constructors
WiFiManager::WiFiManager(Stream& consolePort):_debugPort(consolePort) {
  WiFiManager();
}

WiFiManager::WiFiManager():_debugPort(Serial) {
  if(_debug) debugPlatformInfo();
  _usermode = WiFi.getMode();
  WiFi.persistent(false); // disable persistent so scannetworks and mode switching do not cause overwrites
  
  //parameters
  _max_params = WIFI_MANAGER_MAX_PARAMS;
  _params = (WiFiManagerParameter**)malloc(_max_params * sizeof(WiFiManagerParameter*));  
}

// destructor
WiFiManager::~WiFiManager() {
  if(_userpersistent) WiFi.persistent(true); // reenable persistent, there is no getter we rely on _userpersistent
  if(_usermode != WIFI_OFF) WiFi.mode(_usermode);

  // parameters
  if (_params != NULL){
    DEBUG_WM(F("freeing allocated params!"));
    free(_params);
  }
  DEBUG_WM(F("unloading"));
}

// AUTOCONNECT
boolean WiFiManager::autoConnect() {
  String ssid = _wifissidprefix + "_" + String(WIFI_getChipId());
  return autoConnect(ssid.c_str(), NULL);
}

boolean WiFiManager::autoConnect(char const *apName, char const *apPassword) {
  DEBUG_WM(F("AutoConnect"));

  // attempt to connect using saved settings, on fail fallback to AP config portal
  WiFi_enableSTA(true);
  _usermode = WIFI_STA;

  WiFi_autoReconnect();

  // if already connected, or try stored connect 
  if (WiFi.status() == WL_CONNECTED || connectWifi("", "") == WL_CONNECTED)   {
    //connected
    DEBUG_WM(F("IP Address:"),WiFi.localIP());
    return true;
  }
  // not connected start configportal
  return startConfigPortal(apName, apPassword);
}

// CONFIG PORTAL
bool WiFiManager::startAP(){
  DEBUG_WM(F("StartAP with SSID: "),_apName);

  // setup optional soft AP static ip config
  if (_ap_static_ip) {
    DEBUG_WM(F("Custom AP IP/GW/Subnet:"));
    WiFi.softAPConfig(_ap_static_ip, _ap_static_gw, _ap_static_sn);
  }

  bool ret = true;

  // start soft AP with password or anonymous
  if (_apPassword != "") {
    ret = WiFi.softAP(_apName.c_str(), _apPassword.c_str());//password option
  } else {
    DEBUG_WM(F("AP has anonymous access!"));    
    ret = WiFi.softAP(_apName.c_str());
  }

  debugSoftAPConfig();

  if(!ret) DEBUG_WM("[ERROR] There was a problem starting the AP"); // @bug startAP returns unreliable success status

  delay(500); // slight delay to make sure we get an AP IP
  DEBUG_WM(F("AP IP address:"),WiFi.softAPIP());

  // do AP callback if set
  if ( _apcallback != NULL) {
    _apcallback(this);
  }
  
  return ret;
}

void WiFiManager::startWebPortal() {
  if(configPortalActive || webPortalActive) return;
  setupConfigPortal();
  webPortalActive = true;
}

void WiFiManager::stopWebPortal() {
  if(!configPortalActive && !webPortalActive) return;
  DEBUG_WM(F("Stopping Web Portal"));  
  webPortalActive = false;
  stopConfigPortal();
}

boolean WiFiManager::configPortalHasTimeout(){

    if(_configPortalTimeout == 0 || (_cpClientCheck && (WiFi_softap_num_stations() > 0))){
      if(millis() - timer > 30000){
        timer = millis();
        DEBUG_WM("NUM CLIENTS: " + (String)WiFi_softap_num_stations());
      }
      _configPortalStart = millis(); // kludge, bump configportal start time to skew timeouts
      return false;
    }
    // handle timeout
    if(_webClientCheck && _webPortalAccessed>_configPortalStart>0) _configPortalStart = _webPortalAccessed;

    if(millis() > _configPortalStart + _configPortalTimeout){
      DEBUG_WM(F("config portal has timed out"));
      return true;
    } else {
      // log timeout
      if(_debug){
        uint16_t logintvl = 30000; // how often to emit timeing out counter logging
        if((millis() - timer) > logintvl){
          timer = millis();
          DEBUG_WM("Portal Timeout In " + (String)((_configPortalStart + _configPortalTimeout-millis())/1000) + " seconds");
        }
      }
    }

    return false;
}

void WiFiManager::setupConfigPortal() {

  DEBUG_WM("Starting Web Portal");

  // setup dns and web servers
  dnsServer.reset(new DNSServer());
  #if defined(ESP32) && defined(WEBSERVERSHIM)
    server.reset(new WebServer(80));
  #else
    server.reset(new ESP8266WebServer(80));
  #endif

  /* Setup the DNS server redirecting all the domains to the apIP */
  dnsServer->setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer->start(DNS_PORT, "*", WiFi.softAPIP());

  /* Setup httpd callbacks, web pages: root, wifi config pages, SO captive portal detectors and not found. */
  server->on("/", std::bind(&WiFiManager::handleRoot, this));
  server->on("/wifi", std::bind(&WiFiManager::handleWifi, this, true));
  server->on("/0wifi", std::bind(&WiFiManager::handleWifi, this, false));
  server->on("/wifisave", std::bind(&WiFiManager::handleWifiSave, this));
  server->on("/i", std::bind(&WiFiManager::handleInfo, this));
  server->on("/r", std::bind(&WiFiManager::handleReset, this));
  server->on("/exit", std::bind(&WiFiManager::handleExit, this));
  server->on("/erase", std::bind(&WiFiManager::handleErase, this));
  server->on("/status", std::bind(&WiFiManager::handleWiFiStatus, this));
  //server->on("/fwlink", std::bind(&WiFiManager::handleRoot, this));  //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
  server->onNotFound (std::bind(&WiFiManager::handleNotFound, this));
  
  server->begin(); // Web server start
  DEBUG_WM(F("HTTP server started"));
}

boolean WiFiManager::startConfigPortal() {
  String ssid = _wifissidprefix + "_" + String(WIFI_getChipId());  
  return startConfigPortal(ssid.c_str(), NULL);
}

boolean  WiFiManager::startConfigPortal(char const *apName, char const *apPassword) {
  //setup AP
  
  bool disableSTA = false; // debug, always disable sta

  // HANDLE issues with STA connections, shutdown sta if not connected, or else this will hang channel scanning and softap will not respond
  // @todo sometimes still cannot connect to AP for no known reason, no events in log either
  if(!WiFi.isConnected() || disableSTA){
    // this fixes most ap problems, however, simply doing mode(WIFI_AP) does not work if sta connection is hanging, must `wifi_station_disconnect` 
    WiFi_Disconnect();
    WiFi_enableSTA(false);
    DEBUG_WM("Disabling STA");
  }
  else {
    // @todo even if sta is connected, it is possible that softap connections will fail, IOS says "invalid password", windows says "cannot connect to this network" researching
    WiFi_enableSTA(true);
  }

  DEBUG_WM("Enabling AP");

  _apName     = apName; // @todo check valid apname ?
  _apPassword = apPassword;
  if(!validApPassword()) return false;

  // init configportal globals to known states
  configPortalActive = true;
  bool result = connect = abort = false; // loop flags, connect true success, abort true break
  uint8_t state;

  DEBUG_WM(F("setupConfigPortal"));
  _configPortalStart = millis();

  // start access point
  startAP();

  // init configportal
  setupConfigPortal();

  if(!_configPortalIsBlocking){
    DEBUG_WM(F("Config Portal Running, non blocking/processing"));
    return result;
  }

  DEBUG_WM(F("Config Portal Running, blocking, waiting for clients..."));
  // blocking loop waiting for config
  while(1){

    // if timed out or abort, break
    if(configPortalHasTimeout() || abort){
      DEBUG_WM(F("configportal abort"));
      stopConfigPortal();
      result = abort ? portalAbortResult : portalTimeoutResult; // false, false
      break;
    }

    state = handleConfigPortal();

    // status change, break
    if(state != WL_IDLE_STATUS){
        result = state == WL_CONNECTED; // true if connected
        break;
    }

    yield(); // watchdog
  }

  DEBUG_WM("config portal exiting");
  return result;
}

boolean WiFiManager::process(){
    if(webPortalActive || (configPortalActive && !_configPortalIsBlocking)){
        uint8_t state = handleConfigPortal();
        return state == WL_CONNECTED;
    }
    return false;
}

//using esp enums returns for now, should be fine
uint8_t WiFiManager::handleConfigPortal(){
    //DNS handler
    dnsServer->processNextRequest();
    //HTTP handler
    server->handleClient();

    // Waiting for save...
    if(connect) {
      connect = false;
      DEBUG_WM(F("Connecting to a new AP"));
      if(_enableCaptivePortal) delay(2000); // configportal close delay

      // attempt sta connection to submitted _ssid, _pass
      if (connectWifi(_ssid, _pass) == WL_CONNECTED) {
        DEBUG_WM(F("Connect to new AP [SUCCESS]"));
        DEBUG_WM(F("Got IP Address:"));
        DEBUG_WM(WiFi.localIP());
        stopConfigPortal();
        return WL_CONNECTED; // success
      }

      DEBUG_WM(F("Connect to new AP [FAILED]"));

      if (_shouldBreakAfterConfig) {
        // this is more of an exiting callback than a save
        stopConfigPortal();
        return WL_CONNECT_FAILED; // fail
      }
      else{
        // sta off to stabilize AP on connect failure
        WiFi_Disconnect();
        WiFi_enableSTA(false);
        DEBUG_WM("Disabling STA");
      }
    }

    return WL_IDLE_STATUS;
}

boolean WiFiManager::stopConfigPortal(){
  if(webPortalActive) return false;

  // do save callback, @todo move out of here
  if ( _savecallback != NULL) {
    //todo: confirm or verify data was saved
    _savecallback();
  }

  //DNS handler
  dnsServer->processNextRequest();
  //HTTP handler
  server->handleClient();

  // @todo what is the proper way to shutdown and free the server up
  server->stop();
  server.reset();
  dnsServer.reset();

  if(!configPortalActive) return false;

  // turn off AP
  // @todo bug *WM: disconnect configportal
  // [APdisconnect] set_config failed!
  // *WM: disconnect configportal - softAPdisconnect failed
  DEBUG_WM(F("disconnect configportal"));
  bool ret = WiFi.softAPdisconnect(false);
  if(!ret)DEBUG_WM(F("disconnect configportal - softAPdisconnect failed"));
  WiFi_Mode(_usermode); // restore users wifi mode
  configPortalActive = false;
  return ret;
}

// @todo refactor this up into seperate functions
// one for connecting to flash , one for new client
// clean up, flow is convoluted, and causes bugs
int WiFiManager::connectWifi(String ssid, String pass) {
  DEBUG_WM(F("Connecting as wifi client..."));
  
  bool waitforconx = true;

  // Setup static IP config if provided
  if (_sta_static_ip) {
    WiFi.config(_sta_static_ip, _sta_static_gw, _sta_static_sn);
    DEBUG_WM(F("Custom STA IP/GW/Subnet:"),WiFi.localIP());
  }

  // make sure sta is on before `begin` so it does not call enablesta->mode while persistent is ON ( which would save WM AP state to eeprom !)
  WiFi_Disconnect(); // disconnect before begin, in case anything is hung

  // if ssid argument provided connect to that
  if (ssid != "") {
    DEBUG_WM("Connecting to new AP");
  	WiFi_enableSTA(true,storeSTAmode); // storeSTAmode will also toggle STA on in default opmode (persistent) if true (default)
    WiFi.persistent(true);
    WiFi.begin(ssid.c_str(), pass.c_str());
    WiFi.persistent(false);
    //@todo catch failures in set_config
  } else {
    // connect using saved ssid if there is one
    if (WiFi_hasAutoConnect()) {
      DEBUG_WM("Connecting to saved AP");
  	  WiFi_enableSTA(true,storeSTAmode);
      WiFi.begin();
    } else {
      DEBUG_WM("No saved credentials, skipping wifi");
      waitforconx = false;
    }
  }

  uint8_t connRes = waitforconx ? waitForConnectResult() : WL_NO_SSID_AVAIL;
  DEBUG_WM ("Connection result:",getWLStatusString(connRes));

  // do WPS, if WPS options enabled and not connected and no password was supplied
  // @todo this seems like wrong place for this, is it a fallback or option?
  if (_tryWPS && connRes != WL_CONNECTED && pass == "") {
    startWPS();
    // should be connected at the end of WPS
    connRes = waitForConnectResult();
  }

  return connRes;
}

// @todo uses _connectTimeout for wifi save also, add timeout argument to bypass?
 
uint8_t WiFiManager::waitForConnectResult(uint16_t timeout) {
  return waitForConnectResult(timeout * 1000);
}

uint8_t WiFiManager::waitForConnectResult() {
  if (_connectTimeout == 0){
    DEBUG_WM (F("connectTimeout not set, ESP waitForConnectResult..."));
    return WiFi.waitForConnectResult();
  }

  DEBUG_WM (F("connectTimeout set, waiting for connect...."));
  uint8_t status;
  int timeout = millis() + _connectTimeout;
  
  while(millis() < timeout) {
    status = WiFi.status();
    // @todo detect additional states, connect happens, then dhcp then get ip, there is some delay here, make sure not to timeout if waiting on IP
    if (status == WL_CONNECTED || status == WL_CONNECT_FAILED) {
      return status;
    }
    DEBUG_WM (".");
    delay(100);
  }
  return status;
}

void WiFiManager::startWPS() {
  DEBUG_WM("START WPS");
  #ifdef ESP8266  
    WiFi.beginWPSConfig();
  #else
    // @todo
  #endif
  DEBUG_WM("END WPS");
}

// GETTERS
String WiFiManager::getConfigPortalSSID() {
  return _apName;
}

// SETTERS
void WiFiManager::resetSettings() {
  DEBUG_WM(F("SETTINGS ERASED"));
  WiFi_enableSTA(true,true);
  WiFi.disconnect(true);
}

void WiFiManager::setTimeout(unsigned long seconds) {
  setConfigPortalTimeout(seconds);
}

void WiFiManager::setConfigPortalTimeout(unsigned long seconds) {
  _configPortalTimeout = seconds * 1000;
}

void WiFiManager::setConnectTimeout(unsigned long seconds) {
  _connectTimeout = seconds * 1000;
}

void WiFiManager::setDebugOutput(boolean debug) {
  _debug = debug;
}

void WiFiManager::setAPStaticIPConfig(IPAddress ip, IPAddress gw, IPAddress sn) {
  _ap_static_ip = ip;
  _ap_static_gw = gw;
  _ap_static_sn = sn;
}

void WiFiManager::setSTAStaticIPConfig(IPAddress ip, IPAddress gw, IPAddress sn) {
  _sta_static_ip = ip;
  _sta_static_gw = gw;
  _sta_static_sn = sn;
}

void WiFiManager::setMinimumSignalQuality(int quality) {
  _minimumQuality = quality;
}

void WiFiManager::setBreakAfterConfig(boolean shouldBreak) {
  _shouldBreakAfterConfig = shouldBreak;
}

String WiFiManager::getHTTPHead(String title){
  String page;
  page += FPSTR(HTTP_HEAD);
  page.replace("{v}", title);
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_STYLE);
  page += _customHeadElement;
  page += FPSTR(HTTP_HEAD_END);
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
  DEBUG_WM(F("<- HTTP Root"));
  if (captivePortal()) return; // If captive portal redirect instead of displaying the page
  handleRequest();
  String page = getHTTPHead(FPSTR(S_options)); // @token options
  String str  = FPSTR(HTTP_ROOT_MAIN);
  str.replace("{v}",configPortalActive ? _apName : WiFi.localIP().toString()); // use ip if ap is not active for heading
  page += str;
  page += FPSTR(HTTP_PORTAL_OPTIONS);
  reportStatus(page);
  page += FPSTR(HTTP_END);

  server->sendHeader("Content-Length", String(page.length()));
  server->send(200, "text/html", page);
  // server->close(); // testing reliability fix for content length mismatches during mutiple flood hits
}

/** 
 * HTTPD CALLBACK Wifi config page handler
 */
void WiFiManager::handleWifi(boolean scan) {
  DEBUG_WM("<- HTTP Wifi");
  handleRequest();
  String page = getHTTPHead(FPSTR(S_titlewifi)); // @token titlewifi
  if (scan) {
    page += getScanItemOut();
  }
  String pitem = FPSTR(HTTP_FORM_START);
  pitem.replace("{v}", WiFi_SSID());
  page += pitem;

  page += getStaticOut();
  page += getParamOut();

  page += FPSTR(HTTP_FORM_END);
  page += FPSTR(HTTP_SCAN_LINK);
  reportStatus(page);
  page += FPSTR(HTTP_END);

  server->sendHeader("Content-Length", String(page.length()));
  server->send(200, "text/html", page);
  // server->close(); // testing reliability fix for content length mismatches during mutiple flood hits

  // Serial.println(page);

  DEBUG_WM(F("Sent config page"));
}

String WiFiManager::getScanItemOut(){
    String page;

    int n = WiFi.scanNetworks();
    DEBUG_WM(F("Scan done"));
    if (n == 0) {
      DEBUG_WM(F("No networks found"));
      page += FPSTR(S_nonetworks); // @token nonetworks
    } else {

      //sort networks
      int indices[n];
      for (int i = 0; i < n; i++) {
        indices[i] = i;
      }

      // RSSI SORT

      // old sort
      for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
          if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
            std::swap(indices[i], indices[j]);
          }
        }
      }

      /*std::sort(indices, indices + n, [](const int & a, const int & b) -> bool
        {
        return WiFi.RSSI(a) > WiFi.RSSI(b);
        });*/

      // remove duplicates ( must be RSSI sorted )
      if (_removeDuplicateAPs) {
        String cssid;
        for (int i = 0; i < n; i++) {
          if (indices[i] == -1) continue;
          cssid = WiFi.SSID(indices[i]);
          for (int j = i + 1; j < n; j++) {
            if (cssid == WiFi.SSID(indices[j])) {
              DEBUG_WM("DUP AP:",WiFi.SSID(indices[j]));
              indices[j] = -1; // set dup aps to index -1
            }
          }
        }
      }

      // token precheck, to speed up replacements on large ap lists
      String HTTP_ITEM_STR = FPSTR(HTTP_ITEM);
      bool tok_r = HTTP_ITEM_STR.indexOf("{r}") > 0;
      bool tok_R = HTTP_ITEM_STR.indexOf("{R}") > 0;
      bool tok_e = HTTP_ITEM_STR.indexOf("{e}") > 0;
      bool tok_q = HTTP_ITEM_STR.indexOf("{q}") > 0;
      bool tok_i = HTTP_ITEM_STR.indexOf("{i}") > 0;
 
      //display networks in page
      for (int i = 0; i < n; i++) {
        if (indices[i] == -1) continue; // skip dups

        DEBUG_WM("AP: " + (String)WiFi.RSSI(indices[i]) + " " + (String)WiFi.SSID(indices[i]));

        int rssiperc = getRSSIasQuality(WiFi.RSSI(indices[i]));
        uint8_t enc_type = WiFi.encryptionType(indices[i]);

        if (_minimumQuality == -1 || _minimumQuality < rssiperc) {
          String item = FPSTR(HTTP_ITEM);
          item.replace("{v}", WiFi.SSID(indices[i])); // ssid no encoding
          if(tok_e) item.replace("{e}", encryptionTypeStr(enc_type));
          if(tok_r) item.replace("{r}", (String)rssiperc); // rssi percentage 0-100
          if(tok_R) item.replace("{R}", (String)WiFi.RSSI(indices[i])); // rssi db
          if(tok_q) item.replace("{q}", (String)round(map(rssiperc,0,100,1,4))); //quality icon 1-4
          if(tok_i){
            if (enc_type != WIFI_AUTH_OPEN) {
              item.replace("{i}", "l");
            } else {
              item.replace("{i}", "");
            }
          }
          //DEBUG_WM(item);
          page += item;
          delay(0);
        } else {
          DEBUG_WM(F("Skipping , does not meet _minimumQuality"));
        }

      }
      page += "<br/>";
    }

    return page;
}

String WiFiManager::getStaticOut(){
  String page;
  if (_staShowStaticFields || _sta_static_ip) {

    // @todo how can we get these settings from memory , wifi_get_ip_info does not seem to reveal if struct ip_info is static or not
    // @todo move titles to params for i18n
    
    String item = FPSTR(HTTP_FORM_LABEL);
    item += FPSTR(HTTP_FORM_PARAM);
    item.replace("{i}", "ip");
    item.replace("{n}", "ip");
    item.replace("{p}", "{t}");
    item.replace("{t}", FPSTR(S_staticip)); // @token staticip
    item.replace("{l}", "15");
    item.replace("{v}", (_sta_static_ip ? _sta_static_ip.toString() : ""));
    item.replace("{c}", "");
    
    // actuals as placeholder
    // IPAddress sta_ip = WiFi.localIP();
    // item.replace("{p}", sta_ip.toString());

    page += item;

    item = FPSTR(HTTP_FORM_LABEL);
    item += FPSTR(HTTP_FORM_PARAM);
    item.replace("{i}", "gw");
    item.replace("{n}", "gw");
    item.replace("{p}", "{t}");    
    item.replace("{t}", FPSTR(S_staticgw)); // @token staticgw
    item.replace("{l}", "15");
    item.replace("{v}", (_sta_static_gw ? _sta_static_gw.toString() : ""));
    item.replace("{c}", "");

    // actuals as placeholder
    // IPAddress sta_gateway = WiFi.gatewayIP();
    // item.replace("{p}", sta_gateway.toString());

    page += item;

    item = FPSTR(HTTP_FORM_LABEL);
    item += FPSTR(HTTP_FORM_PARAM);
    item.replace("{i}", "sn");
    item.replace("{n}", "sn");
    item.replace("{p}", "{t}");    
    item.replace("{t}", FPSTR(S_subnet)); // @token subnet
    item.replace("{l}", "15");
    item.replace("{v}", (_sta_static_sn ? _sta_static_sn.toString() : ""));
    item.replace("{c}", "");

    // actuals as placeholder
    // IPAddress sta_subnet = WiFi.subnetMask();
    // item.replace("{p}", sta_subnet.toString());
    page += item;
    page += "<br/>"; // @todo remove these, use css
  }
  return page;
}

String WiFiManager::getParamOut(){
  String page;

  if(_paramsCount > 0){

    String HTTP_PARAM_temp = FPSTR(HTTP_FORM_LABEL);
    HTTP_PARAM_temp += FPSTR(HTTP_FORM_PARAM);
    bool tok_I = HTTP_PARAM_temp.indexOf("{I}") > 0;
    bool tok_i = HTTP_PARAM_temp.indexOf("{i}") > 0;
    bool tok_n = HTTP_PARAM_temp.indexOf("{n}") > 0;
    bool tok_p = HTTP_PARAM_temp.indexOf("{p}") > 0;
    bool tok_t = HTTP_PARAM_temp.indexOf("{t}") > 0;
    bool tok_l = HTTP_PARAM_temp.indexOf("{l}") > 0;
    bool tok_v = HTTP_PARAM_temp.indexOf("{v}") > 0;
    bool tok_c = HTTP_PARAM_temp.indexOf("{c}") > 0;

    page += FPSTR(HTTP_FORM_PARAM_START);

    char parLength[5];
    // add the extra parameters to the form
    for (int i = 0; i < _paramsCount; i++) {
      if (_params[i] == NULL) {
        break;
      }

     // label before or after, this could probably be done via floats however
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

      // if no ID use customhtml for item, else generate from param string
      if (_params[i]->getID() != NULL) {
        if(tok_I)pitem.replace("{I}", "param_"+(String)i);
        if(tok_i)pitem.replace("{i}", _params[i]->getID());
        if(tok_n)pitem.replace("{n}", _params[i]->getID());
        if(tok_p)pitem.replace("{p}", "{t}");
        if(tok_t)pitem.replace("{t}", _params[i]->getPlaceholder());
        snprintf(parLength, 5, "%d", _params[i]->getValueLength());
        if(tok_l)pitem.replace("{l}", parLength);
        if(tok_v)pitem.replace("{v}", _params[i]->getValue());
        if(tok_c)pitem.replace("{c}", _params[i]->getCustomHTML()); // meant for additional attributes, not html
      } else {
        pitem = _params[i]->getCustomHTML();
      }

      page += pitem;
    }
    if (_params[0] != NULL) {
      page += FPSTR(HTTP_FORM_PARAM_END);
    }
  }

  return page;
}

void WiFiManager::handleWiFiStatus(){
  DEBUG_WM(F("<- HTTP WiFi status "));
  handleRequest();
  String page;
  // String page = "{\"result\":true,\"count\":1}";
  page = FPSTR(HTTP_JS);
  server->sendHeader("Content-Length", String(page.length()));
  server->send(200, "text/html", page);
}

/** 
 * HTTPD CALLBACK save form and redirect to WLAN config page again
 */
void WiFiManager::handleWifiSave() {
  DEBUG_WM(F("<- HTTP WiFi save "));
  DEBUG_WM(F("Method:"),server->method() == HTTP_GET  ? "GET" : "POST");
  handleRequest();

  //SAVE/connect here
  _ssid = server->arg("s").c_str();
  _pass = server->arg("p").c_str();

  //parameters
  if(_paramsCount > 0){
    DEBUG_WM("Parameters");
    DEBUG_WM("-----------");
    for (int i = 0; i < _paramsCount; i++) {
      if (_params[i] == NULL) {
        break;
      }
      //read parameter from server
      String value;
      if(server->arg("param_"+(String)i) != NULL) value = server->arg("param_"+(String)i).c_str();
      else value = server->arg(_params[i]->getID()).c_str();

      //store it in array
      value.toCharArray(_params[i]->_value, _params[i]->_length+1); // length+1 null terminated
      DEBUG_WM((String)_params[i]->getID() + ":",value);
    }
    DEBUG_WM("-----------");
  }

  if (server->arg("ip") != "") {
    //_sta_static_ip.fromString(server->arg("ip"));
    String ip = server->arg("ip");
    optionalIPFromString(&_sta_static_ip, ip.c_str());
    DEBUG_WM(F("static ip:"),ip);
  }
  if (server->arg("gw") != "") {
    String gw = server->arg("gw");
    optionalIPFromString(&_sta_static_gw, gw.c_str());
    DEBUG_WM(F("static gateway:"),gw);
  }
  if (server->arg("sn") != "") {
    String sn = server->arg("sn");
    optionalIPFromString(&_sta_static_sn, sn.c_str());
    DEBUG_WM(F("static netmask:"),sn);
  }

  String page = getHTTPHead(FPSTR(S_titlewifisaved)); // @token titlewifisaved
  page += FPSTR(HTTP_SAVED);
  page += FPSTR(HTTP_END);

  server->sendHeader("Content-Length", String(page.length()));
  server->send(200, "text/html", page);

  DEBUG_WM(F("Sent wifi save page"));

  connect = true; //signal ready to connect/reset
}

/** 
 * HTTPD CALLBACK info page
 */
void WiFiManager::handleInfo() {
  DEBUG_WM(F("<- HTTP Info"));
  handleRequest();
  String page = getHTTPHead(FPSTR(S_titleinfo)); // @token titleinfo
  reportStatus(page);

  #ifdef ESP8266  
    String infoids[] = {
    "esphead",
    "uptime",
    "chipid",
    "fchipid", //esp8266
    "idesize", //esp8266
    "flashsize",
    "sdkver",
    "corever", //esp8266
    "bootver", //esp8266
    "cpufreq",
    "freeheap",
    "memsketch", //esp8266
    "memsmeter", //esp8266
    "lastreset", //esp8266
    "wifihead",
    "apip",
    "apmac",
    "apssid",
    "apbssid",
    "staip",
    "stagw",
    "stasub",
    "dnss",
    "host",
    "stamac",
    "conx",
    "autoconx"
    };

    for(int i=0; i<25;i++){
      if(infoids[i] != NULL) page += getInfoData(infoids[i]);
    }
    page += F("</dl>");

  #elif defined(ESP32)
    String infoids[] = {
    "esphead",
    "uptime",
    "chipid",
    "chiprev",
    // "fchipid", //esp8266
    "idesize", //esp8266
    // "flashsize",
    "sdkver",
    // "corever", //esp8266
    // "bootver", //esp8266
    "cpufreq",
    "freeheap",
    // "memsketch", //esp8266
    // "memsmeter", //esp8266
    "lastreset", //esp8266
    "wifihead",
    "apip",
    "apmac",
    "apssid",
    "apbssid",
    "staip",
    "stagw",
    "stasub",
    "dnss",
    "host",
    "stamac",
    "conx",
    "autoconx"
    };

    for(int i=0; i<19;i++){
      if(infoids[i] != NULL) page += getInfoData(infoids[i]);
    }
    page += F("</dl>");
  #endif

  page += FPSTR(HTTP_ERASEBTN);
  page += FPSTR(HTTP_HELP);
  page += FPSTR(HTTP_END);

  server->sendHeader("Content-Length", String(page.length()));
  server->send(200, "text/html", page);

  DEBUG_WM(F("Sent info page"));
}

String WiFiManager::getInfoData(String id){

  String p;
  // @todo add versioning
  if(id=="esphead")p = FPSTR(HTTP_INFO_esphead);
  else if(id=="wifihead")p = FPSTR(HTTP_INFO_wifihead);
  else if(id=="uptime"){
    // subject to rollover!
    p = FPSTR(HTTP_INFO_uptime);
    p.replace("{1}",(String)(millis() / 1000 / 60));
    p.replace("{2}",(String)((millis() / 1000) % 60));
  }
  else if(id=="chipid"){
    p = FPSTR(HTTP_INFO_chipid);
    p.replace("{1}",(String)WIFI_getChipId());
  }
  else if(id=="chiprev"){
    #ifdef ESP32
      p = FPSTR(HTTP_INFO_chiprev);
      p.replace("{1}",(String)ESP.getChipRevision());
    #endif
  }
  else if(id=="fchipid"){
    #ifdef ESP8266
      p = FPSTR(HTTP_INFO_fchipid);
      p.replace("{1}",(String)ESP.getFlashChipId());
    #endif
  }
  else if(id=="idesize"){
    p = FPSTR(HTTP_INFO_idesize);
    p.replace("{1}",(String)ESP.getFlashChipSize());
  }
  else if(id=="flashsize"){
    #ifdef ESP8266
      p = FPSTR(HTTP_INFO_flashsize);
      p.replace("{1}",(String)ESP.getFlashChipRealSize());
    #endif
  }
  else if(id=="sdkver"){
    p = FPSTR(HTTP_INFO_sdkver);
    #ifdef ESP32
      p.replace("{1}",(String)esp_get_idf_version());
    #else
      p.replace("{1}",(String)system_get_sdk_version());
    #endif
  }
  else if(id=="corever"){
    #ifdef ESP8266
      p = FPSTR(HTTP_INFO_corever);
      p.replace("{1}",(String)ESP.getCoreVersion());
    #endif      
  }
  else if(id=="bootver"){
    #ifdef ESP8266
      p = FPSTR(HTTP_INFO_bootver);
      p.replace("{1}",(String)system_get_boot_version());
    #endif  
  }
  else if(id=="cpufreq"){
    p = FPSTR(HTTP_INFO_cpufreq);
    p.replace("{1}",(String)ESP.getCpuFreqMHz());
  }
  else if(id=="freeheap"){
    p = FPSTR(HTTP_INFO_freeheap);
    p.replace("{1}",(String)ESP.getFreeHeap());
  }
  else if(id=="memsketch"){
    #ifdef ESP8266
      p = FPSTR(HTTP_INFO_memsketch);
      p.replace("{1}",(String)((ESP.getSketchSize()+ESP.getFreeSketchSpace())-ESP.getFreeSketchSpace()));
      p.replace("{2}",(String)(ESP.getSketchSize()+ESP.getFreeSketchSpace()));
    #endif  
  }
  else if(id=="memsmeter"){
    #ifdef ESP8266
      p = FPSTR(HTTP_INFO_memsmeter);
      p.replace("{1}",(String)((ESP.getSketchSize()+ESP.getFreeSketchSpace())-ESP.getFreeSketchSpace()));
      p.replace("{2}",(String)(ESP.getSketchSize()+ESP.getFreeSketchSpace()));
    #endif 
  }
  else if(id=="lastreset"){
    #ifdef ESP8266
      p = FPSTR(HTTP_INFO_lastreset);
      p.replace("{1}",(String)ESP.getResetReason());
    #elif defined(ESP32) && defined(_ROM_RTC_H_)
      // requires #include <rom/rtc.h>
      p = FPSTR(HTTP_INFO_lastreset);
      for(int i=0;i<2;i++){
        int reason = rtc_get_reset_reason(i);
        String tok = "{"+(String)(i+1)+"}";
        switch (reason)
        {
          case 1  : p.replace(tok,"Vbat power on reset");break;
          case 3  : p.replace(tok,"Software reset digital core");break;
          case 4  : p.replace(tok,"Legacy watch dog reset digital core");break;
          case 5  : p.replace(tok,"Deep Sleep reset digital core");break;
          case 6  : p.replace(tok,"Reset by SLC module, reset digital core");break;
          case 7  : p.replace(tok,"Timer Group0 Watch dog reset digital core");break;
          case 8  : p.replace(tok,"Timer Group1 Watch dog reset digital core");break;
          case 9  : p.replace(tok,"RTC Watch dog Reset digital core");break;
          case 10 : p.replace(tok,"Instrusion tested to reset CPU");break;
          case 11 : p.replace(tok,"Time Group reset CPU");break;
          case 12 : p.replace(tok,"Software reset CPU");break;
          case 13 : p.replace(tok,"RTC Watch dog Reset CPU");break;
          case 14 : p.replace(tok,"for APP CPU, reseted by PRO CPU");break;
          case 15 : p.replace(tok,"Reset when the vdd voltage is not stable");break;
          case 16 : p.replace(tok,"RTC Watch dog reset digital core and rtc module");break;
          default : p.replace(tok,"NO_MEAN");
        }
      }
    #endif
  }
  else if(id=="apip"){
    p = FPSTR(HTTP_INFO_apip);
    p.replace("{1}",WiFi.softAPIP().toString());
  }
  else if(id=="apmac"){
    p = FPSTR(HTTP_INFO_apmac);
    p.replace("{1}",(String)WiFi.softAPmacAddress());
  }
  else if(id=="apssid"){
    p = FPSTR(HTTP_INFO_apssid);
    p.replace("{1}",(String)WiFi_SSID());
  }
  else if(id=="apbssid"){
    p = FPSTR(HTTP_INFO_apbssid);
    p.replace("{1}",(String)WiFi.BSSIDstr());
  }
  else if(id=="staip"){
    p = FPSTR(HTTP_INFO_staip);
    p.replace("{1}",WiFi.localIP().toString());
  }
  else if(id=="stagw"){
    p = FPSTR(HTTP_INFO_stagw);
    p.replace("{1}",WiFi.gatewayIP().toString());
  }
  else if(id=="stasub"){
    p = FPSTR(HTTP_INFO_stasub);
    p.replace("{1}",WiFi.subnetMask().toString());
  }
  else if(id=="dnss"){
    p = FPSTR(HTTP_INFO_dnss);
    p.replace("{1}",WiFi.dnsIP().toString());
  }
  else if(id=="host"){
    p = FPSTR(HTTP_INFO_host);
    #ifdef ESP32
      p.replace("{1}",WiFi.getHostname());
    #else
      p.replace("{1}",WiFi.hostname());
    #endif
  }
  else if(id=="stamac)"){
    p = FPSTR(HTTP_INFO_stamac);
    p.replace("{1}",WiFi.macAddress());
  }
  else if(id=="conx"){
    p = FPSTR(HTTP_INFO_conx);
    p.replace("{1}",WiFi.isConnected() ? FPSTR(S_y) : FPSTR(S_n));
  }
  else if(id=="autoconx"){
    p = FPSTR(HTTP_INFO_autoconx);
    p.replace("{1}",WiFi.getAutoConnect() ? FPSTR(S_enable) : FPSTR(S_disable));
  }
  return p;
}

/** 
 * HTTPD CALLBACK root or redirect to captive portal
 */
void WiFiManager::handleExit() {
  DEBUG_WM(F("<- HTTP Exit"));
  handleRequest();
  String page = getHTTPHead(FPSTR(S_titleexit)); // @token titleexit
  page += FPSTR(S_exiting); // @token exiting
  server->sendHeader("Content-Length", String(page.length()));
  server->send(200, "text/html", page);
  abort = true;
}

/** 
 * HTTPD CALLBACK reset page
 */
void WiFiManager::handleReset() {
  DEBUG_WM(F("<- HTTP Reset"));
  handleRequest();
  String page = getHTTPHead(FPSTR(S_titlereset)); //@token titlereset
  page += FPSTR(S_resetting); //@token resetting
  page += FPSTR(HTTP_END);

  server->sendHeader("Content-Length", String(page.length()));
  server->send(200, "text/html", page);

  DEBUG_WM(F("RESETTING ESP"));
  delay(1000);
  reboot();
}

/** 
 * HTTPD CALLBACK erase page
 */
void WiFiManager::handleErase() {
  DEBUG_WM(F("<- HTTP Erase"));
  handleRequest();
  String page = getHTTPHead(FPSTR(S_titleerase)); // @token titleerase
  page += FPSTR(HTTP_HEAD_END);

  bool ret = WiFi_eraseConfig();

  if(ret) page += FPSTR(S_resetting); // @token resetting
  else {
    page += FPSTR(S_error); // @token erroroccur
    DEBUG_WM(F("[ERROR] WiFi EraseConfig failed"));
  }

  page += FPSTR(HTTP_END);
  server->sendHeader("Content-Length", String(page.length()));
  server->send(200, "text/html", page);

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
  message += ( server->method() == HTTP_GET ) ? "GET" : "POST";
  message += FPSTR(S_args); // @token args
  message += server->args();
  message += "\n";

  for ( uint8_t i = 0; i < server->args(); i++ ) {
    message += " " + server->argName ( i ) + ": " + server->arg ( i ) + "\n";
  }
  server->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server->sendHeader("Pragma", "no-cache");
  server->sendHeader("Expires", "-1");
  server->sendHeader("Content-Length", String(message.length()));
  server->send ( 404, "text/plain", message );
}

/**
 * HTTPD redirector
 * Redirect to captive portal if we got a request for another domain. 
 * Return true in that case so the page handler do not try to handle the request again. 
 */
boolean WiFiManager::captivePortal() {
  DEBUG_WM("-> " + server->hostHeader());
  
  if(!_enableCaptivePortal) return false; // skip redirections

  if (!isIp(server->hostHeader())) {
    DEBUG_WM(F("<- Request redirected to captive portal"));
    server->sendHeader("Location", String("http://") + toStringIp(server->client().localIP()), true);
    server->send ( 302, "text/plain", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server->client().stop(); // Stop is needed because we sent no content length
    return true;
  }
  return false;
}

void WiFiManager::reportStatus(String &page){
  String str;
  if (WiFi_SSID() != ""){
    if (WiFi.status()==WL_CONNECTED){
      str = FPSTR(HTTP_STATUS_ON);
      str.replace("{u}",WiFi.localIP().toString());
      str.replace("{s}",WiFi_SSID());
    }
    else {
      str = FPSTR(HTTP_STATUS_OFF);
      str.replace("{s}",WiFi_SSID());
    }
  }
  else {
    str = FPSTR(HTTP_STATUS_NONE);
  }
  page += str;
}

// MORE SETTERS

//start up config portal callback
void WiFiManager::setAPCallback( void (*func)(WiFiManager* myWiFiManager) ) {
  _apcallback = func;
}

//start up save config callback
void WiFiManager::setSaveConfigCallback( void (*func)(void) ) {
  _savecallback = func;
}

//sets a custom element to add to head, like a new style tag
void WiFiManager::setCustomHeadElement(const char* element) {
  _customHeadElement = element;
}

//if this is true, remove duplicated Access Points - defaut true
void WiFiManager::setRemoveDuplicateAPs(boolean removeDuplicates) {
  _removeDuplicateAPs = removeDuplicates;
}

//if this is true, remove duplicated Access Points - defaut true
void WiFiManager::setConfigPortalBlocking(boolean shoudlBlock) {
  _configPortalIsBlocking = shoudlBlock;
}

//setter for ESP wifi.persistent so we can remember it and restore user preference, as WIFi._persistent is protected
void WiFiManager::setRestorePersistent(boolean persistent) {
  _userpersistent = persistent;
  if(!persistent) DEBUG_WM(F("persistent is off"));
}

void WiFiManager::setShowStaticFields(boolean alwaysShow){
  _staShowStaticFields = alwaysShow;
}

void WiFiManager::setCaptivePortalEnable(boolean enabled){
  _enableCaptivePortal = enabled;
}

void WiFiManager::setWiFiAutoReconnect(boolean enabled){
  _wifiAutoReconnect = enabled;
}

void WiFiManager::setCaptivePortalClientCheck(boolean enabled){
  _cpClientCheck = enabled;
}

void WiFiManager::setWebPortalClientCheck(boolean enabled){
  _webClientCheck = enabled;
}

// HELPERS

template <typename Generic>
void WiFiManager::DEBUG_WM(Generic text) {
  if (_debug) {
    _debugPort.print("*WM: ");
    _debugPort.print(text);
    _debugPort.print("\n");
  }
}

template <typename Generic, typename Genericb>
void WiFiManager::DEBUG_WM(Generic text,Genericb textb) {
  if (_debug) {
    _debugPort.print("*WM: ");
    _debugPort.print(text);
    _debugPort.print(" ");
    _debugPort.print(textb);
    _debugPort.print("\n");
  }
}

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
    DEBUG_WM(F("--------------------"));
    DEBUG_WM(F("ssid:            "),(char *) config.ssid);
    DEBUG_WM(F("password:        "),(char *) config.password);
    DEBUG_WM(F("ssid_len:        "),config.ssid_len);
    DEBUG_WM(F("channel:         "),config.channel);
    DEBUG_WM(F("authmode:        "),config.authmode);
    DEBUG_WM(F("ssid_hidden:     "),config.ssid_hidden);
    DEBUG_WM(F("max_connection:  "),config.max_connection);
    DEBUG_WM(F("beacon_interval: "),(String)config.beacon_interval + "(ms)");
    DEBUG_WM(F("--------------------"));
}

void WiFiManager::debugPlatformInfo(){
  #ifdef ESP8266
    system_print_meminfo();
    DEBUG_WM("getCoreVersion():",ESP.getCoreVersion());
    DEBUG_WM("system_get_sdk_version():",system_get_sdk_version());
    DEBUG_WM("system_get_boot_version():",system_get_boot_version());
  #elif defined(ESP32)
    size_t freeHeap = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    DEBUG_WM("Free heap: ", freeHeap);
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
  for (int i = 0; i < str.length(); i++) {
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
      DEBUG_WM(F("AccessPoint set password is INVALID"));
      _apPassword = "";
      return false; // @todo FATAL or fallback to empty ?
    }
    DEBUG_WM(F("AccessPoint set password is VALID"));
    DEBUG_WM(_apPassword);
  }
  return true;
}

String WiFiManager::getWLStatusString(uint8_t status){
  const char * const WIFI_STA_STATUS[]
  {
      "WL_IDLE_STATUS",
      "WL_NO_SSID_AVAIL",
      "WL_SCAN_COMPLETED",
      "WL_CONNECTED",
      "WL_CONNECT_FAILED",
      "WL_CONNECTION_LOST",
      "WL_DISCONNECTED"
  };

  if(status >=0 && status <=6) return WIFI_STA_STATUS[status];
  return "UNKNOWN";
}

String WiFiManager::encryptionTypeStr(uint8_t authmode) {
    #ifdef ESP8266
      switch(authmode) {
          case ENC_TYPE_NONE:
              return "None";
          case ENC_TYPE_WEP:
              return "WEP";
          case ENC_TYPE_TKIP:
              return "WPA_PSK";
          case ENC_TYPE_CCMP:
              return "WPA2_PSK";
          case ENC_TYPE_AUTO:
              return "WPA_WPA2_PSK";
          default:
              return "Unknown";
      }
    #elif defined(ESP32)
      switch(authmode) {
          case WIFI_AUTH_OPEN:
              return "None";
          case WIFI_AUTH_WEP:
              return "WEP";
          case WIFI_AUTH_WPA_PSK:
              return "WPA_PSK";
          case WIFI_AUTH_WPA_WPA2_PSK:
              return "WPA2_PSK";
          case WIFI_AUTH_WPA2_ENTERPRISE:
              return "WPA_WPA2_ENTERPRISE";
          case WIFI_AUTH_MAX:
              return "MAX";
          default:
              return "Unknown";
      }
    #endif
}

// set mode ignores WiFi.persistent 
bool WiFiManager::WiFi_Mode(WiFiMode_t m,bool persistent) {
    #ifdef ESP8266
      if((wifi_get_opmode() == (uint8) m ) && !persistent) {
          return true;
      }
      bool ret;
      ETS_UART_INTR_DISABLE();
      if(persistent) ret = wifi_set_opmode(m);
      else ret = wifi_set_opmode_current(m);
      ETS_UART_INTR_ENABLE();

    return ret;
    #elif defined(ESP32)
      return WiFi.mode(m); // @todo persistent not implemented?
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
          DEBUG_WM(F("wifi station disconnect"));
          ETS_UART_INTR_DISABLE(); 
          ret = wifi_station_disconnect();
          ETS_UART_INTR_ENABLE();        
          return ret;
      }
    #elif defined(ESP32)
      DEBUG_WM(F("wifi station disconnect"));
      // @todo why does disconnect call these, might be needed
      // WiFi.getMode(); // @todo wifiLowLevelInit(), probably not needed, for save config only
      // esp_wifi_start(); // @todo can only disconnect if enabled perhaps, prevent failure, or correct for previous call ?
      return esp_wifi_disconnect() == ESP_OK;
      // return WiFi.disconnect();
    #endif
}

// toggle STA without persistent
bool WiFiManager::WiFi_enableSTA(bool enable,bool persistent) {
    WiFiMode_t newMode;
    WiFiMode_t currentMode = WiFi.getMode();
    bool isEnabled         = (currentMode & WIFI_STA) != 0;
    if(enable) newMode     = (WiFiMode_t)(currentMode | WIFI_STA);
    else newMode           = (WiFiMode_t)(currentMode & (~WIFI_STA));

    #ifdef ESP8266
      if((isEnabled != enable) || persistent) {
          if(enable) {
          	if(persistent) DEBUG_WM(F("enableSTA PERSISTENT ON"));
              return WiFi_Mode(newMode,persistent);
          } else {
              return WiFi_Mode(newMode,persistent);
          }
      } else {
          return true;
      }
    #elif defined(ESP32)
      return WiFi.mode(newMode); // @todo persistent not implemented?
    #endif
}
bool WiFiManager::WiFi_enableSTA(bool enable) {
	return WiFi_enableSTA(enable,false);
}

bool WiFiManager::WiFi_eraseConfig(void) {
    #ifdef ESP8266
      return ESP.eraseConfig();
      // https://github.com/esp8266/Arduino/pull/3635
      // erase config BUG polyfill
      // const size_t cfgSize = 0x4000;
      // size_t cfgAddr = ESP.getFlashChipSize() - cfgSize;

      // for (size_t offset = 0; offset < cfgSize; offset += SPI_FLASH_SEC_SIZE) {
      //     if (!ESP.flashEraseSector((cfgAddr + offset) / SPI_FLASH_SEC_SIZE)) {
      //         return false;
      //     }
      // }
      // return true;
    #elif defined(ESP32)
      WiFi.mode(WIFI_AP_STA); // cannot erase if not in STA mode
      return WiFi.disconnect(true);
    #endif
}

void WiFiManager::reboot(){
  ESP.restart();
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
    wifi_config_t conf;
    esp_wifi_get_config(WIFI_IF_STA, &conf);
    return String(reinterpret_cast<const char*>(conf.sta.ssid));
  #endif
}

void WiFiManager::WiFiEvent(WiFiEvent_t event){
  #ifdef ESP32
    WiFiManager _WiFiManager;
    if(event == SYSTEM_EVENT_STA_DISCONNECTED){
      // Serial.println("Event: SYSTEM_EVENT_STA_DISCONNECTED, reconnecting");
      _WiFiManager.DEBUG_WM("ESP32 Event: SYSTEM_EVENT_STA_DISCONNECTED, reconnecting"); // @todo remove debugging from prod, or change static method
      WiFi.reconnect();
    }
  #endif  
}

void WiFiManager::WiFi_autoReconnect(){
  #ifdef ESP8266
    WiFi.setAutoReconnect(_wifiAutoReconnect);
  #elif defined(ESP32)
    if(_wifiAutoReconnect){
      DEBUG_WM("ESP32 autoreconnect handler enabled");
      WiFi.onEvent(WiFiEvent);
    }  
  #endif
}