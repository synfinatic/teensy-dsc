#include <Arduino.h>
#include <AnySerial.h>
#include "teensy_dsc.h"
#include "cli.h"
#include "utils.h"
#include "defaults.h"
#include "rnxv.h"

void setup_commands(cli_ctx *ctx);

/*
 * Initalizer for a CLI context
 */
cli_ctx *
cli_init_cmd(AnySerial *aserial, common_cli_ctx *common, WiFly *wifly,
        Encoder *ra, Encoder *dec) {
    cli_ctx *ctx;

    ctx = (cli_ctx *)malloc(sizeof(cli_ctx));
    bzero(ctx, sizeof(cli_ctx));
    ctx->common = common;
    ctx->state = BASIC_DSC;
    ctx->prev_state = NONE;
    ctx->serial = aserial;
    ctx->wifly = wifly;
    ctx->ra = ra;
    ctx->dec = dec;
    if (wifly == NULL) {
        ctx->eat_errors = true;
    } else {
        ctx->eat_errors = false;
    }
    setup_commands(ctx);

    return ctx;
}

/*
 * Run this every time we change CLI state
 * to update our CTX
 */
void
change_state(cli_ctx *ctx, cli_state new_state) {
    ctx->prev_state = ctx->state;
    ctx->state = new_state;

    setup_commands(ctx);

    // if changing to WIFI mode, then enter now.
    if (ctx->state == WIFI && ctx->wifly != NULL) {
        wifi_interactive(ctx);
    } else if (ctx->state == WIFI) {
        ctx->serial->printf("ctx->wifly not defined. Can't change mode\n");
        ctx->state = ctx->prev_state;
        setup_commands(ctx);
    } else {
        ctx->serial->printf("OK\n");
    }
}

/*
 * Parse the line and execute the given command.
 *
 * Returns true/false depending on if the cmd line
 * passed in was successfully processed and can
 * be freed by the caller
 *
 * TODO: Handle commands of any size more gracefully
 * by actually using the len option
 */
cmd_status
cli_proc_cmd(cli_ctx *ctx, char *line, size_t len) {
    char cmd[READBUFF_SIZE];
    char args[READBUFF_SIZE], byte, *line2;
    size_t pos = 0, line_max;
    bool has_args = false;

    cmd[0] = NULL;
    args[0] = NULL;

    /* split the line into the cmd & args */
    byte = line[0];
    line_max = strlen(line);
    while ((! IS_WORD_END(byte)) && (pos < line_max)) {
        pos ++;
        byte = line[pos];
    }
    if (IS_WORD_END(line[pos])) {
        line[pos] = NULL;
        line2 = &(line[pos+1]);
        strncpy(args, line2, 254);
        has_args = true;
    }
    strncpy(cmd, line, (READBUFF_SIZE-1));

    /* execute the cmd based on the current context */
    pos = 0;
    while (COMMANDS[pos].state != NONE) {
        if ((COMMANDS[pos].state == ctx->state) &&
                (strcmp(COMMANDS[pos].cmd, cmd) == 0) && 
                (COMMANDS[pos].has_args == has_args)) {
            return COMMANDS[pos].fh(ctx, args);
        }
        pos++;
    }
    return E_CMD_NOT_FOUND;
}

/*
 * Prints the current encoder values
 */
cmd_status
dsc_get_values(cli_ctx *ctx, const char *args) {
    char buff[36];
    char *value;
    int32_t ra, dec;

    ra = ctx->ra->read();
    dec = ctx->dec->read();

    value = EncoderValue(ra, true);
    sprintf(buff, "%s\t", value);
    value = EncoderValue(dec, true);
    strcat(buff, value);
    strcat(buff, "\r");
    ctx->serial->printf("%s\n", buff);
    return E_CMD_OK;
}

/*
 * Allows the user to set the Encoder resolution
 */
