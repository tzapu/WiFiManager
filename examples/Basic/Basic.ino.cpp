# 1 "/var/folders/6g/ljwft31n7r5g593rqjywgvmh0000gn/T/tmpg1WvI3"
#include <Arduino.h>
# 1 "/Users/alverson/projects/microcontrollers/dev/libraries/WiFiManager/examples/Basic/Basic.ino"
#include <WiFiManager.h>
void setup();
void loop();
#line 3 "/Users/alverson/projects/microcontrollers/dev/libraries/WiFiManager/examples/Basic/Basic.ino"
void setup() {
    WiFi.mode(WIFI_STA);


    Serial.begin(115200);




    WiFiManager wm;
# 22 "/Users/alverson/projects/microcontrollers/dev/libraries/WiFiManager/examples/Basic/Basic.ino"
    bool res;


    res = wm.autoConnect("AutoConnectAP","password");

    if(!res) {
        Serial.println("Failed to connect");

    }
    else {

        Serial.println("connected...yeey :)");
    }

}

void loop() {


}