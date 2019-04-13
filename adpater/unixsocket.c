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
        if (recv(fd, buf, BUFFER_SIZE, 0) < 0)
        {
            perror("recv error!");
            break;
        }

        buf[BUFFER_SIZE - 1] = 0;
        printf("--------------received: %s.\n", buf);

        char *echo = "{\"res\":\"ok\"}";
        printf("---------------sending: %s.\n", echo);
        if (send(fd, echo, strlen(echo), 0) < 0)
        {
            perror("send error!");
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
    const char *unix_socket = "unix_socket";
    strcpy(serveraddr_un.sun_path, unix_socket); //strncpy min
    unlink(unix_socket);

    do
    {
        listenfd_un = socket(AF_UNIX, SOCK_STREAM, 0);
        printf("create socket......\n");
        if (listenfd_un < 0)
        {
            perror("create socket error!\n");
            return -1;
        }

        if (bind(listenfd_un, (struct sockaddr *)&serveraddr_un, sizeof(struct sockaddr_un)) < 0)
        {
            perror("bind error!\n");
            break;
        }
        printf("bind......\n");

        if (listen(listenfd_un, 20) < 0)
        {
            perror("listen error!");
            break;
        }
        printf("listen......\n");

        while (1)
        {
            connectfd_un = accept(listenfd_un, (struct sockaddr *)&clientaddr_un, &clientaddrlen);
            printf("new connection for......\n");
            if (connectfd_un < 0)
            {
                perror("accept error!");
                break;
            }
            if (pthread_create(&thread, NULL, messagehandle, &connectfd_un) < 0)
            {
                perror("create thread error");
                break;
            }
        }
        printf("here1\n");
    } while (0);
    printf("here2\n");

    unlink(unix_socket);
    close(connectfd_un);
    close(listenfd_un);
    return -1;
}