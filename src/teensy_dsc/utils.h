/* 
 * Copyright 2013-2014 (c) Aaron Turner
 * This code is released under the GPLv3 license.  Please see the LICENSE file 
 * for details.
 *
 */

#include <AnySerial.h>

#ifndef UTILS_H
#define UTILS_H

void serial_printf(const char *fmt, ... );
void dbg_serial_printf(const char *fmt, ... );
char *ftoa(char *a, double f, int precision);
char *get_word(AnySerial *port, uint16_t wait_ms);
char *get_word_r(AnySerial *port, uint16_t wait_ms, char *buff);
char *EncoderValue(long value, bool lead);
int32_t ngc_convert_encoder_value(int32_t encoder, long resolution);


#define IS_WORD_END(x) (((x) == ' ') || ((x) == '\r') || ((x) == '\t') || ((x) == '\n'))
#define GETWORD_BUFFSIZE 128 
#define MAX_GETWORD_SIZE 127

#endif
