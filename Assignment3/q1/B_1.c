#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int philosophers[5] = {0,1,2,3,4};
int counter[5][2] = {{0,0}}; //becomes one for whichever fork is picked up if both are picked up, both are one and the philosopher can eat
int bowls[2] = {0,0}; // if a bowl is taken it becomes 1 else remains 0

void *philosopher(void *arg) {
  int id = *((int *)arg);
  int eaten;
  int both_forks=0;
  int bowl=0;

  while (1) {
    eaten=0;
    if(counter[id][0]==0 && counter[id][1]==0){
        printf("Philosopher %d is thinking\n", id+1);
        sleep(1);
    }
    
    if(counter[id][0]==0 && counter[(id>=1)?id-1:5][1]==0){
        counter[id][0]=1;
        printf("Philosopher %d picked up left fork\n", id+1);
    }

    if(counter[id][1]==0 && counter[(id+1)%5][0]==0){
        counter[id][1]=1;
        printf("Philosopher %d picked up right fork\n", id+1);
    }
    

    if(counter[id][0]==1 && counter[id][1]==1){
        both_forks=1;
    }

    if(bowls[0]==0 && both_forks==1 && bowl==0){
        printf("Philosopher %d picked up bowl 1\n", id+1);
        bowl=1;
        bowls[0]=1;
    }
    else if(bowls[1]==0 && both_forks==1 && bowl==0){
        printf("Philosopher %d picked up bowl 2\n", id+1);
        bowl=2;
        bowls[1]=1;
    }

    if(both_forks==1 && bowl>0){
        printf("Philosopher %d is eating\n", id+1);
        eaten=1;
        sleep(1);
    }
    
    if(eaten==1){
        if(bowl==1){
            printf("Philosopher %d puts down bowl 1\n", id+1);
            bowls[0]=0;
        }
        else{
            printf("Philosopher %d puts down bowl 2\n", id+1);
            bowls[1]=0;
        }
        bowl=0;
    }

    if(eaten==1 && counter[id][0]==1){
        counter[id][0]=0;
        printf("Philosopher %d put down left fork\n", id+1);
    }

    if(eaten==1 && counter[id][1]==1){
        counter[id][1]=0;
        printf("Philosopher %d put down right fork\n", id+1);
    }
  }

  return NULL;
}

void *philosopher_last(void *arg) {
  int id = *((int *)arg);
  int eaten;
  int both_forks=0;
  int bowl=0;

  while (1) {
    if(counter[id][0]==0 && counter[id][1]==0){
        printf("Philosopher %d is thinking\n", id+1);
        sleep(1);
    }
    
    if(counter[id][1]==0 && counter[(id+1)%5][0]==0){
        counter[id][1]=1;
        printf("Philosopher %d picked up right fork\n", id+1);
    }

    if(counter[id][0]==0 && counter[(id>=1)?id-1:5][1]==0){
        counter[id][0]=1;
        printf("Philosopher %d picked up left fork\n", id+1);
    }

    if(counter[id][0]==1 && counter[id][1]==1){
        both_forks=1;
    }

    if(bowls[0]==0 && both_forks==1 && bowl==0){
        printf("Philosopher %d picked up bowl 1\n", id+1);
        bowl=1;
        bowls[0]=1;
    }
    else if(bowls[1]==0 && both_forks==1 && bowl==0){
        printf("Philosopher %d picked up bowl 2\n", id+1);
        bowl=2;
        bowls[1]=1;
    }

    if(both_forks==1 && bowl>0){
        printf("Philosopher %d is eating\n", id+1);
        eaten=1;
        sleep(1);
    }
    
    if(eaten==1){
        if(bowl==1){
            printf("Philosopher %d puts down bowl 1\n", id+1);
            bowls[0]=0;
        }
        else{
            printf("Philosopher %d puts down bowl 2\n", id+1);
            bowls[1]=0;
        }
        bowl = 0;
    }
    
    if(eaten==1 && counter[id][1]==1){
        counter[id][1]=0;
        printf("Philosopher %d put down right fork\n", id+1);
    }

    if(eaten==1 && counter[id][0]==1){
        counter[id][0]=0;
        printf("Philosopher %d put down left fork\n", id+1);
    }
  }

  return NULL;
}

int main(void) {
  pthread_t threads[5];

  for (int i = 0; i < 4; i++) {
    pthread_create(&threads[i], NULL, philosopher, &philosophers[i]);
  }
  pthread_create(&threads[4], NULL, philosopher_last, &philosophers[4]);

  for (int i = 0; i < 5; i++) {
    pthread_join(threads[i], NULL);
  }

  return 0;
}