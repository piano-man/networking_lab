/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char **argv) {
int sock = socket(AF_INET, SOCK_STREAM, 0);
	
  struct sockaddr_in servaddr;  
 

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(6789);

  bind(sock, (struct sockaddr *)&servaddr, sizeof(servaddr));  
  listen(sock, 5);

  while(1) {
    
    int clisock = accept(sock, (struct sockaddr *) NULL, NULL);

    if (clisock >= 0) {
      int messageLength = 160;
      char message[messageLength+1];
      int in, index = 0, limit = messageLength;

      while ((in = recv(clisock, &message[index], messageLength, 0)) > 0) {
        index += in;
        limit -= in;
      }

      printf("%s\n", message);
      close(clisock);
    }

    
  }
}
