#include "teensy_dsc.h"
#include "wifly.h"

/*
 * Initialize the WiFly board
 */
void 
wifi_initial_setup(WiFlySerial *WiFly) {
    char command[50], cmd[25];
    const char *arg;
    size_t i;

    WiFly->getDeviceStatus();

    for (i = 0; i < wifisetup_cmds.count(); i++) {
	wifisetup_cmds[i].copy(cmd, 24, 0);
	arg = wifisetup_args[i];
        if (0 == strcmp(command, LONG_PAUSE)) {
            delay(LONG_PAUSE_DURATION);
        } else if (0 == strcmp(command, PAUSE)) {
            delay(PAUSE_DURATION);
        } else {
	    sprintf(command, cmd, arg);
            WiFly->SendCommandSimple(command, "AOK");
        }
    }

    WiFly->closeConnection();

    WiFly->flush();
    while (WiFly->available()) {
        WiFly->read();
    }
}

