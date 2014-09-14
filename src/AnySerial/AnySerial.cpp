#include "AnySerial.h"
#include "Arduino.h"



AnySerial::AnySerial(HardwareSerial &port) : 
    hardware(port),
#ifdef ENABLE_NEWSOFTSERIAL
    newsoft(true),
#endif
    altsoft(true)
{
    port_type = anyserial_hardware;
}

AnySerial::AnySerial() :
    hardware(FakeHardwareSerial),
#ifdef ENABLE_NEWSOFTSERIAL
    newsoft(true),
#endif
    altsoft()
{
    port_type = anyserial_altsoft;
}

#ifdef ENABLE_NEWSOFTSERIAL
AnySerial::AnySerial(uint8_t rx, uint8_t tx) :
    hardware(FakeHardwareSerial),
    newsoft(rx, tx),
    altsoft(true)
{
    port_type = anyserial_newsoft;
}
#endif

void
AnySerial::begin(uint32_t baud) { 
    switch (port_type) {
        case anyserial_altsoft:
            altsoft.begin(baud);
            break;
#ifdef ENABLE_NEWSOFTSERIAL
        case anyserial_newsoft:
            newsoft.begin(baud);
            break;
#endif
        case anyserial_hardware:
            hardware.begin(baud);
            break;
    }
}


void 
AnySerial::end() {
    switch (port_type) {
        case anyserial_altsoft:
            altsoft.end();
            break;
#ifdef ENABLE_NEWSOFTSERIAL
        case anyserial_newsoft:
            newsoft.end();
            break;
#endif
        case anyserial_hardware:
            hardware.end();
            break;
    }

}

int 
AnySerial::peek() {
    int ret;
    switch (port_type) {
        case anyserial_altsoft:
            ret = altsoft.peek();
            break;
#ifdef ENABLE_NEWSOFTSERIAL
        case anyserial_newsoft:
            ret = newsoft.peek();
            break;
#endif
        case anyserial_hardware:
            ret = hardware.peek();
            break;
    }

    return ret;
}

int
AnySerial::read() {
    int ret;
    switch (port_type) {
        case anyserial_altsoft:
            ret = altsoft.read();
            break;
#ifdef ENABLE_NEWSOFTSERIAL
        case anyserial_newsoft:
            ret = newsoft.read();
            break;
#endif
        case anyserial_hardware:
            ret = hardware.read();
            break;
    }

    return ret;
}

int 
AnySerial::available() {
    int ret;
    switch (port_type) {
        case anyserial_altsoft:
            ret = altsoft.available();
            break;
#ifdef ENABLE_NEWSOFTSERIAL
        case anyserial_newsoft:
            ret = newsoft.available();
            break;
#endif
        case anyserial_hardware:
            ret = hardware.available();
            break;
    }

    return ret;
}

void 
AnySerial::flushInput() {
    switch (port_type) {
        case anyserial_altsoft:
            altsoft.flushInput();
            break;
#ifdef ENABLE_NEWSOFTSERIAL
        case anyserial_newsoft:
            newsoft.flushInput();
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
        case anyserial_altsoft:
            altsoft.flushOutput();
            break;
#ifdef ENABLE_NEWSOFTSERIAL
        case anyserial_newsoft:
            newsoft.flushOutput();
            break;
#endif
        case anyserial_hardware:
            // not implimented
            break;
    }
}


bool 
AnySerial::listen() { 
    bool ret;
    switch (port_type) {
        case anyserial_altsoft:
            ret = altsoft.listen();
            break;
#ifdef ENABLE_NEWSOFTSERIAL
        case anyserial_newsoft:
            ret = newsoft.listen();
            break;
#endif
        case anyserial_hardware:
            ret = hardware.listen();
            break;
    }

    return ret;
}

bool 
AnySerial::isListening() { 
    bool ret;
    switch (port_type) {
        case anyserial_altsoft:
            ret = altsoft.isListening();
            break;
#ifdef ENABLE_NEWSOFTSERIAL
        case anyserial_newsoft:
            ret = newsoft.isListening();
            break;
#endif
        case anyserial_hardware:
            ret = hardware.isListening();
            break;
    }

    return ret;
}

bool 
AnySerial::overflow() { 
    bool ret = false;
    switch (port_type) {
        case anyserial_altsoft:
            ret = altsoft.overflow();
            break;
#ifdef ENABLE_NEWSOFTSERIAL
        case anyserial_newsoft:
            ret = newsoft.overflow();
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
    int ret;
    switch (port_type) {
        case anyserial_altsoft:
            ret = altsoft.library_version();
            break;
#ifdef ENABLE_NEWSOFTSERIAL
        case anyserial_newsoft:
            ret = newsoft.library_version();
            break;
#endif
        case anyserial_hardware:
            ret = hardware.library_version();
            break;
    }

    return ret;
}

/*
 *  Only valid for AltSoftSerial
 */
void 
AnySerial::init(uint32_t cycles_per_bit) {
    switch (port_type) {
        case anyserial_altsoft:
            altsoft.init(cycles_per_bit);
            break;
        case anyserial_newsoft:
        case anyserial_hardware:
            // not implimented
            break;
    }
}

void 
AnySerial::writeByte(uint8_t byte) {
    switch (port_type) {
        case anyserial_altsoft:
            altsoft.writeByte(byte);
            break;
#ifdef ENABLE_NEWSOFTSERIAL
        case anyserial_newsoft:
            newsoft.writeByte(byte);
            break;
#endif
        case anyserial_hardware:
            hardware.writeByte(byte);
            break;
    }
}

size_t 
AnySerial::write(char *str) {
    size_t = ret;
    switch (port_type) {
        case anyserial_altsoft:
            ret = altsoft.write(str);
            break;
#ifdef ENABLE_NEWSOFTSERIAL
        case anyserial_newsoft:
            ret = newsoft.write(str);
            break;
#endif
        case anyserial_hardware:
            ret = hardware.write(str);
            break;
    }
    return ret;
}

size_t 
AnySerial::write(uint8_t buff, size_t len) {
    size_t = ret;
    switch (port_type) {
        case anyserial_altsoft:
            ret = altsoft.write(buff, len);
            break;
#ifdef ENABLE_NEWSOFTSERIAL
        case anyserial_newsoft:
            ret = newsoft.write(buff, len);
            break;
#endif
        case anyserial_hardware:
            ret = hardware.write(buff, len);
            break;
    }
    return ret;
}

void 
AnySerial::end() {
    switch (port_type) {
        case anyserial_altsoft:
            altsoft.end();
            break;
#ifdef ENABLE_NEWSOFTSERIAL
        case anyserial_newsoft:
            newsoft.end();
            break;
#endif
        case anyserial_hardware:
            hardware.end();
            break;
    }
}

AnySerial::~AnySerial() {
    end();
}
