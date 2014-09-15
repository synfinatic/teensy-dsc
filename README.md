# Digital Setting Circles for Teensy 3.1

## Important Note

As of Sept 14, 2014 this project is still heavily in development.  The PCB 
design is completely untested so far!  So don't be going and ordering this 
board off of OSHPark/etc just yet... unless of course you want to help test!
If so, then please contact me first and I can work with you.

## Basic Features
I wanted to do my own digital setting circles (DSC) for my [Apertura AD12](http://www.opticsmart.com/telescopes/dobsonian-telescopes/apertura-ad12-dobsonian-reflector-telescope.html).
The main goals were:
 * Nice PCB & enclosure
 * Support for at least 16K resolution via quadurature decoding ([CUI AMT11 encoders](www.cui.com/product/resource/amt11.pdf))
 * WiFi to allow interfacing via my Apple iPad (unfortunately, Apple doesn't allow 
   connecting to random bluetooth devices)
 * Supporting at least SkySafari
 * Powered via USB or 7-36V battery source

That will be version 1.0.  Future versions will hopefully support other features,
but for now that will be enough for it to work for me.

## Why Teensy 3.1
Why the [Teensy 3.1](http://pjrc.com/store/teensy31.html) and not Arduino?
 * It is *much* faster then the Arduino Uno (72Mhz vs 16Mhz)
 * It supports interrupts on all digital pins which allows for higher resolution encoders
 * Has 3 hardware serial ports which allows interfacing with GoTo hardware, etc
 * It is cheaper ($20/ea)
 * More memory & flash allows for a lot more features

## Supported Software
Please note that I have limited access to software, so this may not be a
complete list.  If you have tested TeensyDSC with some software, I'd love
to hear if it worked or not.  If it did not work, hopefully we can work
together to make it work!

 * SkySafari (OSX, iOS)  = work in progress!

## Similar Projects
There are a few very similar projects to TeensyDSC which you should be aware of:
 * [mrosseel/arduino-dsc](https://github.com/mrosseel/arduino-dsc)
 * [mdw123/arduino-dsc](https://github.com/mdw123/arduino-dsc)
 * [Dave Ek's Setting Circles](http://eksfiles.net/digital-setting-circles/)

## Similar Commercial Products
 * [SkyFi](http://www.southernstars.com/products/skyfi/)
 * [Nexus](http://www.astrodevices.com/products/Nexus/Nexus.html)
 * [Nexus-S](http://www.astrodevices.com/products/NexusS/NexusS.html)

