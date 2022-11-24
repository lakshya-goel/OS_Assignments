#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sched.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

enum{NS_PER_SECOND = 1000000000};
pid_t pid_1, pid_2, pid_3;
struct timespec s1,s2,s3,f1,f2,f3,t1,t2,t3;

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

void genProcess(char* file_addr, int priority, pid_t * pid, struct timespec s){
    clock_gettime(CLOCK_REALTIME,&s);
    pid_t gen_id = fork();
    if(gen_id==0){
        setpriority(0,PRIO_PROCESS,priority);
        execlp(file_addr,NULL);
        perror("execlp error");
        exit(1);
    }
    else if(gen_id==-1){
        perror("fork error");
        exit(1);
    }
    *pid = gen_id;
    return;
}

void waitProcess(struct timespec f, pid_t * processes, char** file,struct timespec tm,struct timespec s[]){
    pid_t id = waitpid(-1,NULL,0);
    clock_gettime(CLOCK_REALTIME,&f);
    for (int i = 0; i < 3; i++)
    {	
        if(id==processes[i]){
            sub_timespec(s[i],f,&tm);
            printf("File: %s Time: %ld.%.9ld\n",file[i],(tm.tv_sec)/1000000000,tm.tv_nsec);
        }
    }
}

int main(int argc, char const *argv[])
{	
	int priorities[] = {20,1,40};
	pid_t process_ids[] = {pid_1,pid_2,pid_3};
	char* file_addresses[] ={"./first.sh","./second.sh","./third.sh"};
	
	struct timespec s[] = {s1,s2,s3};
	struct timespec f[] = {f1,f2,f3};
	struct timespec tm[] = {t1,t2,t3};
	
    for (int i = 0; i < 3; i++)
    {
        genProcess(file_addresses[i],priorities[i],&(process_ids[i]),s[i]);
    }
    for (int i = 0; i < 3; i++)
    {
        waitProcess(f[i],process_ids,file_addresses,tm[i],s);
    }

    return 0;
}
