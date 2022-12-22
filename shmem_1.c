#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define TICK struct timespec
#define NANO 1000000000
#define SZE 1024

#define PATH "ipc_shm_p1.c"

int err;
TICK strt;
TICK fsh;

double watcher(TICK rolly, TICK pasha)
{
    long good_mornin = (rolly.tv_sec * NANO + rolly.tv_nsec);
    long good_night = (pasha.tv_sec * NANO + pasha.tv_nsec);
    double reiss = (double)(good_night - good_mornin);
    return reiss / NANO;
}

char **string_smith()
{
    TICK time_stamp;
    char **fifty_strngs = malloc(50 * sizeof(char *));
    for (int i = 0; i < 50; i++)
    {
        fifty_strngs[i] = malloc(10 * sizeof(char));
    }

    srand(time_stamp.tv_nsec);
    for (int i = 0; i < 7; i++)
    {
        err = clock_gettime(CLOCK_MONOTONIC, &time_stamp);
        if (err)
            perror("time error");
        for (int j = 0; j < 50; j++)
        {
            fifty_strngs[j][i] = (rand() % 26) + 97;
        }
    }
    for (int j = 0; j < 50; j++)
        fifty_strngs[j][7] = '\0';
    return fifty_strngs;
}

char *shared_init(int *key, int *shmid)
{
    *key = ftok(PATH, 'E');
    if (*key < 0)
        perror("key error");
    *shmid = shmget(*key, SZE, IPC_CREAT | 0644);
    if (*shmid < 0)
        perror("shmid error");
    char *ret = shmat(*shmid, NULL, 0);
    if (ret == (char *)-1)
        perror("shmat error");
    return ret;
}

int main(int argc, char const *argv[])
{
    char *id = malloc(2 * sizeof(char));
    char **fifty = string_smith();
    int key, shmid;
    char *shm = shared_init(&key, &shmid);
    int idx_in = 40;
    clock_gettime(CLOCK_REALTIME, &strt);
    while (idx_in < 90)
    {
        int stp = 5 + idx_in - 40;
        int strt = idx_in - 40;
        char *s = shm;
        for (int i = strt; i < stp; i++)
        {
            *s++ = i + 40;
            s = strcpy(s, fifty[i]);
            if (!s)
                perror("strcat error");
            s += 8;
        }
        while (*shm != idx_in + 4)
            ;
        idx_in = (*shm) + 1;
    }
    clock_gettime(CLOCK_REALTIME, &fsh);
    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);
    printf("time elapsed for shared memory : %f\n", watcher(strt, fsh));
    return 0;
}
