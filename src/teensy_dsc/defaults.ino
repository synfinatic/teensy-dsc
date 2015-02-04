#include <EEPROM.h>

#include "defaults.h"
#include "teensy_dsc.h"

extern AnySerial UserSerial;

char *
get_eeprom_data(uint16_t start, uint16_t len) {
    char *buffer;
    uint16_t i;

    buffer = (char *)malloc(EEPROM_BUFFER_SIZE);

    for (i = 0; i < len; i++) {
        buffer[i] = EEPROM.read(start + i);
    }
    return buffer;
}

void
write_eeprom_data(uint16_t start, uint16_t len, char *buffer) {
    uint16_t i;

    for (i = 0; i < len; i++) {
        EEPROM.write(start + i, buffer[i]);
    }
}

/*
 * updates the EEPROM with new network settings
 */
void 
set_network_settings(network_settings_t *settings) {
    write_eeprom_data(NETWORK_SETTINGS_OFFSET, sizeof(network_settings_t),
        (char *)settings);
}

/*
 * Retrieves the network settings from the EEPROM
 */
network_settings_t *
get_network_settings() {
    network_settings_t *buff;
    char empty[68];
    int i;

    for (i = 0; i < 67; i++)
        empty[i] = 0xff;

    buff = (network_settings_t *)get_eeprom_data(NETWORK_SETTINGS_OFFSET,
            sizeof(network_settings_t));

    if (memcmp(buff->ip_address, empty, 16) == 0)
        strcpy(buff->ip_address, IP_ADDRESS);
    if (memcmp(buff->netmask, empty, 16) == 0)
        strcpy(buff->netmask, NETMASK);
    if (memcmp(buff->ssid, empty, 32) == 0)
        strcpy(buff->ssid, SSID);
    if (memcmp(buff->passphrase, empty, 68) == 0)
        strcpy(buff->passphrase, WPA_PASSWORD);

    buff->port = buff->port == 0xffff ?  PORT : buff->port;
    buff->channel = buff->channel == 0xff ? WIFLY_CHANNEL : buff->channel;
    buff->tx_power = buff->tx_power == 0xff ? TX_POWER : buff->tx_power;
    buff->rate = buff->rate == 0xff ? WIFLY_RATE : buff->rate;
    buff->enable_wpa = buff->enable_wpa == 0xff ? ENABLE_WPA : buff->enable_wpa;
    buff->enable_ap = buff->enable_ap == 0xff ? ENABLE_AP : buff->enable_ap;
    buff->debug_wifly = buff->debug_wifly == 0xff ? DEBUG_WIFLY : buff->debug_wifly;
    buff->default_dsc_mode = buff->default_dsc_mode == 0xff ? 0 : buff->default_dsc_mode;
    return buff;
}

/*
 * updates the EEPROM with the settings for a serial port
 */
void 
set_serial_settings(serial_settings_t *settings) {
    write_eeprom_data(SERIAL_SETTINGS_OFFSET,
            sizeof(serial_settings_t),
            (char *)settings);
}

/*
 * Retrieves the settings for a serial port
 */
serial_settings_t *
get_serial_settings() {
    serial_settings_t *buff;
    buff = (serial_settings_t *)get_eeprom_data(SERIAL_SETTINGS_OFFSET,
            sizeof(serial_settings_t));

    // If undefined in EEPROM, use defaults
    buff->wifly_baud = buff->wifly_baud == 0xffffffff ?
        WIFLY_SERIAL_SPEED : buff->wifly_baud;
    buff->seriala_baud = buff->seriala_baud == 0xffffffff ?
        SERIAL_A_SPEED : buff->seriala_baud;
    buff->serialb_baud = buff->serialb_baud == 0xffffffff ?
        SERIAL_B_SPEED : buff->serialb_baud;

    return buff;
}

/*
 * Retrieves the settings for the encoders
 */
encoder_settings_t *
get_encoder_settings() {
    encoder_settings_t *buff;
    buff = (encoder_settings_t *)get_eeprom_data(ENCODER_SETTINGS_OFFSET,
            sizeof(encoder_settings_t));

    // if undefined in EEPROM, use defaults
    buff->ra_cps = (uint32_t)buff->ra_cps == 0xffffffff ? 
        RA_ENCODER_CPS : buff->ra_cps;
    buff->dec_cps = (uint32_t)buff->dec_cps == 0xffffffff ?
        DEC_ENCODER_CPS : buff->dec_cps;
    return buff;
}

/*
 * Write the encoder settings to the EEPROM
 */
void
set_encoder_settings(encoder_settings_t *settings) {
    write_eeprom_data(ENCODER_SETTINGS_OFFSET,
            sizeof(encoder_settings_t),
            (char *)settings);
}

/*
 * Erases the EEPROM and clears all our defaults
 */
void
clear_all_defaults() {
    uint16_t length, i;

    length = sizeof(network_settings_t) + (sizeof(serial_settings_t) * 2);
    for (i = 0; i < length; i++) {
        EEPROM.write(i, 0xff);
    }
}

/*
 * Takes the hard coded settings (see defaults.h) and
 * writes them to the EEPROM where they will be read
 * the next time we power on
 */
void
reset_all_defaults() {
    network_settings_t network;
    serial_settings_t serial;
    encoder_settings_t encoders;

    strcpy(network.ip_address, IP_ADDRESS);
    strcpy(network.netmask, NETMASK);
    strcpy(network.ssid, SSID);
    strcpy(network.passphrase, WPA_PASSWORD);
    network.port = PORT;
    network.channel = WIFLY_CHANNEL;
    network.tx_power = TX_POWER;
    network.rate = WIFLY_RATE;
    network.enable_wpa = ENABLE_WPA;
    network.enable_ap = ENABLE_AP;
    network.debug_wifly = DEBUG_WIFLY;
    set_network_settings(&network);

    serial.seriala_baud = SERIAL_A_SPEED;
    serial.serialb_baud = SERIAL_B_SPEED;
    serial.wifly_baud = WIFLY_SERIAL_SPEED;
    set_serial_settings(&serial);

    encoders.ra_cps = RA_ENCODER_CPS;
    encoders.dec_cps = DEC_ENCODER_CPS;
    set_encoder_settings(&encoders);
}
