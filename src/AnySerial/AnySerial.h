#ifndef __ANYSERIAL_H__
#define __ANYSERIAL_H__

/*
 * Copyright 2014 Aaron Turner
 *
 * Wrapper class around:
 * AltSoftSerial
 * NewSoftSerial
 * HardwareSerial (Serial, Serial[123])
 */

#include <AltSoftSerial.h>
#include <HardwareSerial.h>
#ifdef ENABLE_NEWSOFTSERIAL
#include <NewSoftSerial.h>
#endif

typedef enum {
    anyserial_altsoft,
#ifdef ENABLE_NEWSOFTSERIAL
    anyserial_newsoft,
#endif
    anyserial_hardware
} anyserial_t;

class AnyAltSoftSerial : public AltSoftSerial {
    public:
        AnyAltSoftSerial(bool fake) { return; };
        AnyAltSoftSerial() : AltSoftSerial() {};
};

#ifdef ENABLE_NEWSOFTSERIAL
class AnyNewSoftSerial : public NewSoftSerial {
    public:
        AnyNewSoftSerial(bool fake) {};
        AnyNewSoftSerial(uint8_t rx, uint8_t tx) : NewSoftSerial(rx, tx) { };
};
#endif

class FakeHardwareSerial : public HardwareSerial {
    public:
        FakeHardwareSerial() {};
};

class AnySerial : public Stream {
    public:
        AnySerial(HardwareSerial &port);
        AnySerial();
#ifdef ENABLE_NEWSOFTSERIAL
        AnySerial(uint8_t rx, uint8_t tx);
#endif
        ~AnySerial() { return; }
        void end();
        void begin(uint32_t baud);
        int peek();
        int read();
        int available();
        size_t write(uint8_t byte) { writeByte(byte); return 1; }
        size_t write(char *str);
        size_t write(uint8_t buff, size_t len);
        void flush() { flushOutput(); }
        void flushInput();
        void flushOutput();
        bool overflow();
        int library_version();
private:
        anyserial_t port_type;
        HardwareSerial &hardware;
        AnyAltSoftSerial &altsoft;
#ifdef ENABLE_NEWSOFTSERIAL
        AnyNewSoftSerial &newsoft;
#endif
        void writeByte(uint8_t byte);
};
#endif
