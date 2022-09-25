// wifi_basic.ino

#include <Arduino.h>
#include <WiFi.h>

// #define NVSERASE
#ifdef NVSERASE
#include <nvs.h>
#include <nvs_flash.h>
#endif

void setup(){
    Serial.begin(115200);
    delay(2000);
    Serial.println("Startup....");

    #ifdef NVSERASE
        esp_err_t err;
        err = nvs_flash_init();
        err = nvs_flash_erase();
    #endif

    Serial.setDebugOutput(true);

    WiFi.begin("hellowifi","noonehere");
    
    while (WiFi.status() != WL_CONNECTED && millis()<15000) {
        delay(500);
        Serial.print(".");
    }
    
    if(WiFi.status() == WL_CONNECTED){
        Serial.println("");
        Serial.println("WiFi connected.");
        Serial.println("IP address: ");
        // Serial.println(WiFi.localIP());
    }
    else {
        Serial.println("WiFi NOT CONNECTED, starting ap");
        ///////////////
        /// BUG
        // WiFi.enableSTA(false); // BREAKS softap start, says ok BUT no ap found
        
        delay(2000);    
        WiFi.softAP("espsoftap","12345678");
    }
}

void loop(){

}