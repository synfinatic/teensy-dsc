# TeensyDSC: WiFi enabled Digital Setting Circles

## Important Note

As of Nov 16, 2014 I've been able to order a PCB from OSHPark using the 
[Simple-DSC](https://github.com/synfinatic/teensy-dsc/tree/simple-dsc/eagle)
design and with the latest code from 
[master](https://github.com/synfinatic/teensy-dsc/commit/1c4c6b0b567ba3cc21cd7ba83029384eee6130e7).
I seem to have a working solution using SkySafari Pro on my iPad with 
[US Digital S2-2500](http://www.usdigital.com/products/encoders/incremental/rotary/shaft/S2)
10,000 step optical encoders.

For more information, please see the [TeensyDSC Wiki](https://github.com/synfinatic/teensy-dsc/wiki)

There is also this thread on [CloudyNights](http://www.cloudynights.com/topic/482436-yet-another-diy-digital-setting-circles-project-teensydsc/)

## Basic Features
I wanted to do my own digital setting circles (DSC) for my 
[Apertura AD12](http://www.opticsmart.com/telescopes/dobsonian-telescopes/apertura-ad12-dobsonian-reflector-telescope.html).
The main goals were:

 * Tested to support 40,000 resolution via quadurature decoding using [US Digital S2-10000-250-IE-D-D encoders](http://usdigital.com/products/encoders/incremental/rotary/shaft/S2)
 * Should support resolutions in excess of 100K
 * Built in WiFi access point to allow wirelessly interfacing to an Apple iPad or Android tablet
 * Supporting at least [SkySafari Plus/Pro](http://www.skysafariastronomy.com/)
 * Powered via USB connection or external 7-24V battery source
 * Using industry standard RJ45 connection to encoders
 * Open source software & PCB design (GPLv3)
    * PCB Design done in [Eagle PCB](http://www.cadsoftusa.com/eagle-pcb-design-software/?language=en)
    * PCB's available from [OshPark](http://www.oshpark.com)
 * Small 2"x3"x1" enclosure
 * Inexpensive!  All the components should be available for < $100.

That will be version 1.0.  Future versions will hopefully support other features,
but for now that will be enough for it to work for me.

## Why Teensy 3.1
Why the [Teensy 3.1](http://pjrc.com/store/teensy31.html) and not Arduino?

 * It is *much* faster then the Arduino Uno or Mega (32bit 72Mhz CPU vs 16bit 16Mhz)
 * Small footprint and only costs $20!
 * Runs at 3.3V, but handles 5V just fine which simplifies talking to WiFi chipsets and encoders
 * It supports interrupts on all digital pins which allows for higher resolution encoders
 * Has 3 hardware serial ports which allows interfacing with GoTo hardware, etc
 * More memory & flash allows for a lot more features

## Required Libraries & Software
TeensyDSC requires some additional Arduino libraries to compile/run:

 * [Arduino IDE v1.6.x](http://arduino.cc/en/Main/Software)
 * [Teensyduino](http://pjrc.com/teensy/td_download.html) - Teensy addon for the Arduino IDE
 * [AnySerial](https://github.com/synfinatic/AnySerial) - Serial port abstraction layer for Arduino
 * [WiFi_Shield](https://github.com/synfinatic/WiFi_Shield) - Customized to use AnySerial
 * [Flash](https://github.com/mikalhart/Flash) - Easy access to EEPROM flash memory
 * [MsTimer2](https://github.com/PaulStoffregen/MsTimer2) - MsTimer2 with Teensy3.x support

## Supported DSC Software
Please note that I have limited access to software, so this may not be a
complete list.  If you have tested TeensyDSC with some software, I'd love
to hear if it worked or not.  If it did not work, hopefully we can work
together to make it work!

 * SkySafari (OSX, iOS)  
   * Scope Type: Basic Encoder System
   * For "Encoder Steps per Revolution" click "Get Automatically"

## Similar Projects
There are a bunch of similar projects to TeensyDSC which you should be aware of:

 * [Dave Ek's Setting Circles](http://eksfiles.net/digital-setting-circles/)
 * [mrosseel/arduino-dsc](https://github.com/mrosseel/arduino-dsc)
 * [mdw123/arduino-dsc](https://github.com/mdw123/arduino-dsc)
 * [Arduino Bluetooth DSC](http://orlygoingthirty.blogspot.com/2012/01/arduino-bluetooth-digital-setting.html)
 * [Mike Fulbright's Arduino DSC](http://msfastro.net/articles/arduinodsc/)

## Similar Commercial Products
There are also a number of commercial products which share many of the same features of TeensyDSC:

 * [Southern Stars/Orion SkyFi](http://www.southernstars.com/products/skyfi/)
 * [Astro Devices Nexus](http://www.astrodevices.com/products/Nexus/Nexus.html)
 * [Astro Devices Nexus-S](http://www.astrodevices.com/products/NexusS/NexusS.html)

