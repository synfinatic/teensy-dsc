#include <Arduino.h>
#include <avr/pgmspace.h>
#include <AnySerial.h>
#include "teensy_dsc.h"
#include "wifly.h"

/* need to put this in the .cpp so it's only available once */
FLASH_STRING(wifi_comms_cmds,
	"set uart mode 0x13\r"
	"set comm close " PORT_CLOSE "\r"
	"set comm open " PORT_OPEN "\r"
	"set comm remote 0\r"
	"set comm size %d\r"
	"set comm time %d\r"
	);

/*
 * Initialize the WiFly board
 */
void 
wifi_initial_setup(WiFlySerial *WiFly, network_settings_t *network) {

    WiFly->getDeviceStatus();

    // reset to factory defaults
    dbg_serial_printf("factory RESET...");
    WiFly->uart.print("factory RESET\r");
    delay(LONG_PAUSE_DURATION);
    dbg_serial_printf(" complete!\n");
    wifi_setup_comms(WiFly);
    wifi_setup_network(WiFly, network);
    wifi_setup_wireless(WiFly, network);
//     WiFly->closeConnection();

    // clear out prior requests.
    WiFly->flush();
    dbg_serial_printf("clearing buffer...");
    while (WiFly->available())
        WiFly->read();
    dbg_serial_printf("Done!\n");
}

/*
 * Configure IPv4 Networking
 */
void
wifi_setup_network(WiFlySerial *WiFly, network_settings_t *network) {
    WiFly->setIP(network->ip_address);
    WiFly->setGateway(network->ip_address);
    WiFly->setNetMask(network->netmask);
    WiFly->setLocalPort(network->port);
    WiFly->setDHCPMode(4);
    WiFly->setProtocol(2);
    WiFly->setDNS(network->ip_address);
}

/*
 * Configure wireless
 */
void
wifi_setup_wireless(WiFlySerial *WiFly, network_settings_t *network) {
    char buff[300];
    char indicator[INDICATOR_BUFFER_SIZE];

    WiFly->setSSID(network->ssid);
    sprintf(buff, "%d", network->channel);
    WiFly->setChannel(buff);
    WiFly->setJoinMode(7);
    sprintf(buff, "set wlan tx %d", network->tx_power);
    WiFly->SendCommandSimple(buff, indicator);

    if (network->enable_wpa) {
	WiFly->setPassphrase(network->passphrase);
	WiFly->setAuthMode(network->enable_wpa);
    }
}

/*
 * Configure serial comms
 */
void
wifi_setup_comms(WiFlySerial *WiFly) {
    char buff[300];
    wifi_comms_cmds.copy(buff, 300);
    WiFly->printf(buff,
	    COMM_SIZE,
	    COMM_TIME_MS);
}

void
wifi_save_settings(WiFlySerial *WiFly) {
    WiFly->print("save\r");
    delay(PAUSE_DURATION);
    WiFly->print("reboot\r");
    delay(LONG_PAUSE_DURATION);
}

#define REQUEST_BUFFER_SIZE 120
void
wifi_get_config(WiFlySerial *WiFly, AnySerial *serial) {
    char bufRequest[REQUEST_BUFFER_SIZE];

    WiFly->getIP(bufRequest, REQUEST_BUFFER_SIZE);
    *serial << F("IP: ") << bufRequest << endl;
    WiFly->getNetMask(bufRequest, REQUEST_BUFFER_SIZE);
    *serial << F("Netmask: ") << bufRequest << endl;
    WiFly->getSSID(bufRequest, REQUEST_BUFFER_SIZE);
    *serial << F("SSID: ") << bufRequest << endl;
}
