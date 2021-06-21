#include <WiFiManager.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

WiFiClient espClient;
PubSubClient client(espClient);
char mqtt_server[40];
void mqttCallback(char* topic, byte* payload, unsigned int length) {
       // message received
}

void mqttReconnect() {
       // reconnect code from PubSubClient example
}

void setup() {
  Serial.begin(9600);
     WiFiManager wifiManager;
     wifiManager.setTimeout(180);

     if(!wifiManager.autoConnect("AutoConnectAP")) {
       Serial.println("failed to connect and hit timeout");
       delay(3000);
       ESP.restart();
       delay(5000);
     } 
     // id/name, placeholder/prompt, default, length
WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
wifiManager.addParameter(&custom_mqtt_server);

const char* mqtt_server = custom_mqtt_server.getValue();

     Serial.println("connected...yeey :)");

     client.setServer(mqtt_server, 1883);
     client.setCallback(mqttCallback);
}

void loop() {
     if (!client.connected()) {
       mqttReconnect();
     }
     client.loop();
     yield();
}