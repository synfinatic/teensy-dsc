#!/usr/bin/env python
# Simple test script which connects to the TeensyDSC and just
# constantly queries the encoder values and prints it out

import socket
import sys


if __name__ == '__main__':
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(("10.0.0.1", 4030))

    while True:
        sent = s.send("Q")
        if sent == 0:
            raise RuntimeError("write() socket connection broken")
        chunk = s.recv(100)
        if chunk == '':
            raise RuntimeError("recv() socket connection broken")
        sys.stdout.write(chunk)
