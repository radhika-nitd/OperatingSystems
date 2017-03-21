#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>

#define P(s) semop(s,&pop,1)
#define V(s) semop(s,&vop,1)

	int ins,outs,*in,*out,*b,*sum,mutp,mutc;
	int full,empty,tsum;
	struct sembuf pop,vop;
	int shmid;

void producer(int j){
	int i;
	for(i=0;i<50;i++){
		P(mutp);
		P(empty);
	//	printf("in=%d    producing %d :: %d      and        ",in[0],j,b[in[0]] );
		b[in[0]] = i+1;
		printf("producer %d   producing :: %d\n",j,b[in[0]] );
		in[0] = (in[0]+1)%20;
		V(full);
		V(mutp);
		if(in[0]==0){
			sleep(1);
		}
	}
	exit(0);
}

void consumer(int n,int i){
	while(n--){
		P(mutc);
		P(full);
		sum[0] = sum[0] + b[out[0]];
		printf("\t\t\t\tconsumer %d   consuming :: %d\n",i,b[out[0]] );
		b[out[0]]=0;
		out[0] = (out[0]+1)%20;
		V(empty);
		V(mutc);
		if(out[0]==0){
			sleep(1);
		}
	}
	exit(0);
}
int main(){
	pop.sem_num = vop.sem_num = 0;
	pop.sem_flg = vop.sem_flg = 0;
	pop.sem_op = -1;
	vop.sem_op = 1;

	full = semget(IPC_PRIVATE,1,0777|IPC_CREAT);
	empty = semget(IPC_PRIVATE,1,0777|IPC_CREAT);
	mutp = semget(IPC_PRIVATE,1,0777|IPC_CREAT);
	mutc = semget(IPC_PRIVATE,1,0777|IPC_CREAT);

	shmid = shmget(IPC_PRIVATE,20*sizeof(int),0777|IPC_CREAT);
	tsum = shmget(IPC_PRIVATE,1*sizeof(int),0777|IPC_CREAT);
	ins = shmget(IPC_PRIVATE,1*sizeof(int),0777|IPC_CREAT);
	outs = shmget(IPC_PRIVATE,1*sizeof(int),0777|IPC_CREAT);

	b = shmat(shmid,0,0);
	sum = shmat(tsum,0,0);
	in = shmat(ins,0,0);
	out = shmat(outs,0,0);

	semctl(full,0,SETVAL,0);
	semctl(empty,0,SETVAL,20);
	semctl(mutp,0,SETVAL,1);
	semctl(mutc,0,SETVAL,1);

	int m,n;
	printf("enter m and n:: ");
	scanf("%d%d",&m,&n);
	int i,v;
	sum[0]=0;
	for(i=0;i<m;i++){
		if(!fork()){
			producer(i);
		}
	}

	for(i=0;i<n;i++){
		v=(50*m)/n;
		if(!fork()){
			consumer(v,i);
		}
	}
	while(wait(NULL)>0);
	printf("sum :: %d\n",sum[0] );

	shmdt(b);
	shmdt(sum);
	shmdt(in);
	shmdt(out);

	shmctl(shmid,IPC_RMID,NULL);
	shmctl(tsum,IPC_RMID,NULL);
	shmctl(ins,IPC_RMID,NULL);
	shmctl(outs,IPC_RMID,NULL);
	semctl(full,0,IPC_RMID,0);
	semctl(empty,0,IPC_RMID,0);
	return 0;
}
