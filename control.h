#ifndef __CONTROL_H__
#define __CONTROL_H__


#define TURNON "start"
#define TURNOFF "turnoff"
#define RIGHT "right"
#define LEFT "left"
#define RELEASE "release"

int init_tasks();
void task_read_speed(void *param);
void task_read_altura1(void *param);
void task_read_altura2(void *param);
void task_read_altura3(void *param);
void task_read_right(void *param);
void task_read_left(void *param);
int Readyqueue_enqueue(PREADYQUEUE queue, PTASK task);





#endif
