#ifndef __SENSORS_H__
#define __SENSORS_H__


#define ALTITUDE_1 20010
#define ALTITUDE_2 20011
#define ALTITUDE_3 20012
#define LEFT_SENSOR 20015
#define RIGHT_SENSOR 20016
#define PIPE_NAME "Speedpipe"



extern int fd_ALTITUDE_1;
extern int fd_ALTITUDE_2;
extern int fd_ALTITUDE_3;
extern int fd_LEFT_SENSOR;
extern int fd_RIGHT_SENSOR;
extern int fd_READ;
extern int fd_WRITE;






int init_sensors();
static int connect_socket(int port, int *fd);
static int connect_pipe(char *pipe_name,int *fd_read, int *fd_write);

int read_socket_passive_float(int fd, float *value);
int read_socket_passive_int(int fd, int *value);
int read_pipe_active_float(int fd, float *value);

#endif
