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
 * 0 to 2^16-1
 *
 * It's so far undocumented how encoders mounted backwards
 * with a negative resolution should be handled for the
 * latter situation.  I decided to do the semi-obvious thing
 * of counting backwards.
 */
int32_t
ngc_convert_encoder_value(int32_t encoder, long resolution) {
    int32_t ret, half_res;

    // Different math if resolution can be stored in int16_t
    if ((resolution < INT16_MAX) && (resolution >= INT16_MIN)) {
        half_res = resolution / 2;
        ret = encoder % resolution;
        if (ret > (half_res - 1)) {
            ret = ret - (half_res - 1) - half_res;
        } else if (ret < -half_res) {
            ret = ret + (half_res + 1);
        }
    } else {
        if (resolution > 0) {
            ret = encoder % abs(resolution);
            // use UNIT_MAX resolution
            // if encoder value is out of range of the resolution,
            // then wrap it
            if (ret < 0) {
                ret = resolution + ret;
            }
        } else {
            ret = -encoder % abs(resolution);
            if (ret < 0) {
                ret = (resolution * -1) + ret;
            }
        }
    }
    return ret;
}
