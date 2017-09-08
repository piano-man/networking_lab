#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<unistd.h>
#include<stdint.h>

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

void pcap_fatal(const char *failed_in, const char *errbuf) {
	printf("Fatal Error in %s: %s\n", failed_in, errbuf);
	exit(1);
}

int main() {
	struct pcap_pkthdr header;
	const u_char *packet;
	char errbuf[PCAP_ERRBUF_SIZE];
	char *device;
	pcap_t *pcap_handle;
	int i;

	device = pcap_lookupdev(errbuf);
	if(device == NULL)
		pcap_fatal("pcap_lookupdev", errbuf);

	printf("Sniffing on device %s\n", device);
	
	pcap_handle = pcap_open_live(device, 0, 1, 0, errbuf);
	if(pcap_handle == NULL)
		pcap_fatal("pcap_open_live", errbuf);
	
	for(i=0; i < 3; i++) {
		packet = pcap_next(pcap_handle, &header);
		printf("Got a %d byte packet\n", header.len);
		dump(packet, header.len);
	}

	pcap_close(pcap_handle);
}