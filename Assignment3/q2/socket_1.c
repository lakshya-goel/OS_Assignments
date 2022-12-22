#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define NAME "/tmp/sock"

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
    char toWrite[5][5];
    int index=0;
    int sock, msgsock;
    struct sockaddr_un server;
    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0)
    {
        printf("Make\n");
        exit(1);
    }
    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, NAME);
    if (bind(sock, (struct sockaddr *)&server, sizeof(struct sockaddr_un)))
    {
        perror("Bind\n");
        unlink(NAME);
        exit(1);
    }
    int option = 1;
    listen(sock, 5);
    msgsock = accept(sock, 0, 0);
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    if (msgsock == -1) {
        printf("Bruh\n");
    }
    clock_gettime(CLOCK_REALTIME, &start);
    while (index<50) {
            copy_strings(toWrite,strArr,index);
            write(msgsock, (void *)&toWrite, sizeof(toWrite));
            read(msgsock,buffer,sizeof(buffer));
            index = buffer[4]+1;
            printf("The recieved index from P2 is %d\n",index-1);
    }
    clock_gettime(CLOCK_REALTIME, &finish);
    sub_timespec(start, finish, &gap);
    printf("Time taken: Socket-1 - %ld.%ld\n", gap.tv_sec, gap.tv_nsec);
    close(msgsock);
    unlink(NAME);
    close(sock);
    return (0);
}
