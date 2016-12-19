#include<stdio.h>

int factorial(int n)
{
	if(n <= 1)
		return n;
	else
		return n * factorial(n-1);
}

void main(void)
{
	int n;
	printf("请输入要求阶乘的数：");
	scanf("%d",&n);
	int m = factorial(n);
	printf("%d的阶乘是：%d\n",n,m);
}
