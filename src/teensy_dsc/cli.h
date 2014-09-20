#ifndef __CLI_H__
#define __CLI_H__

typedef enum {
    BASIC_DSC = 0,  // whatever is first, is the default
    WIFI,
    CONFIG,
    NONE
} cli_state;

typedef struct {
    cli_state state;
    char name[10];
} mode_def;

static const mode_def MODES[] = {
    { BASIC_DSC , "DSC" }    , 
    { WIFI      , "WIFI" }   , 
    { CONFIG    , "CONFIG" } , 
    { NONE      , "" }
};

typedef enum {
    E_CMD_OK = 0,
    E_CMD_NOT_FOUND,
    E_CMD_TOO_SHORT,
    E_CMD_TOO_LONG,
    E_CMD_BAD_ARGS
} cmd_status;

/*
 * These values are global
 */
typedef struct {
    // Encoder values & resolution
    long ra_value, dec_value;
    long ra_cps, dec_cps;
} common_cli_ctx;

/*
 * This values are per-Serial port connection
 */
typedef struct {
    cli_state state;
    cli_state prev_state;
    AnySerial *serial;
    // must be big enough to hold max one char commands for a given state
    char one_char_cmds[20]; 
    int longest_cmd;
    common_cli_ctx *common;
} cli_ctx;

/* init & entrance */
cli_ctx *cli_init_cmd(AnySerial *aserial, common_cli_ctx **common);
cmd_status cli_proc_cmd(cli_ctx *ctx, char *line, size_t len);

/* command hooks */
cmd_status dsc_set_resolution(cli_ctx *ctx, const char *args);
cmd_status dsc_get_resolution(cli_ctx *ctx, const char *args);
cmd_status dsc_get_values(cli_ctx *ctx, const char *args);
cmd_status dsc_get_version(cli_ctx *ctx, const char *args);
cmd_status dsc_get_help(cli_ctx *ctx, const char *args);
cmd_status change_cli_state(cli_ctx *ctx, const char *args);

/* map our supported commands */
typedef struct {
    cli_state state;
    const char *cmd;
    bool has_args;
    cmd_status (*fh)(cli_ctx *, const char *args);
} cmd_def;

static const cmd_def COMMANDS[] = {
    { BASIC_DSC , "Q"    , false , dsc_get_values     } , 
    { BASIC_DSC , "R"    , true  , dsc_set_resolution } , 
    { BASIC_DSC , "G"    , false , dsc_get_resolution } , 
    { BASIC_DSC , "V"    , false , dsc_get_version    } , 
    { BASIC_DSC , "?"    , false , dsc_get_help       } ,
    { BASIC_DSC , "MODE" , true  , change_cli_state   } , 
    { WIFI      , "MODE" , true  , change_cli_state   } , 
    { CONFIG    , "MODE" , true  , change_cli_state   } ,
    { NONE      , NULL   , false , NULL }
};



#endif
