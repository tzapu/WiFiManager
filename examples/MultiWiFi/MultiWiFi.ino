/**
 * MultiWiFi.ino
 * Remember several networks, connect to the best one available.
 *
 * Needs build flag -DWM_MULTIWIFI (Arduino IDE: uncomment the define in WiFiManager.h,
 * a #define in the sketch is NOT seen by the library). Optional:
 *   -DWM_MULTIWIFI_ROAM          re-select when the connection is lost
 *   -DWM_MULTIWIFI_ROAM_STRONGER switch to a stronger saved AP while connected
 *   -DWM_MULTIWIFI_MAX=8         number of saved networks (default 5)
 *
 * Every network saved in the portal is added to the list. Saved networks are shown on the
 * wifi page and can be forgotten there, tick "Hidden network" for networks that do not broadcast.
 */
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

WiFiManager wm;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

#ifdef WM_MULTIWIFI
  // networks can also be preloaded from code
  // wm.addWiFiCredential("office", "secret");
  // wm.addWiFiCredential("lab-hidden", "secret", true);

  wm.setMultiWiFiResultCallback([](const char* ssid, bool ok) {
    Serial.printf("[app] %s: %s\n", ssid, ok ? "connected" : "failed");
  });
  #ifdef WM_MULTIWIFI_ROAM
  wm.setMultiWiFiRoaming(true);
  #endif
  #ifdef WM_MULTIWIFI_ROAM_STRONGER
  wm.setMultiWiFiRoamStronger(true, 60000);
  #endif
#else
  #warning "build with -DWM_MULTIWIFI"
#endif

  wm.setConnectTimeout(20);
  // blocking: tries the last good network, then scans and tries saved networks by signal,
  // hidden ones last, then opens the portal
  if (!wm.autoConnect("MultiWiFi-Setup")) {
    Serial.println("not connected");
  }

#ifdef WM_MULTIWIFI
  WiFiManagerCredential c;
  for (uint8_t i = 0; wm.getWiFiCredential(i, c); i++) {
    Serial.printf("saved %u: %s%s%s\n", i, c.ssid, c.hidden() ? " (hidden)" : "", c.unverified() ? " (?)" : "");
  }
#endif
}

void loop() {
  wm.process(); // runs roaming
}
