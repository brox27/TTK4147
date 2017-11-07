#include "taskB_mut.h"

RT_MUTEX mutex;
RT_SEM syncronize;

void rt_busy_wait_milliseconds(unsigned long delay){
	unsigned long count = 0;
	while (count <= delay*10){
		rt_timer_spin(1000*100);
		count++;
	}
}

int rt_task_sleep_milliseconds(unsigned long delay){
	return rt_task_sleep(1000*1000*delay);
}

void task_low_mutex(){
	rt_sem_p(&syncronize, TM_INFINITE);
	rt_printf("startet task LOW \n");
	rt_mutex_acquire(&mutex, TM_INFINITE);
	rt_printf("har låst i task LOW \n");
	rt_busy_wait_milliseconds(300);
	rt_printf("har jobbet 3 runder task LOW \n");
	rt_mutex_release(&mutex);
	rt_printf("har låst opp etter task LOW \n");
}

void task_med_mutex(){
	rt_sem_p(&syncronize, TM_INFINITE);
	rt_printf("startet task MED \n");
	rt_task_sleep_milliseconds(100);
	rt_printf("har sleepet 1 runder task MED \n");
	rt_busy_wait_milliseconds(500);
	rt_printf("har jobbet 5 runder task MED \n");
}

void task_high_mutex(){
	rt_sem_p(&syncronize, TM_INFINITE);
	rt_printf("startet task HIGH \n");
	rt_task_sleep_milliseconds(200);
	rt_printf("sleepet 2 runder task HIGH \n");
	rt_mutex_acquire(&mutex, TM_INFINITE);
	rt_printf("låst task HIGH \n");
	rt_busy_wait_milliseconds(200);
	rt_printf("jobbet 2 runder task HIGH \n");
	rt_mutex_release(&mutex);
	rt_printf("låst opp task HIGH \n");
}

void task_sync(){
 	sleep(1);
	rt_sem_broadcast(&syncronize);
}

int main_B_mut(){
	rt_print_auto_init(1);
	mlockall(MCL_CURRENT|MCL_FUTURE);

	rt_mutex_create(&mutex, "Mutex");
	rt_sem_create(&syncronize, "Syncronize", 0, S_FIFO);

	RT_TASK t1,t2,t3,t4;

	rt_task_create(&t1, "low", 0, 1, T_CPU(1)|T_JOINABLE);
	rt_task_create(&t2, "med", 0, 10, T_CPU(1)|T_JOINABLE);
	rt_task_create(&t3, "high", 0, 20, T_CPU(1)|T_JOINABLE);
	rt_task_create(&t4, "syncronize", 0, 50, T_CPU(1)|T_JOINABLE);

	rt_task_start(&t1, &task_low_mutex, NULL);
	rt_task_start(&t2, &task_med_mutex, NULL);
	rt_task_start(&t3, &task_high_mutex, NULL);
	rt_task_start(&t4, &task_sync, NULL);

	while(1){};
	rt_mutex_delete(&mutex);
	rt_sem_delete(&syncronize);

	return 0;
}
