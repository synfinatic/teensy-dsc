/*
 * Teensy 3.1 Digital Setting Circles
 * Copyright (C) 2014-2015 Aaron Turner
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

// Don't include anything else or problems running unit tests!
#include "dsc.h"
#include <Arduino.h>


/*
 * Looks like NGC uses 16bits to store the encoder value.
 * So depending on the resolution, it either returns a 
 * uint16_t value or int16_t value.
 *
 * So if you have an abs(resolution) < 2^15, then the range is:
 * -resolution/2 to (resolution/2-1)
 *
 * If your resolution is larger, then the range is:
 * 0 to abs(res)-1
 *
 * Note that a negative resolution doesn't change how we 
 * count ticks.  It's purely up to the computer to know that
 * the counting is backwards!
 */
int32_t
ngc_convert_encoder_value(int32_t encoder, long resolution) {
    int32_t ret = 0;
    static int32_t half_res = resolution / 2;
    long res = abs(resolution);

    // Different math if resolution can be stored in int16_t
    if (res < INT16_MAX) {
        encoder = encoder % res;
        if (encoder >= 0) {
            if (encoder <= (half_res -1)) {
                ret = encoder;
            } else {
                ret = -half_res - (half_res - encoder);
            }
        } else {
            // encoder is negative
            if (abs(encoder) <= half_res) {
                ret = encoder;
            } else {
                ret = half_res + (half_res + encoder);
            }
        }
    } else {
        if (encoder >= 0) {
            ret = encoder % res;
        } else {
            ret = (res + (encoder % res)) % res;
        }
    }
    return ret;
}
