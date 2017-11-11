#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>


typedef struct packet{
    char data[1024];
}Packet;

typedef struct frame{
    int frame_kind; //ACK:0, SEQ:1 FIN:2
    int sq_no;
    int ack;
    Packet packet;
}Frame;

int main(int argc, char** argv)
{
    int sockfd;
    int clilen;
    int state;
    int n;
    int sum;
    int recv_result;

    int frame_id = 0;

    Packet packet;
    Frame frame;
    Frame recv_frame;
    char buffer[1024] = "";

    struct timeval tv;

    struct sockaddr_in serveraddr, clientaddr;

    FILE* infile = fopen(argv[2], "r");
    /*hhchsahlaksdn;asdnif(argc != 3)
    {
        perror("error! usage: $./server <PORT> filename\n");
        exit(1);
    }
    if(infile == NULL)
    {
        printf("error! failed to open the file.\n");
        exit(0);
    } */

    clilen = sizeof(clientaddr);
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); //using diagram instead of stream-- UDP
    if(sockfd < 0)
    {
        perror("socket error: ");
        exit(0);
    }

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(atoi(argv[1])); //set port number 9999


    //set socket option -- timeout is 100000 microseconds

    tv.tv_sec = 1;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(struct timeval));

    state = bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if(state == -1)
    {
        perror("bind error: ");
        exit(0);
    }
    //char buff[1024];
    int rd = 0;
    while(rd == -1 || rd == 0) {
        rd = recvfrom(sockfd, buffer, sizeof(buffer), 0,(struct sockaddr*)&clientaddr, &clilen );
        printf("rd - %d\n", rd);
    }

    do{
        n = fscanf(infile, "%s", buffer);

        strcpy(packet.data, buffer);
        memcpy(&(frame.packet), &packet, sizeof(Packet));
        frame.frame_kind = 1; //SEQ
        frame.sq_no = frame_id;
        frame.ack = 0;
        printf("flag2\n");
        while(1)
        {
            sendto(sockfd, &frame, sizeof(frame), 0, (struct sockaddr*)&clientaddr, (socklen_t)clilen);
            printf("Frame %d sent\n", frame_id);
            recv_result = recvfrom(sockfd, &recv_frame, sizeof(recv_frame), 0,(struct sockaddr*)&clientaddr, &clilen );
            //printf("%d -- %d -- %d\n", recv_result, recv_frame.frame_kind, recv_frame.ack);
            if(recv_result > 0 && recv_frame.frame_kind == 0 && recv_frame.ack == frame_id + 1){
                printf("Ack %d received\n", recv_frame.ack);
                break;
            }else{
                printf("Frame %d time expired\n", frame_id);
            }
        }
        frame_id++;
    }while(n > 0);

    printf("finished\n");
    return 0;
}