#ifndef __WIFLY_H__
#define __WIFLY_H__

#include <Flash.h>
#include "defaults.h"

#define PAUSE "_PAUSE_"
#define PAUSE_DURATION 100
#define LONG_PAUSE "_LONG_PAUSE_"
#define LONG_PAUSE_DURATION 500

#define PORT_OPEN "*port opened*"
#define PORT_CLOSE "*port closed*"
#define COMM_SIZE "1420"
#define COMM_TIME_MS "5"

typedef struct {
    char ip_address[16];
    char port[6];
    char netmask[16];
    char ssid[30];
    char channel[3];
    char baudrate[10];
    char passphrase[30];
} wifi_conf_t;

void wifi_initial_setup(WiFlySerial *);


FLASH_STRING_ARRAY(wifisetup_cmds,
	PSTR("factory RESET"),
	PSTR(LONG_PAUSE),
	PSTR("set uart mode 0x13"),
	PSTR("set comm close %s"),
	PSTR("set comm open %s"),
	PSTR("set comm remote 0"),
	PSTR("set comm size %s"),
	PSTR("set comm time %s"),
#ifdef ENABLE_WPA
	PSTR("set wlan phrase %s"),
	PSTR("set wlan auth 4"),
#endif
	PSTR("set wlan rate 0"),
	PSTR("set wlan ssid %s"),
	PSTR("set wlan tx %s"),
	PSTR("set wlan channel %s"),
	PSTR("set wlan rate %s"),
	PSTR("set wlan join 7"),
	PSTR("set dhcp lease 2000"),
	PSTR("set ap link_monitor 0"),
	PSTR("set ip address %s"),
	PSTR("set ip netmask %s"),
	PSTR("set ip gateway %s"),
	PSTR("set ip localport %s"),
	PSTR("set dns address %s"),
	PSTR("set ip protocol 2"),
	PSTR("set ip dhcp 4"),
	PSTR("save"),
	PSTR(PAUSE),
	PSTR("reboot"),
	PSTR(LONG_PAUSE),
);

const char *wifisetup_args[] = {
    "",
    "",
    "",
    PORT_CLOSE, 
    PORT_OPEN,
    "",
    COMM_SIZE,
    COMM_TIME_MS,
#ifdef ENABLE_WPA
    WPA_PASSWORD,
    "",
#endif
    "",
    SSID,
    TX_POWER,
    WIFLY_CHANNEL,
    WIFLY_RATE,
    "",
    "",
    "",
    IP_ADDRESS,
    IP_ADDRESS,
    PORT,
    IP_ADDRESS,
    "",
    NETMASK,
    "",
    "",
    "",
    "",
    "",
    "",
};

#endif
