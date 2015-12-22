/**************************************************************
 * WiFiManager is a library for the ESP8266/Arduino platform
 * (https://github.com/esp8266/Arduino) to enable easy
 * configuration and reconfiguration of WiFi credentials and
 * store them in EEPROM.
 * inspired by: 
 * http://www.esp8266.com/viewtopic.php?f=29&t=2520
 * https://github.com/chriscook8/esp-arduino-apboot
 * https://github.com/esp8266/Arduino/tree/esp8266/hardware/esp8266com/esp8266/libraries/DNSServer/examples/CaptivePortalAdvanced
 * Built by AlexT https://github.com/tzapu
 * Licensed under MIT license
 **************************************************************/

#include "WiFiManager.h"

WiFiManager::WiFiManager() {
}

void WiFiManager::begin() {
  begin("NoNetESP");
}

void WiFiManager::begin(char const *apName) {
  begin(apName,NULL);
}

void WiFiManager::begin(char const *apName, char const *apPasswd) {
  dnsServer.reset(new DNSServer());
  server.reset(new ESP8266WebServer(80));
  
  DEBUG_PRINT("");
  _apName = apName;
  _apPasswd = apPasswd;
  start = millis();

  DEBUG_PRINT("Configuring access point... ");
  DEBUG_PRINT(_apName);
  if (_apPasswd != NULL)
    DEBUG_PRINT(_apPasswd);

  //optional soft ip config
  if (_ip) {
    DEBUG_PRINT("Custom IP/GW/Subnet");
    WiFi.softAPConfig(_ip, _gw, _sn);
  }

  if (_apPasswd != NULL)
    WiFi.softAP(_apName, _apPasswd);//password option
  else
    WiFi.softAP(_apName);

  delay(500); // Without delay I've seen the IP address blank
  DEBUG_PRINT("AP IP address: ");
  DEBUG_PRINT(WiFi.softAPIP());

  /* Setup the DNS server redirecting all the domains to the apIP */  
  dnsServer->setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer->start(DNS_PORT, "*", WiFi.softAPIP());

  /* Setup web pages: root, wifi config pages, SO captive portal detectors and not found. */
  server->on("/", std::bind(&WiFiManager::handleRoot, this));
  server->on("/wifi", std::bind(&WiFiManager::handleWifi, this, true));
  server->on("/0wifi", std::bind(&WiFiManager::handleWifi, this, false));
  server->on("/wifisave", std::bind(&WiFiManager::handleWifiSave, this));
  server->on("/generate_204", std::bind(&WiFiManager::handle204, this));  //Android/Chrome OS captive portal check.
  server->on("/fwlink", std::bind(&WiFiManager::handleRoot, this));  //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
  server->onNotFound (std::bind(&WiFiManager::handleNotFound, this));
  server->begin(); // Web server start
  DEBUG_PRINT("HTTP server started");
}

boolean WiFiManager::autoConnect() {
  String ssid = "ESP" + String(ESP.getChipId());
  return autoConnect(ssid.c_str(),NULL,NULL);
}

boolean WiFiManager::autoConnect(char const *apName) {
  return autoConnect(apName,NULL,NULL);
}

boolean WiFiManager::autoConnect(char const *apName, char const *apPasswd,const char* aNets[][2]) {
  DEBUG_PRINT("");
  DEBUG_PRINT("AutoConnect");
  
  // read eeprom for ssid and pass
  String ssid = getSSID();
  String pass = getPassword();
  //use SDK functions to get SSID and pass
  //String ssid = WiFi.SSID();
  //String pass = WiFi.psk();
  
  WiFi.mode(WIFI_STA);
  
  int s = connectWifi(ssid, pass); //try to connect to stored params
  while (s != WL_CONNECTED && aNets != NULL) 
  {
	for(int f=0;f<sizeof(aNets);f++)
	{
		s=connectWifi(aNets[f][0], aNets[f][1]); //try to connect to stored params
		s = WiFi.status();
		if (s == WL_CONNECTED)
		{
			DEBUG_PRINT("IP Address:");
			DEBUG_PRINT(WiFi.localIP());
			//connected
			return true;
		}
	}
	break; //prevent if no connect for any nets of array
  }
 
  //not connected
  //setup AP
  WiFi.mode(WIFI_AP);
  //notify we entered AP mode
  if( _apcallback != NULL) {
    _apcallback();
  }
  connect = false;
  begin(apName,apPasswd);

  bool looping = true;
  while(timeout == 0 || millis() < start + timeout) {
    //DNS
    dnsServer->processNextRequest();
    //HTTP
    server->handleClient();

    
    if(connect) {
      delay(2000);
      DEBUG_PRINT("Connecting to new AP");
      connect = false;
      //ssid = getSSID();
      //pass = getPassword();
      connectWifi(_ssid, _pass);
      int s = WiFi.status();
      if (s != WL_CONNECTED) {
        DEBUG_PRINT("Failed to connect.");
        //not connected, should retry everything
        //ESP.reset();
        //delay(1000);
        //return false;
      } else {
        //connected 
        WiFi.mode(WIFI_STA);
        break;
      }
 
    }
    yield();    
  }

  server.reset();
  dnsServer.reset();
  return  WiFi.status() == WL_CONNECTED;
}


