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


#line 2 "unit_tests.ino"
#include <ArduinoUnit.h>
#include "dsc.h"

#define TEST_10K(INPUT, CHECK)  {\
    int ret;\
    ret = ngc_convert_encoder_value(INPUT, 10000);\
    assertEqual(ret, CHECK);\
}

#define TEST_N10K(INPUT, CHECK)  {\
    int ret;\
    ret = ngc_convert_encoder_value(INPUT, -10000);\
    assertEqual(ret, CHECK);\
}

#define TEST_40K(INPUT, CHECK)  {\
    int ret;\
    ret = ngc_convert_encoder_value(INPUT, 40000);\
    assertEqual(ret, CHECK);\
}

#define TEST_N40K(INPUT, CHECK)  {\
    int ret;\
    ret = ngc_convert_encoder_value(INPUT, -40000);\
    assertEqual(ret, CHECK);\
}

/*
 * Resolution +10,000
 */
test(test_10k_0) TEST_10K(0, 0)
test(test_10k_1) TEST_10K(1, 1)
test(test_10k_1000) TEST_10K(1000, 1000)
test(test_10k_3000) TEST_10K(3000, 3000)
test(test_10k_4999) TEST_10K(4999, 4999)
test(test_10k_5000) TEST_10K(5000, -4999)
test(test_10k_25000) TEST_10K(25000, -4999)
test(test_10k_n1) TEST_10K(-1, -1)
test(test_10k_n1000) TEST_10K(-1000, -1000)
test(test_10k_n3000) TEST_10K(-3000, -3000)
test(test_10k_n4999) TEST_10K(-4999, -4999)
test(test_10k_n5000) TEST_10K(-5000, -5000)
test(test_10k_n5001) TEST_10K(-5001, 4999)
test(test_10k_n24999) TEST_10K(-24999, -4999)
test(test_10k_n25000) TEST_10K(-25000, -5000)
test(test_10k_n25001) TEST_10K(-25001, 4999)

/*
 * Resolution -10,000
 */
test(test_n10k_0) TEST_N10K(0, 0)
test(test_n10k_1) TEST_N10K(1, 9999)
test(test_n10k_1000) TEST_N10K(1000, 9000)
test(test_n10k_3000) TEST_N10K(3000, 7000)
test(test_n10k_4999) TEST_N10K(4999, 4999)
test(test_n10k_5000) TEST_N10K(5000, 4999)
test(test_n10k_25000) TEST_N10K(25000, -4999)
test(test_n10k_n1) TEST_N10K(-1, -1)
test(test_n10k_n1000) TEST_N10K(-1000, -1000)
test(test_n10k_n3000) TEST_N10K(-3000, -3000)
test(test_n10k_n4999) TEST_N10K(-4999, -4999)
test(test_n10k_n5000) TEST_N10K(-5000, -5000)
test(test_n10k_n5001) TEST_N10K(-5001, 4999)
test(test_n10k_n24999) TEST_N10K(-24999, -4999)
test(test_n10k_n25000) TEST_N10K(-25000, -5000)
test(test_n10k_n25001) TEST_N10K(-25001, 4999)

/*
 * Resolution +40,000
 */
test(test_40k_0) TEST_40K(0, 0)
test(test_40k_1) TEST_40K(1, 1)
test(test_40k_1000) TEST_40K(1000, 1000)
test(test_40k_3000) TEST_40K(3000, 3000)
test(test_40k_4999) TEST_40K(4999, 4999)
test(test_40k_5000) TEST_40K(25000, 25000)
test(test_40k_35000) TEST_40K(35000, 35000)
test(test_40k_39999) TEST_40K(39999, 39999)
test(test_40k_40000) TEST_40K(40000, 0)
test(test_40k_75000) TEST_40K(75000, 35000)
test(test_40k_1200000) TEST_40K(1200000, 0)
test(test_40k_n1) TEST_40K(-1, 39999)
test(test_40k_n1000) TEST_40K(-1000, 39000)
test(test_40k_n3000) TEST_40K(-3000, 37000)
test(test_40k_n4999) TEST_40K(-4999, 35001)
test(test_40k_n15000) TEST_40K(-15000, 25000)
test(test_40k_n35001) TEST_40K(-35001, 4999)
test(test_40k_n39999) TEST_40K(-39999, 1)
test(test_40k_n40000) TEST_40K(-40000, 0)
test(test_40k_n40001) TEST_40K(-40001, 39999)
test(test_40k_n75000) TEST_40K(-75000, 5000)
test(test_40k_n1200000) TEST_40K(-1200000, 0)

/*
 * Resolution -40,000
 */
test(test_n40k_0) TEST_N40K(0, 0)
test(test_n40k_1) TEST_N40K(1, 39999)
test(test_n40k_2) TEST_N40K(2, 39998)
test(test_n40k_5000) TEST_N40K(5000, 35000)
test(test_n40k_19999) TEST_N40K(19999, 20001)
test(test_n40k_20000) TEST_N40K(20000, 20000)
test(test_n40k_20001) TEST_N40K(20001, 19999)
test(test_n40k_39999) TEST_N40K(39999, 1)
test(test_n40k_40000) TEST_N40K(40000, 0)
test(test_n40k_41001) TEST_N40K(40001, 39999)
test(test_n40k_50001) TEST_N40K(50001, 29999)
test(test_n40k_60000) TEST_N40K(60000, 20000)
test(test_n40k_60001) TEST_N40K(60001, 19999)

test(int32_long) {
    assertEqual(sizeof(int32_t), sizeof(long));
}

void setup() {
    Serial.begin(9600);
    while(!Serial); 
}

void loop()
{
    Test::run();
}
