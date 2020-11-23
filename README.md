
# WiFiManager
## DEVELOPMENT BRANCH

ESP8266 WiFi Connection manager with fallback web configuration portal

:warning: This Documentation is out of date for this branch, see notes below

[![Build Status](https://travis-ci.org/tzapu/WiFiManager.svg?branch=development)](https://travis-ci.org/tzapu/WiFiManager)

![ESP8266](https://img.shields.io/badge/ESP-8266-000000.svg?longCache=true&style=flat&colorA=CC101F)
![ESP32](https://img.shields.io/badge/ESP-32-000000.svg?longCache=true&style=flat&colorA=CC101F)

The configuration portal is of the captive variety, so on various devices it will present the configuration dialogue as soon as you connect to the created access point.

First attempt at a library. Lots more changes and fixes to do. Contributions are welcome.

**This works with the ESP8266 Arduino platform**

[https://github.com/esp8266/Arduino](https://github.com/esp8266/Arduino)

**This works with the ESP32 Arduino platform** 

[https://github.com/espressif/arduino-esp32](https://github.com/espressif/arduino-esp32)

### Known Issues
* Documentation needs to be updated, see [https://github.com/tzapu/WiFiManager/issues/500](https://github.com/tzapu/WiFiManager/issues/500)

## Contents
 - [How it works](#how-it-works)
 - [Wishlist](#wishlist)
 - [Quick start](#quick-start)
   - Installing
     - [Arduino - Through Library Manager](#install-through-library-manager)
     - [Arduino - From Github](#checkout-from-github)
     - [PlatformIO](#install-using-platformio)
   - [Using](#using)
 - [Documentation](#documentation)
   - [Access Point Password](#password-protect-the-configuration-access-point)
   - [Callbacks](#callbacks)
   - [Configuration Portal Timeout](#configuration-portal-timeout)
   - [On Demand Configuration](#on-demand-configuration-portal)
   - [Custom Parameters](#custom-parameters)
   - [Custom IP Configuration](#custom-ip-configuration)
   - [Filter Low Quality Networks](#filter-networks)
   - [Debug Output](#debug)
 - [Troubleshooting](#troubleshooting)
 - [Releases](#releases)
 - [Contributors](#contributions-and-thanks)


## How It Works
- When your ESP starts up, it sets it up in Station mode and tries to connect to a previously saved Access Point
- if this is unsuccessful (or no previous network saved) it moves the ESP into Access Point mode and spins up a DNS and WebServer (default ip 192.168.4.1)
- using any wifi enabled device with a browser (computer, phone, tablet) connect to the newly created Access Point
- because of the Captive Portal and the DNS server you will either get a 'Join to network' type of popup or get any domain you try to access redirected to the configuration portal
- choose one of the access points scanned, enter password, click save
- ESP will try to connect. If successful, it relinquishes control back to your app. If not, reconnect to AP and reconfigure.
- There are options to change this behavior or manually start the configportal and webportal independantly as well as run them in non blocking mode.

## How It Looks
![ESP8266 WiFi Captive Portal Homepage](http://i.imgur.com/YPvW9eql.png) ![ESP8266 WiFi Captive Portal Configuration](http://i.imgur.com/oicWJ4gl.png)

## Wishlist
- [x] remove dependency on EEPROM library
- [x] move HTML Strings to PROGMEM
- [x] cleanup and streamline code (although this is ongoing)
- [x] if timeout is set, extend it when a page is fetched in AP mode
- [x] add ability to configure more parameters than ssid/password
- [x] maybe allow setting ip of ESP after reboot
- [x] add to Arduino Library Manager
- [x] add to PlatformIO
- [ ] add multiple sets of network credentials
- [x] allow users to customize CSS
- [ ] rewrite documentation for simplicity, based on scenarios/goals

### Development
- [x] ESP32 support
- [x] rely on the SDK's built in auto connect more than forcing a connect
- [x] add non blocking mode
- [x] easy customization of strings
- [x] hostname support
- [x] fix various bugs and workarounds for esp SDK issues
- [x] additional info page items
- [x] last status display / faiilure reason
- [x] customizeable menu
- [x] seperate custom params page
- [x] ondemand webportal
- [x] complete refactor of code to segment functions
- [x] wiif scan icons or percentage display
- [x] invert class for dark mode
- [x] more template tokens
- [x] progmem for all strings
- [ ] new callbacks
- [ ] new callouts / filters
- [ ] shared web server instance
- [x] latest esp idf/sdk support
- [x] wm is now non persistent, will not erase or change stored esp config on esp8266
- [x] tons of debugging output / levels
- [ ] disable captiveportal
- [ ] preload wiifscans, faster page loads
- [ ] softap stability fixes when sta is not connected


## Quick Start

### Installing
You can either install through the Arduino Library Manager or checkout the latest changes or a release from github

#### Install through Library Manager
__Currently version 0.8+ works with release 2.4.0 or newer of the [ESP8266 core for Arduino](https://github.com/esp8266/Arduino)__
 - in Arduino IDE got to Sketch/Include Library/Manage Libraries
  ![Manage Libraries](http://i.imgur.com/9BkEBkR.png)

 - search for WiFiManager
  ![WiFiManager package](http://i.imgur.com/18yIai8.png)

 - click Install and start [using it](#using)

####  Checkout from github
__Github version works with release 2.4.0 or newer of the [ESP8266 core for Arduino](https://github.com/esp8266/Arduino)__
- Checkout library to your Arduino libraries folder

### Using
- Include in your sketch
```cpp
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
```

- Initialize library, in your setup function add, NOTEif you are using non blocking you will make sure you create this in global scope or handle appropriatly , it will not work if in setup and using non blocking mode.
```cpp
WiFiManager wifiManager;
```

- Also in the setup function add
```cpp
//first parameter is name of access point, second is the password
wifiManager.autoConnect("AP-NAME", "AP-PASSWORD");
```
if you just want an unsecured access point
```cpp
wifiManager.autoConnect("AP-NAME");
```
or if you want to use and auto generated name from 'ESP' and the esp's Chip ID use
```cpp
wifiManager.autoConnect();
```

After you write your sketch and start the ESP, it will try to connect to WiFi. If it fails it starts in Access Point mode.
While in AP mode, connect to it then open a browser to the gateway IP, default 192.168.4.1, configure wifi, save and it should reboot and connect.

Also see [examples](https://github.com/tzapu/WiFiManager/tree/master/examples).

#### Install Using PlatformIO

[PlatformIO](https://platformio.org/) is an emerging ecosystem for IoT development, and 
is an alternative to using the Arduino IDE. Install `WiFiManager`
using the platformio [library manager](https://docs.platformio.org/en/latest/librarymanager/index.htm) in your editor, 
or using the [PlatformIO Core CLI](https://docs.platformio.org/en/latest/userguide/demo.html#library-manager),
or by adding it to your `platformio.ini` as shown below (recommended approach).

The simplest way is to open the `platformio.ini` file at the root of your project, and `WifiManager` to the common top-level env
`lib_deps` key like so:

```
[env]
lib_deps =
	WiFiManager
```

If you want to install the development branch, then you'll need to use the `repository#tag` format instead:

```
[env]
lib_deps =
	https://github.com/tzapu/WiFiManager.git#development
```

## Documentation

#### Password protect the configuration Access Point
You can and should password protect the configuration access point.  Simply add the password as a second parameter to `autoConnect`.
A short password seems to have unpredictable results so use one that's around 8 characters or more in length.
The guidelines are that a wifi password must consist of 8 to 63 ASCII-encoded characters in the range of 32 to 126 (decimal)
```cpp
wifiManager.autoConnect("AutoConnectAP", "password")
```

#### Callbacks
##### Enter Config mode
Use this if you need to do something when your device enters configuration mode on failed WiFi connection attempt.
Before `autoConnect()`
```cpp
wifiManager.setAPCallback(configModeCallback);
```
`configModeCallback` declaration and example
```cpp
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());

  Serial.println(myWiFiManager->getConfigPortalSSID());
}
```

##### Save settings
This gets called when custom parameters have been set **AND** a connection has been established. Use it to set a flag, so when all the configuration finishes, you can save the extra parameters somewhere.

See [AutoConnectWithFSParameters Example](https://github.com/tzapu/WiFiManager/tree/master/examples/AutoConnectWithFSParameters).
```cpp
wifiManager.setSaveConfigCallback(saveConfigCallback);
```
`saveConfigCallback` declaration and example
```cpp
//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}
```

#### Configuration Portal Timeout
If you need to set a timeout so the ESP doesn't hang waiting to be configured, for instance after a power failure, you can add
```cpp
wifiManager.setConfigPortalTimeout(180);
```
which will wait 3 minutes (180 seconds). When the time passes, the autoConnect function will return, no matter the outcome.
Check for connection and if it's still not established do whatever is needed (on some modules I restart them to retry, on others I enter deep sleep)

#### On Demand Configuration Portal
If you would rather start the configuration portal on demand rather than automatically on a failed connection attempt, then this is for you.

Instead of calling `autoConnect()` which does all the connecting and failover configuration portal setup for you, you need to use `startConfigPortal()`. __Do not use BOTH.__

Example usage
```cpp
void loop() {
  // is configuration portal requested?
  if ( digitalRead(TRIGGER_PIN) == LOW ) {
    WiFiManager wifiManager;
    wifiManager.startConfigPortal("OnDemandAP");
    Serial.println("connected...yeey :)");
  }
}
```
See example for a more complex version. [OnDemandConfigPortal](https://github.com/tzapu/WiFiManager/tree/master/examples/OnDemandConfigPortal)

#### Exiting from the Configuration Portal
Normally, once entered, the configuration portal will continue to loop until WiFi credentials have been successfully entered or a timeout is reached.
If you'd prefer to exit without joining a WiFi network, say becuase you're going to put the ESP into AP mode, then press the "Exit" button
on the main webpage.
If started via `autoConnect` or `startConfigPortal` then it will return `false (portalAbortResult)`

#### Custom Parameters
You can use WiFiManager to collect more parameters than just SSID and password.
This could be helpful for configuring stuff like MQTT host and port, [blynk](http://www.blynk.cc) or [emoncms](http://emoncms.org) tokens, just to name a few.
**You are responsible for saving and loading these custom values.** The library just collects and displays the data for you as a convenience.
Usage scenario would be:
- load values from somewhere (EEPROM/FS) or generate some defaults
- add the custom parameters to WiFiManager using
```cpp
 // id/name, placeholder/prompt, default, length
 WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
 wifiManager.addParameter(&custom_mqtt_server);

```
- if connection to AP fails, configuration portal starts and you can set /change the values (or use on demand configuration portal)
- once configuration is done and connection is established [save config callback]() is called
- once WiFiManager returns control to your application, read and save the new values using the `WiFiManagerParameter` object.
```cpp
 mqtt_server = custom_mqtt_server.getValue();
```  
This feature is a lot more involved than all the others, so here are some examples to fully show how it is done.
You should also take a look at adding custom HTML to your form.

- Save and load custom parameters to file system in json form [AutoConnectWithFSParameters](https://github.com/tzapu/WiFiManager/tree/master/examples/AutoConnectWithFSParameters)
- *Save and load custom parameters to EEPROM* (not done yet)

#### Custom IP Configuration
You can set a custom IP for both AP (access point, config mode) and STA (station mode, client mode, normal project state)

##### Custom Access Point IP Configuration
This will set your captive portal to a specific IP should you need/want such a feature. Add the following snippet before `autoConnect()`
```cpp
//set custom ip for portal
wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
```

##### Custom Station (client) Static IP Configuration
This will make use the specified IP configuration instead of using DHCP in station mode.
```cpp
wifiManager.setSTAStaticIPConfig(IPAddress(192,168,0,99), IPAddress(192,168,0,1), IPAddress(255,255,255,0)); // optional DNS 4th argument
```
There are a couple of examples in the examples folder that show you how to set a static IP and even how to configure it through the web configuration portal.

NOTE: You should fill DNS server if you have HTTP requests with hostnames or syncronize time (NTP). It's the same as gateway ip or a popular (Google DNS: 8.8.8.8).

#### Custom HTML, CSS, Javascript
There are various ways in which you can inject custom HTML, CSS or Javascript into the configuration portal.
The options are:
- inject custom head element
You can use this to any html bit to the head of the configuration portal. If you add a `<style>` element, bare in mind it overwrites the included css, not replaces.
```cpp
wifiManager.setCustomHeadElement("<style>html{filter: invert(100%); -webkit-filter: invert(100%);}</style>");
```
- inject a custom bit of html in the configuration/param form
```cpp
WiFiManagerParameter custom_text("<p>This is just a text paragraph</p>");
wifiManager.addParameter(&custom_text);
```
- inject a custom bit of html in a configuration form element
Just add the bit you want added as the last parameter to the custom parameter constructor.
```cpp
WiFiManagerParameter custom_mqtt_server("server", "mqtt server", "iot.eclipse", 40, " readonly");
wifiManager.addParameter(&custom_mqtt_server);
```

#### Theming
You can customize certain elements of the default template with some builtin classes
```CPP
wifiManager.setClass("invert"); // dark theme
wifiManager.setScanDispPerc(true); // display percentages instead of graphs for RSSI
```
There are additional classes in the css you can use in your custom html , see the example template.

#### Filter Networks
You can filter networks based on signal quality and show/hide duplicate networks.

- If you would like to filter low signal quality networks you can tell WiFiManager to not show networks below an arbitrary quality %;
```cpp
wifiManager.setMinimumSignalQuality(10);
```
will not show networks under 10% signal quality. If you omit the parameter it defaults to 8%;

- You can also remove or show duplicate networks (default is remove).
Use this function to show (or hide) all networks.
```cpp
wifiManager.setRemoveDuplicateAPs(false);
```

#### Debug
Debug is enabled by default on `Serial` in non-stable releases. To disable add before autoConnect/startConfigPortal
```cpp
wifiManager.setDebugOutput(false);
```

You can pass in a custom stream via constructor 
```CPP
WiFiManager wifiManager(Serial1);
```

You can customize the debug level by changing `_debugLevel` in source
options are:
* DEBUG_ERROR
* DEBUG_NOTIFY
* DEBUG_VERBOSE
* DEBUG_DEV
* DEBUG_MAX

## Troubleshooting
If you get compilation errors, more often than not, you may need to install a newer version of the ESP8266 core for Arduino.

Changes added on 0.8 should make the latest trunk work without compilation errors. Tested down to ESP8266 core 2.0.0. **Please update to version 0.8**

I am trying to keep releases working with release versions of the core, so they can be installed through boards manager, but if you checkout the latest version directly from github, sometimes, the library will only work if you update the ESP8266 core to the latest version because I am using some newly added function.

If you connect to the created configuration Access Point but the configuration portal does not show up, just open a browser and type in the IP of the web portal, by default `192.168.4.1`.

If trying to connect ends up in an endless loop, try to add `setConnectTimeout(60)` before `autoConnect();`. The parameter is timeout to try connecting in seconds.

I get stuck in ap mode when the power goes out or modem resets, try a setConfigPortalTimeout(seconds). This will cause the configportal to close after no activity, and you can reboot or attempt reconnection in your code.

## Releases
### 1.0.1

### Development Overview

#### Added Public Methods
`setConfigPortalBlocking`

`setShowStaticFields`

`setCaptivePortalEnable`

`setRestorePersistent`

`setCaptivePortalClientCheck`

`setWebPortalClientCheck`

`startWebPortal`

`stopWebPortal`

`process`

`disconnect`

`erase`

` debugSoftAPConfig`

` debugPlatformInfo`

`setScanDispPerc`

`setHostname`

`setMenu(menu_page_t[])`

`setWiFiAutoReconnect`

` setSTAStaticIPConfig(..,dns)`

`setShowDnsFields`

`getLastConxResult`

`getWLStatusString`

`getModeString`

`getWiFiIsSaved`

`setShowInfoErase`

`setEnableConfigPortal`

`setCountry`

`setClass`

`htmleEtities`


#### WiFiManagerParameter
`WiFiManagerParameter(id,label)`

`WiFiManagerParameter.setValue(value,length)`

`getParameters`

`getParametersCount`


#### Constructors
`WiFiManager(Stream& consolePort)`

#### define flags
❗️  **Defines cannot be set in user sketches**
`#define WM_MDNS       // use MDNS`

`#define WM_FIXERASECONFIG // use erase flash fix, esp8266 2.4.0`

`#define WM_ERASE_NVS // esp32 erase(true) will erase NVS`

`#include <rom/rtc.h> // esp32 info page will show last reset reasons if this file is included`

#### Changes Overview
- ESP32 support ( fairly stable )
- complete refactor of strings `strings_en.h`
- adds new tokens for wifiscan, and some classes (left , invert icons, MSG color)
- adds status callout panel default, primary, special colors
-  adds tons of info on info page, and erase capability
- adds signal icons, replaces percentage ( has hover titles )
- adds labels to all inputs (replaces placeholders)
- all html ( and eventually all strings except debug) moved to `strings_en.h`
- added additional debugging, compressed debug lines, debuglevels
- persistent disabled, and restored via de/con-stuctor (uses `setRestorePersistent`)
- should retain all user modes including AP, should not overwrite or persist user modes or configs,even STA (`storeSTAmode`) (BUGGY)
- ⚠️ return values may have changed depending on portal abort, or timeout ( `portalTimeoutResult`,`portalAbortResult`)
- params memory is auto allocated by increment of `WIFI_MANAGER_MAX_PARAMS(5)` when exceeded, user no longer needs to specify this at all.
- addparameter now returns bool, and it returns false if param ID is not alphanum [0-9,A-Z,a-z,_]
- param field ids allow {I} token to use param_n instead of string in case someones wants to change this due to i18n or character issues
- provides `#DEFINE FIXERASECONFIG` to help deal with https://github.com/esp8266/Arduino/pull/3635
- failure reason reporting on portal
- set esp8266 sta hostname, esp32 sta+ap hostname ( DHCP client id)
- pass in debug stream in constructor WiFiManager(Stream& consolePort)
- you can force ip fields off with showxfields(false) if you set _disableIpFields=true
- param menu/page (setup) added to separate params from wifi page, handled automatically by setMenu
- set custom root menu
- disable configportal on autoconnect
- wm parameters init is now protected, allowing child classes, example included
- wifiscans are precached and async for faster page loads, refresh forces rescan
- adds esp32 gettemperature ( currently commented out, useful for relative measurement only )

#### 0.12
- removed 204 header response
- fixed incompatibility with other libs using isnan and other std:: functions without namespace

##### 0.11
- a lot more reliable reconnecting to networks
- custom html in custom parameters (for read only params)
- custom html in custom parameter form (like labels)
- custom head element (like custom css)
- sort networks based on signal quality
- remove duplicate networks

##### 0.10
- some css changes
- bug fixes and speed improvements
- added an alternative to waitForConnectResult() for debugging
- changed `setTimeout(seconds)` to `setConfigPortalTimeout(seconds)`

### Contributions and thanks
The support and help I got from the community has been nothing short of phenomenal. I can't thank you guys enough. This is my first real attept in developing open source stuff and I must say, now I understand why people are so dedicated to it, it is because of all the wonderful people involved.

__THANK YOU__

The esp8266 and esp32 arduino and idf maintainers!

[Shawn A aka tablatronix](https://github.com/tablatronix)

[liebman](https://github.com/liebman)

[Evgeny Dontsov](https://github.com/dontsovcmc)

[Chris Marrin](https://github.com/cmarrin)

[bbx10](https://github.com/bbx10)

[kentaylor](https://github.com/kentaylor)

[Maximiliano Duarte](https://github.com/domonetic)

[alltheblinkythings](https://github.com/alltheblinkythings)

[Niklas Wall](https://github.com/niklaswall)

[Jakub Piasecki](https://github.com/zaporylie)

[Peter Allan](https://github.com/alwynallan)

[John Little](https://github.com/j0hnlittle)

[markaswift](https://github.com/markaswift)

[franklinvv](https://github.com/franklinvv)

[Alberto Ricci Bitti](https://github.com/riccibitti)

[SebiPanther](https://github.com/SebiPanther)

[jonathanendersby](https://github.com/jonathanendersby)

[walthercarsten](https://github.com/walthercarsten)

And countless others

#### Inspiration
 * http://www.esp8266.com/viewtopic.php?f=29&t=2520
 * https://github.com/chriscook8/esp-arduino-apboot
 * https://github.com/esp8266/Arduino/tree/master/libraries/DNSServer/examples/CaptivePortalAdvanced
 * Built by AlexT https://github.com/tzapu

