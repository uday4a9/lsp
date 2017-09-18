#define _GNU_SOURCE
#include <sys/wait.h>
#include <sys/utsname.h>
#include <sched.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
					   } while (0)

#define STACK_SIZE (1024 * 1024)    /* Stack size for cloned child */

static int childFunc(void *arg)
{
    char *args[] = {"container-bash"};
    printf("childFunc pid : %ld\n", (long) getpid());
    printf("About to spawn bash shell\n");
    execvp("/bin/bash", args);// "container-bash");
}

int main(int argc, char **argv)
{
   char *stack;                    /* Start of stack buffer */
   char *stackTop;                 /* End of stack buffer */
   pid_t pid;
   struct utsname uts;

   /* Allocate stack for child */

   stack = malloc(STACK_SIZE);
   if (stack == NULL)
	   errExit("malloc");
   stackTop = stack + STACK_SIZE;  /* Assume stack grows downward */

   /* Create child that has its own UTS namespace;
	  child commences execution in childFunc() */

   //pid = clone(childFunc, stackTop, CLONE_NEWPID | SIGCHLD, "childFunc");
   pid = clone(childFunc, stackTop, CLONE_NEWPID|SIGCHLD, "childFunc");
   if (pid == -1)
	   errExit("clone");
   printf("clone() returned %ld\n", (long) pid);

   /* Parent falls through to here */


   /* Display hostname in parent's UTS namespace. This will be
	  different from hostname in child's UTS namespace. */

   if (uname(&uts) == -1)
	   errExit("uname");
   printf("uts.nodename in parent: %s\n", uts.nodename);

   if (waitpid(pid, NULL, 0) == -1)    /* Wait for child */
	   errExit("waitpid");
   printf("child has terminated\n");

   free(stack);

   exit(EXIT_SUCCESS);
} 
