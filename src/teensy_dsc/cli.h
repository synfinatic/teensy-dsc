#ifndef __CLI_H__
#define __CLI_H__

typedef enum {
    BASIC_DSC = 0,  // whatever is first, is the default
    WIFI,
    CONFIG,
    NONE
} cli_state;

typedef enum {
    E_CMD_OK = 0,
    E_CMD_NOT_FOUND,
    E_CMD_TOO_SHORT,
    E_CMD_EMPTY
} cmd_status;

typedef struct {
    cli_state state;
    AnySerial *serial;
    // Encoder values & resolution
    long ra_value, dec_value;
    long ra_cps, dec_cps;
} cli_ctx;

typedef struct {
    cli_state state;
    const char *cmd;
    void (*fh)(cli_ctx *, const char *args);
} cmd_def;

/* init & entrance */
cli_ctx *cli_init_cmd(AnySerial *aserial);
int cli_proc_cmd(cli_ctx *ctx, char *line);

/* actual command hooks */
void dsc_set_resolution(cli_ctx *ctx, const char *args);
void dsc_get_values(cli_ctx *ctx, const char *args);
void dsc_get_version(cli_ctx *ctx, const char *args);

/* all our supported commands */
static const cmd_def COMMANDS[] = {
    { BASIC_DSC, "Q", dsc_get_values },
    { BASIC_DSC, "R", dsc_set_resolution },
    { BASIC_DSC, "V", dsc_get_version },
    { NONE, NULL, NULL }
};

#endif
