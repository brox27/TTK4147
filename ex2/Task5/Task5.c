#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>

typedef struct {
    int position;
    int count;
    sem_t *forks;
    sem_t *lock;
} params_t;

bool solution = true;

void initialize_semaphores(sem_t *lock, sem_t *forks, int num_forks)
{
    int i;
    for(i = 0; i < num_forks; i++) {
        sem_init(&forks[i], 0, 1);
    }

    sem_init(lock, 0, num_forks - 1);
}

void *philosopher(void *params)
{
    params_t self = *(params_t *)params;

    while(1){
        printf("Philosopher %d thinking...\n", self.position + 1);
        if(solution) sem_wait(self.lock);
        sem_wait(&self.forks[self.position]);
        sem_wait(&self.forks[(self.position + 1) % self.count]);
        printf("Philosopher %d eating...\n", self.position + 1);
        sem_post(&self.forks[self.position]);
        sem_post(&self.forks[(self.position + 1) % self.count]);
        if(solution) sem_post(self.lock);
    }

    pthread_exit(NULL);
}

void createPhilosophers(pthread_t *threads, sem_t *forks, sem_t *lock, int num_philosophers)
{
    int i;
    for(i = 0; i < num_philosophers; i++) {
        params_t *arg = malloc(sizeof(params_t));
        arg->position = i;
        arg->count = num_philosophers;
        arg->lock = lock;
        arg->forks = forks;

        pthread_create(&threads[i], NULL, philosopher, (void *)arg);
    }
}

void main(){
    printf("starting main....\n");

    int num_philosophers = 5;
    sem_t lock;
    sem_t forks[num_philosophers];
    pthread_t philosophers[num_philosophers];
    initialize_semaphores(&lock, forks, num_philosophers);
    createPhilosophers(philosophers, forks, &lock, num_philosophers);

    pthread_exit(NULL);
}
