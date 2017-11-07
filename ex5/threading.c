#include "threading.h"

int numDisturbers = 10;
int task = 3;
int disturbance = 1;

int set_cpu(int cpu_number){
	// setting cpu set to the selected cpu
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);
	// set cpu set to current thread and return
	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}

void timespec_add_us(struct timespec *t, long us)
{
	// add microseconds to timespecs nanosecond counter
	t->tv_nsec += us*1000;

	// if wrapping nanosecond counter, increment second counter
	if (t->tv_nsec > 1000000000)
	{
		t->tv_nsec = t->tv_nsec-1000000000;
		t->tv_sec += 1;
	}
}
/*
	Task 1
*/
void *ThreadFunc1(){
	set_cpu(1);
	while(1){
		if (io_read(1) == 0 ){
			io_write(1, 0);
			usleep(1000);
			io_write(1, 1);
		}
	}
} 

void *ThreadFunc2(){
	set_cpu(1);
	while(1){
		if (io_read(2) == 0 ){
			io_write(2, 0);
			usleep(1000);
			io_write(2, 1);
		}
	}
}

void *ThreadFunc3(){
	set_cpu(1);
	while(1){
		if (io_read(3) == 0 ){
			io_write(3, 0);
			usleep(1000);
			io_write(3, 1);
		}
	}
}

void *Task3Func1(){
	io_write(1, 1);
	struct timespec next;
	clock_gettime(CLOCK_REALTIME, &next);
	while(1)
	{
		clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next, NULL);
		timespec_add_us(&next, 1000);
		if(io_read(1) == 0)
		{
			io_write(1, 0);
			usleep(5);
			io_write(1, 1);
		}
		//pthread_yield();
	}
} 

void *Task3Func2(){
	io_write(2, 1);
	struct timespec next;
	clock_gettime(CLOCK_REALTIME, &next);
	while(1)
	{
		clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next, NULL);
		timespec_add_us(&next, 1000);
		if(io_read(2) == 0)
		{
			io_write(2, 0);
			usleep(5);
			io_write(2, 1);
		}
		//pthread_yield();
	}
}

void *Task3Func3(){
	io_write(3, 1);
	struct timespec next;
	clock_gettime(CLOCK_REALTIME, &next);
	while(1)
	{
		clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next, NULL);
		timespec_add_us(&next, 1000);
		if(io_read(3) == 0)
		{
			io_write(3, 0);
			usleep(5);
			io_write(3, 1);
		}
		//pthread_yield();
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
}


int main(){

	//printf("CPU: %d", hello);
	io_init();
	printf("starting main....\n");

	pthread_t inc_t1;
	pthread_t inc_t2;
	pthread_t inc_t3;

	// Task 1
	if(task == 1 || task == 2){
		pthread_create(&inc_t1, NULL, ThreadFunc1, NULL);
		pthread_create(&inc_t2, NULL, ThreadFunc2, NULL);
		pthread_create(&inc_t3, NULL, ThreadFunc3, NULL);
	}

	// Task 2
	if(disturbance){
		pthread_t disturbers[numDisturbers];
		createDisturbers(disturbers);
	}

	// Task 3
	if(task == 3){
		pthread_create(&inc_t1, NULL, Task3Func1, NULL);
		pthread_create(&inc_t2, NULL, Task3Func2, NULL);
		pthread_create(&inc_t3, NULL, Task3Func3, NULL);
	}

	// Make program continue
	pthread_join(inc_t1, NULL);

	return 0;
}

