#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#define SA struct sockaddr

#define CHAR_LIMIT (sizeof(char) * 256)

void encrypt(char *buf, int size)
{
	int i = 0;
	for (; i < size; i++)
		buf[i] = (buf[i] + 1) % CHAR_LIMIT;
}

int main(int argc,char**argv)
{
	int sockfd;
	char fname[25];
	int len;
	struct sockaddr_in servaddr,cliaddr;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(atoi(argv[1]));
	inet_pton(AF_INET,argv[1],&servaddr.sin_addr);
	connect(sockfd,(SA*)&servaddr,sizeof(servaddr));
	// char *buffer = (char*) malloc (sizeof(char) * 100000;
	char buffer[1000];
	FILE *f;
//	f=fopen("server.c","r");
	f=fopen("input.txt","r");
	int i = 0, j = 0;
	fscanf(f, "%c", &buffer[i]);
	printf("%c %d\n", buffer[i], i);
	i++;
	while (!feof(f)) {
		fscanf(f, "%c", &buffer[i]);
		i++;
		if (i == 1000) {
			// write(sockfd,buffer,1000);
			j++;
			printf("%d %s\n", j, buffer);
			encrypt(buffer, 1000);
			send(sockfd, buffer, 1000, 0);
			i = 0;
		}
	}
	// write(sockfd,buffer,i-1);
	encrypt(buffer, 1000);
	send(sockfd, buffer, i-1, 0);
	printf("the file was sent successfully");
}
