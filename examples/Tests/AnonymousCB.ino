#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

bool _enteredConfigMode = false;

void setup(){
	Serial.begin(115200);
	WiFiManager wifiManager;

	// wifiManager.setAPCallback([this](WiFiManager* wifiManager) {
	wifiManager.setAPCallback([&](WiFiManager* wifiManager) {
		Serial.printf("Entered config mode:ip=%s, ssid='%s'\n", 
                        WiFi.softAPIP().toString().c_str(), 
                        wifiManager->getConfigPortalSSID().c_str());
		_enteredConfigMode = true;
	});
	wifiManager.resetSettings();
	if (!wifiManager.autoConnect()) {
		Serial.printf("*** Failed to connect and hit timeout\n");
		ESP.restart();
		delay(1000);
	}
}

void loop(){

}
