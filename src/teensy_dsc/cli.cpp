#include <Arduino.h>
#include <AnySerial.h>
#include "teensy_dsc.h"
#include "cli.h"
#include "utils.h"
#include "defaults.h"


void
setup_commands(cli_ctx *ctx) {
    uint i = 0, j = 0, longest = 0, len;

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

cli_ctx *
cli_init_cmd(AnySerial *aserial, common_cli_ctx **common) {
    cli_ctx *ctx;
    uint i = 0, j = 0;

    ctx = (cli_ctx *)malloc(sizeof(cli_ctx));
    bzero(ctx, sizeof(cli_ctx));
    ctx->common = *common;
    ctx->state = BASIC_DSC;
    ctx->prev_state = NONE;
    ctx->serial = aserial;

    setup_commands(ctx);

    return ctx;
}

/*
 * Run this every time we change CLI state
 * to update our CTX
 */
void
change_state(cli_ctx *ctx, cli_state new_state) {
    int i = 0, j = 0, longest = 0, len;

    bzero(ctx->one_char_cmds, sizeof(ctx->one_char_cmds));
    ctx->prev_state = ctx->state;
    ctx->state = new_state;

    setup_commands(ctx);
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

    value = EncoderValue(ctx->common->ra_value, true);
    sprintf(buff, "%s\t", value);
    value = EncoderValue(ctx->common->dec_value, true);
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
    ctx->serial->printf("%ld %ld\r", ctx->common->ra_cps, ctx->common->dec_cps);
    return E_CMD_OK;
}

/*
 * TeensyDSC software version
 */
cmd_status
dsc_get_version(cli_ctx *ctx, const char *args) {
    ctx->serial->printf(TEENSY_DSC_VERSION "\n"); 
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
    }

    if (new_state != NONE) {
        if (new_state != ctx->state) {
            change_state(ctx, new_state);
        }
        return E_CMD_OK;
    }
    return E_CMD_BAD_ARGS;
}

cmd_status
dsc_get_help(cli_ctx *ctx, const char *args) {

    ctx->serial->printf(F(\
"Q                      => get encoder values\n" \
"R xxxx xxxx            => set encoder resolution\n" \
"G                      => get encoder resolution\n" \
"V                      => get TeensyDSC version\n" \
"?                      => Help\n" \
"MODE [DSC|WIFI|CONFIG] => change CLI mode\n"));

    return E_CMD_OK;
}
