#include "teensy_dsc.h"
#include "wifly.h"

/*
 * Initialize the WiFly board
 */
void 
wifi_setup() {
    char * commands[] = {
        "factory RESET",
        LONG_PAUSE,
        "set uart mode 0x03",
        "set uart baudrate 115200",
        "set wlan join 7",
        SSID_KEY SSID,
        "set dhcp lease 2000",
        "set ip address " IP_ADDRESS,
        "set ip gateway " IP_ADDRESS,
        "set ip localport " PORT,
        "set dns address " IP_ADDRESS,
        "set ip protocol 2",
        "set ip netmask " NETMASK,
        "set ip dhcp 4",
        "set wlan tx 1",
        "set sys iofunc 0x7",
        "set comm remote 0",
        "set comm open 0",
        "save",
        PAUSE, 
        "reboot",
        LONG_PAUSE,
        DONE
    };

    WiFly.getDeviceStatus();
    wifi_execute_commands(commands);
    WiFly.closeConnection();

    WiFly.flush();
    while(WiFly.available()) {
        WiFly.read();
    }
}


/*
 * Executes a list of commands.
 * The last entry needs to be DONE
 */
void 
wifi_execute_commands(char * commands[]) {
    int cmd = 0;
    char * command = commands[cmd++];
    char bufRequest[REQUEST_BUFFER_SIZE];

    while (0 != strcmp(command, DONE)) {
        if (0 == strncmp(command, SSID_KEY, strlen(SSID_KEY))) {
            WiFly.getMAC(bufRequest, REQUEST_BUFFER_SIZE);
            if (*bufRequest) {
                char ssid_command[100];
                sprintf(ssid_command, "set wlan ssid %s-%s", command + strlen(SSID_KEY), bufRequest + strlen(bufRequest)-2);
                WiFly.SendCommandSimple(ssid_command, "AOK");
            }
        }
        if (0 == strcmp(command, LONG_PAUSE)) {
            delay(LONG_PAUSE_DURATION);
        } else if (0 == strcmp(command, PAUSE)) {
            delay(PAUSE_DURATION);
        } else {
            WiFly.SendCommandSimple(command, "AOK");
        }
        command = commands[cmd++];
    }
}

