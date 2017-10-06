#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
int main(){
  int clientSocket, portNum, nBytes;
  char buffer[100001];
  struct sockaddr_in serverAddr;
  socklen_t addr_size;

  /*Create UDP socket*/

    
    /*printf("Type a sentence to send to server:\n");
    fgets(buffer,1024,stdin);
    printf("You typed: %s",buffer);#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
int main(){
  int clientSocket, portNum, nBytes;
  char buffer[100001];
  struct sockaddr_in serverAddr;
  socklen_t addr_size;

  /*Create UDP socket*/
  clientSocket = socket(AF_INET, SOCK_DGRAM, 0);

  /*Configure settings in address struct*/
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(7891);
  serverAddr.sin_addr.s_addr = INADDR_ANY;
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  /*Initialize size variable to be used later on*/
  addr_size = sizeof(serverAddr);
  //char buffer[10000];
  int limit;
    printf("Enter a message");
    scanf("%s", buffer);
    printf("enter fragment fact i.e max data size in a packet");
    scanf("%d", &limit);
    
    //Function to fragment the data
    int tot_len = strlen(buffer);
    int ct = 1;
    unsigned char* ptr = (unsigned char*) buffer;
  while(tot_len>0){
    
    printf("Fragment no %d\n", ct);
    
    /*printf("Type a sentence to send to server:\n");
    fgets(buffer,1024,stdin);
    printf("You typed: %s",buffer);

    nBytes = strlen(buffer) + 1;*/
    clock_t t;
    
    int size_pck;
    if(tot_len >= limit) size_pck = limit;
    else size_pck = tot_len;
    
    /*Send message to server*/
    sendto(clientSocket,ptr,size_pck,0,(struct sockaddr *)&serverAddr,addr_size);
    
    printf("sent fragment #%d, data \n", ct);
    
    for(int i=0;i<size_pck;i++) printf("%c", ptr[i]);
    printf("\n\n");
    ptr = ptr + limit;
    tot_len -= limit;
    ct++;
    sleep(1);
    //printf("Received from server: %s\n Time Taken %lf\n",buffer, time_taken);

  }



    nBytes = strlen(buffer) + 1;
    clock_t t;
    
    int size_pck;
    if(tot_len >= limit) size_pck = limit;
    else size_pck = tot_len;
    
    /*Send message to server*/
    sendto(clientSocket,ptr,size_pck,0,(struct sockaddr *)&serverAddr,addr_size);
    
    printf("sent fragment #%d, data \n", ct);
    
    for(int i=0;i<size_pck;i++) printf("%c", ptr[i]);
    printf("\n\n");
    ptr = ptr + limit;
    tot_len -= limit;
    ct++;
    sleep(1);
    //printf("Received from server: %s\n Time Taken %lf\n",buffer, time_taken);

  }

