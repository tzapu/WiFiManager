# WiFiManager
ESP8266 WiFi Connection manager with save to EEPROM and web config portal

First attempt at a library. Lots more changes and fixes to do. Adding examples also needs doing.

This works with the ESP8266 Arduino platform https://github.com/esp8266/Arduino

####Quick Start
- Checkout library to your Arduino libraries folder

- Include in your sketch

```Arduino
#include <WiFiClient.h>
#include <EEPROM.h>
#include <ESP8266mDNS.h>
#include <WifiManager.h>
```

- Initialise library, the paramater is the Acces Point Name
```
WiFiManager wifi("ESP-AP");
```

- In your setup function add
```
wifi.autoConnect();
```

After you write your sketch and start the ESP, it will try to connect to WiFi. If it fails it starts in Access Point mode.
While in AP mode, connect to it then open a browser to the gateway IP, default 192.168.4.1, configure wifi, save and it should reboot and connect.


####Inspiration
- http://www.esp8266.com/viewtopic.php?f=29&t=2520

To be continued...
