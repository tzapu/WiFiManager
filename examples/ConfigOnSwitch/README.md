# The Config On Switch Example
In this example we initiate the configuration portal to connect an ESP8266 to WiFi by pushing a button. The example requires this version of WiFi Manager. It will not work with the tzapu version.

## Why Have A Button To Initiate Configuration?
Once the ESP device contains network credentials it will always try to connect to that network in the background and succeed whenever that network becomes visible. The application continues to function and because WiFi networks tend to be flaky only a human has the knowledge of whether a new network is required or it is better to wait for the network to become visible again. Therefore, providing a configuration portal must be human initiated and requiring a button push is a good way to get human input. 

The alternative of automatically going into configuration mode every time a WiFi network becomes invisible will cause the application to stop and the device will sit in configuration mode forever. This is undesirable when a network is temporarily unavailable. 

## Issues With Automatically Going Into Configuration Mode
Providing a timeout on configuration mode is one way to get the application running again when a network becomes temporarily unavailable and no one is around to reboot. However, the application will still not function while waiting for the timeout and the device is vulnerable to sabotage when in config mode. With a push button it will also be vulnerable to sabotage whenever the saboteur has physical access but then it is vulnerable in other ways as well. An assumption that saboteurs will not have physical access to the device is sufficient for most applications. 


