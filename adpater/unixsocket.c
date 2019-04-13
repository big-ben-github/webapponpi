/*
    TO BE DONE
    
    1. multi-thread socket sever
    2. handle and dispatch message 
    3. serial communication to Iot devices
    4. variable length list
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#define BUFFER_SIZE 60

void *messagehandle(void *connectfd_un)
{
    int fd = *(int *)connectfd_un;
    char buf[BUFFER_SIZE] = {0};
    while (1)
    {
        if (recv(fd, buf, BUFFER_SIZE, 0) <= 0) // -1:error, 0:disconnect, n:recv ok
        {
            perror("Recv Error or Disconnect");
            break;
        }

        buf[BUFFER_SIZE - 1] = 0;
        printf("------Received: %s.\n", buf);

        char *echo = "{\"res\":\"ok\"}";
        printf("------Sending: %s.\n", echo);
        if (send(fd, echo, strlen(echo), MSG_NOSIGNAL) < 0)
        {
            perror("Send Error");
            break;
        }
    }
    close(fd);
}

int main(int argc, char **argv)
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
            return -1;
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
            if (pthread_create(&thread, NULL, messagehandle, &connectfd_un) < 0)
            {
                perror("Create Thread Error");
                break;
            }
        }
    } while (0);

    unlink(unix_socket);
    close(connectfd_un);
    close(listenfd_un);
    return -1;
}