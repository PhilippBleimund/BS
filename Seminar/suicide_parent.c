
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

int main(){

  pid_t pid[3];
  int status[3];

  for (int i=0; i<3; i++){
  
    if ((pid[i] = fork()) == 0){
      // child
      while(1);
    }
  }
  
	_exit(0);
}
