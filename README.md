# Arduino DSC code for Arduino Uno

## July 27, 2013
### Changes
* Forked from [mrosseel/arduino-dsc](https://github.com/mrosseel/arduino-dsc)
* Added circuit info for AMT-102 encoders + WiFly RN-XV
* Added program to initialize the WiFly as an access point
* Removed LCD display from original project
* Changed the carriage return/linefeeds to work with SkySafari and Deep Sky Browser
* Removed commands other than 'Q' to return encoder positions due to compatibility issues with SkySafari and DSB
* Removed timer
* Upped the baud rate to 112500
* Changed do the F??? what you want license to MIT, which I guess is whatever the F??? * wanted to do.

### Library dependencies for DSC
* digitalWriteFast: Use the included version, patched in the forked repo.
* PinChangeInt: This version of the encoder software has been tested with [2.19 beta](https://code.google.com/p/arduino-pinchangeint/downloads/list).
* 

### Library dependencies for wifi_setup
* [WiFlySerial](http://sourceforge.net/projects/arduinowifly/files/)

### Instructions

#### Initializing the WiFly
WiFlySerial requires SoftwareSerial to provide the most reliable setup of the WiFly RN-XV. Build a circuit with the RN-XV connected to pins 8 and 9 of your Arduino and run the wifly_setup program. This initializes your WiFly so that it can be connected to the hardware serial pins and used as if it was a physical serial cable.

Initialization is complete when the LED on your Arduino flashes on and off in a 1s cycle.

Because this application initalizes the WiFly for 112500 BPS, you can't run it twice the way it is currently written without resetting the rate on the WiFly. This can be accomplished by applying 3.3V to the reset line on your WiFly.

You can test that initialization was succesful by finding a new wifi network called telescope-XX where XX is the last two letters of your MAC address.

#### Using the DSC application
To use the DSC application, initialize your WiFly, build the circuit per the drawing included in this repository. Connect your device to the telescope-xx network, which provides DHCP and will appropriately initialize your networking for access to the telescope. Configure your application to connect to the IP address 10.0.0.1 on port 4030.

My encoders both provide 8192 ticks, and my azimuth encoder is installed such that it rotates 'opposite' what a typical application would expect so it must be configured to use -8192 ticks. Your encoders and installation may vary.
