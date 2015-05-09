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


def query(sock, print_result=False):
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
        if print_result:
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
    sock.connect((ip, port))
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
            last = now
    except socket.error:
        # we'll likely throw this error when the timer goes off
        pass
    except KeyboardInterrupt:
        runtime = time.time() - start
        return (count, minimum, maximum, warns, runtime)

    finally:
        sock.close()
    runtime = time.time() - start
    return (count, minimum, maximum, warns, runtime)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('-s', '--seconds', default=30, type=int,
                        help='How many seconds to run for')
    parser.add_argument('-v', '--verbose', default=False, action='store_true',
                        help="Prints out responses from TeensyDSC")
    parser.add_argument('-i', '--ip', default='10.0.0.1',
                        help="IP address of TeensyDSC")
    parser.add_argument('-p', '--port', default=4030, type=int,
                        help="Port of TeensyDSC")
    parser.add_argument('-t', '--timeout', default=3, type=int,
                        help="socket timeout")
    parser.add_argument('-d', '--delay', default=None, type=float,
                        help="sec.frac delay between queries")
    parser.add_argument('-w', '--warn', default=0.3, type=float,
                        help="sec.frac to warn on")
    args = parser.parse_args()

    signal.signal(signal.SIGALRM, stop)
    count, minimum, maximum, warns, runtime = test_run(args.ip,
                                                       args.port,
                                                       args.seconds,
                                                       args.timeout,
                                                       args.warn,
                                                       args.verbose,
                                                       args.delay)
    if count > 0:
        rate = float(count) / runtime
        avg = runtime / count
        print("We queried the TeensyDSC an average of %.2f times per sec"
              " over %fsec" % (rate, runtime))
        print "Min = %f\t\tMax = %f\t\tAvg = %f\t\tWarns = %d" \
            % (minimum, maximum, avg, warns)
    else:
        print "Failure: no results"
