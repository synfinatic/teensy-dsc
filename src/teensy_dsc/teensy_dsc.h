#ifndef __TEENSY_DSC_H__
#define __TEENSY_DSC_H__

#define DEBUG 1         // Enable printing out debug info

/* WiFly connections */
#define WIFLY_SERIAL_PORT Serial3
#define WIFLY_SERIAL_SPEED 115200
#define WIFLY_DELAY 5000
#define WIFLY_RESET 14    // Pin to reset the WiFly

/* Serial port for debugging */
#define USER_SERIAL_PORT Serial
#define USER_SERIAL_BAUD 9600

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

#define TEENSY_DSC_VERSION "0.2"

/* poll the encoder values every 100ms */
#define UPDATE_ENCODER_MS 100

#endif // __TEENSY_DSC_H__
