/* 
 * Copyright 2013-2014 (c) Aaron Turner
 * This code is released under the GPLv3 license.  Please see the LICENSE file 
 * for details.
 *
 */

#include "teensy_dsc.h"
#include "utils.h"
#include <AnySerial.h>


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
    Serial.print("DEBUG: ");
    Serial.print(tmp);
#else
    return;
#endif
}

/*
 * Float to ascii
 * Since the sprintf() of the Arduino doesn't support floating point
 * converstion, #include <stdlib.h> for itoa() and then use this function
 * to do the conversion manually
 */
char 
*ftoa(char *a, double f, int precision)
{
    long p[] = { 0,10,100,1000,10000,100000,1000000,10000000,100000000 };

    char *ret = a;
    long heiltal = (long)f;
    itoa(heiltal, a, 10);
    while (*a != '\0') a++;
    *a++ = '.';
    long desimal = abs((long)((f - heiltal) * p[precision]));
    itoa(desimal, a, 10);
    return ret;
}

/*
 * Context agnostic means of getting the next 
 * word which is separated by a space, tab, \r or \n
 *
 * Args:
 * *port = Port we're talking to
 * wait_ms = number of ms to wait before returning.
 *
 * Returns: Either a string or NULL on timeout.  If an 
 * incomplete word is read (due to timeout) then whatever
 * bytes have been read are cached and future calls to 
 * get_word() will contain the old + new bytes.
 *
 * Danger: calling get_word() with two different serial ports
 * is going to cause problems!
 */
char *
get_word(AnySerial *port, uint16_t wait_ms) {
    static char *buff = NULL;
    static bool gotword = false;
    char *ret;

    // allocate buff?
    if (NULL == buff) {
        buff = (char *)malloc(GETWORD_BUFFSIZE);
        bzero(buff, GETWORD_BUFFSIZE);
    }

    // zero buff?
    if (gotword) {
        bzero(buff, GETWORD_BUFFSIZE);
        gotword = false;
    }
    ret = get_word_r(port, wait_ms, buff);
    if (ret)
        gotword = true;
    return ret;
}

/*
 * Same as get_word(), but re-entrant and can handle being
 * called on multiple serial ports
 */
char *
get_word_r(AnySerial *port, uint16_t wait_ms, char *buff) {
    uint16_t until, timer = 0;
    size_t bufflen = 0;
    static bool gotword = false;
    char byte;

    bufflen = strlen(buff);

    timer = millis();
    until = timer + wait_ms;
    do {
        while (((byte = port->read()) > -1) && (! gotword)) {
            if (IS_WORD_END(byte)) {
                gotword = true;
                // consume any extra spaces...
                byte = port->peek();
                while (IS_WORD_END(byte)) {
                    port->read();
                    byte = port->peek();
                }
            } else {
                buff[++bufflen] = byte;
            }
        }
        timer = millis();
    } while ((! gotword) && (timer < until) && (bufflen < MAX_GETWORD_SIZE));

    if (gotword) {
        return buff;
    } else {
        return NULL;
    }
}

/*
 * Converts a 'long' into a encoder value string
 */
char *
EncoderValue(long value, bool printsign) {
    static char buff[BUFF_LEN];
    char sign;
    unsigned long a_value;

    if (printsign) {
        if (value < 0) {
            sign = '-';
        } else {
            sign = '+';
        }
    }

    a_value = abs(value);
    if (printsign) {
        sprintf(buff, "%c%05lu", sign, a_value);
    } else {
        sprintf(buff, "%05lu", a_value);
    }
    return buff;
}

char * 
HexEncoderValue(long value) {
  char low, high;
  static char buff[3];

  high = value / 256;
  low = value - high * 256;
  sprintf(buff, "%02x%02x", low, high);

  return buff;
}

/*
 * Follows the NGC-Max/etc spec
 *
 * If the encoder resolutions are <= 32768, the 
 * output is transmitted as a signed number. For example, if 4000 
 * count encoders are used, the range of output is -2000 to 
 * +1999. If the encoder resolutions are > 32768, the outputs are transmitted 
 * as an unsigned numbers. For example, if the resolution was 
 * set to 40000, the range of output would be +00000 to +39999.  For -40000, 
 * output is +00000 to -39999.
 */
int32_t
ngc_convert_encoder_value(int32_t encoder, long resolution) {
    int32_t ret, half_res;
    half_res = resolution / 2;

    // Different math if resolution can be stored in int16_t
    if ((resolution < 32768) && (resolution >= -32768)) {
        ret = encoder % resolution;
        if (ret > half_res-1) {
            // need to wrap around negative
            ret = -half_res + ret - half_res;
        } else if (ret < -half_res) {
            // need to wrap around positive
            ret = half_res - 1 + ret - half_res;
        }
    } else {
        // Nope, uint16_t, so wrap it between 0 and resolution -1
        if (resolution > 0) {
            // always return 0 -> resolution -1
            if (encoder >= 0) {
                ret = encoder % (resolution - 1);
            } else {
                ret = (encoder % (resolution - 1)) + resolution - 1;
            }
        } else {
            // always return 0 -> resolution + 1
            if (encoder <= 0) {
                ret = encoder % (resolution + 1);
            } else {
                ret = resolution + 1 + abs(encoder % (resolution + 1));
            }
        }
    }
    return ret;
}
