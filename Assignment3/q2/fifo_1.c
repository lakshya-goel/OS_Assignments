#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

enum
{
    NS_PER_SECOND = 1000000000
};

struct timespec start,finish,gap;

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

void genStrings(char strArr[50][4]){
    srand(time(NULL));
    for (int i = 0; i < 50; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            strArr[i][j] = rand()%26 + 65;
        }
    }
}

void copy_strings(char buff[5][5], char src[50][4],int ind){
	for(int i=0;i<5;i++){
		for (int j=0; j<5; j++) {
            if (j==4) buff[i][j]=ind+i;
            else buff[i][j] = src[ind+i][j];
        }
	}
}

int main()
{   
    char strArr[50][4] = {{0}};
    char buffer[5];
    genStrings(strArr);
    int b = mkfifo("myfifo1",0666);
    if (!b) {
        perror("No FIFO\n");
        exit(1);
    }
	char toWrite[5][5];
    int index=0;
    int fd;

    clock_gettime(CLOCK_REALTIME, &start);
    while(index<50){
        fd = open("myfifo1", O_WRONLY);
        if (!fd) {
            perror("NOT OPEN\n");
            exit(1);
        }
        copy_strings(toWrite,strArr,index); 
        write(fd, toWrite, sizeof(toWrite));
        close(fd);
        fd = open("myfifo1", O_RDONLY);
        if (!fd) {
            perror("NOT OPEN\n");
            exit(1);
        }
        read(fd, &buffer, sizeof(buffer));
        printf("Received Max Index from P2: %d\n", buffer[4]);
        close(fd);
        index = buffer[4] + 1;
    }
    clock_gettime(CLOCK_REALTIME, &finish);
    sub_timespec(start, finish, &gap);
    printf("Time taken: FIFO-1 - %ld.%ld\n", gap.tv_sec, gap.tv_nsec);
    return 0;
}
