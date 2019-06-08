#include <stdio.h>
#include "serialcom.h"
#include "unixsocket.h"

int main(void){
	pthread_t thread;
	
	if(init_serial(&serial_fd) == SERIAL_FAIL){
		perror("serial init fail!\n");
		return -1;
	}

	if(pthread_create(&thread, NULL, serial_recv_thread, &serial_fd) < 0){
		perror("create serial recv thread failed!\n");
		return -1;
	}
	
	if(usock_accept_hdl() == USOCK_FAIL){
		perror("unix socket fail!\n");
		return -1;
	}
	
	return 0;
}
