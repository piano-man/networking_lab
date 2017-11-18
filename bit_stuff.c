#include <stdio.h>
#include <string.h>
int main()
{
	char message[1005];
	char endofframe[1005];
	scanf("%s%s",message,endofframe);
	int i,j,c=0;
	char answer[1005];
	for(i=0;i<strlen(endofframe);i++)
	{
		if(endofframe[i]=='1')
		{
			c++;
		}
	}
	//printf("%d\n",c);
	int fo=0,fz=0;
	j=0;
	for(i=0;i<strlen(message);i++)
	{
		if(message[i]=='1')
		{
			fo++;
		}
		else
		{
			fo=0;
		}
		if(fo==c-1)
		{
			answer[j++]=message[i];
			//printf("%d %c\n",i,message[i]);
			fz=i-(c-1);
			if(fz>=0&&message[fz]=='0')
			{
				answer[j++]='0';
			}	
			fz=0;
			fo=0;
		}
		else
		{
			answer[j++]=message[i];
		}
	}
	answer[j]='\0';
	printf("%s\n",answer);
	char org[1005];
	j=0;
	fo=0;
	for(i=0;i<strlen(answer);i++)
	{
		org[j++]=answer[i];
		if(answer[i]=='1')
		{
			fo++;
		}
		else
		{
			fo=0;
		}
		if(fo==c-1)
		{
			fz=i-(c-1);
			if(answer[fz]=='0')
			{
				i++;
			}
		}
	}
	org[j]='\0';
	printf("%s\n",org);
	return 0;
}