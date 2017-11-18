#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>    /* Must precede if*.h */
#include <linux/if.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <sys/ioctl.h>
//                    ETHERNET FRAME
// [{(dest mac id),(source mac id),(ether type)},{(payload)}  ,{(CRC checksum)}]
//         MAC Header (14 bytes)                   data           (checksum)
//    (6 bytes)      (6 bytes)      (2 bytes)   (46-1500 bytes)  (4bytes)
//EtherType is a two-octet field in an Ethernet frame. 
//It is used to indicate which protocol is encapsulated in the payload of the frame
union ethframe
{
  struct
  {
    struct ethhdr    header;
    unsigned char    data[ETH_DATA_LEN];
  } field;
  unsigned char    buffer[ETH_FRAME_LEN];
};
//f4:06:69:8b:c3:7e
int main(int argc, char **argv) {
  char *iface = "eth0";
  unsigned char dest[ETH_ALEN]
           = { 0xf4, 0x06, 0x69, 0x8b, 0xc3, 0x7e };
  unsigned short proto = 0x1234;
  unsigned char *data = "IIM2015501";
  unsigned short data_len = strlen(data);
 
  int s;
  //SOCK_RAW packets are passed to and from the device driver without any changes in the packet data
  //AF_PACKET used if you want to play with packets at the protocol level
  //the htons() function converts values between host and network byte orders
  if ((s = socket(AF_PACKET, SOCK_RAW, htons(proto))) < 0) {
    printf("Error: could not open socket\n");
    return -1;
  }
 
  struct ifreq buffer;
  int ifindex;
  memset(&buffer, 0x00, sizeof(buffer));
  //IFNAMSIZ - max size to hold a interface name.
  //A common use of ioctl is to control hardware devices.
  //ioctl is used on file descriptor of socket to config. network device.
  //SIOCGIFINDEX Retrieve the interface index of the interface into ifr_ifindex.
  strncpy(buffer.ifr_name, iface, IFNAMSIZ);
  if (ioctl(s, SIOCGIFINDEX, &buffer) < 0) {
    printf("Error: could not get interface index\n");
    close(s);
    return -1;
  }
  // IfIndex is a unique identifying number associated with a physical or logical interface
  ifindex = buffer.ifr_ifindex;
 
  unsigned char source[ETH_ALEN];
  //fetch mac id of source network device into buffer.ifr_hwaddr.sa_data ( read s)
  if (ioctl(s, SIOCGIFHWADDR, &buffer)  < 0) {
    printf("Error: could not get interface address\n");
    close(s);
    return -1;
  }
  
  memcpy((void*)source, (void*)(buffer.ifr_hwaddr.sa_data),
         ETH_ALEN);
  union ethframe frame;

  
  // copy dest hardware address to ethernet frame header
  memcpy(frame.field.header.h_dest, dest, ETH_ALEN);
  // copy source hardware address to ethernet frame header
  memcpy(frame.field.header.h_source, source, ETH_ALEN);
  // copy proto to ethernet header
  frame.field.header.h_proto = htons(proto);
  // copy data to ethernet data
  memcpy(frame.field.data, data, data_len);
 // frame length = data_len + header length
  unsigned int frame_len = data_len + ETH_HLEN;
 
  struct sockaddr_ll saddrll;
  memset((void*)&saddrll, 0, sizeof(saddrll));
  saddrll.sll_family = PF_PACKET;   
  saddrll.sll_ifindex = ifindex;
  saddrll.sll_halen = ETH_ALEN;
  memcpy((void*)(saddrll.sll_addr), (void*)dest, ETH_ALEN);
 
  if (sendto(s, frame.buffer, frame_len, 0,
             (struct sockaddr*)&saddrll, sizeof(saddrll)) != -1)
    printf("Success!\n");
  else
    printf("Error, could not send\n");
 
  close(s);
 
  return 0;
}
