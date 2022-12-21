#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define first i%5
#define second (i+2)%5

int philosophers[5] = {1,2,3,4,5};
int forks[5] = {0}; 


void *philosopher(void *arg) {
  int id = *((int *)arg);
  printf("Philosopher %d is thinking\n", id);
  if(forks[id-1]==0 && forks[id%5]==0){
    forks[id-1]=forks[id%5]=1;
    printf("Philosopher %d is eating\n", id);
    sleep(1);
  }
  forks[id-1]=forks[id%5]=0;
  printf("Philosopher %d is thinking\n", id);

  return NULL;
}

int main(void) {
  pthread_t threads[5];

  int i=0;
  while(1){
    pthread_create(&threads[first], NULL, philosopher, &philosophers[first]);
    pthread_create(&threads[second], NULL, philosopher, &philosophers[second]);
    pthread_join(threads[first], NULL);
    pthread_join(threads[second], NULL);
    i++;
  }

  return 0;
}
