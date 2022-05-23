#ifndef __ACTUATORS_H__
#define __ACTUATORS_H__

#define PUMP_1 20000
#define PUMP_2 20001
#define DRIFT 20002
extern int fd_PUMP_1;
extern int fd_PUMP_2;
extern int fd_DRIFT;


int init_actuators();
static int connect_socket(int port, int *fd);

#endif
