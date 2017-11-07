#include "taskA.h"



#define SEM_INIT 1 /* Initial semaphore count */
#define SEM_MODE S_FIFO /* Wait by FIFO order */


void task(void* arg){
	RT_SEM *ptr = (RT_SEM*) arg;
	int err = rt_sem_p(ptr, TM_INFINITE);
	rt_printf("godkent inn \n");
	if(err!=0){rt_printf("err sier: %d \n", err);}
	rt_sem_v(ptr);

}

void broadcast(void* arg){
	RT_SEM *ptr = (RT_SEM*) arg;
	usleep(100000);
	rt_sem_broadcast(ptr);
	usleep(100000);
}


int main_A (){
	rt_print_auto_init(1);
	RT_SEM semaphore;

	mlockall(MCL_CURRENT|MCL_FUTURE);
	int i;
	char name[10];
	int err = rt_sem_create(&semaphore, "Semaphore", SEM_INIT, S_FIFO);

	RT_TASK t1,t2,t3;
	if(err){

		rt_printf("Error %d \n", err);

	}else{

		rt_task_create(&t1, "Task 1", 0, 1, T_CPU(1)|T_JOINABLE);
		rt_task_create(&t2, "Task 2", 0, 2, T_CPU(1)|T_JOINABLE);
		rt_task_create(&t3, "Broadcast", 0, 45, T_CPU(1)|T_JOINABLE);

		rt_task_start(&t1, &task, (void*)&semaphore);
		rt_task_start(&t2, &task, (void*)&semaphore);
		rt_task_start(&t3, &broadcast, (void*)&semaphore);


		rt_printf("Starting program ... \n");

	}

	while(1){};
	rt_sem_delete(&semaphore);

	return 0;
}
