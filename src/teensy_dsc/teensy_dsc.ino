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
#include <WiFly.h>
#include <Flash.h>

#include "defaults.h"
#include "teensy_dsc.h"
#include "rnxv.h"
#include "utils.h"
#include "cli.h"
#include "defaults.h"


/*
 * Init our Encoders & Serial ports
 */
Encoder EncoderRA(CHAN_A_RA, CHAN_B_RA);
Encoder EncoderDEC(CHAN_A_DEC, CHAN_B_DEC);

AnySerial UserSerial(&USER_SERIAL_PORT);
AnySerial WiFlySerialPort(&WIFLY_SERIAL_PORT);
WiFly _WiFly(WiFlySerialPort);

/* our global contexts */
cli_ctx *uctx, *wctx;
common_cli_ctx *common;

void
setup() {
    pinMode(WIFLY_RESET, INPUT);
    pinMode(13, OUTPUT); // blink pin
    encoder_settings_t *encoders;
    serial_settings_t *serial;
    float ver;

    UserSerial.begin(USER_SERIAL_SPEED);
    pinMode(WIFLY_RESET, INPUT);

    common = (common_cli_ctx *)malloc(sizeof(common_cli_ctx));

    serial = get_serial_settings();
    WiFlySerialPort.begin(serial->wifly_baud);

    delay(1000);

    // load the encoder settings stored in the EEPROM
    encoders = get_encoder_settings();
    common->ra_cps = encoders->ra_cps;
    common->dec_cps = encoders->dec_cps;
    common->network = get_network_settings();

    // Init the USB Serial port & context
    UserSerial.printf("Waiting %ums for WiFly...\n", WIFLY_DELAY);
    // Init the WiFly
    delay(WIFLY_DELAY);   // Wait for it to come up

    UserSerial.print("WiFly Version: ");
    ver = _WiFly.version();
    UserSerial.println(ver);
    if (ver < 2.45) {
        Serial.println("Warning! The WiFly's firmware probably doesn't support AP mode.");
        Serial.println("Update the WiFly's firmware with wifly_update example");
    }

    UserSerial.printf("Initializing CLI's...");
    wctx = cli_init_cmd(&WiFlySerialPort, common, NULL, &EncoderRA, &EncoderDEC);
    uctx = cli_init_cmd(&UserSerial, common, &_WiFly, &EncoderRA, &EncoderDEC);
    UserSerial.printf("  OK!\n");

    // Debug mode
    if (common->network->debug_wifly) {
        WiFlySerialPort.attach_debug(&UserSerial);
        WiFlySerialPort.debug(1);
    }

    // don't need this anymore
    free(serial);
    free(encoders);
}

void
loop() {
    if (UserSerial.available() > 0) {
        process_cmd(uctx);
    }

    if (_WiFly.available() > 0) {
        process_cmd(wctx);
    }
}

/*
 * Reads from *serial and trys to execute the given command.
 * 
 * Returns a cmd_status based on if we ran a command
 */
cmd_status
process_cmd(cli_ctx *ctx) {
    AnySerial *serial = ctx->serial;
    static char read_buff[READBUFF_SIZE];
    static size_t pos = 0;
    char temp_buff[READBUFF_SIZE], byte;
    size_t i, len;
    cmd_status status;

    // Short cut for one char commands
    if (pos == 0) {
        i = 0;
        byte = serial->peek();
        while (ctx->one_char_cmds[i] != NULL) {
            if (byte == ctx->one_char_cmds[i]) {
                read_buff[0] = byte;
                read_buff[1] = NULL;
                status = cli_proc_cmd(ctx, read_buff, 1);
                serial->read(); // consume the byte 
                read_buff[0] = NULL;
                return status;
            }
            i++;
        }
    }

    /*
     * Command is multiple bytes followed by '\r'
     */
    len = serial->readBytesUntil('\r', temp_buff, READBUFF_SIZE);
    if (((strlen(read_buff) + len) > READBUFF_SIZE) ||
        (len == READBUFF_SIZE)) {
        // Crap, someone is just sending us crap.  Just eat it.
        pos = 0;
        read_buff[0] = NULL;
        return E_CMD_TOO_LONG;
    }

    // append the last bytes to any bytes we've read before
    strcat(read_buff, temp_buff);
    pos = strlen(read_buff);


    // trim any whitespace on the end
    if (IS_WORD_END(read_buff[len-1])) {
        while (IS_WORD_END(read_buff[len-1])) {
            read_buff[len-1] = NULL;
        }
    } 

    if (strlen(read_buff) == 0) {
        return E_CMD_TOO_SHORT;
    }

    // At this point we should have a whole command.  Go process it!
    status = cli_proc_cmd(ctx, read_buff, strlen(read_buff));

    switch (status) {
        case E_CMD_TOO_LONG:
        case E_CMD_NOT_FOUND:
        case E_CMD_BAD_ARGS:
#ifdef DEBUG
            if (! ctx->eat_errors) {
                serial->printf("ERR [%d]: %s\n", status, read_buff);
            }
#endif
            // fall through
            pos = 0;
            read_buff[0] = NULL;
            break;
        case E_CMD_OK:
            // Eat what we've been given and return
            pos = 0;
            read_buff[0] = NULL;
            break;
        case E_CMD_TOO_SHORT:
            // Keep the buffer for next time 
            break;
    }
    return status;
}


/*
 * Resets the WiFly
 */
void
reset_wifly() {
    UserSerial.write("\nResetting the Xbee...  ");
    pinMode(WIFLY_RESET, OUTPUT);
    digitalWrite(WIFLY_RESET, LOW);
    delay(100);
    pinMode(WIFLY_RESET, INPUT);
    UserSerial.write("Done!\n");
}

