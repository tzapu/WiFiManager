/**
 * MultiWiFiNonBlocking.ino
 * Multi network connect without blocking loop(), portal only on demand.
 * Build flags: -DWM_MULTIWIFI -DWM_MULTIWIFI_ROAM
 */
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

WiFiManager wm;
const int PORTAL_PIN = 0;
bool radioBusy = false; // eg. set while a BLE transfer is running

void setup() {
  Serial.begin(115200);
  pinMode(PORTAL_PIN, INPUT_PULLUP);
  WiFi.mode(WIFI_STA);
  wm.setConfigPortalBlocking(false);
  wm.setConnectTimeout(15);
#ifdef WM_MULTIWIFI
  wm.setMultiWiFiAllowCallback([] { return !radioBusy; }); // postpone scans / attempts
  #ifdef WM_MULTIWIFI_ROAM
  wm.setMultiWiFiRoaming(true, 20000);
  #endif
  if (wm.getWiFiIsSaved()) wm.beginMultiWiFi();
  else wm.startConfigPortal("MultiWiFi-Setup");
#endif
}

void loop() {
  wm.process();
  if (digitalRead(PORTAL_PIN) == LOW && !wm.getConfigPortalActive()) wm.startConfigPortal("MultiWiFi-Setup");

  static unsigned long last = 0;
  if (millis() - last > 5000) {
    last = millis();
#ifdef WM_MULTIWIFI
    if (wm.getMultiWiFiBusy()) Serial.printf("trying %s\n", wm.getMultiWiFiTarget().c_str());
    else
#endif
    Serial.printf("wifi %s %s\n", WiFi.status() == WL_CONNECTED ? "up" : "down", WiFi.SSID().c_str());
  }
}
