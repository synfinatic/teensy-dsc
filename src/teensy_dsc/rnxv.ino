#include <Arduino.h>
#include <avr/pgmspace.h>
#include <AnySerial.h>
#include "teensy_dsc.h"
#include "rnxv.h"

void
rnxv_save_settings(WiFly *wifly) {
    wifly->print("save\r");
    delay(PAUSE_DURATION);
    wifly->print("reboot\r");
    delay(LONG_PAUSE_DURATION);
}

void
rnxv_get_config(WiFly *wifly, AnySerial *serial) {
    char c;

    wifly->sendCommand("get everything\r");
    delay(10);
    while (wifly->receive((uint8_t *)&c, 1, 300)) {
        serial->print((char)c);
    }
    wifly->dataMode();
}

void
rnxv_configure(WiFly *wifly, network_settings_t *network) {
    char buff[30];


    if (network->enable_ap) {
        // AP mode
        sprintf(buff, "set w j 7 i\r");
    } else {
        // Client mode
        sprintf(buff, "set w j 1 i\r");
    }
    wifly->sendCommand(buff, "AOK");        // Enable AP mode
    if (network->enable_ap == 7) {
        wifly->sendCommand("set i d 4\r", "AOK");        // Enable DHCP server
        sprintf(buff, "set i a %s\r", network->ip_address);
        wifly->sendCommand(buff, "AOK"); // Specify IP
        sprintf(buff, "set i n %s\r", network->netmask);
        wifly->sendCommand(buff, "AOK"); // Specify netmask
        sprintf(buff, "set i g %s\r", network->ip_address);
        wifly->sendCommand(buff, "AOK"); // Specify gateway
        sprintf(buff, "set d a %s\r", network->ip_address);
        wifly->sendCommand(buff, "AOK"); // Specify DNS address
        sprintf(buff, "set w c %d\r", network->channel);
        wifly->sendCommand(buff, "AOK");        // Specify the channel to create network
    } else {
        wifly->sendCommand("set i d 1\r", "AOK");        // Enable DHCP client
        // in client mode, don't set the channel
        sprintf(buff, "set w c %d\r", 0);
        wifly->sendCommand(buff, "AOK");        // Specify the channel to create network
    }

    sprintf(buff, "set w s %s\r", network->ssid);
    wifly->sendCommand(buff, "AOK"); // Set network SSID

    wifly->sendCommand("set d name teensydsc\r", "AOK"); // DNS name

    sprintf(buff, "set wlan tx %d\r", network->tx_power);
    wifly->sendCommand(buff, "AOK"); // Specify transmit power

    sprintf(buff, "set wlan rate %d\r", network->rate);
    wifly->sendCommand(buff, "AOK");

    if (network->enable_wpa) {
        wifly->sendCommand("set wlan auth 4\r", "AOK");
        sprintf("set wlan phrase %s\r", network->passphrase);
        wifly->sendCommand(buff, "AOK");
    } else {
        wifly->sendCommand("set wlan auth 0\r", "AOK");
    }
    sprintf(buff, "set i lo %d\r", network->port);
    wifly->sendCommand(buff, "AOK");  // TCP port

    wifly->sendCommand("set i pr 2\r", "AOK"); // Tcp client/server mode


    wifly->sendCommand("set comm remote 0\r", "AOK");
    wifly->sendCommand("set comm open 0\r", "AOK");
    wifly->sendCommand("set comm close 0\r", "AOK");
    sprintf(buff, "set uart baudrate %d\r", WIFLY_SERIAL_SPEED);
    wifly->sendCommand(buff, "AOK"); 

    // sprintf(buff, "join %s\r", network->ssid, 5000);
    // wifly->sendCommand(buff, "AOK");
    wifly->sendCommand("save\r", "Storing", 300);  // save
    wifly->sendCommand("reboot\r");

    delay(1000);  

}