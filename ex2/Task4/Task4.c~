#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>

int running = 1;
int var1 = 0;
int var2 = 0;
pthread_mutex_t lock;

void *ThreadFunc1(){

	
	while(running){
		pthread_mutex_lock(&lock);
		var1++;
		var2 = var1;
		pthread_mutex_unlock(&lock);
	}

} 

void *ThreadFunc2(){

	int i;
	for (i=0; i <20; i++){
		pthread_mutex_lock(&lock);
		printf("number 1 is %i number2 is %i \n", var1, var2);
		pthread_mutex_unlock(&lock);	
		usleep(100000);
	}
	running = 0;

}

void main(){
	printf("starting main....\n");
	pthread_mutex_init(&lock, NULL);
	pthread_t inc_t1;
	pthread_t inc_t2;


	pthread_create(&inc_t1, NULL, ThreadFunc1, NULL);
	pthread_create(&inc_t2, NULL, ThreadFunc2, NULL);


	pthread_join(inc_t1, NULL);
	pthread_join(inc_t2, NULL);

}


//      gcc badcnt.c -o xbadcnt -lpthread 
