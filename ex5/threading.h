#include <sched.h>
#include <comedilib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include "io.h"

int set_cpu(int cpu_number);
