# 1 "c:\\users\\alverson\\appdata\\local\\temp\\tmpqedruz"
#include <Arduino.h>
# 1 "C:/Users/alverson/documents/projects/arduino/sketches/libraries/WiFiManager/examples/OnDemandConfigPortal/OnDemandConfigPortal.ino"
#include <WiFiManager.h>
# 11 "C:/Users/alverson/documents/projects/arduino/sketches/libraries/WiFiManager/examples/OnDemandConfigPortal/OnDemandConfigPortal.ino"
#define TRIGGER_PIN 0

const char* modes[] = { "NULL", "STA", "AP", "STA+AP" };
void setup();
void loop();
#line 21 "C:/Users/alverson/documents/projects/arduino/sketches/libraries/WiFiManager/examples/OnDemandConfigPortal/OnDemandConfigPortal.ino"
void setup() {



  Serial.begin(115200);

  WiFi.printDiag(Serial);

  delay(3000);

  Serial.println("\n Starting");



  Serial.setDebugOutput(true);

  Serial.println(modes[WiFi.getMode()]);
# 49 "C:/Users/alverson/documents/projects/arduino/sketches/libraries/WiFiManager/examples/OnDemandConfigPortal/OnDemandConfigPortal.ino"
  WiFiManager wifiManager(Serial);
# 63 "C:/Users/alverson/documents/projects/arduino/sketches/libraries/WiFiManager/examples/OnDemandConfigPortal/OnDemandConfigPortal.ino"
  wifiManager.setConfigPortalTimeout(180);
# 79 "C:/Users/alverson/documents/projects/arduino/sketches/libraries/WiFiManager/examples/OnDemandConfigPortal/OnDemandConfigPortal.ino"
  if(!wifiManager.autoConnect("AutoConnectAP")) {

    Serial.println("failed to connect and hit timeout");

  }



  pinMode(TRIGGER_PIN, INPUT);

}





void loop() {



  if ( digitalRead(TRIGGER_PIN) == LOW ) {





    WiFiManager wifiManager(Serial);
# 121 "C:/Users/alverson/documents/projects/arduino/sketches/libraries/WiFiManager/examples/OnDemandConfigPortal/OnDemandConfigPortal.ino"
    wifiManager.setConfigPortalTimeout(120);
# 145 "C:/Users/alverson/documents/projects/arduino/sketches/libraries/WiFiManager/examples/OnDemandConfigPortal/OnDemandConfigPortal.ino"
    if (!wifiManager.startConfigPortal("OnDemandAP")) {

      Serial.println("failed to connect and hit timeout");

      delay(3000);

    } else {



      Serial.println("connected...yeey :)");

    }

  }





}