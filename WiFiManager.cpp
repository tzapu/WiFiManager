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


DNSServer dnsServer;

// Web server
ESP8266WebServer server(80);

WiFiManager::WiFiManager() {
}

void WiFiManager::begin() {
  begin("NoNetESP");
}

void WiFiManager::begin(char const *apName) {
  Serial.println("");
  _apName = apName;
  start = millis();

  Serial.print("Configuring access point... ");
  Serial.println(_apName);
  WiFi.softAP(_apName);//TODO: add password option
  delay(500); // Without delay I've seen the IP address blank
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  /* Setup the DNS server redirecting all the domains to the apIP */  
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

  /* Setup web pages: root, wifi config pages, SO captive portal detectors and not found. */
  server.on("/", std::bind(&WiFiManager::handleRoot, this));
  server.on("/wifi", std::bind(&WiFiManager::handleWifi, this));
  server.on("/wifisave", std::bind(&WiFiManager::handleWifiSave, this));
  server.on("/generate_204", std::bind(&WiFiManager::handle204, this));  //Android/Chrome OS captive portal check.
  server.on("/fwlink", std::bind(&WiFiManager::handleRoot, this));  //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
  server.onNotFound (std::bind(&WiFiManager::handleNotFound, this));
  server.begin(); // Web server start
  Serial.println("HTTP server started");
}

boolean WiFiManager::autoConnect() {
  return autoConnect("NoNetESP");
}

boolean WiFiManager::autoConnect(char const *apName) {
  DEBUG_PRINT("");
  DEBUG_PRINT("AutoConnect");
  // read eeprom for ssid and pass
  String ssid = getSSID();
  String pass = getPassword();

  WiFi.mode(WIFI_STA);
  connectWifi(ssid, pass);
  int s = WiFi.status();
  if (s == WL_CONNECTED) {
    //connected
    return true;
  }
 
  
/*

  DEBUG_PRINT("");
  DEBUG_PRINT("AutoConnect");
  // read eeprom for ssid and pass
  String ssid = getSSID();
  String pass = getPassword();

  if ( ssid.length() > 1 ) {
    DEBUG_PRINT("Waiting for Wifi to connect");
    
    WiFi.mode(WIFI_STA);
 
    connectWifi(ssid, pass);
    int s = WiFi.status();
    if (s != WL_CONNECTED) {
      //not connected
      //setup AP
      beginConfigMode();
      //start portal and loop
      startWebConfig();
      return false;     
    }
    return true;// connected
  }
*/

 // delay(1000);
  //not connected
  //setup AP
  WiFi.mode(WIFI_AP);

  connect = false;
  begin(apName);

  while(1) {
    //DNS
    dnsServer.processNextRequest();
    //HTTP
    server.handleClient();

    if(connect) {
      delay(5000);
      ESP.reset();
      delay(1000);
    }
    /*
    if(connect) {
      DEBUG_PRINT("Connecting to new AP");
      connect = false;
      ssid = getSSID();
      pass = getPassword();
      connectWifi(ssid, pass);
      int s = WiFi.status();
      if (s == WL_CONNECTED) {
        //connected
        return true;
      }
 
    }*/
    yield();    
  }

  return false;
}


void WiFiManager::connectWifi(String ssid, String pass) {
  Serial.println("Connecting as wifi client...");
  WiFi.disconnect();
  WiFi.begin(ssid.c_str(), pass.c_str());
  int connRes = WiFi.waitForConnectResult();
  Serial.print ( "connRes: " );
  Serial.println ( connRes );
}


String WiFiManager::getSSID() {
  if (_ssid == "") {
    DEBUG_PRINT("Reading EEPROM SSID");
    _ssid = getEEPROMString(0, 32);
    DEBUG_PRINT("SSID: ");
    DEBUG_PRINT(_ssid);
  }
  return _ssid;
}

String WiFiManager::getPassword() {
  if (_pass == "") {
    DEBUG_PRINT("Reading EEPROM Password");
    _pass = getEEPROMString(32, 64);
    DEBUG_PRINT("Password: ");
    DEBUG_PRINT(_pass);
  }
  return _pass;
}


void WiFiManager::setSSID(String s) {
  _ssid = s;
  setEEPROMString(0, 32, _ssid);
}

void WiFiManager::setPassword(String p) {
  DEBUG_PRINT("Save password:");
  DEBUG_PRINT(p);
  _pass = p;
  setEEPROMString(32, 64, _pass);
}

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

