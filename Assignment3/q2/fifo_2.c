#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

void printStrings(char buff[5][4]){
	printf("Strings read:\n");
    for (int i = 0; i <= 4; i++)
    {	
		for(int j=0;j<4;j++){
			printf("%c",buff[i][j]);
		}
        printf("\n");
        
    }
}

int main()
{   
    char read_buff[5][4];
	int ret;
    int fd, fd1;
    int index=0;
    if((fd = open("myfifo1",O_RDONLY))<0){
			perror("fifo1");
			exit(1);
	}
    if((fd1 = open("myfifo2",O_WRONLY))<0){
			perror("fifo2");
			exit(0);
		}
    while(index<50){
		if((ret = read(fd, read_buff,sizeof(read_buff)))<0){
			perror("error reading");
			exit(1);
		}
		index = read_buff[4][3];
		printStrings(read_buff);
		close(fd);
		if(ret = write(fd, read_buff[4], sizeof(read_buff[4])<0)){
			perror("error writing");
			exit(0);
		}
    }
    close(fd);
    close(fd1);
    return 0;
}
