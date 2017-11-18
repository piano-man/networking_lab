# include<stdio.h>
# include<stdlib.h>
void manchester(int arr[],int size)
{
    int i;
    printf("Manchester:\n");
    for(i=0;i<size;i++)
    {
        if(arr[i]==1)
        printf("%d %d ",1,0);
        else if(arr[i]==0)
        {
            printf("%d %d ",0,1);
        }
    }
    printf("\n");

}
void nrzi(int arr[],int size)
{
    int level = 0;
    int i;
    printf("NRZI: \n");
    for(i=0;i<size;i++)
    {
        if(arr[i]==1)
        {
            if(level==0)
            {
                printf("%d %d ",level,level+1);
                level=level+1;
            }
            else if(level==1)
            {
                printf("%d %d ",level,level-1);
                level=level-1;
            }
            
        }
        else if(arr[i]==0)
        {
            printf("%d %d ",level,level);
        }
    }
    printf("\n");
}

void nrzl(int arr[],int size)
{
    int i;
    printf("nrzl:\n");
    for(i=0;i<size;i++)
    {
        if(arr[i]==0)
        {
            printf("%d %d ",1,1);

        }
        else if(arr[i]==1)
        {
            printf("%d %d ",0,0);
        }
    }
    printf("\n");


}
void diffmanchester(int arr[],int size)
{
    int prev;
    int i;
    printf("Differential Manchester:\n");
    prev = arr[0];
    if(prev==0)
    {
        printf("%d %d ",0,1);
    }
    if(prev==1)
    {
        printf("%d %d ",1,0);
    }
    for(i=1;i<size;i++)
    {
        if(prev==arr[i])
        {
            printf("%d %d ",0,1);
            prev=arr[i];
        }
        else if(prev!=arr[i])
        {
            printf("%d %d ",1,0);
            prev=arr[i];

        }

    }
    printf("\n");

}
void diffmanchester2(int a[],int size)
{
    printf("Differential:\n");

	int ans[500];
	int k = 0;
    int i;
    int n=size;
	int ini;
	if (a[0] == 0) {
		ans[k] = 0;
		k++;
		ans[k] = 1;
		k++;
		ini = 1;
	}
	else if (a[0] == 1) {
		ans[k] = 0;
		k++;
		ans[k] = 1;
		k++;
		ini = 1;
	}
	for (i = 1; i < n; i++) {
		if (a[i] == 1) {
			ans[k] = ini;
			k++;
			ans[k] = 1-ini;
			k++;
			ini = 1-ini;
		}
		else {
			ans[k] = 1-ini;
			k++;
			ans[k] = ini;
			k++;
		}
	}
	for (i = 0; i < k; i++) {
		printf ("%d ", ans[i]);
	}
	printf ("\n");

}
void main()
{
    int arr[9]={1,0,1,0,0,0,1,1,0};
    int size = sizeof(arr)/4;
    printf("%d \n",size);
    manchester(arr,size);
    //diffmanchester(arr,size);
    diffmanchester2(arr,size);
    nrzl(arr,size);
    nrzi(arr,size);

}