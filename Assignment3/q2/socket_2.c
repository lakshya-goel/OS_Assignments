#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
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

int main()
{
    char read_buff[5][5];
    int it;
    int sock;
    struct sockaddr_un server;
    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("opencsocket");
        exit(1);
    }
    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, "/tmp/sock");
    if (connect(sock, (struct sockaddr *)&server, sizeof(struct sockaddr_un)) < 0)
    {
        close(sock);
        perror("connect socket");
        exit(1);
    }

	clock_gettime(CLOCK_REALTIME, &start);

    while(it<50) {
        read(sock,(void*)&read_buff,sizeof(read_buff));
        printStrings(read_buff);
        char buffer[5];
        for (int i=0; i<5; i++) {
            buffer[i] = read_buff[4][i];
        }
        it = read_buff[4][4];
        write(sock,buffer,sizeof(buffer));
    }

	clock_gettime(CLOCK_REALTIME, &finish);
    sub_timespec(start, finish, &gap);
    printf("Time taken: Socket-2 - %ld.%ld\n", gap.tv_sec, gap.tv_nsec);

    close(sock);
    return (0);
}
