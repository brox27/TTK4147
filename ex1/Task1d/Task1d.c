#include <sys/times.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

/*
	Answer task 4:
	Absolute time is the difference between two points in "the real world", and is very accurate
	Sleep calculates CPU cycles and are not meant for precision timing.
*/

void busy_wait_delay(int seconds) 
{ 
    int i, dummy; 
    int tps = sysconf(_SC_CLK_TCK); 
	clock_t start; 
    struct tms exec_time; 
 	times(&exec_time); 
	start = exec_time.tms_utime; 
    while( (exec_time.tms_utime - start) < (seconds * tps)) 
	{ 
		for(i=0; i<1000; i++) 
  		{ 
        	dummy = i; 
  		} 
  		times(&exec_time); 
 	}  
} 

void* sleepPrint(){
	printf("I am sleeping ... \n");
	sleep(5);
	printf("I am sleeping ... \n");
}

void* busyPrint(){
	printf("I am a busy wait thread... \n");
	busy_wait_delay(5);
	printf("I am a busy wait thread... \n");
}

int main(){

	pthread_t T1;
	pthread_t T2;
	
	pthread_create(&T1, NULL, busyPrint, NULL);
	pthread_create(&T2, NULL, busyPrint, NULL);	

	//pthread_create(&T1, NULL, sleepPrint, NULL);
	//pthread_create(&T2, NULL, sleepPrint, NULL);
	
	pthread_join(T1, NULL);
	pthread_join(T2, NULL);
	
	return 1;

}