cmd_status
dsc_set_resolution(cli_ctx *ctx, const char *args) {
    int match = 0;
    long ra, dec;
    encoder_settings_t encoder_settings;

    match = sscanf(args, "%d %d", &ra, &dec);
    if (match != 2) {
        return E_CMD_TOO_SHORT;
    }

    ctx->common->ra_cps = ra;
    ctx->common->dec_cps = dec;

    /* write the values to EEPROM for later */
    encoder_settings.ra_cps = ctx->common->ra_cps;
    encoder_settings.dec_cps = ctx->common->dec_cps;
    set_encoder_settings(&encoder_settings);
    return E_CMD_OK;
}

/*
 * prints the current encoder resolution
 */
cmd_status
dsc_get_resolution(cli_ctx *ctx, const char *args) {
    long ra, dec;
    char ra_pos[2], dec_pos[2];

    ra = abs(ctx->common->ra_cps);
    dec = abs(ctx->common->dec_cps);

    ra_pos[1] = dec_pos[1] = NULL;
    ra_pos[0] = ra == ctx->common->ra_cps ? NULL : '-';
    dec_pos[0] = dec == ctx->common->dec_cps ? NULL : '-';

    ctx->serial->printf("%s%05ld %s%05ld\n", ra_pos, ra, dec_pos, dec);
    return E_CMD_OK;
}

/*
 * TeensyDSC software version
 */
cmd_status
dsc_get_version(cli_ctx *ctx, const char *args) {
    ctx->serial->printf("%s\n", TEENSY_DSC_VERSION); 
    return E_CMD_OK;
}

/*
 * Change the current CLI state
 */
cmd_status
change_cli_state(cli_ctx *ctx, const char *args) {
    int mode = 0;
    cli_state new_state = NONE;

    while (MODES[mode].state != NONE) {
        if (strcmp(MODES[mode].name, args) == 0) {
            new_state = MODES[mode].state;
        }
        mode ++;
    }

    if (new_state != NONE) {
        if (new_state != ctx->state) {

            // Don't let people reconfigure things over wireless
            if ((new_state == WIFI || new_state == CONFIG) &&
                    ctx->wifly == NULL) {
                ctx->serial->printf("Sorry, this is not available via wireless\n");
                return E_CMD_BAD_ARGS;
            }

            change_state(ctx, new_state);
        }
        return E_CMD_OK;
    }
    return E_CMD_BAD_ARGS;
}

cmd_status
dsc_get_help(cli_ctx *ctx, const char *args) {

    ctx->serial->printf(F("\n" \
"Q                      => get encoder values\n" \
"R xxxx xxxx            => set encoder resolution\n" \
"G                      => get encoder resolution\n" \
"V                      => get TeensyDSC version\n" \
"?                      => Help\n" \
"MODE [WIFI|CONFIG]     => change CLI mode\n"));

    return E_CMD_OK;
}

/*
 * in MODE WIFI we connect the user serial port to the WiFly's serial port
 * and let them talk to each other directly.  The only thing we look for is:
 *
 * MODE DSC 
 * MODE CONFIG
 *
 * to change modes & exit.  Note, if the user goes into command mode ($$$)
 * then they better 'exit' as well or things are gonna be hosed.
 */
void
wifi_interactive(cli_ctx *ctx) {
    char byte, line[READBUFF_SIZE];
    int pos = -1;

    delay(250);
    ctx->wifly->write((uint8_t *)"$$$", 3);
    delay(250);

    while (true) {
        if (ctx->serial->available()) {
            pos ++;
            byte = ctx->serial->read();
            line[pos] = byte;
            if (byte == '\r') {
                line[pos] = '\0';
                if (strcmp(line, "MODE DSC") == 0) {
                    ctx->wifly->write((uint8_t *)"exit\r", 5);
                    change_state(ctx, BASIC_DSC);
                    return;
                } else if (strcmp(line, "MODE CONFIG") == 0) {
                    ctx->wifly->write((uint8_t *)"exit\r", 5);
                    change_state(ctx, CONFIG);
                    return;
                }
                pos = -1;
            }
            ctx->wifly->write(byte);
        }

        if (ctx->wifly->available()) {
            byte = ctx->wifly->read();
            ctx->serial->write(byte);
        }
    }
}

