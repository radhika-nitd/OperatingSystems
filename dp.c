#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/wait.h>

int n,count,lr[15];
struct sembuf pop,vop;

#define p(s) semop(s,&pop,1)
#define v(s) semop(s,&vop,1)

void philo(int i)
{

p(count);

p(lr[i]);
printf("PHILOSOPHER %d GOT LEFT FORK ",i);
sleep(1);

p(lr[(i+1)%n]);
printf("\t\t\t PHILOSOPHER %d GOT RIGHT FORK \n",i);
sleep(1);

printf("*****************************PHILOSOPHER %d Dininig******************************* \n",i);
sleep(1);

v(lr[i]);
printf("PHILOSOPHER %d RELEASED LEFT FORK ",i);

v(lr[(i+1)%n]);
printf("\t\tPHILOSOPHER %d RELEASED RIGHT FORK \n",i);

v(count);

exit(0);
}

int main()
{
int i;
pop.sem_num=0;
pop.sem_flg=0;
pop.sem_op=-1;

vop.sem_num=0;
vop.sem_flg=0;
vop.sem_op=1;

printf("enter no of philosopher ");
scanf("%d",&n);

count=semget(IPC_PRIVATE,1,0777|IPC_CREAT);
semctl(count,0,SETVAL,n-1);

for(i=0;i<n-1;i++)
{
	lr[i]=semget(IPC_PRIVATE,1,0777|IPC_CREAT);
	semctl(lr[i],0,SETVAL,1);
}


for(i=0;i<n;i++){
if(!fork())
philo(i);
}

while(wait(NULL)>0);
semctl(count,0,IPC_RMID,0);
for(i=0;i<n-1;i++)
semctl(lr[i],0,IPC_RMID,0);


}
