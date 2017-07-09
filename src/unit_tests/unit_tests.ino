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


#line 21 "unit_tests.ino"
#include <Arduino.h>
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
test(test_10k_n1) TEST_10K(-1, -1)
test(test_10k_1000) TEST_10K(1000, 1000)
test(test_10k_3000) TEST_10K(3000, 3000)
test(test_10k_4999) TEST_10K(4999, 4999)
test(test_10k_5000) TEST_10K(5000, -5000)
test(test_10k_5001) TEST_10K(5001, -4999)
test(test_10k_n2500) TEST_10K(-2500, -2500)
test(test_10k_7500) TEST_10K(7500, -2500)
test(test_10k_10000) TEST_10K(10000, 0)
test(test_10k_14999) TEST_10K(14999, 4999)
test(test_10k_15000) TEST_10K(15000, -5000)
test(test_10k_15001) TEST_10K(15001, -4999)
test(test_10k_19999) TEST_10K(19999, -1)
test(test_10k_24999) TEST_10K(24999, 4999)
test(test_10k_25000) TEST_10K(25000, -5000)
test(test_10k_25001) TEST_10K(25001, -4999)
test(test_10k_n5000) TEST_10K(-5000, -5000)
test(test_10k_n5001) TEST_10K(-5001, 4999)
test(test_10k_n7500) TEST_10K(-7500, 2500)
test(test_10k_n9999) TEST_10K(-9999, 1)
test(test_10k_n10000) TEST_10K(-10000, 0)
test(test_10k_n12500) TEST_10K(-12500, -2500)
test(test_10k_n15000) TEST_10K(-15000, -5000)
test(test_10k_n15001) TEST_10K(-15001, 4999)
test(test_10k_n19999) TEST_10K(-19999, 1)
test(test_10k_n24999) TEST_10K(-24999, -4999)
test(test_10k_n25000) TEST_10K(-25000, -5000)
test(test_10k_n25001) TEST_10K(-25001, 4999)

/*
 * Resolution -10,000
 */
test(test_N10k_0) TEST_N10K(0, 0)
test(test_N10k_1) TEST_N10K(1, 1)
test(test_N10k_n1) TEST_N10K(-1, -1)
test(test_N10k_N1000) TEST_N10K(1000, 1000)
test(test_N10k_3000) TEST_N10K(3000, 3000)
test(test_N10k_4999) TEST_N10K(4999, 4999)
test(test_N10k_5000) TEST_N10K(5000, -5000)
test(test_N10k_5001) TEST_N10K(5001, -4999)
test(test_N10k_n2500) TEST_N10K(-2500, -2500)
test(test_N10k_7500) TEST_N10K(7500, -2500)
test(test_N10k_N10000) TEST_N10K(10000, 0)
test(test_N10k_14999) TEST_N10K(14999, 4999)
test(test_N10k_15000) TEST_N10K(15000, -5000)
test(test_N10k_15001) TEST_N10K(15001, -4999)
test(test_N10k_19999) TEST_N10K(19999, -1)
test(test_N10k_24999) TEST_N10K(24999, 4999)
test(test_N10k_25000) TEST_N10K(25000, -5000)
test(test_N10k_25001) TEST_N10K(25001, -4999)
test(test_N10k_n5000) TEST_N10K(-5000, -5000)
test(test_N10k_n5001) TEST_N10K(-5001, 4999)
test(test_N10k_n7500) TEST_N10K(-7500, 2500)
test(test_N10k_n9999) TEST_N10K(-9999, 1)
test(test_N10k_n10000) TEST_N10K(-10000, 0)
test(test_N10k_n12500) TEST_N10K(-12500, -2500)
test(test_N10k_n15000) TEST_N10K(-15000, -5000)
test(test_N10k_n15001) TEST_N10K(-15001, 4999)
test(test_N10k_n19999) TEST_N10K(-19999, 1)
test(test_N10k_n24999) TEST_N10K(-24999, -4999)
test(test_N10k_n25000) TEST_N10K(-25000, -5000)
test(test_N10k_n25001) TEST_N10K(-25001, 4999)

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
test(test_N40k_0) TEST_N40K(0, 0)
test(test_N40k_1) TEST_N40K(1, 1)
test(test_N40k_1000) TEST_N40K(1000, 1000)
test(test_N40k_3000) TEST_N40K(3000, 3000)
test(test_N40k_4999) TEST_N40K(4999, 4999)
test(test_N40k_5000) TEST_N40K(25000, 25000)
test(test_N40k_35000) TEST_N40K(35000, 35000)
test(test_N40k_39999) TEST_N40K(39999, 39999)
test(test_N40k_N40000) TEST_N40K(40000, 0)
test(test_N40k_75000) TEST_N40K(75000, 35000)
test(test_N40k_1200000) TEST_N40K(1200000, 0)
test(test_N40k_n1) TEST_N40K(-1, 39999)
test(test_N40k_n1000) TEST_N40K(-1000, 39000)
test(test_N40k_n3000) TEST_N40K(-3000, 37000)
test(test_N40k_n4999) TEST_N40K(-4999, 35001)
test(test_N40k_n15000) TEST_N40K(-15000, 25000)
test(test_N40k_n35001) TEST_N40K(-35001, 4999)
test(test_N40k_n39999) TEST_N40K(-39999, 1)
test(test_N40k_n40000) TEST_N40K(-40000, 0)
test(test_N40k_n40001) TEST_N40K(-40001, 39999)
test(test_N40k_n75000) TEST_N40K(-75000, 5000)
test(test_N40k_n1200000) TEST_N40K(-1200000, 0)


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
