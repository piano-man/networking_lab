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
int main()
{
	struct sockaddr_in adr;
	int sockfd;
	if((sockfd=socket(AF_INET,SOCK_DGRAM,0))==-1)
	{
		perror("socket");
		exit(-1);
	}
	

	adr.sin_family=AF_INET;
	adr.sin_port=htons(1234);
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


	struct sockaddr_in arr[24];
	int sz=0,i,j,flag;
	struct sockaddr_in ca;
	len=sizeof(struct sockaddr);

	char buffer[1024],s[1024];
	char name[24][1024];
	int x;
	int plc=0;
	while(1)
	{
		flag=0;
		int rd=recvfrom(sockfd,buffer,1024,0,(struct sockaddr *)(&ca),(socklen_t *)(&len));
		for(i=0;i<sz;i++)
		{
			if(arr[i].sin_port==ca.sin_port&&arr[i].sin_addr.s_addr==ca.sin_addr.s_addr)
			{
				x=i;
				flag=1;
				break;
			}
		}
		if(flag==0)
		{
			x=sz;
			strcpy(name[sz],buffer);
			arr[sz++]=ca;
			printf("%s joined chat!!\n",name[sz-1]);
			continue;
		}
		//printf("Received from %s message %s\n",name[i],buffer);
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
		j=0;
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
		for(i=0;i<sz;i++)
		{
			sendto(sockfd,ts,1024,0,(struct sockaddr *)(&arr[i]),len);
		}
	}
	close(sockfd);

	return 0;
}