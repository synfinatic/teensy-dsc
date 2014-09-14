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

#include <HardwareSerial.h>

typedef enum {
#ifdef AltSoftSerial_h
    anyserial_altsoft,
#endif
#ifdef SoftwareSerial_h
    anyserial_soft,
#endif
    anyserial_hardware
} anyserial_t;

typedef union {
    HardwareSerial *hardware;
#ifdef AltSoftSerial_h
    AltSoftSerial *altsoft;
#endif
#ifdef SoftwareSerial_h
    SoftwareSerial *soft;
#endif
} serialport_t;

class AnySerial : public Stream {
    public:
        AnySerial(HardwareSerial *port);
#ifdef AltSoftSerial_h
        AnySerial(AltSoftSerial *port);
#endif
#ifdef NewSoftSerial_h
        AnySerial(NewSoftSerial *port);
#endif
        ~AnySerial();
        void end();
        void begin(uint32_t baud);
        int peek();
        int read();
        int available();
        bool listen();
        bool isListening();
        size_t write(char *str);
        virtual size_t write(const uint8_t *buff, size_t len);
        virtual size_t write(uint8_t byte) { writeByte(byte); return 1; }
        using Print::write;
        void flush() { flushOutput(); }
        void flushInput();
        void flushOutput();
        bool overflow();
        int library_version();
private:
        anyserial_t port_type;
        serialport_t serialport;
        void writeByte(uint8_t byte);
};
#endif
