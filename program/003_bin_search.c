#include<stdio.h>



void main(void)
{
	int arr[] = {1,2,3,4,5,6,7,8,9,10,11,12,20,40,50};
	int n,min,max,mid;
	printf("请输入需要查找的数:");
	scanf("%d",&n);
	for(min=0,max=sizeof(arr)/sizeof(int),mid=(min+max)/2;min < max; mid=(min+max)/2)
	{
		if(n > arr[mid])
		{
			if(n == arr[max])
			{
				printf("输入的数在第%d位。",max);
				break;
			}
			min = mid + 1;
		}
		else if(n < arr[mid])
		{
			if(n == arr[min])
			{
				printf("输入的数在第%d位置",min);
				break;
			}
			max = mid - 1;
		}
		else
		{
			printf("输入的数在第%d位。", mid);
			break;
		}
	}

	if(min >= max)
	{
		printf("未找到");
	}
}
