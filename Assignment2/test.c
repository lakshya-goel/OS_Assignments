#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>

int main(){
  float matrix1[4][4] = {{1.2, 2.3, 4.9, 5.5}, {2.4, 6.9, 4.3, 9.8}, {1.1, 1.3, 1.6, 3.2}, {5.2, 6.5, 7.8, 4.0}};

  float matrix2[4][4] = {{1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}};

  printf("Original matrices:\n");
  printf("Matrix 1: \n");
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      printf("%f ",matrix1[i][j]);
    }
    printf("\n");
  }
  printf("\n");
  printf("Matrix 2: \n");
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      printf("%f ",matrix2[i][j]);
    }
    printf("\n");
  }
  printf("\n");

  int call = syscall(SYS_kernel_2D_memcpy, matrix1, matrix2, 4);

  if(call<0){
    printf("Error: System call failure.\n");
    exit(1);
  }

  printf("Final matrices: \n");
  printf("Matrix 1: \n");
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      printf("%lf ",matrix1[i][j]);
    }
    printf("\n");
  }
  printf("\n");
  printf("Matrix 2: \n");
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      printf("%lf ",matrix2[i][j]);
    }
    printf("\n");
  }
  return 0;
}
