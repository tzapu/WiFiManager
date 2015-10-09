# WiFiManager
ESP8266 WiFi Connection manager with save to EEPROM and web config portal

Captive portal should also work, I have tested it only on iOS.

First attempt at a library. Lots more changes and fixes to do. Adding examples also needs doing.

This works with the ESP8266 Arduino platform https://github.com/esp8266/Arduino

> v0.1 works with the staging release ver. 1.6.5-1044-g170995a, built on Aug 10, 2015 of the ESP8266 Arduino library.
> latest comit should work with the latest staging version

####Quick Start
- Checkout library to your Arduino libraries folder

- Include in your sketch

```Arduino
#include <EEPROM.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>
```

- Initialise library, in your setup function add
```
//parameter is eeprom start
WiFiManager wifi;
```

- Also in the setup function add
```
//parameter is name of access point
wifi.autoConnect("AP-NAME");
```

After you write your sketch and start the ESP, it will try to connect to WiFi. If it fails it starts in Access Point mode.
While in AP mode, connect to it then open a browser to the gateway IP, default 192.168.4.1, configure wifi, save and it should reboot and connect.

Also see examples.

###Timeout
If you need to set a timeout so the ESP doesn't hang waiting to be configured, for instance after a power failure, you can add 
```
wifi.setTimeout(180);
```
which will wait 3 minutes (180 seconds). When the time passes, the autoConnect function will return, no matter the outcome.
Check for connection and if it's still not established do whatever is needed (on some modules I restart them to retry, on others I enter deep sleep)

###Debug
Debug is enabled by default on Serial. To disable add before autoConnect
```
wifi.setDebugOutput(false);
```

####Inspiration
- http://www.esp8266.com/viewtopic.php?f=29&t=2520

To be continued...
