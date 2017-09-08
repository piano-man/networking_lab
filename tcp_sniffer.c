#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void fatal(char *message)
{
    char error_message[100];

    strcpy(error_message, "[!!] Fatal Error ");
    strncat(error_message, message, 83);
    perror(error_message);
    exit(-1);
}

void dump(const unsigned char *data_buffer, const unsigned int length)
{
    unsigned char byte;
    unsigned int i, j;
    for (i = 0; i < length; i++)
    {
        byte = data_buffer[i];
        printf("%02x ", data_buffer[i]); // display byte in hex
        if (((i % 16) == 15) || (i == length - 1))
        {
            for (j = 0; j < 15 - (i % 16); j++)
                printf("   ");
            printf("| ");
            for (j = (i - (i % 16)); j <= i; j++)
            { // display printable bytes from line
                byte = data_buffer[j];
                if ((byte > 31) && (byte < 127)) // outside printable char range
                    printf("%c", byte);
                else
                    printf(".");
            }
            printf("\n"); // end of the dump line (each line 16 bytes)
        }                 // end if
    }                     // end for
}

int main(void)
{
    int i, recv_length, sockfd;
    u_char buffer[9000];

    if ((sockfd = socket(PF_INET, SOCK_RAW, IPPROTO_TCP)) == -1)
        fatal("in socket");

    for (i = 0; i < 3; i++)
    {
        recv_length = recv(sockfd, buffer, 80, 0);
        printf("Got a %d byte packet\n", recv_length);
        dump(buffer, recv_length);
    }
}