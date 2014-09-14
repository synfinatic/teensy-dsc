#include "AnySerial.h"
#include "Arduino.h"

/*
 * AnySerial - Wrapper class for HardwareSerial, SoftwareSerial & AltSoftSerial
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
 * Wrapper class around:
 * SoftwareSerial
 * AltSoftSerial
 * HardwareSerial (Serial, Serial[123])
 *
 * The goal of AnySerial is to make writing libraries which 
 * use the serial port more cross platform and easier to use.
 *
 * Rather then hard coding a specific Serial port or pins, 
 * you can now just accept an AnySerial object to your 
 * constructor and you don't care what what kind of serial 
 * port it is (hardware or serial), you can just interact
 * with it normally.
 *
 * Just be sure to import the necessary SoftwareSerial.h or
 * AltSoftSerial.h BEFORE importing AnySerial.h so that 
 * AnySerial will be compiled with the necessary support.
 *
 * AnySerial always supports hardware serial ports (Serial, 
 * Serial1, etc)
 */


AnySerial::AnySerial(HardwareSerial *port) {
    serialport.hardware = port;
    port_type = anyserial_hardware;
}

#ifdef AltSoftSerial_h
AnySerial::AnySerial(AltSoftSerial *port) {
    serialport.altsoft = port;
    port_type = anyserial_altsoft;
}
#endif

#ifdef SoftwareSerial_h
AnySerial::AnySerial(SoftwareSerial *port) {
    serialport.soft = port;
    port_type = anyserial_soft;
}
#endif

void
AnySerial::begin(uint32_t baud) { 
    switch (port_type) {
#ifdef AltSoftSerial_h
        case anyserial_altsoft:
            serialport.altsoft->begin(baud);
            break;
#endif
#ifdef SoftwareSerial_h
        case anyserial_soft:
            serialport.soft->begin(baud);
            break;
#endif
        case anyserial_hardware:
            serialport.hardware->begin(baud);
            break;
    }
}


void 
AnySerial::end() {
    switch (port_type) {
#ifdef AltSoftSerial_h
        case anyserial_altsoft:
            serialport.altsoft->end();
            break;
#endif
#ifdef SoftwareSerial_h
        case anyserial_soft:
            serialport.soft->end();
            break;
#endif
        case anyserial_hardware:
            serialport.hardware->end();
            break;
    }

}

int 
AnySerial::peek() {
    int ret = -1;
    switch (port_type) {
#ifdef AltSoftSerial_h
        case anyserial_altsoft:
            ret = serialport.altsoft->peek();
            break;
#endif
#ifdef SoftwareSerial_h
        case anyserial_soft:
            ret = serialport.soft->peek();
            break;
#endif
        case anyserial_hardware:
            ret = serialport.hardware->peek();
            break;
    }

    return ret;
}

int
AnySerial::read() {
    int ret = -1;
    switch (port_type) {
#ifdef AltSoftSerial_h
        case anyserial_altsoft:
            ret = serialport.altsoft->read();
            break;
#endif
#ifdef SoftwareSerial_h
        case anyserial_soft:
            ret = serialport.soft->read();
            break;
#endif
        case anyserial_hardware:
            ret = serialport.hardware->read();
            break;
    }

    return ret;
}

int 
AnySerial::available() {
    int ret = 0;
    switch (port_type) {
#ifdef AltSoftSerial_h
        case anyserial_altsoft:
            ret = serialport.altsoft->available();
            break;
#endif
#ifdef SoftwareSerial_h
        case anyserial_soft:
            ret = serialport.soft->available();
            break;
#endif
        case anyserial_hardware:
            ret = serialport.hardware->available();
            break;
    }

    return ret;
}

void 
AnySerial::flushInput() {
    switch (port_type) {
#ifdef AltSoftSerial_h
        case anyserial_altsoft:
            serialport.altsoft->flushInput();
            break;
#endif
#ifdef SoftwareSerial_h
        case anyserial_soft:
            serialport.soft->flushInput();
            break;
#endif
        case anyserial_hardware:
            // not implimented
            break;
    }
}

