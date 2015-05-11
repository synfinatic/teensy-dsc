#!/usr/bin/env python
__doc__ = """Simple benchmarking script for TeensyDSC"""
import socket
import os
import sys
import time
import signal
import argparse
from serial.serialposix import PosixSerial  # pip install pyserial
import serial.serialutil

loop = True


class MySerial(PosixSerial):
    """Wrapper around serial class to sorta look like a socket"""
    def __init__(self, *args, **kwargs):
        super(MySerial, self).__init__(*args, **kwargs)

    def send(self, *args):
        self.write(*args)

    def recv(self, *args):
        try:
            return self.read(*args)
        except serial.serialutil.SerialException:
            return None


def stop(a, b):
    """SIGALRM callback"""
    global loop
    loop = False


def query(sock, verbose=False):
    """Single query/reponse from the TeensyDSC

    Returns 0 for failure, 1 for success
    """
    try:
        sent = sock.send("Q")
        if sent == 0:
            raise RuntimeError("write() socket connection broken")
        chunk = sock.recv(14)
        if chunk == '':
            raise RuntimeError("recv() socket connection broken")
        if verbose and verbose >= 2:
            sys.stdout.write(chunk)
    except socket.timeout:
        return 0
    return 1


def test_run(sock, runtime, timeout, warn_timeout,
             verbose=False, delay=False):
    """Uses the given socket/fd to test the TeensyDSC"""
    minimum = 1000
    maximum = 0
    count = 0
    warns = 0
    start = time.time()

    signal.alarm(runtime)
    try:
        last = time.time()
        while loop:
            count += query(sock, verbose)
            if delay:
                time.sleep(delay)
            now = time.time()
            delta = now - last
            if (delta < minimum):
                minimum = delta
            elif (delta > maximum):
                maximum = delta
            if (delta >= warn_timeout):
                warns += 1
                if verbose:
                    print "[%d] %f @ %s" % (count, delta, now)
            last = now
    except socket.error, serial.serialutil.SerialException:
        # we'll likely throw this error when the timer goes off
        pass
    except KeyboardInterrupt:
        # Ctrl-C
        runtime = time.time() - start
        return {
            'count': count,
            'min': minimum,
            'max': maximum,
            'warns': warns,
            'runtime': runtime
        }
    finally:
        sock.close()

    runtime = time.time() - start
    return {
        'count': count,
        'min': minimum,
        'max': maximum,
        'warns': warns,
        'runtime': runtime
    }


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('-s', '--seconds', default=30, type=int,
                        help='How many seconds to run for [%(default)s]')
    parser.add_argument('-v', '--verbose', default=False, action='count',
                        help='Prints out responses from TeensyDSC')
    parser.add_argument('-i', '--ip', default='10.0.0.1',
                        help='IP address of TeensyDSC [%(default)s]')
    parser.add_argument('-p', '--port', default=4030, type=int,
                        help='Port of TeensyDSC [%(default)s]')
    parser.add_argument('-t', '--tty', type=str, default=None,
                        help='Use the given serial tty device to talk to the '
                        'TeensyDSC instead of TCP/IP')
    parser.add_argument('-l', '--listdevs', action='store_true',
                        help='List all current serial devices')
    parser.add_argument('-T', '--timeout', default=3, type=int,
                        help='socket timeout in seconds [%(default)s]')
    parser.add_argument('-d', '--delay', default=None, type=float,
                        help='sec.frac delay between queries [%(default)s]')
    parser.add_argument('-w', '--warn', default=0.3, type=float,
                        help='sec.frac to warn on [%(default)s]')
    args = parser.parse_args()

    signal.signal(signal.SIGALRM, stop)

    if args.listdevs:
        import serial.tools.list_ports
        iterator = sorted(serial.tools.list_ports.comports())
        # list them
        for port, desc, hwid in iterator:
            print("%-20s" % (port,))
            print("    desc: %s" % (desc,))
            print("    hwid: %s" % (hwid,))
        sys.exit(0)

    # open our FD or socket
    if args.tty:
        try:
            tty = MySerial(args.tty, 9600, timeout=args.timeout)
        except OSError as e:
            print "Unable to open %s: %s" % (args.tty, e)
            sys.exit(-1)

        tty.flushInput()
        tty.flushOutput()
        result = test_run(tty, args.seconds, args.timeout,
                          args.warn, args.verbose, args.delay)
    else:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.settimeout(timeout)
        try:
            sock.connect((ip, port))
        except socket.error as e:
            print "Unable to connect to %s:%d (%s)" % (ip, port, e)
            sys.exit(-1)

        result = test_run(sock, args.seconds, args.timeout,
                          args.warn, args.verbose, args.delay)

    if result['count'] > 0:
        rate = float(result['count']) / result['runtime']
        avg = result['runtime'] / result['count']
        print "%d queries in %.2f seconds" % (
            result['count'], result['runtime'])
        print "Rate: %.3f/sec" % (rate,)
        print "Min: %f\t\tMax: %f\t\tAvg: %f" % (
            result['min'], result['max'], avg)
        print "%d queries took longer then %f seconds" % (
            result['warns'], args.warn)
    else:
        print "Failure: no results"
