#include <Arduino.h>
#include <AnySerial.h>
#include "teensy_dsc.h"
#include "cli.h"
#include "utils.h"

cli_ctx *
cli_init_cmd(AnySerial *aserial) {
    cli_ctx *ctx;
    ctx = (cli_ctx *)malloc(sizeof(cli_ctx));
    bzero(ctx, sizeof(cli_ctx));
    ctx->state = BASIC_DSC;
    ctx->serial = aserial;
    return ctx;
}

int
cli_proc_cmd(cli_ctx *ctx, char *line) {
    char cmd[MAX_GETWORD_SIZE];
    char args[255], byte, *line2;
    uint16_t pos = 0;

    byte = line[0];
    while (! IS_WORD_END(byte)) {
        byte = line[++pos];
    }
    line[pos] = '\0';
    strcpy(cmd, line);
    line2 = &(line[pos+1]);
    strcpy(args, line2);

    pos = 0;
    while (COMMANDS[pos].state != NONE) {
        if (COMMANDS[pos].state == ctx->state) {
            if (strcmp(COMMANDS[pos].cmd, cmd) == 0) {
                COMMANDS[pos].fh(ctx, args);
            }
        }
    }
}

void
dsc_get_values(cli_ctx *ctx, const char *args) {
    char buff[36];
    char *value;

    value = EncoderValue(ctx->ra_value, true);
    sprintf(buff, "%s\t", value);
    value = EncoderValue(ctx->dec_value, true);
    strcat(buff, value);
    strcat(buff, "\r");
    ctx->serial->printf("%s\n", buff);
}

void
dsc_set_resolution(cli_ctx *ctx, const char *args) {
    sscanf(args, "%ld %ld", &(ctx->ra_cps), &(ctx->dec_cps));
    ctx->serial->printf("New resolution: %ld %ld\n", ctx->ra_cps, ctx->dec_cps);
}

void
dsc_get_version(cli_ctx *ctx, const char *args) {
    ctx->serial->printf(TEENSY_DSC_VERSION "\n"); 
}
