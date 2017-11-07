#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>

void *SemFunc(sem_t *sem_ptr){

	sem_wait(sem_ptr); 

	printf("jada.... \n");
	usleep(100000);
	sem_post(sem_ptr);
} 

void main(){
	sem_t CountSem;
	int numVar = 3;
	sem_init(&CountSem, 0, numVar);


	pthread_t inc_t1;
	pthread_t inc_t2;
	pthread_t inc_t3;
	pthread_t inc_t4;
	pthread_t inc_t5;

	pthread_create(&inc_t1, NULL, SemFunc, &CountSem);
	pthread_create(&inc_t2, NULL, SemFunc, &CountSem);
	pthread_create(&inc_t3, NULL, SemFunc, &CountSem);
	pthread_create(&inc_t4, NULL, SemFunc, &CountSem);
	pthread_create(&inc_t5, NULL, SemFunc, &CountSem);

	pthread_join(inc_t1, NULL);
	pthread_join(inc_t2, NULL);
	pthread_join(inc_t3, NULL);
	pthread_join(inc_t4, NULL);
	pthread_join(inc_t5, NULL);
}


//      gcc badcnt.c -o xbadcnt -lpthread 