void WiFiManager::connectWifi(String ssid, String pass) {
  DEBUG_PRINT("Connecting as wifi client...");
  //WiFi.disconnect();
  WiFi.begin(ssid.c_str(), pass.c_str());
  int connRes = WiFi.waitForConnectResult();
  DEBUG_PRINT ("Connection result: ");
  DEBUG_PRINT ( connRes );
}


String WiFiManager::getSSID() {
  if (_ssid == "") {
    DEBUG_PRINT("Reading EEPROM SSID");
    _ssid = WiFi.SSID();//getEEPROMString(0, 32);
    DEBUG_PRINT("SSID: ");
    DEBUG_PRINT(_ssid);
  }
  return _ssid;
}

String WiFiManager::getPassword() {
  if (_pass == "") {
    DEBUG_PRINT("Reading EEPROM Password");
    _pass = WiFi.psk();//getEEPROMString(32, 64);
    DEBUG_PRINT("Password: " + _pass);
    //DEBUG_PRINT(_pass);
  }
  return _pass;
}

/*
String WiFiManager::getEEPROMString(int start, int len) {
  EEPROM.begin(512);
  delay(10);
  String string = "";
  for (int i = _eepromStart + start; i < _eepromStart + start + len; i++) {
    //DEBUG_PRINT(i);
    string += char(EEPROM.read(i));
  }
  EEPROM.end();
  return string;
}
*/
/*
void WiFiManager::setEEPROMString(int start, int len, String string) {
  EEPROM.begin(512);
  delay(10);
  int si = 0;
  for (int i = _eepromStart + start; i < _eepromStart + start + len; i++) {
    char c;
    if (si < string.length()) {
      c = string[si];
      //DEBUG_PRINT("Wrote: ");
      //DEBUG_PRINT(c);
    } else {
      c = 0;
    }
    EEPROM.write(i, c);
    si++;
  }
  EEPROM.end();
  DEBUG_PRINT("Wrote " + string);
}*/

String WiFiManager::urldecode(const char *src)
{
  String decoded = "";
  char a, b;
  while (*src) {
    if ((*src == '%') &&
        ((a = src[1]) && (b = src[2])) &&
        (isxdigit(a) && isxdigit(b))) {
      if (a >= 'a')
        a -= 'a' - 'A';
      if (a >= 'A')
        a -= ('A' - 10);
      else
        a -= '0';
      if (b >= 'a')
        b -= 'a' - 'A';
      if (b >= 'A')
        b -= ('A' - 10);
      else
        b -= '0';

      decoded += char(16 * a + b);
      src += 3;
    } else if (*src == '+') {
      decoded += ' ';
      *src++;
    } else {
      decoded += *src;
      *src++;
    }
  }
  decoded += '\0';

  return decoded;
}

void WiFiManager::resetSettings() {
  //need to call it only after lib has been started with autoConnect or begin
  DEBUG_PRINT("settings invalidated");
  WiFi.disconnect();
  delay(200);
}

void WiFiManager::setTimeout(unsigned long seconds) {
  timeout = seconds * 1000;
}

void WiFiManager::setDebugOutput(boolean debug) {
  _debug = debug;
}

void WiFiManager::setAPConfig(IPAddress ip, IPAddress gw, IPAddress sn) {
  _ip = ip;
  _gw = gw;
  _sn = sn;
}


