#include<stdio.h>
#include<stdlib.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<pthread.h>
#define p(s) semop(s,&pop,1)
#define v(s) semop(s,&vop,1)

int r,w,i,j,rd,wr,shmid,*rc;
struct sembuf pop,vop;

void writer(int +x)
{
p(wr);
printf("WRITER %d WRITTING \n",x);
v(wr);
exit(0);
}

void reader(int x)
{
p(rd);
printf("READER %d ENTERS\n",x);
rc[0]++;
if(rc[0]==1)
	{//printf("writer  %d waiting \n",x);
		p(wr);}

printf("READER %d READING \n",x);
v(rd);


p(rd);
printf("READER %d LEAVES \n",x);
rc[0]--;
if(rc[0]==0)
	{ //printf("writer  %d awake \n",x);
	v(wr);}
v(rd);	
exit(0);

}

int main()
{

pop.sem_op=-1;
vop.sem_op=1;
pop.sem_num=0;
vop.sem_num=0;
pop.sem_flg=0;
vop.sem_flg=0;

printf("Enter no of writers and readers");
scanf("%d%d",&w,&r);

rd=semget(IPC_PRIVATE,1,0777|IPC_CREAT);
wr=semget(IPC_PRIVATE,1,0777|IPC_CREAT);

semctl(rd,0,SETVAL,1);
semctl(wr,0,SETVAL,1);


shmid=shmget(IPC_PRIVATE,2*sizeof(int),0777|IPC_CREAT);
rc=(int*)shmat(shmid,0,0);
rc[0]=0;

for(i=0;i<w;i++)
{
if(!fork())
writer(i);
}

for(j=0;j<r;j++)
{
if(!fork())
reader(j);
}

while(wait(NULL)>0);

shmdt(rc);

shmctl(shmid,IPC_RMID,0);
semctl(rd,0,IPC_RMID,0);
semctl(wr,0,IPC_RMID,0);


}

