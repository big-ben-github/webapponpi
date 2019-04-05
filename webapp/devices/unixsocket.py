# TBD: Heart-beat mechanism

import socket
import threading
import time
import sys
from queue import Queue

orderqueue = Queue()
resultqueue = Queue()

def runclientsocket():
    serveraddr_un = 'unix_socket'
    socketfd_un = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    #print('connecting to {}'.format(serveraddr_un))
    try:
        socketfd_un.connect(serveraddr_un)
    except socket.error as error:
        print('ERROR: ', error)
        return

    try:
        while True:
            message = b'message content'
            socketfd_un.sendall(message)
            socketfd_un.recv(100)

    finally:
        socketfd_un.close()
