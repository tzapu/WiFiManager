#include <WiFiManager.h> // https://github.com/mrcodetastic/WiFiManager_ESP32
WiFiManager wm;

void setup() {
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP    
    // put your setup code here, to run once:
    Serial.begin(115200);
    
    //reset settings - wipe credentials for testing
    //wm.resetSettings();

    wm.setConfigPortalBlocking(false);
    wm.setConfigPortalTimeout(60);

    // The AP is open by default. To require a password, call
    // wm.setAPPasswordRequired(true) and pass a password to autoConnect().
    // wm.setAPPasswordRequired(true);

    //automatically connect using saved credentials if they exist
    //If connection fails it starts an access point with the specified name
    if(wm.autoConnect("AutoConnectAP")){
        Serial.println("connected...yeey :)");
    }
    else {
        Serial.println("Configportal running");
    }
}

void loop() {
    wm.process();
    // put your main code here, to run repeatedly:
}
