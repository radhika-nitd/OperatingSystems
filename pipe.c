#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<time.h>
#include<stdlib.h>
#include<wait.h>
#define BUFSIZE 80 
void main()
{
     int a[100],n,i,search,status;
     int fd[2];
     char line[BUFSIZE];
     
     printf("Enter the number of elements:");
     scanf("%d",&n);
     for(i=0;i<n;i++)
          scanf("%d",&a[i]);
     printf("Enter the number to be searched:");
     scanf("%d",&search);
     pipe(fd);
     pid_t pid1=fork();
     if (pid1 == 0) 
     {
	close(fd[0]); /* The child will not read and
hence we close fd[0]
*/
        for(i=0;i<n/2;i++)
        { 
            if(a[i]==search)
                  break;
         }
          if(i<n/2)
          {
                 sprintf(line,"%d",i);
                  write(fd[1], line, BUFSIZE);
           }
         else
         {
            	i=-1;
            	sprintf(line,"%d",i);
          	write(fd[1], line, BUFSIZE);
          }
          exit(1);
      }
     else
     {
             pid_t pid2=fork();
             if (pid2 == 0) 
             {
		close(fd[0]); /* The child will not read and
hence we close fd[0]
*/
                for(i=n/2;i<n;i++)
                { 

                           if(a[i]==search)
                                  break;
                  }
                if(i<n)
                {
                         sprintf(line,"%d",i);
          		write(fd[1], line, BUFSIZE);
                 }
               else
                 {
                          i=-1;
                         sprintf(line,"%d",i);
                          write(fd[1], line, BUFSIZE);
                  }
                exit(2);
               }
             
	         else
                  {   wait(&status);
                       wait(&status);
         
                     close(fd[1]);
                     read(fd[0], line, BUFSIZE);
                      sscanf(line,"%d",&n);
                      if(n!=-1)
                       {
                              printf("value found in the first half at index:%d\n",n);
                       }
                       read(fd[0], line, BUFSIZE);
                       sscanf(line,"%d",&n);
                       if(n!=-1)
                       {
                                     printf("value found in the second half at index:%d\n",n);
                       }
                  }
           }
}
