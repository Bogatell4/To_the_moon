#include "sensors.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>

int fd_ALTITUDE_1,fd_ALTITUDE_2,fd_ALTITUDE_3,fd_RIGHT_SENSOR,fd_LEFT_SENSOR,fd_WRITE,fd_READ;

int init_sensors(){
 
connect_pipe(PIPE_NAME , &fd_READ, &fd_WRITE);
connect_socket( ALTITUDE_1, &fd_ALTITUDE_1);
connect_socket( ALTITUDE_2, &fd_ALTITUDE_2);
connect_socket( ALTITUDE_3, &fd_ALTITUDE_3);
connect_socket( LEFT_SENSOR, &fd_LEFT_SENSOR);
connect_socket( RIGHT_SENSOR, &fd_RIGHT_SENSOR);

    return 0;
}


static int connect_socket(int port, int *fd){

	struct sockaddr_in serv_addr;
	int try;
	if ((*fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("\n Socket creation error \n");
		return -1;
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	// Convert IPv4 and IPv6 addresses from text to binary form

	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0){

		printf("\nInvalid address/ Address not supported \n");
		return -1;

	}

	try=0;
	while ((connect(*fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <0)&&(try<5)){
		try++;
		usleep(500000);
	}
	if (try==5) return -1;
	return 0;
}
static int connect_pipe(char *pipe_name, int *fd_read, int *fd_write){
	
	char channel_read[256];
	char channel_write[256];
	int try;

	sprintf(channel_read, "%s_read", pipe_name);
	sprintf(channel_write, "%s_write", pipe_name);

	try=0;
	
	while (((*fd_read = open(channel_read, O_RDONLY))<0)&&(try<5)){
		usleep(500000);
		try++;
	}

	if (*fd_read<0) return -1;

	try=0;

	while (((*fd_write = open(channel_write, O_RDWR|O_NONBLOCK))<0)&&(try<5)){
		usleep(500000);
		try++;
	}

	if (*fd_write<0) return -1;

	return 0;
}




int read_socket_passive_float(int fd,float *value){
	char c=0;
	send(fd, (char*)&c, sizeof(char),0);
	recv(fd, value, sizeof(float),0);
	return 0;
}

int read_socket_passive_int(int fd,int *value){
	char c=0;
	send(fd, (char*)&c, sizeof(char),0);
	recv(fd, value, sizeof(int),0);
	return 0;
}

int read_pipe_active_float(int fd,float *value){
	read(fd, value, sizeof(float));
	return 0;
}
