# Connect To WiFi
Connect an ESP8266 to WiFi using a web configuration portal served from the ESP8266 operating as an access point.

[![Build Status](https://travis-ci.org/kentaylor/WiFiManager.svg?branch=master)](https://travis-ci.org/kentaylor/WiFiManager)

The configuration portal is captive, so it will present the configuration dialogue regardless of the web address selected, excluding https requests.

This is an extensive modification of an existing library. The major changes are:-

- Portal [continues to exist](#portal-continues-to-exist-after-configuration) after configuring the ESP device to provide user feedback on success or otherwise.
- Provides [guidance](#user-guidance-at-the-same-address-regardless-of-network) to users when they are not connected to the correct network.
- Supports [configuration from apps](#programmatic-configuration) including the [ESP Connect](https://play.google.com/store/apps/details?id=au.com.umranium.espconnect) Android app. 
- Relies on [automatic connectivity.](#relies-on-automatic-connectivity)
- Configuration portal initiation [not automatic](#configuration-portal-initiation-not-automatic) when a configured WiFi network is not visible.
- Configuration portal initiated by [double pressing](#configuration-portal-initiated-by-double-pressing-the-reset-button) the reset button. 
- Selectively operates in [dual mode](#selectively-operates-in-dual-mode)

This works with the ESP8266 Arduino platform with a recent stable release(2.0.0 or newer) https://github.com/esp8266/Arduino

## Contents
 - [Features](#features)
   - [Portal Continues To Exist After Configuration](#portal-continues-to-exist-after-configuration)
   - [User Guidance At The Same Address Regardless of Network](#user-guidance-at-the-same-address-regardless-of-network)
   - [Programmatic Configuration](#programmatic-configuration)
   - [Relies On Automatic Connectivity](#relies-on-automatic-connectivity)
   - [Configuration Portal Initiation Not Automatic](#configuration-portal-initiation-not-automatic)
   - [Configuration portal initiated by double pressing the reset button ](#configuration-portal-initiated-by-double-pressing-the-reset-button) 
   - [Selectively Operates In Dual Mode](#selectively-operates-in-dual-mode)
 - [How It Works](#how-it-works)
 - [How It Looks](#how-it-looks)
 - [Wishlist](#wishlist)
 - [Quick Start](#quick-start)
   - [Installing](#installing)
     - [From Github](#checkout-from-github)
   - [Using](#using)
 - [Documentation](#documentation)
   - [Access Point Password](#password-protect-the-configuration-access-point)
   - [Callbacks](#callbacks)
   - [Configuration Portal Timeout](#configuration-portal-timeout)
   - [On Demand Configuration](#on-demand-configuration-portal)
   - [Custom Parameters](#custom-parameters)
   - [Custom IP Configuration](#custom-ip-configuration)
   - [Custom HTML, CSS, Javascript](#custom-html-css-javascript)
   - [Filter Low Quality Networks](#filter-networks)
   - [Debug Output](#debug)
 - [Troubleshooting](#troubleshooting)
 - [Releases](#releases)
 - [Contributors](#contributions-and-thanks)

## Features
### Portal Continues To Exist After Configuration
This version does not close the configuration portal immediately on setting the WiFi values but continues to supply information, including network connectivity or otherwise, until the configuration portal is commanded to close.

The Tzapu version of WiFi Manager appropriates the procedure for connecting the browsing device to a network that requires interaction through a web interface. Connecting the users device to a network is not the same thing as connecting a device which the user is browsing to a network and the Tzapu procedure is not robust in this scenario. It [works well in this video](https://youtu.be/6PTj79cWves) which shows an ideal situation but there are problems when the browsing device is different to that in the video or there are issues in connecting to the specified network. In [this discussion](https://github.com/tzapu/WiFiManager/issues/170) there is another [video](https://youtu.be/6ukz_bAwpkI) that shows a situation where the procedure for connecting the browsing device to a network fails.

It is arguable whether manual or automatic portal closure is better for browser based configuration but manual closure is undoubtedly better for app based configuration.

### User Guidance At The Same Address Regardless of Network
Feedback is provided at the same web address, [wifi.urremote.com](http://wifi.urremote.com), when connected to the internet and when connected to the ESP device network. Any requests to any domain name are redirected to this domain name on the device. The IP address 192.168.1.4 works as well but doesn't exist on the internet. This avoids confusion when the browsing computer is connected to the wrong network which can occur because:-
- Often a user fails to realise they need to change WiFi networks.
- During configuation the device network is temporarily shut down and restarted which often causes the browsing device to swap networks in the background leaving the user unaware of the network change.

### Programmatic Configuration
Configuration using an app can be easier because WiFi configuration through a web browser is non intuitive due to a requirement to switch back and forward between WiFi networks. URL's /state and /scan are provided that respond with data in JSON format for easy control of the web interface from external applications. The [ESP Connect](https://play.google.com/store/apps/details?id=au.com.umranium.espconnect) Android app uses this interface. 

### Relies On Automatic Connectivity
An ESP8266 stores access point credentials in non volatile memory so these details should only be set once, unless they need to be changed. The ESP8266 is set by default to auto connect and will always do it's best to connect to a network when it has an access point name (SSID) configured. If the access point is visible and the password is correct it will connect in a few seconds. If the access point is not visible or goes away it will reestablish the connection automatically when the access point becomes visible. There are no calls to the Espressif ESP8266 library that can connect to a network faster or more reliably. 

Older versions of the Espressif library [had a bug](https://github.com/esp8266/Arduino/issues/1997) so that trying to use calls to the Espressif library to connect to a network can sometimes cause the WiFi connectivity to fail until it is rebooted and occasionally can also brick the ESP8266. If you have a device bricked this way it [can be recovered](https://github.com/kentaylor/EraseEsp8266Flash).

### Configuration Portal Initiation Not Automatic
WiFi networks can be unavailable temporarily and only a human can know whether the solution to failing to connect is to change the WiFi connection data. Therefore a configuration portal is not initiated automatically when the ESP device can not connect to the configured WiFi access point. 

The configuration portal is launched if no WiFi configuration data has been stored or a button is pressed. On a Node MCU board the Flash button can be reused as the configuration portal button. 

A configuration portal can be launched on every start up for a minute or so, [if programmed that way](examples/ConfigOnStartup "Example of a configuration portal launched at start up"), but the delay in application start up is unacceptible for most use cases.

### Configuration portal initiated by double pressing the reset button.
This method avoids the use of a pin for launching the configuration portal.  In the [provided example](examples/ConfigOnDoubleReset "Launch configuration on double reset example")  a double press on a button connected to the reset pin can be used to initiate a configuration portal. This works well on development boards that have a reset button like the [Wemos](https://www.wemos.cc/product/d1-mini-pro.html).

### Selectively Operates In Dual Mode
The ESP8266 can simultaneously connect to a Wifi network and run it's own WiFi network in which case it switches the radio channel of it's own network to match the network to which it is attached. When it is searching for a network, the radio channel is changing all the time which makes connecting to it's network flaky. 

So if the device is successfully connected to a network the configuration portal will be available on both networks but if it is not already connected to a network it will operate in access point mode and not try to connect until new credential are entered to increase reliability. 

While trying to connect to the new network it's own WiFi network will necesarily become flaky and it will shutdown and restart it's own WiFi network on a [different channel](http://bbs.espressif.com/viewtopic.php?t=324) after it successfully connects. This can cause a browsing device to switch over to a different network.

## How It Works

- The [ConfigOnSwitch](examples/ConfigOnswitch) example shows how it works and should be used as the basis for a sketch that uses this library.
- The concept of ConfigOnSwitch is that a new ESP8266 will start a WiFi configuration portal when powered up and save the configuration data in non volatile memory. Thereafter, the configuration portal will only be started again if a button is pushed on the ESP8266 module.
- Using any WiFi enabled device with a browser (computer, phone, tablet) connect to the newly created Access Point and type in any http address.
- Because of the Captive Portal and the DNS server you will either get a 'Join to network' type of popup or get any domain you try to access redirected to the configuration portal.
- All http web addresses will be redirected to wifi.urremote.com which will be at IP address 192.168.4.1 . This address is also a valid internet address where the user will see advice that they are connected to the wrong network.
- Choose one of the access points scanned, enter password, click save.
- ESP will try to connect. If successful, the IP address on the new network will be displayed in the configuration portal. 
- The configuration portal will now be visible on two networks, these being it's own network and the network to which it has connected.  On it's own network it will have two IP addresses, the original 192.168.4.1 and the same IP address it has on the network to which it connected.
- Selecting "close configuration portal" will shutdown the web server, shutdown the ESP8266 WiFi network and return control to the following sketch code.

## How It Looks
**Default Home Page**

 ![ESP8266 WiFi Captive Portal Homepage](http://i.imgur.com/v7tyqRJ.png)
**Configuration Page**

![ESP8266 WiFi Captive Portal Configuration](http://i.imgur.com/FwdXhTp.png)


## Wishlist
- A non blocking configuration portal. 
- Initiate configuration portal from a double reset.
- More usability testing. 
- Add javascript to undertake scan in the background and update WiFi page when complete.
- Add multiple sets of network credentials.

## Quick Start

### Installing
See ["Installing Arduino libraries"](https://www.arduino.cc/en/Guide/Libraries) and use the Importing a .zip Library method or preferably use manual installation as you can checkout the release from github and use that. This makes it easier to keep current with updates. Installing using the  Library Manager does not work with this version of the library.

####  Checkout from github
Github version works with release 2.0.0 or newer of the [ESP8266 core for Arduino](https://github.com/esp8266/Arduino)
- Checkout [library](https://github.com/kentaylor/WiFiManager) to your Arduino libraries folder. Must be [https://github.com/kentaylor/WiFiManager](https://github.com/kentaylor/WiFiManager) version.

### Using
- Include in your sketch
```cpp
#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)

#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/kentaylor/WiFiManager WiFi 
```

- When you want open a config portal initialize library, add
```cpp
WiFiManager wifiManager;
```
then call

```cpp
wifiManager.startConfigPortal()
```
While in AP mode, connect to it then open a browser to the gateway IP, default 192.168.4.1, configure wifi, save and it should save WiFi connection information in non volatile memory, reboot and connect.


Once WiFi network information is saved in the ESP8266, it will try to connect to WiFi every time it is started without requiring any function calls in the sketch.


Also see [ConfigOnSwitch example](examples/ConfigOnswitch).

## Documentation

#### Password protect the configuration Access Point
You can password protect the configuration access point.  Simply add an SSID as the first parameter and the password as a second parameter to `startConfigPortal`.
A short password seems to have unpredictable results so use one that's around 8 characters or more in length.
The guidelines are that a wifi password must consist of 8 to 63 ASCII-encoded characters in the range of 32 to 126 (decimal)
```cpp
wifiManager.startConfigPortal( SSID , password )
```

#### Callbacks

##### Save settings
This gets called when custom parameters have been set **AND** a connection has been established. Use it to set a flag, so when all the configuration finishes, you can save the extra parameters somewhere.

See [AutoConnectWithFSParameters Example](examples/AutoConnectWithFSParameters).
```cpp
wifiManager.setSaveConfigCallback(saveConfigCallback);
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
    wifiManager.startConfigPortal();
    Serial.println("connected...yeey :)");
  }
}
```
See  [ConfigOnSwitch](https://github.com/kentaylor/WiFiManager/tree/master/examples/ConfigOnswitch) example for a more complex version.

#### Custom Parameters
Many applications need configuration parameters like MQTT host and port, [blynk](http://www.blynk.cc) or [emoncms](http://emoncms.org) tokens, etc. While it is possible to use WiFiManager to collect additional parameters it is better to read these parameters from a web service once WiFiManager has been used to connect to the internet. This makes WiFiManager simple to code and use, parameters can be edited on a regular web server and can be changed remotely after deployment. A web service that can provide these parameters is at [configure.urremote.com](http://configure.urremote.com/).

To capture other parameters with WiFiManager is a lot more involved than all the other features and requires adding custom HTML to your form. If you want to do it with WiFiManager see the example [ConfigOnSwitchFS](examples/ConfigOnSwitchFS)

#### Custom IP Configuration
You can set a custom IP for both AP (access point, config mode) and STA (station mode, client mode, normal project state)

##### Custom Access Point IP Configuration
This will set your captive portal to a specific IP should you need/want such a feature. Add the following snippet before `startConfigPortal()`
```cpp
//set custom ip for portal
wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
```

##### Custom Station (client) Static IP Configuration
This will use the specified IP configuration instead of using DHCP in station mode.
```cpp
wifiManager.setSTAStaticIPConfig(IPAddress(192,168,0,99), IPAddress(192,168,0,1), IPAddress(255,255,255,0));
```

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
Debug is enabled by default on Serial. To disable add before `startConfigPortal()`
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
Forked from [tzapu](https://github.com/tzapu/WiFiManager) and additional contributions from [Battika](https://github.com/battika) and  [DataCute](https://github.com/datacute/DoubleResetDetector "Double Reset Detector Repository"). Android partner app by [Umran Abdulla](https://play.google.com/store/apps/details?id=au.com.umranium.espconnect).


#### Inspiration
I expected to knock the WiFi connection code over in an afternoon after being inspired by
http://www.esp8266.com/viewtopic.php?f=29&t=2520 . It was taking longer. Then I came across tzapu's implementation which failed for me and this is my attempt to address the issues I discovered. Some weeks have gone by.