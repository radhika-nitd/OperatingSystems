#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/wait.h>

#define p(s) semop(s,&pop,1)
#define v(s) semop(s,&vop,1)

struct sembuf pop,vop;  //sembuf as used in semop()
int n,m,full,empty,mutex,v,mutexc,shmid,shmid1,shmid2,shmid3;
int *b,*sum,*in,*out;

void producer(int i)
{
	 p(mutex);//allows only one producer at a time
	int j;
	for(j=0;j<50;j++)
	{
		
		p(empty); //checks if the buffer is empty
		b[in[0]]=j+1;
		printf("PRODUCER %d WRITTING ---->%d\n",i,b[in[0]]);
		in[0]=(in[0]+1)%20;
		v(full);  //signals that buffer is incremented by one
		
		if(in[0]==0)
			sleep(1);
		
	}v(mutex); //signals that work of producer is over
	exit(0);	
}


void consumer(int y,int i)
{
	
	p(mutexc); //allows only one consumer at a time
	while(y--)
	{
		
		p(full);	
		
		sum[0]=sum[0]+b[out[0]];
		printf("\t\tconsumer %d reading ---->%d\n",i,b[out[0]]);
		b[out[0]]=0;
		out[0]=(out[0]+1)%20;
		
		v(empty);  //signals that buffer is incremented by one
		
		
		if(out[0]==0)
		sleep(1);
			
	}v(mutexc); //signals that work of producer is over
	exit(0);
	
}


int main()
{

printf("enter the value of m and n: ");
scanf("%d%d",&m,&n);

//allocate shared memory...........................
shmid=shmget(IPC_PRIVATE,20*sizeof(int),0777|IPC_CREAT);
shmid1=shmget(IPC_PRIVATE,1*sizeof(int),0775|IPC_CREAT);
shmid2=shmget(IPC_PRIVATE,1*sizeof(int),0666|IPC_CREAT);
shmid3=shmget(IPC_PRIVATE,1*sizeof(int),0776|IPC_CREAT);


//attach shared memory.............................
b=shmat(shmid,0,0);
sum=shmat(shmid1,0,0);
in=shmat(shmid2,0,0);
out=shmat(shmid3,0,0);


//allocate semaphores..............................
full=semget(IPC_PRIVATE,1,0767|IPC_CREAT);
empty=semget(IPC_PRIVATE,1,0770|IPC_CREAT);
mutex=semget(IPC_PRIVATE,1,0771|IPC_CREAT);
mutexc=semget(IPC_PRIVATE,1,0772|IPC_CREAT);


//initialize sembuf values.........................
pop.sem_num=0;vop.sem_num=0;
pop.sem_flg=0;vop.sem_flg=0;
pop.sem_op=-1;
vop.sem_op=1;

//initialize semaphore values .....................
semctl(full,0,SETVAL,0);
semctl(empty,0,SETVAL,20);
semctl(mutex,0,SETVAL,1);
semctl(mutexc,0,SETVAL,1);

sum[0]=0; //initialze sum value as 0
//in[0]=0;
//out[0]=0;
//pid_t pid=fork();
int i,j;
for(i=0;i<m;i++)
{
if(!fork())
producer(i);
}

for(j=0;j<n;j++)
{v=(50*m)/n;
if(!fork())
consumer(v,j);
}

while(wait(NULL)>0);
printf("sum is : %d",sum[0]);

//detach from shared memory...........................
shmdt(b);
shmdt(sum);
shmdt(in);
shmdt(out);

//delete shared memory................................
shmctl(shmid,IPC_RMID,0);
shmctl(shmid1,IPC_RMID,0);
shmctl(shmid2,IPC_RMID,0);
shmctl(shmid3,IPC_RMID,0);

//delete semaphores...................................
semctl(full,0,IPC_RMID,0);
semctl(empty,0,IPC_RMID,0);
semctl(mutex,0,IPC_RMID,0);
semctl(mutexc,0,IPC_RMID,0);




}
