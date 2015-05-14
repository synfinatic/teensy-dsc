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
#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <Flash.h>
#include <AnySerial.h>
#include <WiFly.h>
#include "defaults.h"

#define PAUSE_DURATION 100
#define LONG_PAUSE_DURATION 500

#define PORT_OPEN "*port opened*"
#define PORT_CLOSE "*port closed*"
#define COMM_SIZE 1420
#define COMM_TIME_MS 5

void rnxv_configure(WiFly *, network_settings_t *);
void rnxv_get_config(WiFly *, AnySerial *);

#endif
