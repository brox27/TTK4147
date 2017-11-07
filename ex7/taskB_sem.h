#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sched.h>
#include <pthread.h>
#include <native/task.h>
#include <native/timer.h>
#include <native/sem.h>
#include <rtdk.h>
#include <sys/io.h>

#define SEM_INIT 1 /* Initial semaphore count */
#define SEM_MODE S_FIFO /* Wait by FIFO order */
