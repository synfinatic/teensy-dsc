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

typedef enum {
    anyserial_altsoft,
    anyserial_hardware
} anyserial_t;

typedef union {
    HardwareSerial *hardware;
    AltSoftSerial *altserial;
} serialport_t;

class AnySerial : public Stream {
    public:
        AnySerial(HardwareSerial *port);
        AnySerial(AltSoftSerial *port);
        ~AnySerial();
        void end();
        void begin(uint32_t baud);
        int peek();
        int read();
        int available();
        bool listen();
        bool isListening();
        size_t write(uint8_t byte) { writeByte(byte); return 1; }
        size_t write(char *str);
        size_t write(uint8_t *buff, size_t len);
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
