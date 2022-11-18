#include <stdio.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdlib.h>

void date(int argc, char* argv[],int op_u, int op_r)
{   

  int i =1;
  if(op_r || op_u){
      i=2;
  }

  if(op_r){
    char * name = argv[i];
    struct stat f_stat;

    if(stat(name,&f_stat)<0){
        perror(name);
        exit(1);
    }

    printf("%s",ctime(&f_stat.st_mtime));
    exit(1);
  }

  time_t rawtime = time(NULL);
  if (rawtime == -1)
  {
      perror("Error");
      return;
  }
  struct tm *ptm;
  char buf1[4], buf2[4], buf3[6];
  ptm = localtime(&rawtime);
  if(op_u){
    ptm = gmtime(&rawtime);
  }
  if (ptm == NULL)
  {
      perror("error");
      return;
  }
  strftime(buf1, 4, "%a", ptm);
  strftime(buf2, 4, "%b", ptm);
  strftime(buf3, 6, "%z", ptm);
  printf("%s %s %02d %02d:%02d:%02d %s %d\n", buf1, buf2, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, buf3, ptm->tm_year + 1900);
}

int main(int argc, char** argv){
  int op_u=0,op_r=0;

  if(argv[1][0]=='-'){
      char f = argv[1][1];
      if(f=='u'){
          op_u++;
      }
      else if(f=='r'){
          op_r++;
      }
      else{
          printf("Option not available.\n");
          exit(1);
      }
  }
  date(argc,argv,op_u,op_r);
  return 0;
}