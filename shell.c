#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
  while(1){
    char input[30];
    char *parse;
    struct rusage cmdinfo;
    char *token[10];
    int commandLen;

    pid_t pid;
    int status;
  
    printf("$> ");
    fgets(input, 30, stdin);
    commandLen = strlen(input);
    input[commandLen - 1] = '\0';
  
    parse = strtok(input, " ");
    for(int i = 0; i < commandLen; i++){
	token[i] = parse;
	parse = strtok(NULL, " ");
    }

    if(strcmp(input, "quit") == 0){
	break;
    }
  
    if ((pid = fork()) < 0){
	perror("fork failure");
	exit(1);
    }
    
    else if (pid == 0){
      if (execvp(token[0], token)) {
        perror("exec failed");
        exit(1);
      }
    }
    
    else {
	wait(&status);
	getrusage(RUSAGE_CHILDREN, &cmdinfo);
	printf("Cpu time used: %ld.%06ld sec\n", cmdinfo.ru_utime.tv_sec, cmdinfo.ru_utime.tv_usec);
	printf("Context Switches: %ld\n", cmdinfo.ru_nivcsw);
    }
  }
  printf("Exiting shell..\n");
  return 0;
}

