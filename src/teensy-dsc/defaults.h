#ifndef __DEFAULTS_H__
#define __DEFAULTS_H__

/*
 * These are all the hard coded defaults for new
 * devices.  You can change them here or override
 * them via some kind of CLI interface (TBD) which
 * will write values in the EEPROM.
 */

/*
 * Defaults for Wireless.   Feel free to change
 * these as desired
 */
#define NETWORK "10.0.0.0"
#define IP_ADDRESS "10.0.0.1"
#define NETMASK "255.255.255.0"
#define PORT "4030"
#define SSID "teensydsc"


/*
 * Defaults for serial ports exposed as RJ11 jacks
 */
#define SERIAL_A_BAUD 9600
#define SERIAL_A_BITS 8
#define SERIAL_A_PARITY 'N'
#define SERIAL_A_STOP 1

#define SERIAL_B_BAUD 9600
#define SERIAL_B_BITS 8
#define SERIAL_B_PARITY 'N'
#define SERIAL_B_STOP 1

/*************************************************************** 
 * Ok, don't be changing anything below this point unless you 
 * really know what you're doing
 ***************************************************************/

/*
 * this is the size of our static buffer for EEPROM config
 * data.  It only needs to be big enough to hold *ONE*
 * network_settings_t or serial_settings_t struct.
 */
#define EEPROM_BUFFER_SIZE 255

typedef struct _network_settings {
    uint32_t network;
    uint32_t ip_address;
    uint32_t netmask;
    char ssid[30];
    uint16_t port;
    uint8_t channel;
    uint8_t tx_power; 
} network_settings_t;

typedef enum _serial_port {
    SERIAL_A,
    SERIAL_B,
} serial_port_t;

typedef struct _serial_settings {
    uint32_t baud;
    char parity;
    uint8_t stop_bits;
} serial_settings_t;

void set_network_defaults(network_settings_t *);
network_settings_t *get_network_defaults();
void set_serial_defaults(serial_port_t, serial_settings_t *);
serial_settings_t *get_serial_defaults(serial_port_t);
void clear_all_defaults();

#define NETWORK_SETTINGS_OFFSET 0
#define SERIAL_A_SETTINGS_OFFSET (sizeof(network_settings_t))
#define SERIAL_B_SETTINGS_OFFSET (SERIAL_A_SETTINGS_OFFSET + sizeof(serial_settings_t))

#endif