void WiFiManager::setEEPROMString(int start, int len, String string) {
  EEPROM.begin(512);
  delay(10);
  int si = 0;
  for (int i = _eepromStart + start; i < _eepromStart + start + len; i++) {
    char c;
    if (si < string.length()) {
      c = string[si];
      DEBUG_PRINT("Wrote: ");
      DEBUG_PRINT(c);
    } else {
      c = 0;
    }
    EEPROM.write(i, c);
    si++;
  }
  EEPROM.end();
}

/*
void WiFiManager::startWebConfig() {
  keepLooping = true;
  
  DEBUG_PRINT("");
  DEBUG_PRINT("Starting Config Portal");
  DEBUG_PRINT(WiFi.localIP());
  DEBUG_PRINT(WiFi.softAPIP());

  while (keepLooping) {
    //looping
    if(timeout > 0 && start + timeout < millis()) {
      //we passed timeout value, release
      DEBUG_PRINT("timeout reached");
      keepLooping = false;
    }
    
    //DNS
    dnsServer.processNextRequest();
    //HTTP
    server.handleClient();
    yield();
  }

  DEBUG_PRINT("Exited setup");
}*/

/*
String WiFiManager::beginConfigMode(void) {

  WiFi.softAP(_apName);
  delay(500); // Without delay I've seen the IP address blank
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  / * Setup the DNS server redirecting all the domains to the apIP * /  
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

  / * Setup web pages: root, wifi config pages, SO captive portal detectors and not found. * /
  server.on("/", std::bind(&WiFiManager::handleRoot, this));
  server.on("/wifi", std::bind(&WiFiManager::handleWifi, this));
  server.on("/wifisave", std::bind(&WiFiManager::handleWifiSave, this));
  server.on("/generate_204", std::bind(&WiFiManager::handleRoot, this));  //Android captive portal. Maybe not needed. Might be handled by notFound handler.
  server.on("/fwlink", std::bind(&WiFiManager::handleRoot, this));  //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
  server.onNotFound (std::bind(&WiFiManager::handleNotFound, this));
  server.begin(); // Web server start
  Serial.println("HTTP server started");
  
}
*/

int WiFiManager::serverLoop()
{
  

/*
  // Check if a client has connected
//  WiFiClient client = server.available();
  if (!client) {
    return (WM_WAIT);
  }

  DEBUG_PRINT("New client");

  // Wait for data from client to become available
  while (client.connected() && !client.available()) {
    delay(1);
  }

  // Read the first line of HTTP request
  String req = client.readStringUntil('\r');

  // First line of HTTP request looks like "GET /path HTTP/1.1"
  // Retrieve the "/path" part by finding the spaces
  int addr_start = req.indexOf(' ');
  int addr_end = req.indexOf(' ', addr_start + 1);
  if (addr_start == -1 || addr_end == -1) {
    DEBUG_PRINT("Invalid request: ");
    DEBUG_PRINT(req);
    return (WM_WAIT);
  }
  req = req.substring(addr_start + 1, addr_end);
  DEBUG_PRINT("Request: ");
  DEBUG_PRINT(req);
  client.flush();

  String s;
  if (req == "/")
  {

    s = HTTP_200;
    String head = HTTP_HEAD;
    head.replace("{v}", "Config ESP");
    s += head;
    s += HTTP_SCRIPT;
    s += HTTP_STYLE;
    s += HTTP_HEAD_END;

    int n = WiFi.scanNetworks();
    DEBUG_PRINT("scan done");
    if (n == 0) {
      DEBUG_PRINT("no networks found");
      s += "<div>No networks found. Refresh to scan again.</div>";
    }
    else {
      for (int i = 0; i < n; ++i)
      {
        DEBUG_PRINT(WiFi.SSID(i));
        DEBUG_PRINT(WiFi.RSSI(i));
        String item = HTTP_ITEM;
        item.replace("{v}", WiFi.SSID(i));
        s += item;
        delay(10);
      }
    }

    s += HTTP_FORM;
    s += HTTP_END;

    DEBUG_PRINT("Sending config page");
  }
  else if ( req.startsWith("/s") ) {
    String qssid;
    qssid = urldecode(req.substring(8, req.indexOf('&')).c_str());
    DEBUG_PRINT(qssid);
    DEBUG_PRINT("");
    String qpass;
    qpass = urldecode(req.substring(req.lastIndexOf('=') + 1).c_str());
    DEBUG_PRINT(qpass);
    DEBUG_PRINT("");

    setEEPROMString(0, 32, qssid);
    setEEPROMString(32, 64, qpass);

    EEPROM.commit();

    s = HTTP_200;
    String head = HTTP_HEAD;
    head.replace("{v}", "Saved config");
    s += HTTP_STYLE;
    s += HTTP_HEAD_END;
    s += "saved to eeprom...<br/>resetting in 10 seconds";
    s += HTTP_END;
    client.print(s);
    client.flush();

    DEBUG_PRINT("Saved WiFiConfig...restarting.");
    return WM_DONE;
  }
  else
  {
    s = HTTP_404;
    DEBUG_PRINT("Sending 404");
  }

  client.print(s);
  DEBUG_PRINT("Done with client");
  return (WM_WAIT);

  */
}

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
  setEEPROMString(0, 32, "-");
  setEEPROMString(32, 64, "-");

  DEBUG_PRINT("settings invalidated");
  delay(200);
  WiFi.disconnect();
}

