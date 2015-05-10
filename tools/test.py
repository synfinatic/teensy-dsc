#!/usr/bin/env python
__doc__ = """Simple benchmarking script for TeensyDSC"""
import socket
import sys
import time
import signal
import argparse


loop = True


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
        chunk = sock.recv(100)
        if chunk == '':
            raise RuntimeError("recv() socket connection broken")
        if verbose and verbose >= 2:
            sys.stdout.write(chunk)
    except socket.timeout:
        return 0
    return 1


def test_run(ip, port, runtime, timeout, warn_timeout,
             verbose=False, delay=False):
    """Connects to the TeensyDSC and does the test run for the given
       period of time

       Returns the total completed queries"""
    minimum = 1000
    maximum = 0
    count = 0
    warns = 0
    start = time.time()

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.settimeout(timeout)
    try:
        sock.connect((ip, port))
    except socket.error as e:
        print "Unable to connect to %s:%d (%s)" % (ip, port, e)
        sys.exit(-1)

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
    except socket.error:
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
    parser.add_argument('-t', '--timeout', default=3, type=int,
                        help='socket timeout in seconds [%(default)s]')
    parser.add_argument('-d', '--delay', default=None, type=float,
                        help='sec.frac delay between queries [%(default)s]')
    parser.add_argument('-w', '--warn', default=0.3, type=float,
                        help='sec.frac to warn on [%(default)s]')
    args = parser.parse_args()

    signal.signal(signal.SIGALRM, stop)
    result = test_run(args.ip, args.port, args.seconds,
                      args.timeout, args.warn, args.verbose, args.delay)
    if result['count'] > 0:
        rate = float(result['count']) / result['runtime']
        avg = result['runtime'] / result['count']
        print "Rate: %.3f/sec over a period of %.2f seconds" % (
            rate, result['runtime'])
        print "Min: %f\t\tMax: %f\t\tAvg: %f" % (
            result['min'], result['max'], avg)
        print "%d queries took longer then %f seconds" % (
            result['warns'], args.warn)
    else:
        print "Failure: no results"
