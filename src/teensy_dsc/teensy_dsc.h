#ifndef __TEENSY_DSC_H__
#define __TEENSY_DSC_H__

/*
 * Our Serial ports
 */

#define SerialWiFly Serial3
#define SerialA Serial1
#define SerialB Serial2


#define CHAN_A_RA  6
#define CHAN_B_RA  5
#define CHAN_A_DEC 4
#define CHAN_B_DEC 3

#define WIFLY_RESET 2
#define WIFLY_DELAY 5000
#define WIFLY_SERIAL_SPEED 115200
#define DEBUG 1

#define BUFF_LEN 10
#define USER_TIMEOUT 5000 // 5sec
#define WIFLY_TIMEOUT 500  // 0.5sec
#endif
