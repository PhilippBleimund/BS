
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
      _exit(i);
    }
  }
  
  // parent
  for (int i=0; i<3; i++){
    waitpid(pid[i], &status[i],0);
	  if (WIFEXITED(status[i])){
		  printf("[process exited] (%d) [status = %d]\n", pid[i], (status[i]));
	  }
	}
}
