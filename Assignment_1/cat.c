#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void cat(int argc,char** argv, int op_b, int op_n){
    FILE *fp;
    const int bs = 4096;
    char buff[bs];

    if(argv[1])

    if(argc>1){
        fp = fopen(argv[argc-1],"rb");
        if(fp==NULL){
            fprintf(stderr,"%s: No such file or directory\n",argv[argc-1]);
            exit(1);
        }

    }
    int line=1;

    while(fgets(buff,bs,(fp==NULL?stdin:fp))){
        int l = strlen(buff);
        buff[l-1] = '\0';

        if(op_b){
            l = strlen(buff);
            if(l>=1){
                char * tmp = strdup(buff);
                buff[0] = '\0';
                sprintf(buff,"%*d\t",6,line++);
                strcat(buff,tmp);
            }
        }
        else if(op_n){
            char * tmp = strdup(buff);
            buff[0] = '\0';
            sprintf(buff,"%*d\t",6,line++);
            strcat(buff,tmp);
        }
        fprintf(stdout,"%s\n",buff);
    }
}

int main(int argc, char** argv){
    int op_b=0,op_n=0;

    if(argc==1){
      printf("Please name a file/directory to read.\n");
    }
    if(argv[1][0]=='-'){
        char f = argv[1][1];
        if(f=='b'){
            op_b++;
        }
        else if(f=='n'){
            op_n++;
        }
        else{
            printf("Option not available.\n");
            exit(1);
        }
    }
    cat(argc,argv,op_b,op_n);
    return 0;
}