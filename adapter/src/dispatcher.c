#include <stdio.h>
#include <string.h>
#include "serialcom.h"
#include "dispatcher.h"
#include "cJSON.h"

int send_dispatcher(char* buf){
	//json parse
	cJSON* root = NULL;
	char* device = NULL;
	char* cmd = NULL;
	
	if((root = cJSON_Parse(buf)) == NULL)
		return DISPATCH_FIAL;
	device = ((cJSON*)cJSON_GetObjectItem(root, "device"))->valuestring;
	cmd = ((cJSON*)cJSON_GetObjectItem(root, "operation"))->valuestring;
	
	//message dispatch to device
	if(memcmp(device, "lamp", strlen("lamp")) == 0){
		serial_fd.send_message = cmd;
		//serial_fd.send_bufsize = strlen(cmd);
		if(serial_send(&serial_fd) == SERIAL_FAIL)
			return DISPATCH_FIAL;
	}
	cJSON_Delete(root);
	return DISPATCH_OK;
}

int recv_dispatcher(char* buf){
	sem_wait(&serial_recv_sem);
	printf("-------serial_fd.recv_message: (%s)---------\n", serial_fd.recv_message);
	char* result;
	//cmd parse
	if(memcmp("ok", serial_fd.recv_message, 2) == 0){
		result = "{\"res\":\"ok\"}";
	}
	else{
		result = "{\"res\":\"fail\"}";
	}
	//message dispatch to web
	memset(buf, 0, 100);
	memcpy(buf, result, strlen(result));
	printf("------buf: (%s)-----\n", buf);
	return DISPATCH_OK; 
}

