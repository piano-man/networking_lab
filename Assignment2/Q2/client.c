#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char **argv) {

  struct sockaddr_in servaddr;
  int sock = socket(AF_INET, SOCK_STREAM, 0);

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(6789);
  servaddr.sin_addr.s_addr = inet_addr("172.20.34.118");

  while(1) {

    char message[161]={'c','o','n','n','e','c','t','e','d'};
//    fgets(message, 161, stdin);
    /* Replacing '\n' with '\0' */
    char *tmp = strchr(message, '\n');
    if (tmp) *tmp = '\0';

    connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr));
    send(sock, message, strlen(message), 0);
    close(sock);
  }
}
