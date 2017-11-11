#include <stdio.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/udp.h>
#include <netinet/ether.h>
#include <linux/if_packet.h>
#include <sys/ioctl.h>

unsigned short in_cksum(unsigned short *ptr, int nbytes);
int read_answer(int *sock);
/*
   Two functionprotyps are needed. With "in_cksum" we calculate the checksum of the ICMP packet
   (what a checksum is should be clear...) and with "read_answer" we wait for the echo-reply.
 */
unsigned short in_cksum(unsigned short *ptr, int nbytes)
{

	register long                  sum;                /* assumes long == 32 bits */
	u_short                         oddbyte;
	register u_short answer;            /* assumes u_short == 16 bits */
	/*
	 * Our algorithm is simple, using a 32-bit accumulator (sum),
	 * we add sequential 16-bit words to it, and at the end, fold back
	 * all the carry bits from the top 16 bits into the lower 16 bits.
	 */
	sum = 0;
	while (nbytes > 1)  {
		sum += *ptr++;
		nbytes -= 2;
	}
	/* mop up an odd byte, if necessary */
	if (nbytes == 1) {
		oddbyte = 0;                 /* make sure top half is zero */
		*((u_char *) &oddbyte) = *(u_char *)ptr;   /* one byte only */
		sum += oddbyte;
	}
	/*
	 * Add back carry outs from top 16 bits to low 16 bits.
	 */
	sum  = (sum >> 16) + (sum & 0xffff);      /* add high-16 to low-16 */
	sum += (sum >> 16);                            /* add carry */
	answer = ~sum;                        /* ones-complement, then truncate to 16
						 bits */
	return(answer);
}
/*
   When the checksum are calculated, often the above outlined fucntion is used. This is not from
   the author, but is always used by him with great pleasure   ;).
 */
int read_answer(int *sock)
{
	char buff[1024];
	struct iphdr *ip;
	struct icmphdr *icmp;
	ip = (struct iphdr *)buff;
	icmp = (struct icmphdr *) (buff + sizeof(struct iphdr));
	if(read(*sock, buff, sizeof(buff)) > 0) {
		if(icmp->type == 0 && icmp->code == 0) return 1;
		else return -1;
	}
	return 0;
}
/*
   To this function we pass our raw-ICMP-socket, on which we later listen for an answer. First
   we declare a datapuffer of 1024 bytes and set the ip and afterwards the ICMP structur into
   it. Now we call "read" and listen for an answer. When we read an ICMP message, we first look
   at the type and code. If we are working wiht an echo-reply, the function gives a returnvalue
   of 1. Else -1 is put out.
 */
int main(int argc, char **argv)
{
	int sockfd, test = 1;
	char *packet;
	struct iphdr *ip;
	struct icmphdr *icmp;
	struct sockaddr_in server;
	char *tests = "hallo";
	if(argc != 3) {
		printf("usage: pong [sourceip] [destip]\n");
		return -1;
	}
	ip = (struct iphdr *) malloc(sizeof(struct iphdr));
	icmp = (struct icmphdr *) malloc(sizeof(struct icmphdr));
	packet = (char *) malloc(sizeof(struct iphdr) + sizeof(struct icmphdr)+sizeof(tests)+1);
	memset(packet, '\0',sizeof(packet));
	ip = (struct iphdr *)packet;
	icmp = (struct icmphdr *) (packet + sizeof(struct iphdr));
	strcpy(packet+sizeof(struct iphdr)+sizeof(struct icmphdr),tests);
	ip->ihl = 5;
	ip->version = 4;
	ip->tos = 0;
	ip->tot_len = sizeof(struct iphdr) + sizeof(struct icmphdr)+sizeof(tests)+1;
	ip->id = htons(getuid());
	ip->ttl = 255;
	ip->protocol = IPPROTO_ICMP;
	ip->saddr = inet_addr(argv[1]);
	ip->daddr = inet_addr(argv[2]);
	sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	if(sockfd < 0) {
		printf("error cannto creat socket\n");
		return -1;
	}

	if( (setsockopt(sockfd, IPPROTO_IP,IP_HDRINCL,&test,sizeof(test))) < 0) {
		printf("couldnt set IP_HDRINCL\n");
		return -1;
	}

	icmp->type = 8;
	icmp->code = 0;
	icmp->un.echo.id = 0;
	icmp->un.echo.sequence = 0;
	icmp->checksum = 0;
	icmp->checksum = in_cksum((unsigned short *)icmp,sizeof(struct icmphdr)+sizeof(tests)+1);
	ip->check = in_cksum((unsigned short *)ip, sizeof(struct iphdr));
	server.sin_family = AF_INET;
	server.sin_port = htons(80);  /* doesnt matter */
	server.sin_addr.s_addr = inet_addr(argv[2]);
	if( (sendto(sockfd,packet,ip->tot_len,0,(struct sockaddr *)&server,sizeof(struct sockaddr))) < ip->tot_len) {
		printf("cannot send the packet\n");
		return -1;
	}
	printf("done!\n");
	if(read_answer(&sockfd) == 1) printf("received answer- host is up\n");
	else printf("didnt receive answer\n");
	return 0;

}
/*
    We declare like before first our datapacket and the needed header. More we put the
    dataconent. We will send the string that "test" contains as datacontent. That dos not have a
    deeper meaning, but bring the usage of the dataarea nearer to the user. Now we check if the
    programm is called corectly. The first argument has to be the source IP-adress and the second
    argument has to be the dest IP-adress. Then it is time to fill the ip-header. Then we create
    us our ICMP-raw-socket and realise now a new function.

    Throughthis form of call of the function, we have the kernel to keep his hands off of our
    selfcreated datapacket. If we would not use this function, the kernel would more bad than
    good define the fields like ip-source. The konstant "IP_HDRINCL" tells the kenel, that the /////////IP_HDRINCL 
    ip-header should be untouched. We don´t go deeper into this function, cause we only need this
    function in this form. For more Information.:
  */
