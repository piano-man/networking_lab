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
char name[1024];
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
		int gb=0;
		if(f2==0)
		{
			struct sockaddr_in ca;
			int len=sizeof(struct sockaddr);
			int rd=recvfrom(sockfd,buffer,1024,0,(struct sockaddr *)(&adr),(socklen_t *)(&len));
			int i,l=strlen(buffer),fld=0;
			char fn[1024];
			if(l>12)
			{
				char ts[20];
				int j;
				for(j=0;j<l;j++)
				{
					for(i=j;i<12+j;i++)
					{
						ts[i-j]=buffer[i];
					}
					ts[i-j]='\0';
					if(strcmp(ts,"Sending File")==0)
					{
						fld=1;
					}
				}
			}
			//printf("%d\n",fld);
			if(fld==1)
			{
					char ttt[1024];
					int sttt=0;
					for(i=0;i<l;i++)
					{
						if(buffer[i]==':')
						{
							break;
						}
						ttt[sttt++]=buffer[i];
					}
					ttt[sttt]='\0';
					if(strcmp(name,ttt)==0)
					{
						gb=1;
					}
					i+=2;
					i+=12;
					int jj=0;
					for(;i<l;i++)
					{
						fn[jj++]=buffer[i];
					}
					fn[jj]='\0';
					//printf("%s\n",fn);
					char sst[1024];
					strcpy(sst,name);
					strcat(sst,"Recived file ");
					strcat(sst,fn);
					strcpy(buffer,sst);
					FILE *fptr;
					if(gb==0)
					{
						fptr=fopen(buffer,"w+");
					}
					while(1)
					{
						recvfrom(sockfd,buffer,1024,0,(struct sockaddr *)(&ca),(socklen_t *)(&len));
						for(i=0;i<strlen(buffer);i++)
						{
							if(buffer[i]==':')
							{
								break;
							}
						}
						i++;
						sst[0]='\0';
						i++;
						int jj=0;
						for(;i<strlen(buffer);i++)
						{
							sst[jj++]=buffer[i];
						}
						sst[jj]='\0';
						strcpy(buffer,sst);
						if(strcmp(buffer,"Closing File")==0)
						{
							break;
						}
						if(gb==0)
						{
							fprintf(fptr,"%s\n",buffer);
						}
					}
					if(gb==0)
					{
						fclose(fptr);
					}
					sst[0]='\0';
					strcat(sst,"Recived File from ");
					strcat(sst,ttt);
					strcat(sst," ");
					strcat(sst,fn);
					buffer[0]='\0';
					strcpy(buffer,sst);
			}
			if(gb==0)
			{
				f2=1;
			}
		}
	}
}
int main()
{

	if((sockfd=socket(AF_INET,SOCK_DGRAM,0))==-1)
	{
		perror("socket");
		exit(-1);
	}
	

	adr.sin_family=AF_INET;
	adr.sin_port=htons(1235);
	struct in_addr sin_addr;
	inet_aton("127.0.0.1",&sin_addr);
	adr.sin_addr=sin_addr;
	len=sizeof(struct sockaddr_in);
	bzero(&adr.sin_zero, 0);	

	scanf("%s",s);
	sendto(sockfd,s,1024,0,(struct sockaddr *)(&adr),len);
	strcpy(name,s);
	
	pthread_t tid1,tid2;
	pthread_create(&tid1,NULL,func1,(void *)1);
	pthread_create(&tid2,NULL,func2,(void *)1);

	while(1)
	{
		if(f1==1)
		{
			char checks[20];
			int i,mn;
			mn=4;
			if(mn>strlen(s))
			{
				mn=strlen(s);
			}
			for(i=0;i<mn;i++)
			{
				checks[i]=s[i];
			}
			checks[i]='\0';
			if(strcmp(checks,"File")==0)
			{
				for(i=5;i<strlen(s);i++)
				{
					checks[i-5]=s[i];
				}
				checks[i-5]='\0';
				FILE *fptr;
				fptr=fopen(checks,"r+");
				//s="Sending File ";
				strcpy(s,"Sending File");
				strcat(s,checks);

				do
				{
					sendto(sockfd,s,1024,0,(struct sockaddr *)(&adr),len);
				}while(fscanf(fptr," %[^\n]s",s)>0);
				fclose(fptr);
				strcpy(s,"Closing File");
				sendto(sockfd,s,1024,0,(struct sockaddr *)(&adr),len);
			}
			else
			{
				sendto(sockfd,s,1024,0,(struct sockaddr *)(&adr),len);
				if(strcmp(s,"bye")==0)
				{
					break;
				}
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