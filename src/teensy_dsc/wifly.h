#ifndef __WIFLY_H__
#define __WIFLY_H__

#define REQUEST_BUFFER_SIZE 120
#define PAUSE "pause"
#define PAUSE_DURATION 100
#define LONG_PAUSE "longpause"
#define LONG_PAUSE_DURATION 500
#define SSID_KEY "SSID:"
#define DONE "done"

void wifi_execute_commands(char ** commands);
void wifi_setup();

#endif
