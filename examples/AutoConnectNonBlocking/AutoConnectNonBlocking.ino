//needed for library
#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

WiFiManager wifiManager;

unsigned long lastTick = 0;

void setup() {
    // Init serial communication
    Serial.begin(9600);

    pinMode(BUILTIN_LED, OUTPUT);
    digitalWrite(BUILTIN_LED, 1);

    lastTick = millis();

    // WifiManager init
    // Setting the timeouts is not necessary, but encouraged
    wifiManager.setNonBlocking(true);
    wifiManager.setConnectTimeout(15);
    wifiManager.setConfigPortalTimeout(180);
    wifiManager.autoConnect();
}

void loop() {
    // This should be called regularily
    wifiManager.process();

    yield();

    // Blinking led to prove: the main loop does not get blocked
    if (millis() - lastTick < 200) return;
    lastTick = millis();
    digitalWrite(BUILTIN_LED, !digitalRead(BUILTIN_LED));

}