/*
 * Call this everytime we change our cli state to find all
 * the one_char_cmds & our longest command
 */
void
setup_commands(cli_ctx *ctx) {
    uint i = 0, j = 0, longest = 0, len;

    bzero(ctx->one_char_cmds, sizeof(ctx->one_char_cmds));

    /*
     * Find all the 1 char commands & the longest command
     */
    while (COMMANDS[i].state != NONE) {
        if (COMMANDS[i].state == ctx->state) {
            len = strlen(COMMANDS[i].cmd);
            if (len == 1 && (! COMMANDS[i].has_args)) {
                ctx->one_char_cmds[j] = COMMANDS[i].cmd[0];
                j++;
            }
            longest = longest < len ? len : longest;
        }
        i++;
    }
    ctx->longest_cmd = longest;
}

/*
 * AP commands
 */
cmd_status
wifi_ap_commands(cli_ctx *ctx, const char *args) {
    if (strcmp(args, "SAVE") == 0) {
        rnxv_configure(ctx->wifly, ctx->common->network);
        ctx->serial->printf("OK\n");
    } else if (strcmp(args, "SHOW") == 0) {
        rnxv_get_config(ctx->wifly, ctx->serial);
        ctx->serial->printf("OK\n");
    } else if (strcmp(args, "RESET") == 0) {
        ctx->wifly->reset();
        ctx->serial->printf("OK\n");
    } else {
        return E_CMD_NOT_FOUND;
    }
    return E_CMD_OK;
} 

/*
 * CONFIG mode HELP
 */
cmd_status
wifi_get_help(cli_ctx *ctx, const char *args) {
    ctx->serial->printf(
F("AP SAVE               => Save WiFi access point config\n" \
"AP SHOW               => Shows the WiFi configuration\n"    \
"AP RESET              => Reset WiFi to factory defaults\n"  \
"SET SSID <ssid>       => Set the WiFi SSID\n"               \
"SET PASS <pass>       => Set the WiFi passphrase\n"         \
"SET IP <x.x.x.x>      => Set the WiFi IP address\n"         \
"SET MASK <x.x.x.x>    => Set the WiFi netmask\n"            \
"SET CHAN <x>          => Set the WiFi channel [0-12]\n"     \
"SET PORT <x>          => Set the TCP PORT [1-65535]\n"      \
"SET RATE <x>          => Set the WiFi rate [0-15]\n"        \
"SET TXP <x>           => Set the WiFi TX Power [0-12]\n"    \
"SET AUTH [WPA2|OPEN]  => Enable WPA2-PSK or no security\n"  \
"SET MODE [AP|CLIENT]  => Enable AP or client mode\n"        \
"SET ALT xxxx          => set ALT encoder resolution\n"      \
"SET AZ xxxx           => set AZ encoder resolution\n"       \
"GET <OPTION>          => Get a config option\n"             \
"GET IPA               => Get IP of WiFi AP\n"               \
"GET ALL               => Get all saved EEPROM settings\n"   \
"SAVE                  => Save settings to EEPROM\n"         \
"RESET                 => Reset all EEPROM settings\n"       \
"?                     => Help\n"                            \
"MODE [DSC|WIFI]       => Change CLI mode\n")
    );
    return E_CMD_OK;
}

