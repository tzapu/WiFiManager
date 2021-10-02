/**
 * OnDemandWebPortal.ino
 * example of running the webportal (always NON blocking)
 */
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

// select which pin will trigger the configuration portal when set to LOW
#define TRIGGER_PIN 0

WiFiManager wm;

bool portalRunning = false;

void setup() {
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP 
  // put your setup code here, to run once
  Serial.begin(115200);
  Serial.println("\n Starting");
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
}

void loop() {
  checkButton();
  // put your main code here, to run repeatedly:
}

void checkButton(){
  // is auto timeout portal running
  if(portalRunning){
    wm.process();
  }

  // is configuration portal requested?
  if(digitalRead(TRIGGER_PIN) == LOW) {
    delay(50);
    if(digitalRead(TRIGGER_PIN) == LOW) {
      if(!portalRunning){
        Serial.println("Button Pressed, Starting Portal");
        wm.startWebPortal();
        portalRunning = true;
      }
      else{
        Serial.println("Button Pressed, Stopping Portal");
        wm.stopWebPortal();
        portalRunning = false;
      }
    }
  }
}


