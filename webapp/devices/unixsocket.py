# Method0 --- use nothing, every request use a socket to send and receive and
#             it is sync, it is simple
# Method1 --- use async mechnism like coroutine, every request has its own
#             clinet socket (can be improved?)
#             if use asyncio, may use new thread to run loop event
# Method2 --- use message queue, both client and server have two threads which
#             are recvive and send thread, and there are two queue(send $ recv),
#             then only one connected socket, so Heart-beat mechanism maybe needed

import socket
import json
import sys
import os


def clientsocket(message):  # need unix client socket
    #print(os.getcwd())
    serveraddr_un = 'unix_socket'
    res = {'res': 'fail'}
    socketfd_un = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

    # connect
    try:
        socketfd_un.connect(serveraddr_un)
    except socket.error as error:
        print('Client Socket Connect Error: ', error)
        return json.dumps(res)

    # send and receive
    try:
        socketfd_un.sendall(message.encode())
        res = socketfd_un.recv(30)

    finally:
        socketfd_un.close()
    return res.decode()