cmd_status
wifi_set_option(cli_ctx *ctx, const char *args) {
    // 65 bytes is just big enough for the passphrase + \0
    char option[65];
    char value[65];
    int i, j;
    bool isOption = true;
    network_settings_t *network = ctx->common->network;
   
    // split the option & value from args[] 
    j = 0;
    for (i = 0; args[i] != '\0'; i++) {
        if (args[i] != ' ' && isOption) {
            option[j] = args[i];
            j++;
        } else if (args[i] == ' ') {
            option[j] = '\0';
            j = 0;
            isOption = false;
        } else {
            value[j] = args[i];
            j++;
        }
        if (j == 65) {
            // command too long 
            return E_CMD_TOO_LONG;
        }
    }
    value[j] = '\0';
    if (strcmp("SSID", option) == 0) {
        strncpy(network->ssid, value, 30);
        ctx->serial->printf("OK: %s\n", value);
    } else if (strcmp("PASS", option) == 0) {
        strncpy(network->passphrase, value, 64);
        ctx->serial->printf("OK: %s\n", value);
    } else if (strcmp("IP", option) == 0) {
        strncpy(network->ip_address, value, 15);
        ctx->serial->printf("OK: %s\n", value);
    } else if (strcmp("MASK", option) == 0) {
        strncpy(network->netmask, value, 15);
        ctx->serial->printf("OK: %s\n", value);
    } else if (strcmp("CHAN", option) == 0) {
        i = atoi(value);
        if (i < 0 || i > 12) {
            ctx->serial->printf(F("Invalid channel: %d\n"), i);
            return E_CMD_BAD_ARGS;
        }
        network->channel = (uint8_t)i;
        ctx->serial->printf("OK: %d\n", i);
    } else if (strcmp("PORT", option) == 0) {
        i = atoi(value);
        if (i < 1 || i > 65535) {
            ctx->serial->printf("Invalid port: %d\n", i);
            return E_CMD_BAD_ARGS;
        }
        network->port = (uint16_t)i;
        ctx->serial->printf("OK: %d\n", i);
    } else if (strcmp("AUTH", option) == 0) {
        if (strcmp("OPEN", value) == 0) {
            network->enable_wpa = 0;
            ctx->serial->printf("OK: %d\n", 0);
        } else if (strcmp("WPA2", value) == 0) {
            network->enable_wpa = 4;
        } else {
            ctx->serial->printf(F("Invalid auth mode: %s\n"), value);
            return E_CMD_BAD_ARGS;
        }
    } else if (strcmp("MODE", option) == 0) {
        if (strcmp("AP", value) == 0) {
            network->enable_ap = 1;
            ctx->serial->printf("OK: %d\n", 7);
        } else if (strcmp("CLIENT", value) == 0) {
            network->enable_ap = 1;
            ctx->serial->printf("OK: %d\n", 1);
        } else {
            ctx->serial->printf(F("Invalid Wifi mode: %d\n"), value);
            return E_CMD_BAD_ARGS;
        }
    } else if (strcmp("RATE", option) == 0) {
        i = atoi(value);
        if (i < 0 || i > 15) {
            ctx->serial-printf("Invalid rate: %d\n", i);
            return E_CMD_BAD_ARGS;
        }
        network->rate = i;
        ctx->serial->printf("OK: %d\n", i);
    } else if (strcmp("TXP", option) == 0) {
        i = atoi(value);
        if (i < 0 || 0 > 12) {
            ctx->serial-printf("Invalid TX Power: %d\n", i);
            return E_CMD_BAD_ARGS;
        }
        network->tx_power = i;
        ctx->serial->printf("OK: %d\n", i);
    } else if (strcmp("ALT", option) == 0) {
        i = atoi(value);
        ctx->common->dec_cps = i;
        ctx->serial->printf("OK: %d\n", i);
    } else if (strcmp("AZ", option) == 0) {
        i = atoi(value);
        ctx->common->ra_cps = i;
        ctx->serial->printf("OK: %d\n", i);
    } else if (strcmp("DEBUG", option) == 0) {
        i = atoi(value);
        ctx->common->network->debug_wifly = i;
        ctx->serial->printf(F("OK: %d (please save & reboot!)\n"));
    } else {
        ctx->serial->printf(F("Unknown command: %s\n"), option);
        return E_CMD_NOT_FOUND;
    }
    return E_CMD_OK;
}

/*
 * Get a config option
 */
