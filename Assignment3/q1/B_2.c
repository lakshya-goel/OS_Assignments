#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

sem_t forks[5];
int philosophers[5]={1,2,3,4,5};
sem_t bowls[2];

void *philosopher(void *arg) {
	int p_id = *((int *)arg);
    int bowl1;
    int bowl2;

	while (1)
	{
		printf("Philosopher %d is thinking\n", p_id);
		sleep(1);

		sem_wait(&forks[p_id-1]);
		printf("Philosopher %d picked up left fork\n", p_id);

		sem_wait(&forks[p_id%5]);
		printf("Philosopher %d picked up right fork\n", p_id);

        sem_getvalue(&bowls[0], &bowl1); 
        sem_getvalue(&bowls[1], &bowl2);

        if(bowl1>0){
            sem_wait(&bowls[0]);
            printf("Philosopher %d picked up bowl 1\n", p_id);

            printf("Philosopher %d is eating\n", p_id);
            sleep(1);
            
            sem_post(&bowls[0]);
            printf("Philosopher %d puts down bowl 1\n", p_id);
        }
        else if(bowl2>0){
            sem_wait(&bowls[1]);
            printf("Philosopher %d picked up bowl 2\n", p_id);

            printf("Philosopher %d is eating\n", p_id);
            sleep(1);
            
            sem_post(&bowls[1]);
            printf("Philosopher %d puts down bowl 2\n", p_id);
        }

		sem_post(&forks[p_id-1]);
		printf("Philosopher %d put down left fork\n", p_id);

		sem_post(&forks[p_id%5]);
		printf("Philosopher %d put down right fork\n", p_id);
	}
	
}

void *philosopher_last(void *arg) {
	int p_id = *((int *)arg);
    int bowl1;
    int bowl2;

	while (1)
	{
		printf("Philosopher %d is thinking\n", p_id);
		sleep(1);

		sem_wait(&forks[p_id%5]);
		printf("Philosopher %d picked up right fork\n", p_id);

		sem_wait(&forks[p_id-1]);
		printf("Philosopher %d picked up left fork\n", p_id);

		sem_getvalue(&bowls[0], &bowl1); 
        sem_getvalue(&bowls[1], &bowl2);

        if(bowl2>0){
            sem_wait(&bowls[1]);
            printf("Philosopher %d picked up bowl 2\n", p_id);

            printf("Philosopher %d is eating\n", p_id);
            sleep(1);
            
            sem_post(&bowls[1]);
            printf("Philosopher %d puts down bowl 2\n", p_id);
        }
        else if(bowl1>0){
            sem_wait(&bowls[0]);
            printf("Philosopher %d picked up bowl 1\n", p_id);

            printf("Philosopher %d is eating\n", p_id);
            sleep(1);
            
            sem_post(&bowls[0]);
            printf("Philosopher %d puts down bowl 1\n", p_id);
        }

		sem_post(&forks[p_id%5]);
		printf("Philosopher %d put down right fork\n", p_id);
		
		sem_post(&forks[p_id-1]);
		printf("Philosopher %d put down left fork\n", p_id);
	}
	
}

int main(void) {
	pthread_t threads[5];

	for (int i = 0; i < 5; i++) {
		sem_init(&forks[i], 0, 1);
	}
    for (int i = 0; i < 2; i++) {
		sem_init(&bowls[i], 0, 1);
	}
	for (int i = 0; i < 4; i++) {
		pthread_create(&threads[i], NULL, philosopher, &philosophers[i]);
	}
	pthread_create(&threads[4], NULL, philosopher_last, &philosophers[4]);

	for (int i = 0; i < 5; i++) {
		pthread_join(threads[i], NULL);
	}
	for (int i = 0; i < 5; i++) {
		sem_destroy(&forks[i]);
	}
    for (int i = 0; i < 2; i++) {
		sem_destroy(&bowls[i]);
	}

	return 0;
}