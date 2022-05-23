#include "os.h"

#include "control.h"
#include "sensors.h"
#include "actuators.h"
#include "architecture.h"
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

static float previ_speed=0.0;
static float integral=0.0;
static float speed=0.0;

static float alt1=0.0;
static float alt2=0.0;
static float alt3=0.0;
static int right=0;
static int left=0;



PTASK task_speed=NULL;
PTASK task_alt1=NULL;
PTASK task_alt2=NULL;
PTASK task_alt3=NULL;
PTASK task_right=NULL;
PTASK task_left=NULL;
PTASK task_ctr_alt=NULL;
PTASK task_ctr_lat=NULL;
PTASK task_ctr_altPID=NULL;


//Llegir Velocitat
void task_read_speed(void *param) {
	previ_speed=speed;
	read_pipe_active_float(fd_READ, &speed);
	integral=integral+speed;
}
//Llegir altura 1
void task_read_altura1(void *param) {
	read_socket_passive_float(fd_ALTITUDE_1, &alt1);
}
//Llegir altura 2
void task_read_altura2(void *param) {
	read_socket_passive_float(fd_ALTITUDE_2, &alt2);
}
//Llegir altura 3
void task_read_altura3(void *param) {
	read_socket_passive_float(fd_ALTITUDE_3, &alt3);
}
//Llegir distancia dreta
void task_read_right(void *param) {
	read_socket_passive_int(fd_RIGHT_SENSOR, &right);
}
//Llegir distancia esquerra
void task_read_left(void *param) {
	read_socket_passive_int(fd_LEFT_SENSOR, &left);
}
//Control d'altura
/*void task_control_alt(void *param){
	int lengthp=strlen(TURNON)+1;
	int lengthb=strlen(TURNOFF)+1;
	if(speed > 0.1){
        	send(fd_PUMP_1, (char*)&lengthp, sizeof(int), 0);
        	send(fd_PUMP_1, TURNON, lengthp, 0);
    	}
    	else if(speed < 0.1){
        	send(fd_PUMP_1, (char*)&lengthb, sizeof(int), 0);
        	send(fd_PUMP_1, TURNOFF, lengthb, 0);
        
    	}
    	if((alt1)<=3.0 || (alt2)<=3.0 || (alt3)<=3.0){
        	send(fd_PUMP_2, (char*)&lengthp, sizeof(int), 0);
        	send(fd_PUMP_2, TURNON, lengthp, 0);  
    	}
    	else{
        	send(fd_PUMP_2, (char*)&lengthb, sizeof(int), 0);
        	send(fd_PUMP_2, TURNOFF, lengthb, 0);
    	}
}
*/
//Anar cap a la dreta
void task_control_drift(void *param){
	
	int lengthr=strlen(RIGHT)+1;
	int lengthl=strlen(LEFT)+1;
	int lengthre=strlen(RELEASE)+1;
	int lengthp=strlen(TURNON)+1;
	int lengthb=strlen(TURNOFF)+1;
    	//Control altura
	if((alt1)<=3.0 || (alt2)<=3.0 || (alt3)<=3.0){
        	send(fd_PUMP_2, (char*)&lengthp, sizeof(int), 0);
        	send(fd_PUMP_2, TURNON, lengthp, 0);  
    	}
    	else{
        	send(fd_PUMP_2, (char*)&lengthb, sizeof(int), 0);
        	send(fd_PUMP_2, TURNOFF, lengthb, 0);
    	}
	if(speed > 0.1){
        	send(fd_PUMP_1, (char*)&lengthp, sizeof(int), 0);
        	send(fd_PUMP_1, TURNON, lengthp, 0);
    	}
    	else if(speed < 0.10){
        	send(fd_PUMP_1, (char*)&lengthb, sizeof(int), 0);
        	send(fd_PUMP_1, TURNOFF, lengthb, 0);
        
	}
	//Control lateral
/*	if (right>=6){
		
        	send(fd_DRIFT, (char*)&lengthl, sizeof(int), 0);
        	send(fd_DRIFT,LEFT, lengthl, 0);

	}
	else{
	
        	send(fd_DRIFT, (char*)&lengthre, sizeof(int), 0);
        	send(fd_DRIFT, RELEASE, lengthre, 0);
        	if(speed <0.15){
			send(fd_PUMP_1, (char*)&lengthp, sizeof(int), 0);
        		send(fd_PUMP_1, TURNON, lengthp, 0);
		}
		else{
			
        		send(fd_DRIFT, (char*)&lengthl, sizeof(int), 0);
        		send(fd_DRIFT, LEFT, lengthl, 0);
		
		}
	}*/
}


void buga_PID_driftcontrol(void *param){
	int lengthp=strlen(TURNON)+1;
	int lengthb=strlen(TURNOFF)+1;

	//PID control design parameters
	float P=4;
	float D=4;
	float I=1;
	float ref=0;
	float x;
	x=ref-(speed*P-(previ_speed-speed)*D+integral*I);

	if(x<0.0){
        send(fd_PUMP_2, (char*)&lengthp, sizeof(int), 0);
        send(fd_PUMP_2, TURNON, lengthp, 0);  
	}else{
        send(fd_PUMP_2, (char*)&lengthb, sizeof(int), 0);
        send(fd_PUMP_2, TURNOFF, lengthb, 0);
	}
	//Readyqueue_enqueue(tasks_queue, task_speed);

}

int init_tasks(){

	Task_create(&task_speed, "Read speed", task_read_speed, NULL, 10, 3);
//	Task_create(&task_alt1, "Read alt1", task_read_altura1, NULL, 0, 100);
//	Task_create(&task_alt2, "Read alt2", task_read_altura2, NULL, 0, 100);
//	Task_create(&task_alt3, "Read alt3", task_read_altura3, NULL, 0, 100);
//	Task_create(&task_right, "Read right", task_read_right, NULL, 0, 300);
//	Task_create(&task_left, "Read left", task_read_left, NULL, 0, 300);
//	Task_create(&task_ctr_alt, "Altitude CTR",task_control_alt, NULL, 1, 50);
	Task_create(&task_ctr_lat, "Altitude LAT",task_control_drift, NULL, 1, 50);
//	Task_create(&task_ctr_altPID, "Altitude PID",buga_PID_driftcontrol, NULL, 50, 50);


	//Queue de tasks

	Readyqueue_enqueue(tasks_queue, task_speed);
//	Readyqueue_enqueue(tasks_queue, task_alt1);
//	Readyqueue_enqueue(tasks_queue, task_alt2);    
//	Readyqueue_enqueue(tasks_queue, task_alt3);
//	Readyqueue_enqueue(tasks_queue, task_right);
//	Readyqueue_enqueue(tasks_queue, task_left);
//	Readyqueue_enqueue(tasks_queue, task_ctr_alt);
	Readyqueue_enqueue(tasks_queue, task_ctr_lat);
//	Readyqueue_enqueue(tasks_queue, task_ctr_altPID);

	return 0;
}