cmd_status 
wifi_get_option(cli_ctx *ctx, const char *args) {
    AnySerial *serial = ctx->serial;
    network_settings_t *network = ctx->common->network;
    char c, *value;

    if (strcmp("SSID", args) == 0) {
        serial->printf("SSID: %s\n", network->ssid);
    } else if (strcmp("PASS", args) == 0) {
        serial->printf("PASS: %s\n", network->passphrase);
    } else if (strcmp("IP", args) == 0) {
        serial->printf("IP: %s\n", network->ip_address);
    } else if (strcmp("MASK", args) == 0) {
        serial->printf("MASK: %s\n", network->netmask);
    } else if (strcmp("CHAN", args) == 0) {
        serial->printf("CHAN: %d\n", network->channel);
    } else if (strcmp("PORT", args) == 0) {
        serial->printf("PORT: %d\n", network->port);
    } else if (strcmp("AUTH", args) == 0) {
        switch (network->enable_wpa) {
            case 0:
                serial->printf(F("AUTH: OPEN\n"));
                break;
            case 4:
                serial->printf(F("AUTH: WPA2\n"));
                break;
            default:
                serial->printf(F("AUTH: Unknown value: %d\n"), network->enable_wpa);
        }
    } else if (strcmp("MODE", args) == 0) {
        switch (network->enable_ap) {
            case 7:
                serial->printf("MODE: AP\n");
                break;
            case 1:
                serial->printf(F("MODE: WiFi Client\n"));
                break;
            default:
                serial->printf(F("Mode: Unknown: %d\n"), network->enable_ap);
        }
    } else if (strcmp("RATE", args) == 0) {
        serial->printf("RATE: %d\n", network->rate);
    } else if (strcmp("TXP", args) == 0) {
        serial->printf("TXP: %d\n", network->tx_power);
    } else if (strcmp("IPA", args) == 0) {
        ctx->wifly->sendCommand("get ip a\r");
        serial->print("IPA: ");
        while (ctx->wifly->receive((uint8_t *)&c, 1, 300)) {
            serial->print((char)c);
        }
        ctx->wifly->dataMode();
    } else if (strcmp("ALT", args) == 0) {
        serial->printf("ALT/Dec: %ld\n", ctx->common->dec_cps);
    } else if (strcmp("AZ", args) == 0) {
        serial->printf("AZ/RA: %ld\n", ctx->common->ra_cps);
    } else if (strcmp("DEBUG", args) == 0) {
        if (network->debug_wifly) {
            serial->printf(F("Debuging: enabled\n"));
        } else {
            serial->printf(F("Debuging: disabled\n"));
        }
    } else if (strcmp("ALL", args) == 0) {
        serial->printf("SSID: %s\n", network->ssid);
        serial->printf("PASS: %s\n", network->passphrase);
        serial->printf("IP: %s\n", network->ip_address);
        serial->printf("MASK: %s\n", network->netmask);
        serial->printf("CHAN: %d\n", network->channel);
        serial->printf("PORT: %d\n", network->port);
        switch (network->enable_wpa) {
            case 0:
                serial->printf(F("AUTH: OPEN\n"));
                break;
            case 4:
                serial->printf(F("AUTH: WPA2\n"));
                break;
            default:
                serial->printf(F("AUTH: Unknown value: %d\n"), network->enable_wpa);
        }
        switch (network->enable_ap) {
            case 7:
                serial->printf("MODE: AP\n");
                break;
            case 1:
                serial->printf(F("MODE: WiFi Client\n"));
                break;
            default:
                serial->printf(F("Mode: Unknown: %d\n"), network->enable_ap);
        }
        serial->printf("RATE: %d\n", network->rate);
        serial->printf("TXP: %d\n", network->tx_power);
        serial->printf("ALT/Dec: %ld\n", ctx->common->dec_cps);
        serial->printf("AZ/RA: %ld\n", ctx->common->ra_cps);
    }

    return E_CMD_OK;
}

/*
 * Save Wifi Settings to EEPROM
 */
cmd_status 
wifi_save_settings(cli_ctx *ctx, const char *args) {
    set_network_settings(ctx->common->network);
    ctx->serial->printf("OK\n");
    return E_CMD_OK;
}

/*
 * Reset settings to our defaults in the EEPROM
 */
cmd_status
eeprom_reset_settings(cli_ctx *ctx, const char *args) {
    reset_all_defaults();
    ctx->serial->printf("OK\n");
    return E_CMD_OK;
}
