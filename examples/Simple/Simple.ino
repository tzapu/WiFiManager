#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);

    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wm;
    
    //reset settings - wipe credentials for testing
    //wm.resetSettings();
    
    //set custom ip for portal
    //wm.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

    //automatically connect using saved credentials if they exist
    //If connection fails it starts an access point with the specified name
    //here  "AutoConnectAP" if empty will auto generate basedcon chipid, if password is blank it will be anonymous
    //and goes into a blocking loop awaiting configuration
    wm.autoConnect("AutoConnectAP");
    //or use this for auto generated name ESP + ChipID
    //wm.autoConnect();

    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
}

void loop() {
    // put your main code here, to run repeatedly:
    
}
