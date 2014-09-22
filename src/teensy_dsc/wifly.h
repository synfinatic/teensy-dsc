#ifndef __WIFLY_H__
#define __WIFLY_H__

#include <Flash.h>
#include <WiFlySerial.h>
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
void wifi_get_config(WiFlySerial *, AnySerial *);

#endif
