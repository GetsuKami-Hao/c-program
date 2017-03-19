#include<stdio.h>

void *fun1(int a,int b)
{
	printf("%d\n",a+b);
}
					//void *fun3()
int fun2(void *fun3(int ,int),int a,int b)
{
	fun3(a,b);
}

int funn(int a,int b)
{
	return a+b;
}


int main(void)
{

	int (*function)(int ,int);
	//int *fun(int,int);
	
	
	function = funn;
	printf("%d\n",function(22,12));
	fun2((void *)fun1,3,4);
	fun2(fun1,1,2);

	return 0;

}
