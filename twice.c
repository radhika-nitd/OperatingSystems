#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include<stdio.h>
int main(int argc, char *argv[])
{
	char *str=argv[argc-2],conct[100];
	printf("\nProcess id is %d\n",getpid());
	int i=0,p,r=0;

	/*while(str[i]!='\0')
	{
 		p=str[i];
		p=p-48;

		r=r*10+p;
		i++;
	}*/
	r=atoi(str);	
   	r=2*r;
	strcpy(conct,"./");
	strcat(conct,argv[1]);
	
	if(argc==3)
	{
		printf("%d\n",r);
		exit(0);
	}
	else
	{
		sprintf(str,"%d",r);
		argv[argc-2]=str;
		argv[1]=conct;
		argv++;
		execvp(conct,argv);
	}
return 0;
}	
