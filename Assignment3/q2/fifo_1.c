#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

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

void copy_strings(char dest[5][5], char src[50][4],int ind){
	char buff[5];
	for(int i=0;i<5;i++){
		for (int j=0; j<5; j++) {
            if (j==4) buff[j]=ind+i;
            else buff[j] = src[ind+i][j];
        }
      	strcpy(dest[i], buff);
	}
}

int main()
{   
    char strArr[50][4] = {{0}};
    genStrings(strArr);
    mkfifo("myfifo1",0666);
	mkfifo("myfifo2",0665);
    char read_buff[5];
	char toWrite[5][5];
	int len,ret;
    int index=0;
    int fd;
    while(index<50){
		if((fd = open("myfifo1",O_WRONLY))<0){
			perror("fifo1");
			exit(0);
		}
        copy_strings(toWrite,strArr,index);
        write(fd, toWrite, sizeof(toWrite));
        close(fd);
        if((fd = open("myfifo2",O_RDONLY))<0){
			perror("fifo2");
			exit(0);
		}
		if((ret = read(fd, read_buff,sizeof(read_buff)))<0){
				perror("error reading");
				exit(0);
			}
        index = read_buff[4];
        printf("Last printed index: %d\n",index);
        index++;
        close(fd);
    }
    unlink("myfifo1");
	unlink("myfifo2");
    return 0;
}
