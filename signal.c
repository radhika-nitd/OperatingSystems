/*
ROLL NO : 12/CS/09,12/CS/08
NAME    : T SANGEETA, SAMADRITA PAL
GROUP NO: 4
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

childSigHandler(int sig)
{
  if (sig == SIGALRM)
 {
   printf("Signal Received %d \n",sig);
    printf("Parent recieved signal");
 }
 else if (sig == SIGCHLD)
 {
   printf("Signal Received %d \n",sig);
    printf("Child recieved signal");
 }
exit(0);
}

int main ()
{
 int pid;

 pid = fork();
 if (pid)
 {
   /* Parent process */
   signal(SIGCHLD, childSigHandler);
   sleep(1);
    kill(pid, SIGALRM);
   /* Send signal to child */

   while(1)
   printf("Parent waitng\n");

}
 else {
       /* Child process */
       signal(SIGALRM, childSigHandler);
       /* Register SIGUSR1 handler*/

       while (1)
       printf("Child waiting\n");


       /* Sleep until a signal is received from parent*/
      }
exit(0);
}

