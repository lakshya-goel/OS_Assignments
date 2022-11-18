#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void rm(int argc, char *argv[],int op_i,int op_d) 
{   
    int i =1;
    if(op_d || op_i){
        i=2;
    }
    int fd;
    fd=open(argv[i],O_RDONLY);
    if(fd!=-1){
        close(fd);
        if(op_i){
            char ch;
            printf("Are you sure you wish to remove/delete: %s?",argv[i]);
            if (scanf("%c", &ch) != 1) {
                printf("Failed to read your answer.\n");
            }
            if(ch=='y' || ch=='Y'){
            }
            else{
                exit(1);
            }
        }
        int s = unlink(argv[i]);
        if(s==0){
            exit(1);
        }
        else{
            if(op_d){
                if(remove(argv[i])!=0){
                    perror(argv[i]);
                }
            }
            else{
                perror(argv[i]);
            }
        }
    }
    else{
        perror(argv[i]);
    }
}

int main(int argc, char** argv){
    int op_i=0,op_d=0;

    if(argc<2){
        printf("Need a valid file/directory name to delete.\n");
        exit(1);
    }

    if(argv[1][0]=='-'){
        char f = argv[1][1];
        if(f=='i'){
            op_i++;
        }
        else if(f=='d'){
            op_d++;
        }
        else{
            printf("Option not available.\n");
            exit(1);
        }
    }
    rm(argc,argv,op_i,op_d);
    return 0;
}