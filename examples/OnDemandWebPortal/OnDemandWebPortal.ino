/**
 * OnDemandWebPortal.ino
 * example of running the webportal manually, independantly from the captiveportal
 * trigger pin will start a webportal for 120 seconds then turn it off.
 * 
 */
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

// select which pin will trigger the configuration portal when set to LOW
// ESP-01 users please note: the only pins available (0 and 2), are shared 
// with the bootloader, so always set them HIGH at power-up
#define TRIGGER_PIN 0

int timeout          = 120; // seconds

WiFiManager wifiManager;

bool webportalrunning = false;
int start             = millis();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("\n Starting");
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
}


void loop() {
  
  if(webportalrunning){
    wifiManager.process();
    if(millis()-start > timeout*1000){
      Serial.println("webportaltimeout");
      webportalrunning = false;
      wifiManager.stopWebPortal(); // auto off
   }
  }

  // is configuration portal requested?
  if ( digitalRead(TRIGGER_PIN) == LOW && !webportalrunning) {
    wifiManager.startWebPortal();
    webportalrunning = true;
    start = millis();
  }
  // put your main code here, to run repeatedly:
}
