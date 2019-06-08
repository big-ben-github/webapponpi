#ifndef _SERIALCOM_H_
#define _SERIALCOM_H_

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define SERIAL_SUCCESS  0
#define SERIAL_FAIL    -1

//#define SERIAL_DEBUG
#ifdef  SERIAL_DEBUG
#define SERIAL_DEBUG_PRINT(format,...)\
        do{\
			printf("SERIAL_DEBUG: File[%s], Fun[%s], Line[%d]"format, \
			       __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);\
		}while(0)
#else
#define SERIAL_DEBUG_PRINT(formatmessage, ...)
#endif

typedef struct serial_message_tag{
	int fd;
	int send_bufsize;
	char* send_message;
	int recv_bufsize;
	char* recv_message;
	pthread_mutex_t recv_mutex;
}Serial_Fd;

extern sem_t serial_recv_sem;

extern Serial_Fd serial_fd;

int init_serial(Serial_Fd* serial_fd);

int serial_send(Serial_Fd* serial_fd);

void* serial_recv_thread(void* s_fd);

#endif
