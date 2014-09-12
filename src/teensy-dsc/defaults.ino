#include <EEPROM.h>

#include "teensy-dsc.h"
#include "defaults.h"

char *
get_eeprom_buffer() {
    static char *buffer = NULL;
    if (buffer == NULL) {
        buffer = malloc(EEPROM_BUFFER_SIZE);
    }
    memset(buffer, NULL, sizeof(buffer));
    return buffer;
}

char *
get_eeprom_data(uint16_t start, uint16_t len) {
    char *buffer;
    uint16_t i;

    buffer = get_eeprom_buffer();

    for (i = 0, i < len, i++) {
        buffer[i] = EEPROM.read(start + i);
    }
    return buffer;
}

void
write_eeprom_data(uint16_t start, uint16_t len, char *buffer) {
    uint16_t i;

    for (i = 0, i < len, i++) {
        EEPROM.write(i + start, buffer[i])
    }
}

/*
 * updates the EEPROM with new network settings
 */
void 
set_network_defaults(network_settings_t *settings) {
    write_eeprom_data(NETWORK_SETTINGS_OFFSET, sizeof(network_settings_t),
        settings);
}

/*
 * Retrieves the network settings from the EEPROM
 */
network_settings_t *
get_network_defaults() {
    char *buffer;
    buffer = get_eeprom_data(NETWORK_SETTINGS_OFFSET, sizeof(network_settings_t));
    return (network_settings_t *)buffer;
}

/*
 * updates the EEPROM with the settings for a serial port
 */
void 
set_serial_defaults(serial_port_t port, serial_settings_t *settings) {
    switch (port) {
        case SERIAL_A:
            write_eeprom_data(SERIAL_A_SETTINGS_OFFSET,
                    sizeof(serial_settings_t),
                    settings);
            break;
        case SERIAL_B:
            write_eeprom_data(SERIAL_B_SETTINGS_OFFSET,
                    sizeof(serial_settings_t),
                    settings);
            break;
    }
}

/*
 * Retrieves the settings for a serial port
 */
serial_settings_t *
get_serial_defaults(serial_port_t port) {
    char *buffer;

    switch (port) {
        case SERIAL_A:
            buffer = get_eeprom_data(SERIAL_A_SETTINGS_OFFSET,
                    sizeof(serial_settings_t));
            break;
        case SERIAL_B:
            buffer = get_eeprom_data(SERIAL_B_SETTINGS_OFFSET,
                    sizeof(serial_settings_t));
            break;
    }
    return (serial_settings_t *)buffer;
}

/*
 * Erases the EEPROM and clears all our defaults
 */
void
clear_all_defaults() {
    uint16_t length, i;

    length = sizeof(network_settings_t) + (sizeof(serial_settings_t) * 2);
    for (i = 0, i < length, i++) {
        EEPROM.write(i, 255);
    }
}
