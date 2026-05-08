/**
 * WiFiManager – AP+STA Provisioning Flow Example
 * ================================================
 * Demonstrates the non-blocking AP+STA provisioning flow:
 *
 *   1. On first boot (or when stored credentials fail) the device starts a
 *      SoftAP so users can connect and reach the captive-portal setup page.
 *   2. The OS automatically opens the portal thanks to the built-in captive-
 *      portal probe handlers (Android /generate_204, iOS /hotspot-detect.html,
 *      Windows /ncsi.txt).
 *   3. When the user selects a network and submits a password the device starts
 *      a STA connection attempt WITHOUT tearing down the AP or rebooting.
 *   4. The browser polls /status (JSON) and displays real-time feedback
 *      (connecting → connected / failed).
 *   5. On success: credentials are saved, the IP address is shown, and the AP
 *      shuts down after a configurable delay (default 15 s).
 *   6. On failure: a descriptive error is shown and the user can retry – the AP
 *      stays up and no credentials are saved.
 *   7. On subsequent boots the stored credentials are tried first; if they fail
 *      the provisioning portal starts again automatically.
 *
 * LED feedback (NeoPixel RGB on pin 38)
 * ---------------------------------------
 *   Orange  – No WiFi configured          (stays on indefinitely)
 *   Blue    – Connecting (pulsing, 5 s)
 *   Green   – Connected                   (15 s then off)
 *   Red     – Connection failed           (stays on indefinitely)
 *
 * Custom animated SVGs on the status page
 * ----------------------------------------
 *   Connecting : spinning ring (blue)
 *   Success    : animated check mark (green)
 *   Failure    : pulsing X (red)
 *
 * Works on ESP32 and ESP8266.
 * Library: https://github.com/tzapu/WiFiManager
 */

#include <WiFiManager.h>
#include <Adafruit_NeoPixel.h>

// ---- NeoPixel configuration ----
#define LED_PIN    38
#define LED_COUNT  1
Adafruit_NeoPixel pixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// ---- user-configurable options ----
static const char AP_NAME[]  = "ESP-Setup";
static const char AP_PASS[]  = "";         // leave empty for open AP
static const unsigned long AP_SHUTDOWN_DELAY_MS = 15000; // 15 s
// -----------------------------------

WiFiManager wm;
bool provisioningDone = false;

// ----- animated SVG strings -----

// Connecting: spinning blue ring
static const char SVG_CONNECTING[] PROGMEM =
  "<svg xmlns='http://www.w3.org/2000/svg' width='64' height='64' viewBox='0 0 64 64'>"
    "<circle cx='32' cy='32' r='26' fill='none' stroke='#e0f0ff' stroke-width='6'/>"
    "<circle cx='32' cy='32' r='26' fill='none' stroke='#1fa3ec' stroke-width='6'"
      " stroke-dasharray='50 113' stroke-linecap='round'>"
      "<animateTransform attributeName='transform' type='rotate'"
        " from='0 32 32' to='360 32 32' dur='0.9s' repeatCount='indefinite'/>"
    "</circle>"
  "</svg>";

// Success: animated check-mark in a green circle
static const char SVG_SUCCESS[] PROGMEM =
  "<svg xmlns='http://www.w3.org/2000/svg' width='64' height='64' viewBox='0 0 64 64'>"
    "<circle cx='32' cy='32' r='28' fill='#5cb85c'/>"
    "<polyline points='18,33 28,43 46,22' fill='none' stroke='#fff' stroke-width='5'"
      " stroke-linecap='round' stroke-linejoin='round'"
      " stroke-dasharray='40' stroke-dashoffset='40'>"
      "<animate attributeName='stroke-dashoffset' from='40' to='0' dur='0.5s'"
        " fill='freeze' begin='0.1s'/>"
    "</polyline>"
  "</svg>";

// Failure: pulsing red X
static const char SVG_FAILURE[] PROGMEM =
  "<svg xmlns='http://www.w3.org/2000/svg' width='64' height='64' viewBox='0 0 64 64'>"
    "<circle cx='32' cy='32' r='28' fill='#dc3630'>"
      "<animate attributeName='opacity' values='1;0.5;1' dur='1.5s' repeatCount='indefinite'/>"
    "</circle>"
    "<line x1='20' y1='20' x2='44' y2='44' stroke='#fff' stroke-width='5' stroke-linecap='round'/>"
    "<line x1='44' y1='20' x2='20' y2='44' stroke='#fff' stroke-width='5' stroke-linecap='round'/>"
  "</svg>";

// ----- LED helpers -----

// Pulse blue: brightness oscillates between ~20% and 100% driven by millis()
void updateConnectingPulse() {
  float t = (millis() % 1000) / 1000.0f;
  uint8_t brightness = (uint8_t)(51 + 204 * (0.5f + 0.5f * sin(t * 2.0f * 3.14159f)));
  pixel.setPixelColor(0, pixel.Color(0, 0, brightness));
  pixel.show();
}

void ledSolid(uint8_t r, uint8_t g, uint8_t b) {
  pixel.setPixelColor(0, pixel.Color(r, g, b));
  pixel.show();
}

void ledOff() {
  pixel.clear();
  pixel.show();
}

