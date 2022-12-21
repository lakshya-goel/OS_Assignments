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

void printStrings(char buff[5][5]){
	printf("Strings read from P1:\n");
    for(int i=0;i<5;i++){
		    for (int j=0; j<4; j++) {
                printf("%c", buff[i][j]);
            }
            printf("\n");
	    } 
}

int main() {   
    char read_buff[5][5];
    int fd;
    int index=0;
	clock_gettime(CLOCK_REALTIME, &start);
    while(index<10) {
        fd = open("myfifo1",O_RDONLY);
        if (!fd) {
            perror("NOT OPEN\n");
            exit(1);
        }
		read(fd, &read_buff, sizeof(read_buff));
		printStrings(read_buff);
        close(fd);
        char buffer[5];
        for (int i=0; i<5; i++) {
            buffer[i] = read_buff[4][i];
        }
        fd = open("myfifo1", O_WRONLY);
        if (!fd) {
            perror("NOT OPEN\n");
            exit(1);
        }
        write(fd, buffer, sizeof(buffer));
        close(fd);
        index++;
	}
	clock_gettime(CLOCK_REALTIME, &finish);
    sub_timespec(start, finish, &gap);
    printf("Time taken: FIFO-2 - %ld.%ld\n", gap.tv_sec, gap.tv_nsec);
}
