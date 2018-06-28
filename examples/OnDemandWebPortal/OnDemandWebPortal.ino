/**
 * OnDemandWebPortal.ino
 * example of running the webportal manually, independantly from the captiveportal
 * trigger pin will start a webportal for 120 seconds then turn it off.
 * 
 */
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

// select which pin will trigger the configuration portal when set to LOW
#define TRIGGER_PIN 0

WiFiManager wm;

int timeout           = 120; // seconds to run for
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
    wm.process();
    if(millis()-start > timeout*1000){
      Serial.println("webportaltimeout");
      webportalrunning = false;
      wm.stopWebPortal(); // auto off
   }
  }
  // is configuration portal requested?
  if ( digitalRead(TRIGGER_PIN) == LOW && !webportalrunning) {
    wm.startWebPortal();
    // wm.startWebPortal();
    webportalrunning = true;
    start = millis();
  }
  // put your main code here, to run repeatedly:
}
