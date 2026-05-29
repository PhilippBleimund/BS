
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
      if (i == 1){
        while(1);
      }else{
        _exit(0);
      }
    }
  }
  
  // parent
  sleep(5);
  for (int i=0; i<3; i++){
    kill(pid[i], SIGKILL);
  }
  
  for (int i=0; i<3; i++){
    waitpid(pid[i], &status[i],0);
    if (WIFEXITED(status[i])){
		  printf("[process exited] [status = %d]\n", WEXITSTATUS(status[i]));
    }else if (WIFSIGNALED(status[i])) {
		  printf("[process (%d) killed by signal] [status = %d]\n", pid[i], WTERMSIG(status[i]));
	  }
	}
}
