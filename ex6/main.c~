#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sched.h>
#include <pthread.h>

#include <native/task.h>
#include <native/timer.h>

#include  <rtdk.h>

#include <sys/io.h>
#include "io.h"

int task = 2;
int numDisturbers = 10;

int printing(){
	rt_task_set_periodic(NULL, TM_NOW, 1000000000);

	while(1) {
		printf("Hello \n");
		rt_task_wait_period(NULL);
	}
}



int set_cpu(int cpu_number){
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);
	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}


int periodic(void* param){
	int id = (int) param;
	printf("Starting thread %d\n" , id);
	rt_task_set_periodic(NULL, TM_NOW, 10000);
	io_write(id, 1);

	while(1) {
		rt_task_wait_period(NULL);
		if (io_read(id) == 0) {
			io_write(id, 0);
			usleep(5);
			io_write(id, 1);
		}
	}
}

void *disturbanceWorker(){
	
	printf("Starting disturbance worker \n");
	set_cpu(1);
	while(1){

	}
	
}


void createDisturbers(pthread_t *threads){
	int i;
	for(i = 0; i < numDisturbers; i++) {
		pthread_create(&threads[i], NULL, disturbanceWorker, NULL);
	}
	for(i = 0; i < numDisturbers; i++) {
		pthread_join(threads[i], NULL);
	}
}



int main(){
	
	io_init();
	mlockall(MCL_CURRENT|MCL_FUTURE);

	if(task == 0){
		RT_TASK task0;
		rt_task_create(&task0, "Testing periodic printing", 0, 0, T_CPU(1)|T_JOINABLE);
		rt_task_start(&task0, &printing, (void*) 0);
	}

	if(task == 1 || task == 2){
		int i;
		char name[10];
		RT_TASK tasks[3];
		for(i = 1; i <= 3; i++){
			//printf("Starting thread %d \n", i);
			rt_task_create(&tasks[i-1], name + i, 0, i, T_CPU(1)|T_JOINABLE);
			rt_task_start(&tasks[i-1], &periodic, (void*) i);
		}
	}

	if(task == 2){
		pthread_t disturbers[numDisturbers];
		createDisturbers(disturbers);
	}

 	//Prevent program from stopping
	while (1){}
	return 0;	
}

