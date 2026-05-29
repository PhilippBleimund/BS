/*
 * ===========================================================================
 *
 * tsh.cpp --
 *
 * ===========================================================================
 */


#include "makeargv.h"

#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/wait.h>
#include <signal.h>

// maximal length of user input
#define MAX_COMMAND_LINE_LENGTH 1024

// exit handler
int foreground_process_id = 0;

// internal process id counter
int ipid = 0;

/* type definitions **************************************************************/

// constants for job status
typedef enum {
  JOB_RUNNING, JOB_FINISHED
} job_status_t;

// job descriptor
typedef struct {
  char* command;            // c-string containing the original command
  int internal_id;          // job id (shell-internal)
  pid_t pid;                  // process id of the job (from OS)
  int exit_status;          // exit status of the job
  job_status_t job_status;  // status of the job
} job_descr_t;

/* global variables **************************************************************/

// list of all jobs ever started in the shell;
// list elements are POINTERS to job descriptors
static std::vector<job_descr_t*> job_list;



/* functions ********************************************************************/

// force exit handler. On normal opperation close tsh itself.
// when an foreground process is running SIGINT is send to the process
void exit_handler(int sig){
	if (foreground_process_id != 0){
		job_descr_t* process = job_list[foreground_process_id];
		if (process){
			process->exit_status = kill(process->pid, SIGINT);
		}
	}else{
		printf("force exit tsh\n");
		_exit( 0 );
	}
}

// print info on how to use tsh
void tsh_out_info(){
	printf("use info or help to print this\n");
}

// function for printing all processes and their status of the current session
void tsh_out_list(){
	for(unsigned i=0; i<job_list.size(); i++){
		job_descr_t* curr_job = job_list[i];
		printf("%3i | pid=%5i , exit=%3i | %s\n", curr_job->internal_id, curr_job->pid, curr_job->exit_status, curr_job->command);
	}
}

pid_t tsh_start_process(char** argv, int first_token, int numtokens){
	job_descr_t* new_process = (job_descr_t*) malloc(sizeof(job_descr_t));
	job_list.push_back(new_process);
	
	// copy command to new_process
	new_process->command = (char*) malloc((MAX_COMMAND_LINE_LENGTH+1)*sizeof(char));
	strcpy(new_process->command, argv[0]);
	/*for(int i=1; i<numtokens; i++){
		strcat(new_process.command, argv[i]);
	}*/
	
	new_process->internal_id = ++ipid;
	new_process->exit_status = JOB_RUNNING;
	
	// create new fork
	int status;
	int child_status;
	if ((new_process->pid = fork()) == 0){
		// inside child execute given command
		child_status = execvp(argv[0], argv);
		_exit(child_status);
	}else{
		// set for exit handler
		foreground_process_id = new_process->internal_id;
		waitpid(-1, &status,0);
		// unset for exit handler
		foreground_process_id = 0;
		new_process->exit_status = status;
		if (WIFEXITED(status)){
			if (WEXITSTATUS(status) == 255){
				printf("[command not found (or general execution error)] [status = %d]\n", WEXITSTATUS(status));
			}
		} else if (WIFSIGNALED(status)) {
			printf("[process killed by signal] [status = %d]\n", WTERMSIG(status));
		} else if (WIFSTOPPED(status)) {
			printf("[process stoped by signal] [status = %d]\n", WSTOPSIG(status));
		} else{
			printf("exited other way\n");
		}
	}
	
	return new_process->pid;
}

// start foreground job. The shell is blocking while the process is running.
// Emergency exit through ctrl+c


// function for the processing of the user input;
// shows prompt and waits for user input;
// user input is parsed and appropriate action is taken...
void
tsh_prompt_and_process()
{
  // buffer for user input
  static char buffer[MAX_COMMAND_LINE_LENGTH+1];

  char** argv_intern = NULL;  // holds the user input after separation into tokens
  int numtokens = 0;          // number of tokens

  // show prompt and wait for user input
  printf( "tsh> " ); fflush( stdout );
  fgets( buffer, MAX_COMMAND_LINE_LENGTH, stdin );
  
  // separate char string in buffer (user input) into single tokens
  numtokens = makeargv( buffer, " \n", &argv_intern );

  // ======================================================================
  // BEGIN: HERE GOES YOUR CODE! 
  // ======================================================================
  
  // TODO: REMOVE THIS FOR LOOP AFTER YOUR SOLUTION IS FINISHED
  for( int i=0; i < numtokens; i++ ) {
    printf( "%s\n", argv_intern[i] );
  }

  // check if at least one token was entered at the command line
  // (if not: do nothing)
  if( numtokens > 0 )
  {
      if( (strcmp( "quit", argv_intern[0] ) == 0) || (strcmp( "exit", argv_intern[0] ) == 0))
      {
          // leave tsh
          _exit( 0 );
      }
      else if( (strcmp( "info", argv_intern[0] ) == 0) || (strcmp( "help", argv_intern[0] ) == 0))
      {
          tsh_out_info();
      }
      else if( strcmp( "list", argv_intern[0] ) == 0 )
      {
          tsh_out_list();
      }
      else if( strcmp( "wait", argv_intern[0] ) == 0 )
      {
          // TODO: Here goes your code for the wait command
          // (good idea: put it into an extra function)
      }
      else if( strcmp( "kill", argv_intern[0] ) == 0 )
      {
          // TODO: Here goes your code for the kill command
          // (good idea: put it into an extra function)
      }
      else if( strcmp( "job", argv_intern[0] ) == 0 )
      {
          // TODO: Here goes your code for the job command
          // (good idea: put it into an extra function)
      }
      else
      {
          tsh_start_process(argv_intern, 0, numtokens);
      }
  }

  // ======================================================================
  // END: HERE GOES YOUR CODE! 
  // ======================================================================
  
  // clean up
  if( argv_intern )
    freemakeargv( argv_intern );

  return;
}


/* main **************************************************************************/

int
main( int argc, char **argv )
{
	// init SIGINT handler
	signal(SIGINT, exit_handler);

  // init lists
  job_list.resize(0);

  // main loop
  while( 1 ) {
    // process user input
    tsh_prompt_and_process();
  }

  _exit( 0 );
}
