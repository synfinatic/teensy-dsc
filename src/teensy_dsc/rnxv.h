#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <Flash.h>
#include <AnySerial.h>
#include <WiFly.h>
#include "defaults.h"

#define PAUSE_DURATION 100
#define LONG_PAUSE_DURATION 500

#define PORT_OPEN "*port opened*"
#define PORT_CLOSE "*port closed*"
#define COMM_SIZE 1420
#define COMM_TIME_MS 5

void rnxv_configure(WiFly *, network_settings_t *);
void rnxv_get_config(WiFly *, AnySerial *);

#endif
