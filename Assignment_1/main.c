#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <errno.h>
#include <pthread.h>

char* loc;

void type_prompt() {
    printf(":) ");
}

void slice(const char *str, char *result, size_t start, size_t end)
{
    strncpy(result, str + start, end - start);
}

int check_flag(char* par[]){
    if(par[1]==NULL){
        return 0;
    }
    char c = par[1][0];
    if(c=='-'){
        return 1;
    }
    return 0;
}

int read_cmd(char command[],char cmd[], char *par[]) {
    int count = 0, i = 0, j = 0;
    char **all_tokens = (char**)malloc(25*sizeof(char*));
    char *token = (char*)malloc(100*sizeof(char));

    for (;;) {
        int c = fgetc(stdin);
        command[count++] = (char) c;
        if (c == '\n') {
            break;
        }
    }
    if (count == 1) {
        return 0;
    }

    char s[] = " \n";
    token = strtok(command, s);

    while (token != NULL)
    {
        all_tokens[i] = strdup(token);
        token = strtok(NULL, s);
        i++;
    }

    strcpy(cmd, all_tokens[0]);
    //i represents num of tokens
    for (int j = 0; j < i; j++) {
        par[j] = all_tokens[j]; 
    }
    par[i] = NULL;
    free(all_tokens);
    free(token);
    return i;
}

void pwd(){
    char wd[260];
    if(!(getcwd(wd,sizeof(wd)))){
        perror("getcwd() error");
        return;
    }
    printf("%s",getcwd(wd,sizeof(wd)));
    printf("\n");
}

void cd(char* par[]){
    int i=1;
    if(check_flag(par)){
        i=2;
    }
    char* path = par[i];
    if((chdir(path))!=0){
        fprintf(stderr, "chdir: %s: %s\n", strerror(errno), path);
        return;
    }
}

void echo(char* par[], int n){
    int i=1;
    int op_n=0,op_e=0;
    if(check_flag(par)){
        if(par[1][1]=='n'){
            op_n=1;
        }
        else if(par[1][1]=='E'){
            op_e=1;
        }
        else{
            printf("Option not available.\n");
            exit(1);
        }
        i=2;
    }
    char * concatenated = (char*)malloc(1000*sizeof(char));
    while(par[i]!=NULL && i<n){
        strcat(concatenated," ");
        strcat(concatenated,par[i]);
        i++;
    }
    printf("%s",concatenated);
    if(!op_n){
        printf("\n");
    }
    free(concatenated);
}

void external_cmd_n(char* cmd,char* par[]){
    char* path = (char*)malloc(260 * sizeof(char));
    strcat(path,loc);
    strcat(path,"/");
    strcat(path,cmd);
    char* arg = par[0];
    char* arg1 = par[1];
    char* arg2 = par[2];
    if(fork()!=0){
        wait(NULL);
    }
    else{
        execl(path,arg,arg1,arg2,(char*)NULL);
    }
}

void* thread_func(void *command)
{
    char* com = (char *)command;
    if(system(com)<0){
        perror(com);
    }
}

void external_cmd_t(char* cmd,char* par[]){
    char* path = (char*)malloc(260 * sizeof(char));
    strcat(path,loc);
    strcat(path,"/");
    strcat(path,cmd);
    
    int i=1;
    char* com = (char*)malloc(260*sizeof(char));
    strcat(com,path);
    while(par[i]!=NULL){
        strcat(com," ");
        strcat(com,par[i]);
        i++;
    }
    pthread_t new;

    pthread_create(&new, NULL,thread_func,com);
    pthread_join(new,NULL);
}

void external_cmd(char* cmd,char* par[], char t){
    if(t=='n'){
        external_cmd_n(cmd,par);
    }
    else if(t=='t'){
        external_cmd_t(cmd,par);
    }
}

int main(){

    loc = (char*)malloc(260*sizeof(char));
    if(!(getcwd(loc,260))){
        perror("getcwd() error");
        exit(1);
    }

    printf(":) Welcome to Da Shell!\n");
    printf("\n");

    char command[1000];
    
    while(true){
        char t;
        char* cmd = (char*)malloc(100*sizeof(char));
        char **par = (char**) malloc(25*sizeof(char*));
        type_prompt();
        int n = read_cmd(command,cmd,par);
        if(n==0){
            continue;
        }
        if(strcmp(par[n-1],"&t")==0){
            par[n-1] = NULL;
            t = 't';
        }
        else{
            t = 'n';
        }
        if(strcmp(cmd,"cd")==0){
            cd(par);
        }
        else if(strcmp(cmd,"pwd")==0){
            pwd();
        }
        else if(strcmp(cmd,"echo")==0){
            echo(par,n);
        }
        else if(strcmp(cmd,"ls")==0){
            external_cmd(cmd,par,t);
        }
        else if(strcmp(cmd,"cat")==0){
            external_cmd(cmd,par,t);
        }
        else if(strcmp(cmd,"rm")==0){
            external_cmd(cmd,par,t);
        }
        else if(strcmp(cmd,"mkdir")==0){
            external_cmd(cmd,par,t);
        }
        else if(strcmp(cmd,"date")==0){
            external_cmd(cmd,par,t);
        }
        else if(strcmp(cmd,"halt")==0){
            break;
        }
        else{
            printf("Invalid command: Command not found\nPlease enter a valid command\n");
        }
        free(cmd);
        free(par);
    }
    return 0;
}