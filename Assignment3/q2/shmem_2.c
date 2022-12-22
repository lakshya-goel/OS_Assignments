#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>

#define SHMBLC "ipc_shm_p1.c"

struct timespec strt;
struct timespec fsh;

struct timespec start, finish, gap;

enum
{
    NS_PER_SECOND = 1000000000
};

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

void **printStrings(char *buff)
{
    int err;
    char **bolokc = malloc(50 * sizeof(char *));
    for (int i = 0; i < 50; i++)
    {
        bolokc[i] = malloc(10 * sizeof(char));
    }

    for (int i = 0; i < 7; i++)
    {
        if (err)
            perror("time error");
        for (int j = 0; j < 50; j++)
        {
            bolokc[j][i] = (rand() % 26) + 97;
        }
    }
    for (int j = 0; j < 50; j++)
        bolokc[j][7] = '\0';
    return bolokc;
}

int main(int argc, char const *argv[])
{
    int err;
    char *id = malloc(2 * sizeof(char));
    char **fifty = string_smith();
    int key, shmid;
    char *shm = shared_init(&key, &shmid);
    key = ftok(SHMBLC, 65);
    if (key < 0)
        perror("key error");
    shmid = shmget(key, 1025, IPC_CREAT | 0644);
    if (shmid < 0)
        perror("shmid error");
    char *ret = shmat(shmid, NULL, 0);
    if (ret == (char *)-1)
        perror("shmat error");
    return ret;
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
    sub_timespec(strt, fsh, &gap);
    printf("time elapsed for shared memory : %f\n", gap.tv_nsec);
    return 0;
}
