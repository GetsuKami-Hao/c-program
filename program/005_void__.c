#include<stdio.h>

char *man(void **arg)
{
	char **a = (char **)arg;
	
	*a = "man1: 1234";

	return *a;
}

void man2(void *arg)
{
	int *a = arg;
	printf("man2: %d\n",*a);
}


int main(void)
{
	char *arg;
	int a = 10;
	man2(&a);
	printf("%s\n",man((void *)&arg));
	return 0;
}
