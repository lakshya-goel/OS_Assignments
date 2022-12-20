#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

void genStrings(char strArr[50][5]){
    srand(time(NULL));
    for (int i = 0; i < 50; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            strArr[i][j] = rand()%26 + 65;
        }
    }
}

void printString(char strArr[5][5]){
    for (int i = 0; i < 5; i++)
    {
        printf("%s\n",strArr[i]);
        
    }
    
}

void copy_strings(char dest[5][5], char src[50][5],int ind){
	char buff[5];
	for(int i=0;i<5;i++){
		snprintf(buff, 5, "%s%d",src[ind+i], ind+i); // puts string into buffer
      		strcpy(dest[i], buff);
	}
}

int main()
{   
    char strArr[50][5] = {{0}};
    genStrings(strArr);
    mkfifo("myfifo",0666);
    char str1[1000];
	char toWrite[5][5];
    int index=0;
    int fd;
    while(index<50){
        fd = open("myfifo",O_WRONLY);
        copy_strings(toWrite,strArr,index);
        for (int i = 0; i < 5; i++)
        {	
		printf("%s\n",toWrite[i]);
            write(fd,toWrite[i],6);
        }
        close(fd);
        fd = open("myfifo",O_RDONLY);
        read(fd, str1,sizeof(str1));
        index = atoi(str1);
        printf("Last printed index: %d\n",index);
        index++;
        close(fd);
    }
    unlink("myfifo");
    return 0;
}
