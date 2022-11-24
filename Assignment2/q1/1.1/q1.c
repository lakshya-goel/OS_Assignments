#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>

enum
{
    NS_PER_SECOND = 1000000000
};

struct timespec alpha, beta, delta;
pthread_t pid_1, pid_2, pid_3;
struct sched_param param;

unsigned long multiplier()
{
    unsigned long int i = 1;
    for (int j = 0; j < 32; j++)
        i *= 2;
    return i;
}

void sub_timespec(struct timespec t1, struct timespec t2, struct timespec *td)
{
    td->tv_nsec = t2.tv_nsec - t1.tv_nsec;
    td->tv_sec = t2.tv_sec - t1.tv_sec;
    if (td->tv_sec > 0 && td->tv_nsec < 0)
    {
        td->tv_nsec += NS_PER_SECOND;
        td->tv_sec--;
    }
    else if (td->tv_sec < 0 && td->tv_nsec > 0)
    {
        td->tv_nsec -= NS_PER_SECOND;
        td->tv_sec++;
    }
}

void *countA(void *arg)
{
    param.sched_priority = 0;
    pthread_setschedparam(pid_1, SCHED_OTHER, &param);
    int *thread_no = (int *)arg;
    struct timespec start, finish;
    unsigned long int i;
    unsigned long int upperLimit = multiplier();
    clock_gettime(CLOCK_REALTIME, &start);
    for (i = 1; i <= upperLimit; i++)
    {
    }

    clock_gettime(CLOCK_REALTIME, &finish);
    sub_timespec(start, finish, &alpha);
    printf("Thread: %d - %ld.%ld\n", *thread_no, alpha.tv_sec, alpha.tv_nsec);
    return NULL;
}

void *countB(void *arg)
{
    param.sched_priority = 47;
    pthread_setschedparam(pid_2, SCHED_RR, &param);
    int *thread_no = (int *)arg;
    struct timespec start, finish;
    clock_gettime(CLOCK_REALTIME, &start);
    unsigned long int i;
    unsigned long int upperLimit = multiplier();
    for (i = 1; i <= upperLimit; i++)
    {
    }

    clock_gettime(CLOCK_REALTIME, &finish);
    sub_timespec(start, finish, &beta);
    printf("Thread: %d - %ld.%ld\n", *thread_no, beta.tv_sec, beta.tv_nsec);
    return NULL;
}

void *countC(void *arg)
{
    param.sched_priority = 47;
    pthread_setschedparam(pid_3, SCHED_FIFO, &param);
    int *thread_no = (int *)arg;
    struct timespec start, finish;
    clock_gettime(CLOCK_REALTIME, &start);

    unsigned long int i;
    unsigned long int upperLimit = multiplier();
    for (i = 1; i <= upperLimit; i++)
    {
    }

    clock_gettime(CLOCK_REALTIME, &finish);
    sub_timespec(start, finish, &delta);
    printf("Thread: %d - %ld.%ld\n", *thread_no, delta.tv_sec, delta.tv_nsec);
    return NULL;
}

int main(void)
{
    int a = 1;
    int b = 2;
    int c = 3;
    pthread_create(&pid_1, NULL, countA, (void *)&a);
    pthread_create(&pid_2, NULL, countB, (void *)&b);
    pthread_create(&pid_3, NULL, countC, (void *)&c);

    pthread_join(pid_3, NULL);
    pthread_join(pid_2, NULL);
    pthread_join(pid_1, NULL);

    return 0;
}
