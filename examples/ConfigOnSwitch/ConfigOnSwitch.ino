#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>          //https://github.com/kentaylor/WiFiManager
// select wich pin will trigger the configuraton portal when set to LOW
// ESP-01 users please note: the only pins available (0 and 2), are shared 
// with the bootloader, so always set them HIGH at power-up
// Onboard LED I/O pin on NodeMCU board
const int PIN_LED = 16;      // Controls the onboard LED.
/*Trigger for inititating config mode Pin D3 and also flash button on NodeMCU
 * Flash button is convenient to use but if it is pressed it will stuff up the serial port device driver 
 * until the computer is rebooted on windows machines.
 */
const int TRIGGER_PIN = D3; 
/*
 * Alternative trigger pin. Needs to be connected to a button to use this pin. It must be a momentary connection
 * not connected permanently to ground. Either trigger pin will work.
 */
const int TRIGGER_PIN2 = D7;

void setup() {
  // put your setup code here, to run once:
  // initialize the LED digital pin as an output.
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(115200);
  Serial.println("\n Starting");
  WiFi.printDiag(Serial);
  Serial.print("Stored Password: ");
  Serial.println(WiFi.psk());
  Serial.print("Stored PSSID: ");
  Serial.println(WiFi.SSID());
  if (WiFi.SSID()==""){
    Serial.println("We never ever got access point credentials, so get them now");   
    digitalWrite(PIN_LED, LOW); // turn the LED on by making the voltage LOW to tell us we are in configuration mode.
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    if (!wifiManager.startConfigPortal()) {
      Serial.println("failed to connect and should not get here");
      delay(3000);
      //reset and try again, or maybe put it to deep sleep
      ESP.reset();
      delay(5000);
    }
  }
  else{
  digitalWrite(PIN_LED, HIGH); // Turn led off as we are not in configuration mode.
  Serial.print("local ip: ");
  Serial.println(WiFi.localIP());
  unsigned long startedAt = millis();
  while(millis() - startedAt < 10000)
        {
        delay(100);
        if (WiFi.status()==WL_CONNECTED) {
          float waited = (millis()- startedAt);
          Serial.print("After waiting ");
          Serial.print(waited/1000);
          Serial.print(" secs in setup() local ip: ");
          Serial.println(WiFi.localIP());
          break;
          }
       }
  }
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  pinMode(TRIGGER_PIN2, INPUT_PULLUP);
  /*Serial.print("Pin ");
  Serial.print(TRIGGER_PIN);
  Serial.print(": ");
  Serial.println(digitalRead(TRIGGER_PIN));*/
  if (WiFi.status()!=WL_CONNECTED){
    Serial.println("failed to connect, finnishing setup anyway");
  }
}


void loop() {
  // is configuration portal requested?
  if ( (digitalRead(TRIGGER_PIN) == LOW) || (digitalRead(TRIGGER_PIN2) == LOW)) {
     Serial.println("Configuration portal requested.");
     digitalWrite(PIN_LED, LOW); // turn the LED on by making the voltage LOW to tell us we are in configuration mode.
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;

    //reset settings - for testing
    //wifiManager.resetSettings();

    //sets timeout until configuration portal gets turned off
    //useful to make it all retry or go to sleep
    //in seconds
    wifiManager.setConfigPortalTimeout(120);

    //it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration

    //WITHOUT THIS THE AP DOES NOT SEEM TO WORK PROPERLY WITH SDK 1.5 , update to at least 1.5.1
    //WiFi.mode(WIFI_STA);
    
    if (!wifiManager.startConfigPortal()) {
      Serial.println("Hit ConfigPortal timeout. Continue with existing credentials");
    } else {
      //if you get here you have connected to the WiFi
      Serial.println("connected...yeey :)");
    }
    digitalWrite(PIN_LED, HIGH); // Turn led off as we are not in configuration mode.
  }


  // put your main code here, to run repeatedly:

}
