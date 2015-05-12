// Don't include anything else or problems running unit tests!
#include "dsc.h"
#include <Arduino.h>


/*
 * Looks like NGC uses 16bits to store the encoder value.
 * So depending on the resolution, it either returns a 
 * uint16_t value or int16_t value.
 *
 * So if you have a resolution < 2^15, then the range is:
 * -resolution to (resolution-1)
 *
 * If your resolution is larger, then the range is:
 * 0 to 2^16-1
 *
 * It's so far undocumented how encoders mounted backwards
 * with a negative resolution should be handled for the
 * latter situation.  I decided to do the semi-obvious thing
 * of wrapping between 0-(Resolution - 1)
 */

int32_t
ngc_convert_encoder_value(int32_t encoder, long resolution) {
    int32_t ret, half_res;
    half_res = resolution / 2;

    // Different math if resolution can be stored in int16_t
    if ((resolution < INT16_MAX) && (resolution >= INT16_MIN)) {
        ret = encoder % resolution;
        if (ret > (half_res-1)) {
            ret = ret - (half_res - 1) - half_res;
        } else if (ret < -half_res) {
            ret = ret + (half_res * 2);
        }
    } else {
	// use UNIT_MAX resolution
        ret = encoder % abs(resolution);
        // if encoder value is out of range of the resolution,
        // then wrap it
        if ((ret < 0) && (resolution > 0)) {
            ret = resolution + ret;
        } else if ((ret > 0) && (resolution < 0)) { 
            ret = resolution - ret;
        }
    }
    return ret;
}
