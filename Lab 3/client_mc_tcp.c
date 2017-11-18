#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h> 
#include <fcntl.h> 
#include <unistd.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
struct sockaddr_in adr;
int sockfd;
char buffer[1024],s[1024];
int f1,f2;
int len;
void *func1(void *arg)
{
	while(1)
	{
		if(f1==0)
		{
			scanf("%s",s);
			f1=1;
		}
	}
}
void *func2(void *arg)
{
	while(1)
	{
		if(f2==0)
		{
			int rd=read(sockfd,buffer,1024);
			f2=1;
		}
	}
}
int main()
{

	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket");
		exit(-1);
	}
	

	adr.sin_family=AF_INET;
	adr.sin_port=htons(1238);
	struct in_addr sin_addr;
	inet_aton("127.0.0.1",&sin_addr);
	adr.sin_addr=sin_addr;
	len=sizeof(struct sockaddr_in);
	bzero(&adr.sin_zero, 0);	
	
	if(connect(sockfd,(struct sockaddr *)(&adr),len)<0)
	{
		perror("connect");
		exit(-1);
	}
	
	scanf("%s",s);
	int z=send(sockfd,s,1024,0);
	//printf("%s %d\n",s,z);
	pthread_t tid1,tid2;
	pthread_create(&tid1,NULL,func1,(void *)1);
	pthread_create(&tid2,NULL,func2,(void *)1);

	while(1)
	{
		if(f1==1)
		{
			send(sockfd,s,1024,0);
			if(strcmp(s,"bye")==0)
			{
				break;
			}
			f1=0;
		}
		if(f2==1)
		{
			printf("%s\n",buffer);
			f2=0;
		}
	}
	close(sockfd);


	return 0;
}