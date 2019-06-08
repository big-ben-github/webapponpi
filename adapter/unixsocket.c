/*
    #TO BE DONE#
    1. multi-thread socket sever
    2. variable length list
*/

#include <unixsocket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include "dispatcher.h"

static void *usock_recv_hdl(void *connectfd_un)
{
    int fd = *(int *)connectfd_un;
    char buf[USOCK_BUFFER_SIZE] = {0};
    while (1)
    {
        if (recv(fd, buf, USOCK_BUFFER_SIZE, 0) <= 0) // -1:error, 0:disconnect, n:recv ok
        {
            perror("Recv Error or Disconnect\n");
            break;
        }
        
        if(send_dispatcher(buf) == DISPATCH_FIAL){
            perror("Serial send fail\n");
            break;
        }
        
        recv_dispatcher(buf);

        if (send(fd, buf, strlen(buf), MSG_NOSIGNAL) < 0)
        {
            perror("Send Error");
            break;
        }
    }
    return NULL;
}

int usock_accept_hdl(void)
{
    int listenfd_un, connectfd_un;
    struct sockaddr_un serveraddr_un = {};
    struct sockaddr_un clientaddr_un = {};
    socklen_t clientaddrlen = sizeof(clientaddr_un);

    pthread_t thread;

    serveraddr_un.sun_family = AF_UNIX;
    const char *unix_socket = "../unix_socket";
    strcpy(serveraddr_un.sun_path, unix_socket); //strncpy min
    unlink(unix_socket);

    do
    {
        listenfd_un = socket(AF_UNIX, SOCK_STREAM, 0);
        printf("Create Socket......\n");
        if (listenfd_un < 0)
        {
            perror("Create Socket Error\n");
            return USOCK_FAIL;
        }

        if (bind(listenfd_un, (struct sockaddr *)&serveraddr_un, sizeof(struct sockaddr_un)) < 0)
        {
            perror("Bind Error\n");
            break;
        }
        printf("Bind Socket......\n");

        if (listen(listenfd_un, 20) < 0)
        {
            perror("Listen Error");
            break;
        }
        printf("Listen Port......\n");

        while (1)
        {
            connectfd_un = accept(listenfd_un, (struct sockaddr *)&clientaddr_un, &clientaddrlen);
            printf("New Connection fd (%d)......\n", connectfd_un);
            if (connectfd_un < 0)
            {
                perror("Accept Error");
                break;
            }
            if (pthread_create(&thread, NULL, usock_recv_hdl, &connectfd_un) < 0)
            {
                perror("Create Thread Error");
                break;
            }
        }
    } while (0);

    close(connectfd_un);
    close(listenfd_un);
    unlink(unix_socket);
    return USOCK_FAIL;
}
