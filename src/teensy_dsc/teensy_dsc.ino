/*
 * Teensy 3.1 Digital Setting Circles
 *
 * Copyright (c) 2014 Aaron Turner
 */

#include <Streaming.h>
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include <AltSoftSerial.h>
#include <AnySerial.h>
#include <WiFlySerial.h>

#include "defaults.h"
#include "teensy_dsc.h"
#include "wifly.h"
#include "utils.h"

Encoder EncoderRA(CHAN_A_RA, CHAN_B_RA);
Encoder EncoderDEC(CHAN_A_DEC, CHAN_B_DEC);
AnySerial WiFlySerialPort(Serial3);
WiFlySerial WiFly(WiFlySerialPort);

long ra_value, dec_value;
long ra_cps, dec_cps;

void
setup() {
    pinMode(WIFLY_RESET, INPUT);
    Serial.begin(9600);  // USB Serial
    Serial.setTimeout(USER_TIMEOUT);

    // It takes a while for the XBee radio to initialize
    delay(WIFLY_DELAY);
}

void
loop() {
    if (Serial.available() > 0) {
        ra_value = EncoderRA.read();
        dec_value = EncoderDEC.read();
        process_user_cmd();
    }
    if (SerialWiFly.available() > 0) {
        ra_value = EncoderRA.read();
        dec_value = EncoderDEC.read();
        process_dsc_cmd(ra_value, dec_value);
    }
}

void
process_user_cmd() {
    char cmd;
    char *buff;
    char read_buff[255];

    cmd = Serial.read();
    if (cmd == 'Q') {
        buff = EncoderValue(ra_value, true);
        Serial.print(buff);
        Serial.print("\t");
        buff = EncoderValue(dec_value, true);
        Serial.print(buff);
        Serial.print("\r");
    } else if (cmd == 'R') {
        Serial.readBytesUntil('\r', read_buff, 255);
        sscanf(read_buff, "%ld %ld", &ra_cps, &dec_cps);
    } else {
        WiFly.print("ERR\r");
    }



}

void
process_dsc_cmd(long ra_value, long dec_value) {
    char cmd;
    char *buff;
    char read_buff[255];

    cmd = Serial2.read();
    if (cmd == 'Q') {
        buff = EncoderValue(ra_value, true);
        Serial2.print(buff);
        Serial2.print("\t");
        buff = EncoderValue(dec_value, true);
        Serial2.print(buff);
        Serial2.print("\r");
    } else if (cmd == 'R') {
        Serial2.readBytesUntil('\r', read_buff, 255);
        sscanf(read_buff, "%ld %ld", &ra_cps, &dec_cps);
    } else {
        Serial2.print("ERR\r");
    }
}

char *
EncoderValue(long value, bool lead) {
    static char buff[BUFF_LEN];
    char sign;
    unsigned long a_value;
    int buff_pos = 0;

    if (lead) {
        if (value < 0) {
            sign = '-';
        } else {
            sign = '+';
        }
        buff_pos = 1;
    }

    a_value = abs(value);
    if (lead) {
        sprintf(buff, "%c%05lu", sign, a_value);
    } else {
        sprintf(buff, "%05lu", a_value);
    }
    return buff;
}


/*
 * Resets the WiFly
 */
void
reset_wifly() {
    Serial.write("\nResetting the Xbee...  ");
    pinMode(WIFLY_RESET, OUTPUT);
    digitalWrite(WIFLY_RESET, LOW);
    delay(100);
    pinMode(WIFLY_RESET, INPUT);
    Serial.write("Done!\n");
}
