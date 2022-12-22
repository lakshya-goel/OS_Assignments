#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define TICK struct timespec
#define SZE 1024

#define PATH "shmem_1.c"

int err;

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
    int key, shmid;
    char *shm = shared_init(&key, &shmid);
    int strt = 0;
    while (1)
    {
        int idx;
        char *s = shm;
        for (int i = strt; i < (strt + 5); i++)
        {
            idx = *s++ - 40;
            printf("%d ", idx);
            printf("%s\n", s);
            s += 8;
        }
        *shm = (idx + 40);
        if (strt == 45)
            break;
        while (*shm != (idx + 41))
            ;
        strt += 5;
    }
    shmdt(shm);
    shmctl(shmid,IPC_RMID,NULL);
    return 0;
}
