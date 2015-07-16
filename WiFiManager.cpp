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


WiFiManager::WiFiManager(int eepromStart)
{
    _eepromStart = eepromStart;
}

void WiFiManager::begin() {
    begin("NoNetESP");
}

void WiFiManager::begin(char const *apName) {
    _apName = apName;

    EEPROM.begin(512);
    delay(10);
}

boolean WiFiManager::autoConnect() {
    autoConnect("NoNetESP");
}

boolean WiFiManager::autoConnect(char const *apName) {
    begin(apName);
    
    
    DEBUG_PRINT("");
    DEBUG_PRINT("AutoConnect");
    // read eeprom for ssid and pass
    String ssid = getSSID();
    String pass = getPassword();
    
    if ( ssid.length() > 1 ) {
        DEBUG_PRINT("Waiting for Wifi to connect");

        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid.c_str(), pass.c_str());
        if ( hasConnected() ) {
            return true;
        }
    }
    //setup AP
    beginConfigMode();
    //start portal and loop
    startWebConfig();
    return false;
}

String WiFiManager::getSSID() {
    if(_ssid == "") {
        DEBUG_PRINT("Reading EEPROM SSID");
        _ssid = getEEPROMString(0, 32);
        DEBUG_PRINT("SSID: ");
        DEBUG_PRINT(_ssid);
    }
    return _ssid;
}

String WiFiManager::getPassword() {
    if(_pass == "") {
        DEBUG_PRINT("Reading EEPROM Password");
        _pass = getEEPROMString(32, 64);
        DEBUG_PRINT("Password: ");
        DEBUG_PRINT(_pass);
    }
    return _pass;
}

String WiFiManager::getEEPROMString(int start, int len) {
    String string = "";
    for (int i = _eepromStart + start; i < _eepromStart + start + len; i++) {
        //DEBUG_PRINT(i);
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
            DEBUG_PRINT("Wrote: ");
            DEBUG_PRINT(c);
            
        } else {
            c = 0;
        }
        EEPROM.write(i, c);
        si++;
    }
    
}


boolean WiFiManager::hasConnected(void) {
    int c = 0;
    while ( c < 50 ) {
        if (WiFi.status() == WL_CONNECTED) {
            return true;
        }
        delay(200);
        DEBUG_PRINT(".");
        c++;
    }
    DEBUG_PRINT("");
    DEBUG_PRINT("Could not connect to WiFi");
    return false;
}

void WiFiManager::startWebConfig() {
    DEBUG_PRINT("");
    DEBUG_PRINT("WiFi connected");
    DEBUG_PRINT(WiFi.localIP());
    DEBUG_PRINT(WiFi.softAPIP());
    if (!mdns.begin(_apName, WiFi.localIP())) {
        DEBUG_PRINT("Error setting up MDNS responder!");
        while(1) {
            delay(1000);
        }
    }
    DEBUG_PRINT("mDNS responder started");
    // Start the server
    server.begin();
    DEBUG_PRINT("Server started");

    while(serverLoop() == WM_WAIT) {
        //looping
    }

    DEBUG_PRINT("Setup done");
    delay(10000);
    ESP.reset();

}

String WiFiManager::beginConfigMode(void) {
    
    WiFi.softAP(_apName);
    DEBUG_PRINT("Started Soft Access Point");
    IPAddress apIp = WiFi.softAPIP();
    char ip[24];
    sprintf(ip, "%d.%d.%d.%d", apIp[0], apIp[1], apIp[2], apIp[3]);
    return ip;
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

    DEBUG_PRINT("New client");
    
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
        DEBUG_PRINT("Invalid request: ");
        DEBUG_PRINT(req);
        return(WM_WAIT);
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
        qssid = urldecode(req.substring(8,req.indexOf('&')).c_str());
        DEBUG_PRINT(qssid);
        DEBUG_PRINT("");
        String qpass;
        qpass = urldecode(req.substring(req.lastIndexOf('=')+1).c_str());
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