// Track whether we're in connecting-pulse mode so loop() can keep updating it
bool ledPulsing = false;

// LED state callback – invoked by WiFiManager on every state change
void onLEDState(wm_ledstate_t state) {
  ledPulsing = false;
  switch (state) {
    case WM_LED_NOWIFI:
      ledSolid(255, 100, 0);   // Orange: no WiFi configured
      Serial.println(F("[LED] Orange – no WiFi configured"));
      break;
    case WM_LED_CONNECTING:
      ledPulsing = true;       // Blue pulsing: will be updated in loop()
      Serial.println(F("[LED] Blue pulsing – connecting"));
      break;
    case WM_LED_CONNECTED:
      ledSolid(0, 200, 0);     // Green: connected
      Serial.println(F("[LED] Green – connected"));
      break;
    case WM_LED_FAILED:
      ledSolid(220, 0, 0);     // Red: connection failed
      Serial.println(F("[LED] Red – connection failed"));
      break;
    case WM_LED_OFF:
      ledOff();                // Timeout elapsed: LED off
      Serial.println(F("[LED] Off"));
      break;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("\n\n=== WiFiManager AP+STA Provisioning Example ==="));

  // Init NeoPixel
  pixel.begin();
  pixel.setBrightness(80); // 0-255; 80 is ~31% – comfortable for indoor use
  ledOff();

  // Optional: uncomment to wipe saved credentials and force provisioning
  // wm.resetSettings();

  // ---- AP+STA provisioning options ----
  wm.setKeepAPDuringSTAConnect(true);          // Keep AP while STA connects
  wm.setAPShutdownDelay(AP_SHUTDOWN_DELAY_MS);  // Shut down AP 15 s after success
  wm.setDetailedFailureReasons(true);           // Show "Wrong password" etc.
  wm.setCaptivePortalCompatibility(true);       // Android/iOS/Windows auto-open (default)
  wm.setShowPassword(true);

  // ---- LED callback ----
  wm.setLEDCallback(onLEDState);
  wm.setLEDTimeoutConnected(15000);  // Green stays on for 15 s, then off
  wm.setLEDTimeoutConnecting(5000);  // Blue pulsing shown for max 5 s even if still connecting
  wm.setLEDTimeoutNoWifi(0);         // Orange stays on indefinitely
  wm.setLEDTimeoutFailed(0);         // Red stays on indefinitely

  // ---- Custom animated SVGs on the provisioning status page ----
  wm.setCustomConnectingSVG(SVG_CONNECTING);
  wm.setCustomSuccessSVG(SVG_SUCCESS);
  wm.setCustomFailureSVG(SVG_FAILURE);

  // ---- general portal options ----
  wm.setConfigPortalBlocking(false);  // Non-blocking so loop() keeps running
  wm.setConfigPortalTimeout(0);       // 0 = never time out while user is present

  // Callback: called when SoftAP is started
  wm.setAPCallback([](WiFiManager *w) {
    Serial.print(F("AP started: "));
    Serial.println(w->getConfigPortalSSID());
    Serial.print(F("AP IP: "));
    Serial.println(WiFi.softAPIP());
  });

  // Callback: called when credentials are saved and STA is confirmed connected
  wm.setSaveConfigCallback([]() {
    Serial.println(F("Credentials saved! STA connected."));
  });

  // Try stored credentials; on failure start provisioning portal
  if (wm.autoConnect(AP_NAME, AP_PASS[0] ? AP_PASS : nullptr)) {
    // Stored credentials worked – device is already connected
    Serial.print(F("Auto-connected! IP: "));
    Serial.println(WiFi.localIP());
    provisioningDone = true;
  } else {
    Serial.println(F("No stored credentials (or they failed). Provisioning portal running."));
    Serial.println(F("Connect to the AP and open the captive portal to configure WiFi."));
  }
}

void loop() {
  // Keep the blue pulse animation alive while connecting
  if (ledPulsing) {
    updateConnectingPulse();
  }

  // Drive the non-blocking portal (processes DNS, HTTP, and the provisioning
  // state machine).  Returns true once STA is connected AND the AP has been
  // shut down (i.e. provisioning is complete).
  if (!provisioningDone) {
    if (wm.process()) {
      provisioningDone = true;
      Serial.print(F("Provisioning complete! STA IP: "));
      Serial.println(WiFi.localIP());
    }

    // You can also inspect the state directly:
    wm_provstate_t state = wm.getProvisioningState();
    static wm_provstate_t lastState = WM_PROV_IDLE;
    if (state != lastState) {
      lastState = state;
      switch (state) {
        case WM_PROV_CONNECTING:
          Serial.println(F("[Provisioning] Connecting to STA..."));
          break;
        case WM_PROV_CONNECTED:
          Serial.print(F("[Provisioning] STA connected, IP: "));
          Serial.println(WiFi.localIP());
          Serial.println(F("AP will shut down shortly."));
          break;
        case WM_PROV_FAILED:
          Serial.println(F("[Provisioning] Connection failed. Portal still open."));
          break;
        default:
          break;
      }
    }
  }

  // ---- put your normal application code here ----
  // It runs concurrently while the provisioning portal is active.
}
