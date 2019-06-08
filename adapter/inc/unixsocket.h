#ifndef _UNIXSOCKET_H_
#define _UNIXSOCKET_H_

//#include <pthread.h>

#define USOCK_SUCCESS  0
#define USOCK_FAIL    -1

#define USOCK_BUFFER_SIZE 100

/*
typedef struct usock_message_tag{
	int fd;
	char* msg_buf[100];
}USock_Msg;

typedef struct usock_mempool_tag{
	USock_Msg usock_msg[USOCK_BUFFER_SIZE];
	int customer;
	int producer;
	int used;
	pthread_mutex_t msg_mutex;
}USock_Mempool;
int init_usock();
*/

int usock_accept_hdl(void);

#endif
