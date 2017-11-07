#include "taskC.h"

RT_MUTEX mutexA;
RT_MUTEX mutexB;
RT_SEM syncronize;
RT_TASK t1,t2,t3;

void rt_busy_wait_ms2(unsigned long delay){
	unsigned long count = 0;
	while (count <= delay*10){
		rt_timer_spin(1000*100);
		count++;
	}
}

int rt_task_sleep_ms2(unsigned long delay){
	return rt_task_sleep(1000*1000*delay);
}

void task_low(){
	rt_sem_p(&syncronize, TM_INFINITE);
	rt_printf("startet task LOW \n");
	rt_printf("LOW vil låse A \n");
	rt_mutex_acquire(&mutexA,TM_INFINITE);
	rt_task_set_priority(&t1, 20);
	rt_printf("LOW har låst A \n");
	rt_busy_wait_ms2(300);
	rt_printf("LOW vil låse B \n");
	rt_mutex_acquire(&mutexB,TM_INFINITE);
	rt_printf("LOW har låst B \n");
	rt_busy_wait_ms2(300);
	rt_printf("HIGH låser opp B \n");
	rt_mutex_release(&mutexB);
	rt_printf("HIGH låser opp A \n");
	rt_mutex_release(&mutexA);
	rt_task_set_priority(&t1, 1);
	rt_busy_wait_ms2(100);
	rt_printf("LOW er ferdig <3 \n");
}


void task_high(){
	rt_sem_p(&syncronize, TM_INFINITE);
	rt_printf("startet task HIGH \n");
	rt_task_sleep_ms2(100);
	rt_printf("HIGH vil låse B \n");
	rt_mutex_acquire(&mutexB,TM_INFINITE);
	rt_printf("HIGH har låst B \n");
	rt_busy_wait_ms2(100);
	rt_printf("HIGH vil låse A \n");
	rt_mutex_acquire(&mutexA,TM_INFINITE);
	rt_printf("HIGH har låst A \n");
	rt_busy_wait_ms2(200);
	rt_printf("HIGH låser opp A \n");
	rt_mutex_release(&mutexA);
	rt_printf("HIGH låser opp B \n");
	rt_mutex_release(&mutexB);
	rt_busy_wait_ms2(100);
	rt_printf("HIGH er ferdig <3 \n");
}

void task_syncronize2(){
	sleep(1);
	rt_sem_broadcast(&syncronize);
}

int main_C (){
	rt_print_auto_init(1);
	mlockall(MCL_CURRENT|MCL_FUTURE);

	rt_mutex_create(&mutexA, "MutexA");
	rt_mutex_create(&mutexB, "MutexB");
	rt_sem_create(&syncronize, "Syncronize", 0, S_FIFO);


	rt_task_create(&t1, "low", 0, 1, T_CPU(1)|T_JOINABLE);
	rt_task_create(&t2, "high", 0, 20, T_CPU(1)|T_JOINABLE);
	rt_task_create(&t3, "syncronize", 0, 50, T_CPU(1)|T_JOINABLE);

	rt_task_start(&t1, &task_low, NULL);
	rt_task_start(&t2, &task_high, NULL);
	rt_task_start(&t3, &task_syncronize2, NULL);

	while(1){};
	rt_mutex_delete(&mutexA);
	rt_mutex_delete(&mutexB);
	rt_sem_delete(&syncronize);

	return 0;
}