void 
AnySerial::flushOutput() {
    switch (port_type) {
#ifdef AltSoftSerial_h
        case anyserial_altsoft:
            serialport.altsoft->flushOutput();
            break;
#endif
#ifdef SoftwareSerial_h
        case anyserial_soft:
            serialport.soft->flushOutput();
            break;
#endif
        case anyserial_hardware:
            // not implimented
            break;
    }
}


bool 
AnySerial::listen() { 
    bool ret = true;
    switch (port_type) {
#ifdef AltSoftSerial_h
        case anyserial_altsoft:
            ret = serialport.altsoft->listen();
            break;
#endif
#ifdef SoftwareSerial_h
        case anyserial_soft:
            ret = serialport.soft->listen();
            break;
#endif
        case anyserial_hardware:
            // not implimented
            break;
    }

    return ret;
}

bool 
AnySerial::isListening() { 
    bool ret = false;
    switch (port_type) {
#ifdef AltSoftSerial_h
        case anyserial_altsoft:
            ret = serialport.altsoft->isListening();
            break;
#endif
#ifdef SoftwareSerial_h
        case anyserial_soft:
            ret = serialport.soft->isListening();
            break;
#endif
        case anyserial_hardware:
            // not implimented
            break;
    }

    return ret;
}

bool 
AnySerial::overflow() { 
    bool ret = false;
    switch (port_type) {
#ifdef AltSoftSerial_h
        case anyserial_altsoft:
            ret = serialport.altsoft->overflow();
            break;
#endif
#ifdef SoftwareSerial_h
        case anyserial_soft:
            ret = serialport.soft->overflow();
            break;
#endif
        case anyserial_hardware:
            // not implimented
            break;
    }

    return ret;
}

int 
AnySerial::library_version() { 
    int ret = 0;
    switch (port_type) {
#ifdef AltSoftSerial_h
        case anyserial_altsoft:
            ret = serialport.altsoft->library_version();
            break;
#endif
#ifdef SoftwareSerial_h
        case anyserial_soft:
            ret = serialport.soft->library_version();
            break;
#endif
        case anyserial_hardware:
            // not implimented
            break;
    }

    return ret;
}


size_t 
AnySerial::write(char *str) {
    size_t ret = 0;
    switch (port_type) {
#ifdef AltSoftSerial_h
        case anyserial_altsoft:
            ret = serialport.altsoft->write(str);
            break;
#endif
#ifdef SoftwareSerial_h
        case anyserial_soft:
            ret = serialport.soft->write(str);
            break;
#endif
        case anyserial_hardware:
            ret = serialport.hardware->write(str);
            break;
    }
    return ret;
}

size_t 
AnySerial::write(const uint8_t *buff, size_t len) {
    size_t ret = 0;
    switch (port_type) {
#ifdef AltSoftSerial_h
        case anyserial_altsoft:
            ret = serialport.altsoft->write(buff, len);
            break;
#endif
#ifdef SoftwareSerial_h
        case anyserial_soft:
            ret = serialport.soft->write(buff, len);
            break;
#endif
        case anyserial_hardware:
            ret = serialport.hardware->write(buff, len);
            break;
    }
    return ret;
}

AnySerial::~AnySerial() {
    end();
}

void 
AnySerial::writeByte(uint8_t byte) {
    switch (port_type) {
#ifdef AltSoftSerial_h
        case anyserial_altsoft:
            serialport.altsoft->write(&byte, 1);
            break;
#endif
#ifdef SoftwareSerial_h
        case anyserial_soft:
            serialport.soft->write(&byte, 1);
            break;
#endif
        case anyserial_hardware:
            serialport.hardware->write(&byte, 1);
            break;
    }
}
