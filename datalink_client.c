#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

typedef struct packet{
    char data[1024];
}Packet;

typedef struct frame{
    int frame_kind; //ACK:0, SEQ:1 FIN:2
    int sq_no;
    int ack;
    Packet packet;
}Frame;

int main(int argc, char *argv[])
{
    int sock;
    int str_len;
    socklen_t adr_sz;
    struct sockaddr_in serv_adr, from_adr;
    Frame ackframe, recv_frame;
    int frame_id;
    int recv_result;
    struct timeval tv;
    FILE* outfile = fopen("output.txt", "w+");
    if(argc!=3){
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    sock=socket(PF_INET, SOCK_DGRAM, 0);   
    if(sock==-1)
    {
        printf("error! failed to open the socket\n");
        exit(1);
    }

    memset(&serv_adr, 0, sizeof(serv_adr));
    adr_sz = sizeof(serv_adr);
    serv_adr.sin_family=AF_INET;
    //inet_aton("127.0.0.1" , &serv_adr.sin_addr);
    serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_adr.sin_port=htons(atoi(argv[2]));
    char s[1024];
    scanf("%s",s);
    sendto(sock,s,1024,0,(struct sockaddr *)(&serv_adr),adr_sz);
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(struct timeval));

    frame_id = 0;
    while(1)
    {
        recv_result = recvfrom(sock, &recv_frame, sizeof(recv_frame), 0,(struct sockaddr*)&serv_adr, &adr_sz);
        //perror("error: ");
        printf("data %s\n", recv_frame.packet.data);
        printf("recv result %d\n", recv_result);
        printf("recv frame=%d, frame_kind = %d\n", recv_frame.sq_no, recv_frame.frame_kind);
        if(recv_result > 0 && recv_frame.frame_kind == 1 && recv_frame.sq_no==frame_id){
            printf("Frame %d received\n", recv_frame.sq_no);
            fprintf(outfile, "%s", recv_frame.packet.data);
            ackframe.frame_kind = 0;
            ackframe.ack = recv_frame.sq_no + 1;
            printf("Ack %d sent\n", ackframe.ack);
            sendto(sock, &ackframe, sizeof(ackframe), 0,(struct sockaddr*)&serv_adr, adr_sz);
            frame_id++;
            //break;
        }else{
            printf("Frame %d time expired\n", frame_id);
        }
        //fclose(outfile);
    }   
    close(sock);
    
    return 0;
}