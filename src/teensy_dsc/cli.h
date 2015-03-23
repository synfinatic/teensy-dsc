#include "rnxv.h"
#include <Encoder.h>

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
    volatile int32_t ra_value, dec_value;
    int32_t ra_cps, dec_cps;
    network_settings_t *network;
} common_cli_ctx;

/*
 * This values are per-Serial port connection
 */
typedef struct cli_ctx_s {
    cli_state state;
    cli_state prev_state;
    AnySerial *serial;
    WiFly *wifly;
    // must be big enough to hold max one char commands for a given state
    char one_char_cmds[20]; 
    int longest_cmd;
    bool eat_errors;
    common_cli_ctx *common;
    Encoder *ra;
    Encoder *dec;
} cli_ctx;

/* init & entrance */
cli_ctx *cli_init_cmd(AnySerial *, common_cli_ctx *, WiFly *,
        Encoder *, Encoder *);
cmd_status cli_proc_cmd(cli_ctx *, char *, size_t);

/* dsc commands */
cmd_status dsc_set_resolution(cli_ctx *, const char *);
cmd_status dsc_get_resolution(cli_ctx *, const char *);
cmd_status dsc_get_values(cli_ctx *, const char *);
cmd_status dsc_get_version(cli_ctx *, const char *);
cmd_status dsc_get_help(cli_ctx *, const char *);
cmd_status change_cli_state(cli_ctx *, const char *);
cmd_status bbx_set_resolution(cli_ctx *, const char *);
cmd_status bbx_get_status(cli_ctx *, const char *);

/* wifi mode */
void wifi_interactive(cli_ctx *);

/* config commands */
cmd_status wifi_ap_commands(cli_ctx *, const char *);
cmd_status wifi_get_help(cli_ctx *, const char *);
cmd_status wifi_get_option(cli_ctx *, const char *);
cmd_status wifi_set_option(cli_ctx *, const char *);
cmd_status wifi_save_settings(cli_ctx *, const char *);
cmd_status eeprom_reset_settings(cli_ctx *, const char *);

/* map our supported commands */
typedef struct {
    cli_state state;
    const char *cmd;
    bool has_args;
    cmd_status (*fh)(cli_ctx *, const char *args);
} cmd_def;

static const cmd_def COMMANDS[] = {
    { BASIC_DSC , "Q"     , false , dsc_get_values     } , 
    { BASIC_DSC , "R"     , true  , dsc_set_resolution } , 
    { BASIC_DSC , "Z"     , true  , bbx_set_resolution } , 
    { BASIC_DSC , "P"     , false , bbx_get_status     } , 
    { BASIC_DSC , "G"     , false , dsc_get_resolution } , 
    { BASIC_DSC , "H"     , false , dsc_get_resolution } , 
    { BASIC_DSC , "V"     , false , dsc_get_version    } , 
    { BASIC_DSC , "?"     , false , dsc_get_help       } , 
    { BASIC_DSC , "MODE"  , true  , change_cli_state   } , 
    { WIFI      , "MODE"  , true  , change_cli_state   } , 
    { CONFIG    , "MODE"  , true  , change_cli_state   } ,
    { CONFIG    , "AP"    , true  , wifi_ap_commands   } , 
    { CONFIG    , "?"     , false , wifi_get_help      } ,
    { CONFIG    , "SET"   , true  , wifi_set_option    } ,
    { CONFIG    , "GET"   , true  , wifi_get_option    } ,
    { CONFIG    , "SAVE"  , false , wifi_save_settings } ,
    { CONFIG    , "RESET" , false , eeprom_reset_settings },
    /*
    { CONFIG    , "SETUP" , false , wifi_initial_setup } , 
    { CONFIG    , "SAVE"  , false , wifi_save_settings } , 
    { CONFIG    , "SHOW"  , false , wifi_get_config    } , 
    { CONFIG    , "SET"   , true  , wifi_set_option    } , 
    { CONFIG    , "GET"   , true  , wifi_get_option    } , 
    */
    { NONE      , NULL    , false , NULL }
};



#endif
