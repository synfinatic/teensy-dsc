#include "AnySerial.h"
#include "Arduino.h"



AnySerial::AnySerial(HardwareSerial *port) {
    serialport.hardware = port;
    port_type = anyserial_hardware;
}

AnySerial::AnySerial(AltSoftSerial *port) {
    serialport.altserial = port;
    port_type = anyserial_altsoft;
}


void
AnySerial::begin(uint32_t baud) { 
    switch (port_type) {
        case anyserial_altsoft:
            serialport.altserial->begin(baud);
            break;
        case anyserial_hardware:
            serialport.hardware->begin(baud);
            break;
    }
}


void 
AnySerial::end() {
    switch (port_type) {
        case anyserial_altsoft:
            serialport.altserial->end();
            break;
        case anyserial_hardware:
            serialport.hardware->end();
            break;
    }

}

int 
AnySerial::peek() {
    int ret;
    switch (port_type) {
        case anyserial_altsoft:
            ret = serialport.altserial->peek();
            break;
        case anyserial_hardware:
            ret = serialport.hardware->peek();
            break;
    }

    return ret;
}

int
AnySerial::read() {
    int ret;
    switch (port_type) {
        case anyserial_altsoft:
            ret = serialport.altserial->read();
            break;
        case anyserial_hardware:
            ret = serialport.hardware->read();
            break;
    }

    return ret;
}

int 
AnySerial::available() {
    int ret;
    switch (port_type) {
        case anyserial_altsoft:
            ret = serialport.altserial->available();
            break;
        case anyserial_hardware:
            ret = serialport.hardware->available();
            break;
    }

    return ret;
}

void 
AnySerial::flushInput() {
    switch (port_type) {
        case anyserial_altsoft:
            serialport.altserial->flushInput();
            break;
        case anyserial_hardware:
            // not implimented
            break;
    }
}

void 
AnySerial::flushOutput() {
    switch (port_type) {
        case anyserial_altsoft:
            serialport.altserial->flushOutput();
            break;
        case anyserial_hardware:
            // not implimented
            break;
    }
}


bool 
AnySerial::listen() { 
    bool ret = true;
    switch (port_type) {
        case anyserial_altsoft:
            ret = serialport.altserial->listen();
            break;
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
        case anyserial_altsoft:
            ret = serialport.altserial->isListening();
            break;
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
        case anyserial_altsoft:
            ret = serialport.altserial->overflow();
            break;
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
        case anyserial_altsoft:
            ret = serialport.altserial->library_version();
            break;
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
        case anyserial_altsoft:
            ret = serialport.altserial->write(str);
            break;
        case anyserial_hardware:
            ret = serialport.hardware->write(str);
            break;
    }
    return ret;
}

size_t 
AnySerial::write(uint8_t *buff, size_t len) {
    size_t ret = 0;
    switch (port_type) {
        case anyserial_altsoft:
            ret = serialport.altserial->write(buff, len);
            break;
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
        case anyserial_altsoft:
            serialport.altserial->write(&byte, 1);
            break;
        case anyserial_hardware:
            serialport.hardware->write(&byte, 1);
            break;
    }
}
