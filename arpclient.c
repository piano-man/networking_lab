#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/shm.h>
main()
{
int shmid,a;
char *ptr,*shmptr;
char ptr2[51],ip[12],mac[26];
shmid=shmget(3000,10,0666);
shmptr=shmat(shmid,NULL,0);
puts("The ARPtable is:");
printf("%s",shmptr);
printf("\n1.ARP\n2.RARP\n3.EXIT\n");
scanf("%d",&a);
switch(a)
{
case 1:
puts("Enter ip address:");
scanf("%s",ip);
ptr=strstr(shmptr,ip);
ptr-=8;
sscanf(ptr,"%s%*s",ptr2);
printf("mac addr is:%s",ptr2);
break;
case 2:
puts("Enter mac addr");
scanf("%s",mac);
ptr=strstr(shmptr,mac);
sscanf(ptr,"%*s%s",ptr2);
printf("%s",ptr2);
break;
case 3:
exit(1);
}
}