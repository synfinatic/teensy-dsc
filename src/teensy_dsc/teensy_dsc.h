#ifndef __TEENSY_DSC_H__
#define __TEENSY_DSC_H__

#define DEBUG 1         // Enable printing out debug info

/* WiFly connections */
#define WIFLY_SERIAL_PORT Serial3
#define WIFLY_SERIAL_SPEED 9600
#define WIFLY_DELAY 3000
#define WIFLY_RESET 2    // Pin to reset the WiFly

/* Serial port for debugging */
#define USER_SERIAL_PORT Serial
#define USER_SERIAL_SPEED 9600

/* RJ11 Serial Ports */
#define SerialA Serial1  // J1
#define SerialB Serial2  // J2

/* Encoder pins on RJ45 */
#define CHAN_A_RA  6     // RJ45 pin 3
#define CHAN_B_RA  5     // RJ45 pin 1
#define CHAN_A_DEC 4     // RJ45 pin 7
#define CHAN_B_DEC 3     // RJ45 pin 6

/* Camera Shutter pins */
#define FOCUS 20 
#define SHUTTER 21

/* Audio Jack Switch pins */
#define SW1 22
#define SW2 23

/* Misc settings */
#define BUFF_LEN 10
#define USER_TIMEOUT 5000 // 5sec
#define WIFLY_TIMEOUT 500  // 0.5sec
#define READBUFF_SIZE 255

#define TEENSY_DSC_VERSION "0.4"

/* Blink the onboard LED every 1/4 sec */
#define BLINK_LED_MS 250 
#define BLINK_LED 13 // pin 13

/* How often to print the values in test mode */
#define TEST_MODE_DELAY 100  // msec

#endif // __TEENSY_DSC_H__
