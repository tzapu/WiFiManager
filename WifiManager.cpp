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

#include "WiFiManager.h"

MDNSResponder mdns;
WiFiServer server(80);


WiFiManager::WiFiManager(char *apName)
{
    _apName = apName;
}

void WiFiManager::begin() {
    begin(0);
}
void WiFiManager::begin(int eepromStart) {
    _eepromStart = eepromStart;
    EEPROM.begin(512);
}

boolean WiFiManager::autoConnect() {
    autoConnect(0);
}

boolean WiFiManager::autoConnect(int eepromStart) {
    begin(eepromStart);
    
    delay(10);
    
    Serial.println();
    Serial.println("AutoConnect");
    // read eeprom for ssid and pass
    String ssid = getSSID();
    String pass = getPassword();
    
    if ( ssid.length() > 1 ) {
        Serial.println("Waiting for Wifi to connect");

        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid.c_str(), pass.c_str());
        if ( hasConnected() ) {
            return true;
        }
    }
    //oled_log("\nno network");
    beginConfigMode();
    return false;
}

String WiFiManager::getSSID() {
    if(_ssid == "") {
        Serial.println("Reading EEPROM SSID");
        _ssid = getEEPROMString(0, 32);
        Serial.print("SSID: ");
        Serial.println(_ssid);
    }
    return _ssid;
}

String WiFiManager::getPassword() {
    if(_pass == "") {
        Serial.println("Reading EEPROM Password");
        _pass = getEEPROMString(32, 64);
        Serial.print("Password: ");
        Serial.println(_pass);
    }
    return _pass;
}

String WiFiManager::getEEPROMString(int start, int len) {
    String string = "";
    for (int i = _eepromStart + start; i < _eepromStart + start + len; i++) {
        //Serial.println(i);
        string += char(EEPROM.read(i));
    }
    return string;
}

void WiFiManager::setEEPROMString(int start, int len, String string) {
    int si = 0;
    for (int i = _eepromStart + start; i < _eepromStart + start + len; i++) {
        char c;
        if(si < string.length()) {
            c = string[si];
            Serial.print("Wrote: ");
            Serial.println(c);
            
        } else {
            c = 0;
        }
        EEPROM.write(i, c);
        si++;
    }
    
}


boolean WiFiManager::hasConnected(void) {
    int c = 0;
    while ( c < 20 ) {
        if (WiFi.status() == WL_CONNECTED) {
            return true;
        }
        delay(500);
        Serial.print(".");
        c++;
    }
    Serial.println("");
    Serial.println("Could not connect to WiFi");
    return false;
}

void WiFiManager::startWebConfig() {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.softAPIP());
    if (!mdns.begin(_apName, WiFi.localIP())) {
        Serial.println("Error setting up MDNS responder!");
        while(1) {
            delay(1000);
        }
    }
    Serial.println("mDNS responder started");
    // Start the server
    server.begin();
    Serial.println("Server started");

    while(serverLoop() == WM_WAIT) {
        //looping
    }
}

void WiFiManager::beginConfigMode(void) {
    
    WiFi.softAP(_apName);
    Serial.println("Started Soft Access Point");
    
    startWebConfig();
    Serial.println("Setup done");
    delay(10000);
    ESP.reset();
}

int WiFiManager::serverLoop()
{
    // Check for any mDNS queries and send responses
    mdns.update();
    
    // Check if a client has connected
    WiFiClient client = server.available();
    if (!client) {
        return(WM_WAIT);
    }

    Serial.println("New client");
    
    // Wait for data from client to become available
    while(client.connected() && !client.available()){
        delay(1);
    }
    
    // Read the first line of HTTP request
    String req = client.readStringUntil('\r');
    
    // First line of HTTP request looks like "GET /path HTTP/1.1"
    // Retrieve the "/path" part by finding the spaces
    int addr_start = req.indexOf(' ');
    int addr_end = req.indexOf(' ', addr_start + 1);
    if (addr_start == -1 || addr_end == -1) {
        Serial.print("Invalid request: ");
        Serial.println(req);
        return(WM_WAIT);
    }
    req = req.substring(addr_start + 1, addr_end);
    Serial.print("Request: ");
    Serial.println(req);
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
        Serial.println("scan done");
        if (n == 0) {
            Serial.println("no networks found");
            s += "<div>No networks found. Refresh to scan again.</div>";
        }
        else {
            for (int i = 0; i < n; ++i)
            {
                Serial.print(WiFi.SSID(i));
                Serial.print(" ");
                Serial.println(WiFi.RSSI(i));
                String item = HTTP_ITEM;
                item.replace("{v}", WiFi.SSID(i));
                s += item;
                delay(10);
            }
        }
        
        s += HTTP_FORM;
        s += HTTP_END;
        
        Serial.println("Sending config page");
    }
    else if ( req.startsWith("/s") ) {
        String qssid;
        qssid = urldecode(req.substring(8,req.indexOf('&')).c_str());
        Serial.println(qssid);
        Serial.println("");
        String qpass;
        qpass = urldecode(req.substring(req.lastIndexOf('=')+1).c_str());
        Serial.println(qpass);
        Serial.println("");
        
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
        
        Serial.println("Saved WiFiConfig...restarting.");
        return WM_DONE;
    }
    else
    {
        s = HTTP_404;
        Serial.println("Sending 404");
    }
    
    client.print(s);
    Serial.println("Done with client");
    return(WM_WAIT);
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
                a -= 'a'-'A';
            if (a >= 'A')
                a -= ('A' - 10);
            else
                a -= '0';
            if (b >= 'a')
                b -= 'a'-'A';
            if (b >= 'A')
                b -= ('A' - 10);
            else
                b -= '0';
            
            decoded += char(16*a+b);
            src+=3;
        } 
        else {
            decoded += *src;
            *src++;
        }
    }
    decoded += '\0';
    Serial.print("source ");
    Serial.println(decoded);
    
    return decoded;
}

