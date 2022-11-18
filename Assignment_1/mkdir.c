#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

void slice(const char *str, char *result, size_t start, size_t end)
{
    strncpy(result, str + start, end - start);
}

void mk(int argc, char* argv[],int op_v, int op_p){
    int i =1;
    if(op_p || op_v){
        i=2;
    }
    
    int s;

    if(op_p){
        char** toks=malloc(10*sizeof(char*));
        char* tok;
        tok = strtok(argv[i], "/");
        int j=0;
        while (tok != NULL)
        {   
            toks[j] = strdup(tok);
            tok = strtok(NULL, "/");
			j++;
        }
        int k=0;
        while(toks[k]!=NULL){
            s=mkdir(toks[k],0777);
            int r = chdir(toks[k]);
            k++;
        }
        argv[i]=toks[k-1];
    }
    else{
        s = mkdir(argv[i],0777);
    }
    if(s==-1){
        perror(argv[i]);
    }
    else if(op_v){
        printf("mkdir : created directory %s\n",argv[i]);
    }
}

int main(int argc, char** argv){
    int op_v=0,op_p=0;

    if(argc<2){
        printf("mkdir: missing operand.\n");
        exit(1);
    }

    if(argv[1][0]=='-'){
        char f = argv[1][1];
        if(f=='v'){
            op_v++;
        }
        else if(f=='p'){
            op_p++;
        }
        else{
            printf("Option not available.\n");
            exit(1);
        }
    }
    mk(argc,argv,op_v,op_p);
    return 0;
}