#include <EEPROM.h>

#include "defaults.h"

char *
get_eeprom_buffer() {
    static char *buffer = NULL;
    if (buffer == NULL) {
        buffer = (char *)malloc(EEPROM_BUFFER_SIZE);
    }
    memset(buffer, 0, sizeof(buffer));
    return buffer;
}

char *
get_eeprom_data(uint16_t start, uint16_t len) {
    char *buffer;
    uint16_t i;

    buffer = get_eeprom_buffer();

    for (i = 0; i < len; i++) {
        buffer[i] = EEPROM.read(start + i);
    }
    return buffer;
}

void
write_eeprom_data(uint16_t start, uint16_t len, char *buffer) {
    uint16_t i;

    for (i = 0; i < len; i++) {
        EEPROM.write(i + start, buffer[i]);
    }
}

/*
 * updates the EEPROM with new network settings
 */
void 
set_network_defaults(network_settings_t *settings) {
    write_eeprom_data(NETWORK_SETTINGS_OFFSET, sizeof(network_settings_t),
        (char *)settings);
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
                    (char *)settings);
            break;
        case SERIAL_B:
            write_eeprom_data(SERIAL_B_SETTINGS_OFFSET,
                    sizeof(serial_settings_t),
                    (char *)settings);
            break;
    }
}

/*
 * Retrieves the settings for a serial port
 */
serial_settings_t *
get_serial_defaults(serial_port_t port) {
    char *buffer = NULL;

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
 * Retrieves the settings for the encoders
 */
encoder_settings_t *
get_encoder_settings() {
    char *buffer = NULL;
    buffer = get_eeprom_data(ENCODER_SETTINGS_OFFSET,
            sizeof(encoder_settings_t));
    return (encoder_settings_t *)buffer;
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
        EEPROM.write(i, 255);
    }
}
