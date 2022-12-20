#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

sem_t semaphores[5];
int philosophers[5]={1,2,3,4,5};

void *philosopher(void *arg) {
	int p_id = *((int *)arg);

	while (1)
	{
		printf("Philosopher %d is thinking\n", p_id);
		sleep(1);

		sem_wait(&semaphores[p_id-1]);
		printf("Philosopher %d picked up left fork\n", p_id);

		sem_wait(&semaphores[p_id%5]);
		printf("Philosopher %d picked up right fork\n", p_id);

		printf("Philosopher %d is eating\n", p_id);
		sleep(1);

		sem_post(&semaphores[p_id-1]);
		printf("Philosopher %d put down left fork\n", p_id);

		sem_post(&semaphores[p_id%5]);
		printf("Philosopher %d put down right fork\n", p_id);
	}
	
}

void *philosopher_last(void *arg) {
	int p_id = *((int *)arg);

	while (1)
	{
		printf("Philosopher %d is thinking\n", p_id);
		sleep(1);

		sem_wait(&semaphores[p_id%5]);
		printf("Philosopher %d picked up right fork\n", p_id);

		sem_wait(&semaphores[p_id-1]);
		printf("Philosopher %d picked up left fork\n", p_id);

		printf("Philosopher %d is eating\n", p_id);
		sleep(1);

		sem_post(&semaphores[p_id%5]);
		printf("Philosopher %d put down right fork\n", p_id);
		
		sem_post(&semaphores[p_id-1]);
		printf("Philosopher %d put down left fork\n", p_id);
	}
	
}

int main(void) {
	pthread_t threads[5];

	for (int i = 0; i < 5; i++) {
		sem_init(&semaphores[i], 0, 1);
	}
	for (int i = 0; i < 4; i++) {
		pthread_create(&threads[i], NULL, philosopher, &philosophers[i]);
		// sleep(1);
	}
	pthread_create(&threads[4], NULL, philosopher_last, &philosophers[4]);

	for (int i = 0; i < 5; i++) {
		pthread_join(threads[i], NULL);
	}
	for (int i = 0; i < 5; i++) {
		sem_destroy(&semaphores[i]);
	}

	return 0;
}