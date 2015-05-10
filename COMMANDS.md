# Supported Commands

## Basic DSC Mode (BDSC)

##### Decoder Key:
 * XXXXX = a five digit number in decimal format
 * [a|b] = "a" or "b"
 * [1-10] = Any value between 1 and 10 inclusive
 * \r = \<CR\> 
 * \t = \<TAB\>
 * \<some text\> = a variable length string like this description

#### Commands:
 * A
   * Returns: <nothing>
   * Action: Set aligned flag = True
 * a
   * Returns: Y or N
   * Action: Returns if the scope has been aligned or not
 * Q 
   * Returns: [+|-]XXXXX\t[+|-]XXXXX\r
   * Action: Get encoder DSC values (RA, DEC)
 * R XXXXX XXXXX\r
   * Returns: \<nothing\>
   * Action: Set encoder resolution (RA, DEC)
 * Z XXXXX XXXXX\r
   * Returns: *
   * Action: Set encoder resolution (RA, DEC)
 * G
   * Returns: [+|-]XXXXX\t[+|-]XXXXX\r
   * Action: Get encoder resolution (RA, DEC)
 * H
   * Returns: [+|-]XXXXX\t[+|-]XXXXX\r
   * Action: Get encoder resolution (RA, DEC)
 * V
   * Returns: \<version\>\r
   * Action: Get the software version of TeensyDSC
 * q
   * Returns: XXXXX\r
   * Action: Get the error count
 * T
   * Returns: XXXXX,XXXXX\r
   * Action: Toggle Test mode.  Prints encoder resolutions every 100ms
 * P
   * Returns: P
   * Action: Pretends to "power up" the encoders
 * MODE WIFI\r
   * Action: Switch to WiFly Command mode (see below)
 * MODE CONFIG\r
   * Action: Switch to TeensyDSC Command mode (see below)
 * MODE SERIAL\r
   * Action: Connect to Serial Port A
   
## WiFly Commands (MODE WIFI)
In this mode, most everything you type is passed directly to the WiFly chip.  In order to talk to it, you'll want to read the [WiFly User's Guide](http://dlnmh9ip6v2uc.cloudfront.net/datasheets/Wireless/WiFi/rn-wiflycr-ug-v1.2r.pdf).

Note, while in this mode the digital setting circiles (DSC) feature of the device will be unusable.

##### Warnings: 
 * Manually configuring your WiFly board may "brick" it and render it un-operable!
 * If you enter "command mode" ($$$), make sure you also exit command mode too! (exit)
 * Do not change the command to enter "command mode" as described in section 2.6.1 of the user manual!  Doing so will break TeensyDSC!

* EXIT\r
  * Action: Stop talking to the WiFly and return to the previous mode
 
  
## TeensyDSC Commands (MODE CONF)

#### WiFI Config commands
 * showconf\r
   * Action: prints the current TeensyDSC config
 * ssid \<ssid\>\r
   * Action: Changes the SSID of the AP
 * wpa [on|off]\r
   * Action: Enable/disable WPA security of the AP
 * wpa pass \<password\>\r
   * Action: Changes the WPA password of the AP
 * channel [1-13]\r
   * Action: Changes the WiFi channel of the AP
 * save\r
   * Action: Saves any WiFi changes to the AP EEPROM
 * reboot wifi\r
   * Resets the WiFly.  May be required for some changes to take effect!
 * reset defaults\r
   * Resets all the stored defaults in the TeensyDSC
 * EXIT\r
   * Action: Stop configuring the TeensyDSC and return to the previous mode     
