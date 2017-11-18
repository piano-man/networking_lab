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
int arr[24];
int sockfd;
int sz=0;
int flag[24];
char name[24][1024];
char buffer[1024];
int plc=0;
void *fun1(void *arg)
{
	while(1)
	{
		int i,x,ns;
		int rd=0;
		int *p=arg;
		ns=p;
		//printf("About to read from %d\n",ns);
		while(rd==0)
		{
			rd=read(ns,buffer,1024);
		}

		//printf("%s %d\n",buffer,rd);
		for(i=0;i<sz;i++)
		{
			if(arr[i]==ns)
			{
				x=i;
				break;
			}
		}
		if(flag[x]==0)
		{
			strcpy(name[x],buffer);
			printf("%s joined chat!!\n",name[x]);
			flag[x]++;
			continue;
		}
		if(strcmp(buffer,"bye")==0)
		{
			printf("%s left chat!!\n",name[x]);
			plc++;
		}
		if(plc==sz&&plc)
		{
			break;
		}
		char ts[1024];
		int j=0;
		for(i=0;i<strlen(name[x]);i++)
		{
			ts[j++]=name[x][i];
		}
		ts[j++]=':';
		ts[j++]=' ';
		for(i=0;i<strlen(buffer);i++)
		{
			ts[j++]=buffer[i];
		}
		ts[j]='\0';
		//printf("%s\n",ts);

		for(i=0;i<sz;i++)
		{
			if(arr[i]!=ns)
			{
				send(arr[i],ts,1024,0);
			}
			
		}
	}

}
void *func(void *arg)
{
	while(1)
	{
	struct sockaddr_in clientadr;
	int len=sizeof(clientadr);
	int ns;
	if((ns=accept(sockfd,(struct sockaddr *)(&clientadr),(socklen_t *)(&len)))<0)
	{
		perror("accept");
		exit(-1);
	}
	//printf("accept %d\n",ns);
	arr[sz++]=ns;
	pthread_t tid;
	//int *ptr=&ns;
	tid=pthread_create(&tid,NULL,fun1,(void *)(ns)); 
	//printf("jhfhkvdfh\n");
	}
}
int main()
{
	struct sockaddr_in adr;
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
	int len=sizeof(struct sockaddr_in);
	bzero(&adr.sin_zero, 0);	

	if((bind(sockfd,(struct sockaddr *)(&adr),len))==-1)
	{
		perror("Bind");
		exit(-1);
	}
	
	if((listen(sockfd,2))==-1)
	{
		perror("listen");
		exit(-1);
	}
	
	pthread_t tid;
	pthread_create(&tid,NULL,func,(void *)1); 


	int i,j,flag;
	struct sockaddr_in ca;
	len=sizeof(struct sockaddr);
	
	char s[1024];
	int x;
	int ns;
	while(1)
	{
		//printf("Received from %s message %s\n",name[i],buffer);
		if(plc==sz&&plc)
		{
			break;
		}

	}
	close(sockfd);

	return 0;
}