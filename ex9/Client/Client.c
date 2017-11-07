#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <sys/dispatch.h>
#include <sys/mman.h>

struct pid_data{
	pthread_mutex_t pid_mutex;
	pid_t pid;
};

typedef struct message{
	char content[25];
};

typedef struct thread_arguments {
    int channel;
    int priority;
};

void* accessSharedMemory(char location[]){
	printf("Accessing memory at location %s \n", location);
	int descriptor = shm_open(location, O_RDWR, S_IRWXU);
	return mmap(0, sizeof(struct pid_data), PROT_READ|PROT_WRITE, MAP_SHARED, descriptor, 0);
}

int setPriority(int priority) {

	int policy;
	struct sched_param param;
	// check priority in range
	if (priority < 10 || priority > 63) return -1;

	// set priority
	pthread_getschedparam(pthread_self(), &policy, &param);
	param.sched_priority = priority;
	return pthread_setschedparam(pthread_self(), policy, &param);

}

int getPriority()
{

	int policy;
	struct sched_param param;
	// get priority
	pthread_getschedparam(pthread_self(), &policy, &param);
	return param.sched_curpriority;

}

int* sender(struct thread_arguments *args){

	setPriority(args->priority);
	printf("Starting new thread with priority %d \n", getPriority());
	for(;;){
		sendMessage(args->channel, "I am a client");
		//usleep(1000);
	}

}

int sendMessage(int channel, char content[]){

	struct message request;
	struct message buffer;
	strcpy( request.content, content);

	printf("Requesting: %s \n", request.content);
	MsgSend(channel, &request , sizeof(struct message), &buffer, sizeof(struct message));
	printf("Reply from server: %s \n", buffer.content);

}

int main(int argc, char *argv[]) {
	printf("TTK4147 - Exercise 9 - Client \n");

	/*
	 * Into
	*/

	struct pid_data* data;
	void* memoryLocation = accessSharedMemory("/sharedpid");
	data = (struct pid_data*) memoryLocation;

	printf("Received pid: %d \n", data->pid);

	/*
	 *	Assignment A
	*/

	int channel = ConnectAttach(0, data->pid, 1,0,0);

	struct message request;
	struct message buffer;
	strcpy( request.content, "Received PID");

	/*
	 *Assignment B
	 * Confirm access to shared memory
	*/

	sendMessage(channel, "Received PID");

	/*
	 * Assignment C
	 * Multiple clients
	*/

	int i;
	int num_threads = 4;
	pthread_t threads[num_threads];

	for (i = 0; i < num_threads; i++){
		struct thread_arguments args;
		args.channel = channel;
		// Priority must be over 10
		if(i < 2){
			args.priority = i + 10;
		}else{
			args.priority = i + 25;
		}
		pthread_create(&(threads[i]), NULL, sender, &args);
		pthread_join(&(threads[i]), NULL);
	}

	for(;;);
	ConnectDetach(data->pid);
	printf("Stopping client \n");
	return EXIT_SUCCESS;
}
