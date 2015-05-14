/*
 * Teensy 3.1 Digital Setting Circles
 * Copyright (C) 2014-2015 Aaron Turner
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifndef __DEFAULTS_H__
#define __DEFAULTS_H__


/*
 * These are all the hard coded defaults for new
 * devices.  You can change them here or override
 * them via some kind of CLI interface (TBD) which
 * will write values in the EEPROM.
 */

// Default encoder CPS
#define RA_ENCODER_CPS 10000
#define DEC_ENCODER_CPS -10000

/*
 * Defaults for Wireless.   Feel free to change
 * these as desired.  All values need to be stored as strings!
 */
#define IP_ADDRESS "10.0.0.1"
#define NETMASK "255.255.255.0"
#define PORT 4030
#define SSID "TeensyDSC"
#define WPA_PASSWORD "teensydsc"
#define WIFLY_RATE 12
#define WIFLY_CHANNEL 1
#define TX_POWER 1
#define ENABLE_WPA 0
#define ENABLE_AP 1
#define DEBUG_WIFLY 0

/*
 * Defaults for serial ports exposed as RJ11 jacks
 */
#define SERIAL_A_SPEED 9600
#define SERIAL_B_SPEED 9600

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

typedef struct {
    char ip_address[16];
    char netmask[16];
    char ssid[32];
    char passphrase[68];
    uint16_t port;
    uint8_t channel;
    uint8_t tx_power; 
    uint8_t rate;
    uint8_t enable_wpa;
    uint8_t enable_ap;
    uint8_t debug_wifly;
} network_settings_t;

typedef struct {
    uint32_t wifly_baud;
    uint32_t seriala_baud;
    uint32_t serialb_baud;
} serial_settings_t;

typedef struct {
    long ra_cps;
    long dec_cps;
} encoder_settings_t;

void set_network_settings(network_settings_t *);
network_settings_t *get_network_settings();

void set_serial_settings(serial_settings_t *);
serial_settings_t *get_serial_settings();

void set_encoder_settings(encoder_settings_t *);
encoder_settings_t *get_encoder_settings();

void clear_all_defaults();
void reset_all_defaults();

#define NETWORK_SETTINGS_OFFSET 0
#define SERIAL_SETTINGS_OFFSET (sizeof(network_settings_t))
#define ENCODER_SETTINGS_OFFSET (SERIAL_SETTINGS_OFFSET + sizeof(serial_settings_t))

#endif
