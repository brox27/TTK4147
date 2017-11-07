#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

int Global = 0;

void *increment(void *local_pointer){
	
	int *local = (int *)local_pointer;

	(*local)++;
	Global++;
	printf("increment kjorte med local: %i og Global: %i \n", *local, Global);
//	return NULL;
}

void main(){
	printf("Starter...\n");
	int local = 0;
	pthread_t inc_t1;
	pthread_t inc_t2;

	pthread_create(&inc_t1, NULL, increment, &local);
	pthread_create(&inc_t2, NULL, increment, &local);

	pthread_join(inc_t1, NULL);
	pthread_join(inc_t2, NULL);


}


