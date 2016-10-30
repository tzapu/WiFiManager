# Launch A WiFi Configuration Portal By Double Pressing An ESP8266 Reset Button.

This method works well on Wemos boards which have a single reset button on board. It avoids using a pin for launching the configuration portal.

## How It Works
When the ESP8266 loses power all data in RAM is lost but when it is reset the contents of a small region of RAM is preserved. So when the device starts up it checks this region of ram for a flag to see if it has been recently reset. If so it launches a configuration portal, if not it sets the reset flag. After running for a while this flag is cleared so that it will only launch the configuration portal in response to closely spaced resets.

## Settings
There are two values to be set in the sketch.
#### DRD_TIMEOUT
Number of seconds to wait for the second reset. Set to 10 in the example.
#### DRD_ADDRESS
The address in RTC RAM to store the flag. This memory must not be used for other purposes in the same sketch. Set to 0 in the example.
## Additional Library
This example, contributed by [DataCute](https://github.com/datacute/DoubleResetDetector "Double Reset Detector Repository"), needs the [DataCute](https://github.com/datacute/DoubleResetDetector "Double Reset Detector Repository") Double Reset Detector library.