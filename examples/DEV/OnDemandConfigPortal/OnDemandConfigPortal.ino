/**
 * This is a kind of unit test for DEV for now
 */
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

#define TRIGGER_PIN 0
const char* modes[] = { "NULL", "STA", "AP", "STA+AP" };

WiFiManager wm;

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

char ssid[] = "*************";  //  your network SSID (name)
char pass[] = "********";       // your network password

unsigned int localPort = 2390;      // local port to listen for UDP packets

/* Don't hardwire the IP address or we won't get the benefits of the pool.
 *  Lookup the IP address for the host name instead */
//IPAddress timeServer(129, 6, 15, 28); // time.nist.gov NTP server
IPAddress timeServerIP; // time.nist.gov NTP server address
const char* ntpServerName = "time.nist.gov";
// const char* ntpServerName = "ns1.mtsu.edu";
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setDebugOutput(true);  
  delay(3000);
  Serial.println("\n Starting");

  Serial.println("Starting UDP");
  udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());

  // WiFi.mode(WIFI_STA);
  WiFi.printDiag(Serial);
  // Serial.println(modes[WiFi.getMode()]);
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  // WiFiManager wm;

  wm.debugPlatformInfo();

  //Local intialization. Once its business is done, there is no need to keep it around
  //reset settings - for testing
  // wm.resetSettings();
  // 
  wm.setClass("invert");

  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  // wm.setConfigPortalTimeout(600);
  wm.setConnectTimeout(10);
  // wm.setShowStaticFields(true);

  // uint8_t menu[] = {wm.MENU_WIFI,wm.MENU_INFO,wm.MENU_PARAM,wm.MENU_CLOSE};
  // wm.setMenu(menu);

  // std::vector<WiFiManager::menu_page_t> menu = {wm.MENU_WIFI,wm.MENU_INFO,wm.MENU_PARAM,wm.MENU_CLOSE,wm.MENU_SEP,wm.MENU_ERASE,wm.MENU_EXIT};
  // wm.setMenu(menu);

// std::vector<WiFiManager::menu_page_t> menu = {wm.MENU_WIFI,wm.MENU_INFO,wm.MENU_PARAM,wm.MENU_CLOSE,wm.MENU_SEP,wm.MENU_ERASE,wm.MENU_EXIT};
  // wm.setMenu(menu);


  WiFiManagerParameter custom_html("<p>This Is Custom HTML</p>");
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", "", 40);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", "", 6);
  WiFiManagerParameter custom_token("api_token", "api token", "", 16);
  WiFiManagerParameter custom_tokenb("invalid token", "invalid token", "", 0);

  //add all your parameters here
  wm.addParameter(&custom_html);
  wm.addParameter(&custom_mqtt_server);
  wm.addParameter(&custom_mqtt_port);
  wm.addParameter(&custom_token);
  wm.addParameter(&custom_tokenb);

  custom_html.setValue("test",4);
  custom_token.setValue("test",4);

        // MENU_WIFI       = 0,
        // MENU_WIFINOSCAN = 1,
        // MENU_INFO       = 2,
        // MENU_PARAM      = 3,
        // MENU_CLOSE      = 4,
        // MENU_RESTART    = 5,
        // MENU_EXIT       = 6,
        // MENU_ERASE      = 7,
        // MENU_SEP        = 8

  // const char* menu[] = {"wifi","wifinoscan","info","param","close","sep","erase","restart","exit"};
  // wm.setMenu(menu,9);

  std::vector<const char *> menu = {"wifi","wifinoscan","info","param","close","sep","erase","restart","exit"};
  wm.setMenu(menu);
  
  //set static ip
  // wm.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
  // wm.setShowStaticFields(false);
  // wm.setShowDnsFields(false);

  // WiFi.mode(WIFI_STA);
  // const wifi_country_t COUNTRY_US{"US",1,11,WIFI_COUNTRY_POLICY_AUTO};
  // const wifi_country_t COUNTRY_CN{"CN",1,13,WIFI_COUNTRY_POLICY_AUTO};
  // const wifi_country_t COUNTRY_JP{"JP",1,14,WIFI_COUNTRY_POLICY_AUTO};
  // esp_wifi_set_country(&COUNTRY_US);

  // wm.setCountry("US");
  
  wm.setConfigPortalTimeout(120);
  // wm.startConfigPortal("AutoConnectAP", "password");

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  
  if(!wm.autoConnect("AutoConnectAP")) {
    Serial.println("failed to connect and hit timeout");
  }
  else {
    // start configportal always
    // wm.setConfigPortalTimeout(60);
    // wm.startConfigPortal();
  }

  pinMode(TRIGGER_PIN, INPUT);
}


void loop() {
  // is configuration portal requested?
  if ( digitalRead(TRIGGER_PIN) == LOW ) {
    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    //reset settings - for testing
    //wm.resetSettings();

    //sets timeout until configuration portal gets turned off
    //useful to make it all retry or go to sleep
    //in seconds
    wm.setConfigPortalTimeout(120);

    //it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration

    //WITHOUT THIS THE AP DOES NOT SEEM TO WORK PROPERLY WITH SDK 1.5 , update to at least 1.5.1
    //WiFi.mode(WIFI_STA);
    
    // disable captive portal redirection
    // wm.setCaptivePortalEnable(false);
    
    if (!wm.startConfigPortal("OnDemandAP")) {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
    } else {
      //if you get here you have connected to the WiFi
      Serial.println("connected...yeey :)");
    }  
  }

  // put your main code here, to run repeatedly:
  getTime();
  delay(10000);
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress& address)
{
  Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

void getTime(){
    //get a random server from the pool
  WiFi.hostByName(ntpServerName, timeServerIP); 

  sendNTPpacket(timeServerIP); // send an NTP packet to a time server
  // wait to see if a reply is available
  delay(1000);
  
  int cb = udp.parsePacket();
  if (!cb) {
    Serial.println("no packet yet");
  }
  else {
    Serial.print("packet received, length=");
    Serial.println(cb);
    // We've received a packet, read the data from it
    udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    Serial.print("Seconds since Jan 1 1900 = " );
    Serial.println(secsSince1900);

    // now convert NTP time into everyday time:
    Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    // print Unix time:
    Serial.println(epoch);


    // print the hour, minute and second:
    Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
    Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
    Serial.print(':');
    if ( ((epoch % 3600) / 60) < 10 ) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
    Serial.print(':');
    if ( (epoch % 60) < 10 ) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.println(epoch % 60); // print the second
  }
}