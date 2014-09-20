#ifndef __WIFLY_H__
#define __WIFLY_H__

#include <Flash.h>
#include "defaults.h"

#define PAUSE_DURATION 100
#define LONG_PAUSE_DURATION 500

#define PORT_OPEN "*port opened*"
#define PORT_CLOSE "*port closed*"
#define COMM_SIZE 1420
#define COMM_TIME_MS 5

void wifi_initial_setup(WiFlySerial *, network_settings_t *);
void wifi_setup_comms(WiFlySerial *);
void wifi_setup_network(WiFlySerial *, network_settings_t *);
void wifi_setup_wireless(WiFlySerial *, network_settings_t *);

FLASH_STRING(wifi_comms_cmds,
	"set uart mode 0x13\r"
	"set comm close " PORT_CLOSE "\r"
	"set comm open " PORT_OPEN "\r"
	"set comm remote 0\r"
	"set comm size %d\r"
	"set comm time %d\r"
	);

#endif
