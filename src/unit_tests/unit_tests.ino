#line 2 "unit_tests.ino"
#include <ArduinoUnit.h>

#define TEST_10K(INPUT, CHECK)  {\
    assertEqual(ngc_convert_encoder_value(INPUT, 10000), CHECK);\
}

#define TEST_40K(INPUT, CHECK)  {\
    assertEqual(ngc_convert_encoder_value(INPUT, 40000), CHECK);\
}


test(test_10k_0) TEST_10K(0, 0)
test(test_10k_1) TEST_10K(1, 1)
test(test_10k_n1) TEST_10K(-1, -1)
test(test_10k_1000) TEST_10K(1000, 1000)
test(test_10k_n1000) TEST_10K(-1000, -1000)
test(test_10k_3000) TEST_10K(3000, 3000)
test(test_10k_n3000) TEST_10K(-3000, -3000)
test(test_10k_4999) TEST_10K(4999, 4999)
test(test_10k_n4999) TEST_10K(-4999, -4999)
test(test_10k_5000) TEST_10K(5000, -4999)
test(test_10k_n5000) TEST_10K(-5000, -5000)
test(test_10k_n5001) TEST_10K(-5001, 4999)
test(test_10k_n24999) TEST_10K(-24999, -4999)
test(test_10k_25000) TEST_10K(25000, -4999)
test(test_10k_n25000) TEST_10K(-25000, -5000)
test(test_10k_n25001) TEST_10K(-25001, 4999)

test(test_40k_0) TEST_40K(0, 0)
test(test_40k_1) TEST_40K(1, 1)
test(test_40k_n1) TEST_40K(-1, -1)
test(test_40k_1000) TEST_40K(1000, 1000)
test(test_40k_n1000) TEST_40K(-1000, 39000)
test(test_40k_3000) TEST_40K(3000, 3000)
test(test_40k_n3000) TEST_40K(-3000, 37000)
test(test_40k_4999) TEST_40K(4999, 4999)
test(test_40k_n4999) TEST_40K(-4999, 25001)
test(test_40k_5000) TEST_40K(25000, 25000)
test(test_40k_n15000) TEST_40K(-15000, 25000)
test(test_40k_35000) TEST_40K(35000, 35000)
test(test_40k_n35001) TEST_40K(-35001, 5000)
test(test_40k_n39999) TEST_40K(39999, 39999)
test(test_40k_40000) TEST_40K(40000, 0)
test(test_40k_75000) TEST_40K(75000, 35000)
test(test_40k_n75000) TEST_40K(-75000, 5000)
test(test_40k_1200000) TEST_40K(1200000, 0)

void setup() {
    Serial.begin(9600);
    while(!Serial); 
}

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
 * latter situation.  Perhaps we just need to know to 
 * flip the signage pit post procssing???  Do we convert it
 * to a positive range and just wrap the values accordingly?
 */

#define INT16_MIN -32768
#define INT16_MAX 32767
#define UINT_MAX 65535

int32_t
ngc_convert_encoder_value(int32_t encoder, long resolution) {
    int32_t ret, half_res;
    half_res = (resolution / 2) - 1;

    // Different math if resolution can be stored in int16_t
    if ((resolution < INT16_MAX) && (resolution >= INT16_MIN)) {
        ret = encoder % resolution;
        if (ret > half_res-1) {
            ret = ret - (half_res-1) + -half_res;
        } else if (ret < -half_res) {
            ret = -ret - half_res + half_res;
        }
    } else {
	// use UNIT_MAX resolution
        ret = encoder % resolution;
        // if encoder value is out of range of the resolution,
        // then wrap it
        if (((ret < 0) && (resolution > 0)) || 
            ((ret > 0) && (resolution < 0))) { 
            ret = resolution - ret;
        }
    }
    return ret;
}

void loop()
{
    Test::run();
}
