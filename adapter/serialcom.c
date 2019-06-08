#include "serialcom.h"
#include <wiringPi.h>
#include <wiringSerial.h>
#include <unistd.h>

sem_t serial_recv_sem;
Serial_Fd serial_fd = {0};

int init_serial(Serial_Fd* serial_fd){
	sem_init(&serial_recv_sem, 0, 2);
	char recv_buf[100] = {0};
	
	if (wiringPiSetup() < 0 || (serial_fd->fd = serialOpen("/dev/ttyAMA0", 115200)) < 0)
		return SERIAL_FAIL;
	if(pthread_mutex_init(&serial_fd->recv_mutex, NULL) != 0)
		return SERIAL_FAIL;
	serial_fd->send_bufsize = 0;
	serial_fd->send_message = NULL;
	serial_fd->recv_bufsize = 100;
	serial_fd->recv_message = recv_buf;
	return SERIAL_SUCCESS;
}

int serial_send(Serial_Fd* serial_fd){
	if(serial_fd->send_message == NULL)
		return SERIAL_FAIL;
	serialPuts(serial_fd->fd, serial_fd->send_message);
	//serialFlush(serial_fd->fd);
	return SERIAL_SUCCESS;
}

void* serial_recv_thread(void* s_fd){
	Serial_Fd* serial_fd = (Serial_Fd*)s_fd;
	int num = 0, counter = 0, freq = 0;
	char ch = 0;
	
	while(1){
		if((num =serialDataAvail(serial_fd->fd)) <= 0){
			usleep(10000);
			continue;
		}
		freq = (150 - ((num > 100)? 100: num)); usleep(freq*300); //dynamicly change wait time
		usleep(freq*10);
		SERIAL_DEBUG_PRINT("Serial Received: %u bytes.\n", num);
		
		ch = 1; counter = 0;
		pthread_mutex_lock(&serial_fd->recv_mutex);
		while(counter < ((num <= serial_fd->recv_bufsize)? num: serial_fd->recv_bufsize)){
			ch =serialGetchar(serial_fd->fd);
			serial_fd->recv_message[counter++] = ch;
			printf("%c", ch);
			SERIAL_DEBUG_PRINT("%c", ch);
		}
		pthread_mutex_unlock(&serial_fd->recv_mutex);
		sem_post(&serial_recv_sem);
	}
	
	serialClose(serial_fd->fd);
	SERIAL_DEBUG_PRINT("Serial Comm Closed.\n");
}

/*### for test here ###*/
/*
#include <pthread.h>
int main(void){
	pthread_t thread;
	Serial_Fd serial_fd;
	char buf[100] = {0};
	
	if (init_serial(&serial_fd) == SERIAL_FAIL)
		return -1;
	
	serial_fd.recv_bufsize = 100;
	serial_fd.recv_message = buf;
	if(pthread_create(&thread, NULL, serial_recv_thread, &serial_fd) < 0)
		return -1; 
	
	serial_fd.send_message = "I love you!\n\r";
	while(1){
		if(serial_send(&serial_fd) == SERIAL_FAIL)
			return -1;
		printf("Serial Sent: I love you!\n");
		
		delay(1000);
	}
	
	serialClose(serial_fd.fd);
	printf("Serial Comm Closed.\n");
	return 0;
}
*/
