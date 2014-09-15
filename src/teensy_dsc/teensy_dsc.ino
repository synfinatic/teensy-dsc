/*
 * Teensy 3.1 Digital Setting Circles
 * Copyright (C) 2014 Aaron Turner
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include <Arduino.h>
#include <Streaming.h>
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include <AnySerial.h>
#include <WiFlySerial.h>

#include "defaults.h"
#include "teensy_dsc.h"
#include "wifly.h"
#include "utils.h"
#include "cli.h"

/*
 * Init our Encoders & Serial ports
 */
Encoder EncoderRA(CHAN_A_RA, CHAN_B_RA);
Encoder EncoderDEC(CHAN_A_DEC, CHAN_B_DEC);
AnySerial UserSerial;

AnySerial WiFlySerialPort(&SerialWiFly);
WiFlySerial WiFly(WiFlySerialPort);
cli_ctx *cctx;

void
setup() {
    pinMode(WIFLY_RESET, INPUT);

    // We should be reading the EEPROM config here
    UserSerial.attach(&SerialDBG);
    cctx = cli_init_cmd(&UserSerial);

    cctx->ra_cps = 10000;
    cctx->dec_cps = 10000;

    UserSerial.begin(9600);  // USB Serial
    UserSerial.setTimeout(USER_TIMEOUT);

    // It takes a while for the XBee radio to initialize
    delay(WIFLY_DELAY);
}

void
loop() {
    if (UserSerial.available() > 0) {
        cctx->ra_value = EncoderRA.read();
        cctx->dec_value = EncoderDEC.read();
        process_cmd(&UserSerial);
    }
    if (SerialWiFly.available() > 0) {
        cctx->ra_value = EncoderRA.read();
        cctx->dec_value = EncoderDEC.read();
        process_cmd(&WiFlySerialPort);
    }
}

void
process_cmd(AnySerial *serial) {
    char cmd;
    char *buff;
    char read_buff[255];

    cmd = serial->read();
    if (cmd == 'Q') {
        cli_proc_cmd(cctx, "Q");
    } else if (cmd == 'R') {
        serial->readBytesUntil('\r', &read_buff[1], 255);
        read_buff[0] = 'R';
        cli_proc_cmd(cctx, read_buff);
    } else {
        WiFly.print("ERR\r");
    }
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
