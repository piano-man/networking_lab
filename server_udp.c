#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

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

int main()
{
    int udpSocket, nBytes;
    char buffer[1024];
    struct sockaddr_in serverAddr, clientAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size, client_addr_size;
    int i;

    /*Create UDP socket*/
    udpSocket = socket(PF_INET, SOCK_DGRAM, 0);

    /*Configure settings in address struct*/
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7891);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    /*Bind socket with address struct*/
    bind(udpSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    /*Initialize size variable to be used later on*/
    addr_size = sizeof serverStorage;

    while (1)
    {
        /* Try to receive any incoming UDP datagram. Address and port of 
    requesting client will be stored on serverStorage variable */
        nBytes = recvfrom(udpSocket, buffer, 1024, 0, (struct sockaddr *)&serverStorage, &addr_size);
        while (nBytes > 0)
        {
            printf("RECV:%d bytes\n", nBytes);
            dump(buffer, nBytes);
            nBytes = recvfrom(udpSocket, buffer, 1024, 0, (struct sockaddr *)&serverStorage, &addr_size);
        }

        /*Convert message received to uppercase*/
        /*for (i = 0; i < nBytes - 1; i++)
            buffer[i] = toupper(buffer[i]);

        /*Send uppercase message back to client, using serverStorage as the address*/
        //sendto(udpSocket, buffer, nBytes, 0, (struct sockaddr *)&serverStorage, addr_size);
    }

    return 0;
}