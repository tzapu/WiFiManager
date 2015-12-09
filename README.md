# WiFiManager
ESP8266 WiFi Connection manager with fallback web configuration portal

The configuration portal is of the captive variety, so on various devices it will present the configuration dialogue as soon as you connect to the created access point.

First attempt at a library. Lots more changes and fixes to do. Contributions are welcome.

## How It works
- when your ESP starts up, it sets it up in Station mode and tries to connect to a previously saved Access Point
- if this is unsuccessful (or no previous network saved) it moves the ESP into Access Point mode and spins up a DNS and WebServer (default ip 192.168.4.1)
- using any wifi enabled device with a browser (computer, phone, tablet) connect to the newly created Access Point
- because of the Captive Portal and the DNS server you will either get a 'Join to network' type of popup or get any domain you try to access redirected to the configuration portal
- choose one of the access points scanned, enter password, click save
- ESP will try to connect. If successful, it relinquishes control back to your app. If not, reconnect to AP and reconfigure.

## Still left to do
- ~~remove dependency on EEPROM library~~
- move HTML Strings to PROGMEM
- cleanup and streamline code
- if timeout is set, extend it when a page is fetched in AP mode
- add ability to configure more parameters than ssid/password
- maybe allow setting ip of ESP after reboot

This works with the ESP8266 Arduino platform https://github.com/esp8266/Arduino

## Releases
> v0.3 removed the need for EEPROM and works with the 2.0.0 and above stable release of the ESP8266 for Arduino IDE package

> **v0.2 and newer need the latest staging version (or at least a recent release of the staging version) to work**

> v0.1 works with the staging release ver. 1.6.5-1044-g170995a, built on Aug 10, 2015 of the ESP8266 Arduino library.


## Quick Start
- Make sure you have a recent package of the ESP8266 for Arduino IDE

- Checkout library to your Arduino libraries folder

- Include in your sketch
```cpp
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
```

- Initialise library, in your setup function add
```cpp
WiFiManager wifiManager;
```

- Also in the setup function add
```cpp
//parameter is name of access point
wifiManager.autoConnect("AP-NAME");
```
or if you want to use and auto generated name from 'ESP' and the esp's Chip ID use
```cpp
wifiManager.autoConnect();
```

After you write your sketch and start the ESP, it will try to connect to WiFi. If it fails it starts in Access Point mode.
While in AP mode, connect to it then open a browser to the gateway IP, default 192.168.4.1, configure wifi, save and it should reboot and connect.

Also see examples.

### Timeout
If you need to set a timeout so the ESP doesn't hang waiting to be configured, for instance after a power failure, you can add
```cpp
wifiManager.setTimeout(180);
```
which will wait 3 minutes (180 seconds). When the time passes, the autoConnect function will return, no matter the outcome.
Check for connection and if it's still not established do whatever is needed (on some modules I restart them to retry, on others I enter deep sleep)

### Debug
Debug is enabled by default on Serial. To disable add before autoConnect
```cpp
wifiManager.setDebugOutput(false);
```

### Custom IP Configuration
This will set your captive portal to a specific IP should you need/want such a feature. Add the following snippet before `autoConnect()`
```cpp
//set custom ip for portal
wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
```

#### Inspiration
- http://www.esp8266.com/viewtopic.php?f=29&t=2520

To be continued...