/** Handle root or redirect to captive portal */
void WiFiManager::handleRoot() {
  DEBUG_PRINT("Handle root");
  if (captivePortal()) { // If caprive portal redirect instead of displaying the page.
    return;
  }

  server->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server->sendHeader("Pragma", "no-cache");
  server->sendHeader("Expires", "-1");
  server->send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.

  String head = HTTP_HEAD;
  head.replace("{v}", "Options");
  server->sendContent(head);
  server->sendContent(HTTP_SCRIPT);
  server->sendContent(HTTP_STYLE);
  server->sendContent(HTTP_HEAD_END);
  
  server->sendContent(
    "<form action=\"/wifi\" method=\"get\"><button>Configure WiFi</button></form><br/>"
  );
  server->sendContent(
    "<form action=\"/0wifi\" method=\"get\"><button>Configure WiFi (No Scan)</button></form>"
  );
  
  server->sendContent(HTTP_END);

  server->client().stop(); // Stop is needed because we sent no content length
}

/** Wifi config page handler */
void WiFiManager::handleWifi(bool scan) {
  server->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server->sendHeader("Pragma", "no-cache");
  server->sendHeader("Expires", "-1");
  server->send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.


  String head = HTTP_HEAD;
  head.replace("{v}", "Config ESP");
  server->sendContent(head);
  server->sendContent(HTTP_SCRIPT);
  server->sendContent(HTTP_STYLE);
  server->sendContent(HTTP_HEAD_END);

  if (scan) {
    int n = WiFi.scanNetworks();
    DEBUG_PRINT("Scan done");
    if (n == 0) {
      DEBUG_PRINT("No networks found");
      server->sendContent("<div>No networks found. Refresh to scan again.</div>");
    }
    else {
      for (int i = 0; i < n; ++i)
      {
        DEBUG_PRINT(WiFi.SSID(i));
        DEBUG_PRINT(WiFi.RSSI(i));
        String item = HTTP_ITEM;
        item.replace("{v}", WiFi.SSID(i));
        server->sendContent(item);
        yield();
      }
    }
  }
  
  server->sendContent(HTTP_FORM);
  server->sendContent(HTTP_END);
  server->client().stop();
  
  DEBUG_PRINT("Sent config page");  
}

/** Handle the WLAN save form and redirect to WLAN config page again */
void WiFiManager::handleWifiSave() {
  DEBUG_PRINT("WiFi save");

  //SAVE/connect here
  _ssid = urldecode(server->arg("s").c_str());
  _pass = urldecode(server->arg("p").c_str());

  server->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server->sendHeader("Pragma", "no-cache");
  server->sendHeader("Expires", "-1");
  server->send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.

  String head = HTTP_HEAD;
  head.replace("{v}", "Credentials Saved");
  server->sendContent(head);
  server->sendContent(HTTP_SCRIPT);
  server->sendContent(HTTP_STYLE);
  server->sendContent(HTTP_HEAD_END);
  
  server->sendContent(HTTP_SAVED);

  server->sendContent(HTTP_END);
  server->client().stop();
  
  DEBUG_PRINT("Sent wifi save page");  
  
  connect = true; //signal ready to connect/reset
}

void WiFiManager::handle204() {
  DEBUG_PRINT("204 No Response");  
  server->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server->sendHeader("Pragma", "no-cache");
  server->sendHeader("Expires", "-1");
  server->send ( 204, "text/plain", "");
}

void WiFiManager::handleNotFound() {
  if (captivePortal()) { // If captive portal redirect instead of displaying the error page.
    return;
  }
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server->uri();
  message += "\nMethod: ";
  message += ( server->method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server->args();
  message += "\n";

  for ( uint8_t i = 0; i < server->args(); i++ ) {
    message += " " + server->argName ( i ) + ": " + server->arg ( i ) + "\n";
  }
  server->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server->sendHeader("Pragma", "no-cache");
  server->sendHeader("Expires", "-1");
  server->send ( 404, "text/plain", message );
}


/** Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
boolean WiFiManager::captivePortal() {
  if (!isIp(server->hostHeader()) ) {
    DEBUG_PRINT("Request redirected to captive portal");
    server->sendHeader("Location", String("http://") + toStringIp(server->client().localIP()), true);
    server->send ( 302, "text/plain", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server->client().stop(); // Stop is needed because we sent no content length
    return true;
  }
  return false;
}

//start up config portal callback
void WiFiManager::setAPCallback( void (*func)(void) ) {
  _apcallback = func;
}


template <typename Generic>
void WiFiManager::DEBUG_PRINT(Generic text) {
  if(_debug) {
    Serial.print("*WM: ");
    Serial.println(text);    
  }
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