void WiFiManager::setTimeout(unsigned long seconds) {
  timeout = seconds * 1000;
}






/** Handle root or redirect to captive portal */
void WiFiManager::handleRoot() {
  DEBUG_PRINT("Handle root");
  if (captivePortal()) { // If caprive portal redirect instead of displaying the page.
    return;
  }

  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.

  String head = HTTP_HEAD;
  head.replace("{v}", "Options");
  server.sendContent(head);
  server.sendContent(HTTP_SCRIPT);
  server.sendContent(HTTP_STYLE);
  server.sendContent(HTTP_HEAD_END);
  
  server.sendContent(
    "<form action=\"/wifi\" method=\"get\"><button>Configure WiFi</button></form>"
  );
  
  server.sendContent(HTTP_END);

  server.client().stop(); // Stop is needed because we sent no content length
}

/** Wifi config page handler */
void WiFiManager::handleWifi() {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.


  String head = HTTP_HEAD;
  head.replace("{v}", "Config ESP");
  server.sendContent(head);
  server.sendContent(HTTP_SCRIPT);
  server.sendContent(HTTP_STYLE);
  server.sendContent(HTTP_HEAD_END);

  int n = WiFi.scanNetworks();
  DEBUG_PRINT("scan done");
  if (n == 0) {
    DEBUG_PRINT("no networks found");
    server.sendContent("<div>No networks found. Refresh to scan again.</div>");
  }
  else {
    for (int i = 0; i < n; ++i)
    {
      DEBUG_PRINT(WiFi.SSID(i));
      DEBUG_PRINT(WiFi.RSSI(i));
      String item = HTTP_ITEM;
      item.replace("{v}", WiFi.SSID(i));
      server.sendContent(item);
      yield();
    }
  }

  server.sendContent(HTTP_FORM);
  server.sendContent(HTTP_END);
  server.client().stop();
  
  DEBUG_PRINT("Sent config page");  
}

/** Handle the WLAN save form and redirect to WLAN config page again */
void WiFiManager::handleWifiSave() {
  DEBUG_PRINT("wifi save");
  //server.arg("s").toCharArray(ssid, sizeof(ssid) - 1);
  //server.arg("p").toCharArray(password, sizeof(password) - 1);
  setSSID(urldecode(server.arg("s").c_str()));
  setPassword(urldecode(server.arg("p").c_str()));

  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.

 String head = HTTP_HEAD;
  head.replace("{v}", "Credentials Saved");
  server.sendContent(head);
  server.sendContent(HTTP_SCRIPT);
  server.sendContent(HTTP_STYLE);
  server.sendContent(HTTP_HEAD_END);
  
  server.sendContent(HTTP_SAVED);

  server.sendContent(HTTP_END);
  server.client().stop();
  
  DEBUG_PRINT("Sent wifi save page");  

  /*
  server.sendHeader("Location", "wifi", true);
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send ( 302, "text/plain", "");  // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.client().stop(); // Stop is needed because we sent no content length*/
  //saveCredentials();
  connect = true; //signal ready to connect/reset
}

void WiFiManager::handle204() {
  DEBUG_PRINT("204 No Response");  
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send ( 204, "text/plain", "");
}


void WiFiManager::handleNotFound() {
  if (captivePortal()) { // If captive portal redirect instead of displaying the error page.
    return;
  }
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send ( 404, "text/plain", message );
}


/** Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
boolean WiFiManager::captivePortal() {
  if (!isIp(server.hostHeader()) ) {
    Serial.print("Request redirected to captive portal");
    server.sendHeader("Location", String("http://") + toStringIp(server.client().localIP()), true);
    server.send ( 302, "text/plain", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server.client().stop(); // Stop is needed because we sent no content length
    return true;
  }
  return false;
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


