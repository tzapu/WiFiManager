# Connect To WiFi
Connect an ESP8266 to WiFi using a web configuration portal served from the ESP8266 operating as an access point.

[![Build Status](https://travis-ci.org/kentaylor/WiFiManager.svg?branch=master)](https://travis-ci.org/kentaylor/WiFiManager)

The configuration portal is captive, so it will present the configuration dialogue with a http connection to any web address as soon as you connect to the created access point. Captive portals do not work with https web addresses.

This is the first attempt at extensively modifying an existing library which appears to be based on an incorrect premise. This causes the base library to include unneeded functionality which causes the WiFiManager to fail intermittently and sometimes brick the ESP8266 module. The ESP8266 module can be recovered by reflashing with different software. The library also includes other changes. For example it previously spoofed that it provided a path to the internet in access point mode which was removed because it caused problems with other applications running on the browsing device and the ESP8266 can not reliably serve web pages in combined access point/station mode when not connected to a network so changes to station mode only when this occurs. Changes are detailed in the commit messages.

An ESP8266 stores access point credentials in non volatile memory. Therefore these details should only be set once, unless they need to be changed. The ESP8266 is set by default to autoconnect and will always do it's best to connect to a network when it has an access point name (SSID) configured. If the access point is visible and the password is correct it will connect in a few seconds. If the access point is not visible or goes away it will reestablish the connection automatically when the access point becomes visible. There are no calls to the Espressif ESP8266 library that can connect to a network faster or more reliably and trying to use calls to the Espressif library to connect to a network can sometimes brick the ESP8266. Specifically, calling WiFi.begin(SSID, Password) with a different SSID or password to those already in non volatile memory or calling WiFi.begin() should not be done while it is trying to connect to a network. It will usually be OK but sometimes it will fail, depending, I suspect, on what stage it is up to in the connection process.

The inbuilt autoconnect functionality makes the autoconnect examples in the library from which this was forked superfluous so those that don't demonstrate some additional functionality have been deleted.
The [ConfigOnSwitch] (https://github.com/kentaylor/WiFiManager/tree/master/examples/ConfigOnswitch) example is the only one tested after the changes have been made. 

#### This works with the ESP8266 Arduino platform with a recent stable release(2.0.0 or newer) https://github.com/esp8266/Arduino

## Contents
 - [How it works](#how-it-works)
 - [Wishlist](#wishlist)
 - [Evidence The Premise Is Wrong And Of The Problems It Causes](#Evidence-The-Premise-Is Wrong-And-Of-The-Problems-It=Causes)
 - [Quick start](#quick-start)
   - Installing(#Installing)
     - [From Github](#checkout-from-github)
   - [Using](#using)
 - [Documentation](#documentation)
   - [Access Point Password](#password-protect-the-configuration-access-point)
   - [Callbacks](#callbacks)
   - [Configuration Portal Timeout](#configuration-portal-timeout)
   - [On Demand Configuration](#on-demand-configuration-portal)
   - [Custom Parameters](#custom-parameters)
   - [Custom IP Configuration](#custom-ip-configuration)
   - [Custom HTML, CSS, Javascript](#Custom-HTML,-CSS,-Javascript)
   - [Filter Low Quality Networks](#filter-networks)
   - [Debug Output](#debug)
 - [Troubleshooting](#troubleshooting)
 - [Releases](#releases)
 - [Contributors](#contributions-and-thanks)


## How It Works

- The [ConfigOnSwitch] (https://github.com/kentaylor/WiFiManager/tree/master/examples/ConfigOnswitch) example shows how it works and should be used as the basis for a sketch that uses this library.
- The concept of ConfigOnSwitch is that a new ESP8266 will start a WiFi configuration portal when powered up and save the configuration data in non volatile memory. Thereafter, the configuration portal will only be started again if a button is pushed on the ESP8266 module.
- The configuration web portal is established by putting the ESP8266 into joint Access Point/Station mode and starting up a DNS and WebServer (default ip 192.168.4.1)
- Using any WiFi enabled device with a browser (computer, phone, tablet) connect to the newly created Access Point
- Because of the Captive Portal and the DNS server you will either get a 'Join to network' type of popup or get any domain you try to access redirected to the configuration portal.
- Choose one of the access points scanned, enter password, click save.
- ESP will try to connect. If successful, the IP address on the new network will be displayed in the configuration portal. 
- The configuration portal will now be visible on two networks, these being it's own network and the network to which it has connected.  On it's own network it will have two IP addresses, the original 192.168.4.1 and the same IP address it has on the network to which it connected.
- Selecting "close configuration portal" will shutdown the web server, shutdown the ESP8266 WiFi network and return control to the following sketch code.

## Wishlist
- Testing by others to confirm or repudiate the claim that the premise underlying the forked library is incorrect.
- Add changes from other forks of original library.
- More usability testing. Initial testing showed that vanishing WiFi networks are confusing to users as their browsing device will continue to show networks that are no longer there and reconnect to other networks in the background. This creates confusion and causes web pages users are expecting to see to fail with confusing browser messages. The workflow has been modified to improve these issues but probably needs further improvement.
- Add javascript to undertake scan in the background and update WiFi page when complete.
- Add multiple sets of network credentials.
- Provide an app to set up WiFi credentials using the json interfaces provided as usability testing has shown that people find switching back and forward between WiFi networks to be confusing. An app can swap back and forward between networks in the background.
- Incorporate these changes into WiFiManager library from which this version was forked.

## Evidence The Premise Is Wrong And Of The Problems It Causes
It seems bold to claim the premise underlying the library from which this version was forked is wrong when it was developed over some time and is already widely used. So I thought I should provide some evidence. 

I started out investigating why my ESP8266's were occasionally bricking which I'd suspected was due to faulty modules and ended up forming a view the problem was software related. As the problems are intermittent and not reliably repeatable and after spending much more time on this than intended I'm still not sure that my claim of what causes the ESP8266 to brick is correct or that I have identified the complete solution. The solution here has worked for me so far and I'm hopeful others may identify any further errors or provide new insight.

When the ESP8266 has an SSID in non volatile memory and it is set for autoconnect there is no need to do anything in a sketch to establish WiFi connectivity. This can be seen in the [ConfigOnSwitch](https://github.com/kentaylor/WiFiManager/tree/master/examples/ConfigOnswitch) example which connects to a network effectively whenever network credentials are available in non volatile memory. Therefore the claim that calling WiFi.begin is unnecessary is easy to sustain.

Evidence that calling WiFi.begin can be harmful is more circumstantial. Looking through the issues reported and code revisions in WiFi manager I can see discussion of the same problems I was having and efforts to fix them. Specifically:-
- There has been an attempt to fix a problem identified as an ["auto connect racing issue"] ( https://github.com/tzapu/WiFiManager/commit/b99487785d2c8319df1223b204e1bba2b321f3f3) by not calling WiFi.begin under some circumstances. This fix has a side effect identified in [issue 141](https://github.com/tzapu/WiFiManager/issues/141) that "WiFiManager will not connect to a newly selected access point if a connection to another access point already exists."
- Issue [149] (https://github.com/tzapu/WiFiManager/issues/149) Problem is a side effect of trying to manage WiFi connectivity in code.
- Issue [147](https://github.com/tzapu/WiFiManager/issues/147) I think this issue is occuring because WiFi.begin was called when it wasn't required. There is a probability it will fail every time it is called but when deep sleep is used with frequent wakeups the cumulative probability of failure becomes high. Tzapu said he had seen the same problem once.
- Issue [143](https://github.com/tzapu/WiFiManager/issues/143) Looks like it may be same issue as issue 149 but its not clear from the information provided.
- Issue [102](https://github.com/tzapu/WiFiManager/issues/102) Arises when WiFi.begin called when already connected.
- Issue[71](https://github.com/tzapu/WiFiManager/issues/71) Assumes that WiFi.begin is required when it isn't.


## Quick Start

### Installing
See ["Installing Arduino libraries"](https://www.arduino.cc/en/Guide/Libraries) and use the Importing a .zip Library method or preferably use manual installation as you can checkout the release from github and use that. This makes it easier to keep current with updates. Installing using the  Library Manager does not work with this version of the library.

####  Checkout from github
__Github version works with release 2.0.0 or newer of the [ESP8266 core for Arduino](https://github.com/esp8266/Arduino)__
- Checkout library to your Arduino libraries folder

### Using
- Include in your sketch
```cpp
#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)

#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/kentaylor/WiFiManager WiFi Configuration Magic
```

- When you want open a config portal initialize library, add
```cpp
WiFiManager wifiManager;
Configuration Magic
```
then call

```cpp
wifiManager.startConfigPortal()
Configuration Magic
```
While in AP mode, connect to it then open a browser to the gateway IP, default 192.168.4.1, configure wifi, save and it should save WiFi connection information in non volatile memory, reboot and connect.


Once WiFi network information is saved in the ESP8266, it will try to connect to WiFi every time it is started without requiring any function calls in the sketch.


Also see [ConfigOnSwitch example](https://github.com/kentaylor/WiFiManager/tree/master/examples/ConfigOnswitch).

## Documentation

#### Password protect the configuration Access Point
You can password protect the configuration access point.  Simply add an SSID as the first parameter and the password as a second parameter to `startConfigPortal`.
A short password seems to have unpredictable results so use one that's around 8 characters or more in length.
The guidelines are that a wifi password must consist of 8 to 63 ASCII-encoded characters in the range of 32 to 126 (decimal)
```cpp
wifiManager.startConfigPortal()
Configuration Magic
```

#### Callbacks

##### Save settings
This gets called when custom parameters have been set **AND** a connection has been established. Use it to set a flag, so when all the configuration finishes, you can save the extra parameters somewhere.

See [AutoConnectWithFSParameters Example](https://github.com/kentaylor/WiFiManager/tree/master/examples/AutoConnectWithFSParameters).
```cpp
wifiManager.setSaveConfigCallback(saveConfigCallback);
Configuration Magic
```
saveConfigCallback declaration and example
```cpp
//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}
Configuration Magic
```

#### Configuration Portal Timeout
If you need to set a timeout so the ESP8266 doesn't hang waiting to be configured for ever. 
```cpp
wifiManager.setConfigPortalTimeout(600);
```
which will wait 10 minutes (600 seconds). When the time passes, the startConfigPortal function will return, no matter the outcome and continue the sketch.


#### On Demand Configuration Portal

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
See example for a more complex version. [OnDemandConfigPortal](https://github.com/kentaylor/WiFiManager/tree/master/examples/ConfigOnswitch)

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
wifiManager.setSTAStaticIPConfig(IPAddress(192,168,0,99), IPAddress(192,168,0,1), IPAddress(255,255,255,0));
```
There are a couple of examples in the examples folder that show you how to set a static IP and even how to configure it through the web configuration portal.

#### Custom HTML, CSS, Javascript
There are various ways in which you can inject custom HTML, CSS or Javascript into the configuration portal.
The options are:
- inject custom head element
You can use this to any html bit to the head of the configuration portal. If you add a `<style>` element, bare in mind it overwrites the included css, not replaces.
```cpp
wifiManager.setCustomHeadElement("<style>html{filter: invert(100%); -webkit-filter: invert(100%);}</style>");
```
- inject a custom bit of html in the configuration form
```cpp
WiFiManagerParameter custom_text("<p>This is just a text paragraph</p>");
wifiManager.addParameter(&custom_text);
```
- inject a custom bit of html in a configuration form element
Just add the bit you want added as the last parameter to the custom parameter constructor.
```cpp
WiFiManagerParameter custom_mqtt_server("server", "mqtt server", "iot.eclipse", 40, " readonly");
```

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
Debug is enabled by default on Serial. To disable add before autoConnect
```cpp
wifiManager.setDebugOutput(false);
```

## Troubleshooting
If you get compilation errors, more often than not, you may need to install a newer version of the ESP8266 core for Arduino.

Tested down to ESP8266 core 2.0.0. 

Sometimes, the library will only work if you update the ESP8266 core to the latest version because I am using some newly added function.

If you connect to the created configuration Access Point but the configuration portal does not show up, just open a browser and type in the IP of the web portal, by default `192.168.4.1`.


## Releases
#### 0.11
- forked from this version of the tzapu's WiFi Manager.

See [tzapu's version](https://github.com/tzapu/WiFiManager) for previous release information.


### Contributions and thanks
Forked from [tzapu](https://github.com/tzapu/WiFiManager)


#### Inspiration
I expected to knock the WiFi connection code over in an afternoon after being inspired by
http://www.esp8266.com/viewtopic.php?f=29&t=2520 . It was taking longer. Then I came across tzapu's implementation which failed for me and this is my attempt to address the issues I discovered. Some weeks have gone by.
