/**
 * SecurePersistence.ino
 *
 * Demonstrates the WiFiManager 3.x feature set:
 * - Preferences-backed credential and custom-parameter persistence
 * - Multiple saved networks with automatic retry
 * - HTTP basic auth for the portal and OTA
 * - AP password enforcement
 * - Optional OTA SHA-256 integrity verification
 * - Structured state and result reporting
 *
 * The custom parameter below is persisted automatically. You do not need
 * to save it in a callback; WiFiManager restores it when addParameter()
 * is called on the next boot.
 */

#include <WiFiManager.h>

WiFiManager wm;
WiFiManagerParameter device_name("device", "Device name", "esp32-node", 32);

// Replace with the SHA-256 of your firmware image to enable OTA integrity
// verification. Leave empty to disable the check.
//
// Example:
//   sha256sum firmware.bin
//   const char *OTA_SHA256 = "9f86d081884c7d659a2feaa0c55ad015...";
const char *OTA_SHA256 = "";

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\nWiFiManager secure persistence example");

  // Require a password for the configuration AP.
  wm.setAPPasswordRequired(true);

  // Protect the portal and OTA with HTTP basic auth.
  // OTA is disabled until setAuth() is called.
  wm.setAuth("admin", "change-me");

  // Optional OTA integrity verification.
  if (strlen(OTA_SHA256) > 0) {
    wm.setOTASHA256(OTA_SHA256);
  }

  // Custom parameters are persisted automatically.
  wm.addParameter(&device_name);

  // Keep loop() responsive while the portal is running.
  wm.setConfigPortalBlocking(false);
  wm.setConfigPortalTimeout(180);

  if (wm.autoConnect("WiFiManager-Secure", "configure-me")) {
    Serial.println("Connected");
  } else {
    Serial.println("Config portal running");
  }
}

void loop() {
  wm.process();

  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 5000) {
    lastPrint = millis();
    Serial.print("state=");
    Serial.print((int)wm.getState());
    Serial.print(" result=");
    Serial.print((int)wm.getLastResult());
    Serial.print(" device=");
    Serial.println(device_name.getValue());
  }
}
