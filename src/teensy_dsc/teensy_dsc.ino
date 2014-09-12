/*
 * Teensy 3.1 Digital Setting Circles
 *
 * Copyright (c) 2014 Aaron Turner
 */

#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include <Xbee.h>

Encoder EncoderRA(CHAN_A_RA, CHAN_B_RA);
Encoder EncoderDEC(CHAN_A_DEC, CHAN_B_DEC);
WiFlySerial WiFly(XBEE_RX, XBEE_TX);

long ra_value, dec_value;
long ra_cps, dec_cps;

void
setup() {
    pinMode(XBEE_RESET, INPUT);
    Serial.begin(9600);  // USB Serial
    Serial.setTimeout(USER_TIMEOUT);
    Serial2.begin(XBEE_SERIAL_SPEED); // Xbee Serial 
    Serial2.setTimeout(XBEE_TIMEOUT);
    Xbee.begin(Serial2);

    // It takes a while for the XBee radio to initialize
    sleep(XBEE_DELAY);
}

void
loop() {
    if (Serial.available() > 0) {
        ra_value = EncoderRA.read();
        dec_value = EncoderDEC.read();
        process_user_cmd();
    }
    if (Serial2.available() > 0) {
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
        buff = printEncoderValue(dec_value, true);
        Serial.print(buff);
        Serial.print("\r");
    } else if (cmd == 'R') {
        Serial.readBytesUntil('\r', read_buff, 255);
        sscanf(read_buff, "%ld %ld", &ra_cps, &dec_cps);
    } else {
        Serial2.print("ERR\r");
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
        sprintf(buff, "%c%05d", sign, a_value);
    } else {
        sprintf(buff, "%05d", a_value);
    }
    return &buff;
}


/*
 * Resets the Xbee
 */
void
reset_xbee() {
    serial_printf("\nResetting the Xbee...");
    pinMode(XBEE_RESET, OUTPUT);
    digitalWrite(XBEE_RESET, LOW);
    sleep(100);
    pinMode(XBEE_RESET, INPUT);
    serial_printf("Done!\n");
}


/*
 * printf to the HW serial port, useful for debugging.  Note 128char limit!
 */
void 
serial_printf(const char *fmt, ...) {
    char tmp[128]; // resulting string limited to 128 chars
    va_list args;
    va_start(args, fmt);
    vsnprintf(tmp, 128, fmt, args);
    va_end(args);
    Serial.print(tmp);
}

/*
 * printf to HW serial port, if we have debugging enabled
 */
void
dbg_serial_printf(const char *fmt, ...) {
#ifdef DEBUG
    char tmp[128]; // resulting string limited to 128 chars
    va_list args;
    va_start(args, fmt);
    vsnprintf(tmp, 128, fmt, args);
    va_end(args);
    Serial.print(tmp);
#else
    return;
#endif
}
