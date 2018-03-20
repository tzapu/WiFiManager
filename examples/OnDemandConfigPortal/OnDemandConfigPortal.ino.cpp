# 1 "c:\\users\\alverson\\appdata\\local\\temp\\tmplbmgdl"
#include <Arduino.h>
# 1 "C:/Users/alverson/documents/projects/arduino/sketches/libraries/WiFiManager/examples/OnDemandConfigPortal/OnDemandConfigPortal.ino"
#include <WiFiManager.h>




#define TRIGGER_PIN 0
const char* modes[] = { "NULL", "STA", "AP", "STA+AP" };

WiFiManager wifiManager;
void setup();
void loop();
#line 12 "C:/Users/alverson/documents/projects/arduino/sketches/libraries/WiFiManager/examples/OnDemandConfigPortal/OnDemandConfigPortal.ino"
void setup() {

  Serial.begin(115200);
  delay(3000);
  Serial.println("\n Starting");

  Serial.setDebugOutput(true);
  Serial.println(modes[WiFi.getMode()]);
  WiFi.printDiag(Serial);



  WiFiManager wifiManager;
# 33 "C:/Users/alverson/documents/projects/arduino/sketches/libraries/WiFiManager/examples/OnDemandConfigPortal/OnDemandConfigPortal.ino"
  wifiManager.setConfigPortalTimeout(180);
# 46 "C:/Users/alverson/documents/projects/arduino/sketches/libraries/WiFiManager/examples/OnDemandConfigPortal/OnDemandConfigPortal.ino"
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", "", 40);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", "", 6);
  WiFiManagerParameter custom_blynk_token("api", "api token", "", 0);


  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_blynk_token);
# 65 "C:/Users/alverson/documents/projects/arduino/sketches/libraries/WiFiManager/examples/OnDemandConfigPortal/OnDemandConfigPortal.ino"
  const char* menu[] = {"wifi","wifinoscan","info","param","close","sep","erase","restart","exit"};
  wifiManager.setMenu(menu,9);





  wifiManager.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0));







  if(!wifiManager.autoConnect("AutoConnectAP")) {
    Serial.println("failed to connect and hit timeout");
  }

  pinMode(TRIGGER_PIN, INPUT);
}


void loop() {

  if ( digitalRead(TRIGGER_PIN) == LOW ) {
# 100 "C:/Users/alverson/documents/projects/arduino/sketches/libraries/WiFiManager/examples/OnDemandConfigPortal/OnDemandConfigPortal.ino"
    wifiManager.setConfigPortalTimeout(120);
# 112 "C:/Users/alverson/documents/projects/arduino/sketches/libraries/WiFiManager/examples/OnDemandConfigPortal/OnDemandConfigPortal.ino"
    if (!wifiManager.startConfigPortal("OnDemandAP")) {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
    } else {

      Serial.println("connected...yeey :)");
    }
  }


}