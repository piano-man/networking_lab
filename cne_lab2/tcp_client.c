#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>

int main(){
  int clientSocket, portNum, nBytes;
  char buffer[1024];
  struct sockaddr_in serverAddr;
  socklen_t addr_size;

  clientSocket = socket(PF_INET, SOCK_STREAM, 0);

  portNum = 3990;

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(portNum);
  serverAddr.sin_addr.s_addr = INADDR_ANY;//inet_addr("127.0.0.1");
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  addr_size = sizeof serverAddr;
  connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

  while(1){
    printf("Type a sentence to send to server:\n");
    fgets(buffer,1024,stdin);
    printf("You typed: %s",buffer);
    clock_t t;
    nBytes = strlen(buffer) + 1;

    if(send(clientSocket,buffer,nBytes,0)==-1) {printf("connection not established\n"); continue;}
    t = clock();
    recv(clientSocket, buffer, 1024, 0);
    if(!recv(clientSocket,buffer,1024,0)==-1) {printf("connection not established\n"); continue;}
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Received from server: %s\ntime taken : %lf\n",buffer,time_taken);   
  }

  return 0;
}
