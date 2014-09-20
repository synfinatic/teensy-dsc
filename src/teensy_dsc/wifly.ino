#include <Arduino.h>
#include <avr/pgmspace.h>
#include <AnySerial.h>
#include "teensy_dsc.h"
#include "wifly.h"

/*
 * Initialize the WiFly board
 */
void 
wifi_initial_setup(WiFlySerial *WiFly, network_settings_t *network) {
    char command[50], cmd[25];
    const char *arg;
    size_t i;

    WiFly->getDeviceStatus();

    // reset to factory defaults
    WiFly->uart.print("factory RESET\r");
    delay(LONG_PAUSE_DURATION);
    wifi_setup_comms(WiFly);
    wifi_setup_network(WiFly, network);
    wifi_setup_wireless(WiFly, network);
    WiFly->closeConnection();

    // clear out prior requests.
    WiFly->flush();
    while (WiFly->available())
        WiFly->read();
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
	WiFly->setAuthMode(4);
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
#define BODY_BUFFER_SIZE 255
void
wifi_get_config(WiFlySerial *WiFly, AnySerial *serial) {
    char bufRequest[REQUEST_BUFFER_SIZE];
    char bufBody[BODY_BUFFER_SIZE];
    *serial << F("IP: ") << WiFly->getIP(bufRequest, REQUEST_BUFFER_SIZE) << endl <<
        F("Netmask: ") << WiFly->getNetMask(bufRequest, REQUEST_BUFFER_SIZE) << endl <<
        F("Gateway: ") << WiFly->getGateway(bufRequest, REQUEST_BUFFER_SIZE) << endl <<
        F("DNS: ") << WiFly->getDNS(bufRequest, REQUEST_BUFFER_SIZE) << endl
        << F("WiFly Sensors: ") << bufBody <<  WiFly->SendCommand("show q 0x177 ",">", bufBody, BODY_BUFFER_SIZE) << endl
        << F("WiFly Temp: ") <<  WiFly->SendCommand("show q t ",">", bufBody, BODY_BUFFER_SIZE) << endl;
}
