#include<stdio.h>

int fuc(int (*a)[4]);
int fuc(int (*a)[4])
{
	printf("a[0] = %d\n",**a);
	return 0;
}

void main(void)
{
	int a[4] = {1,2,3,4};
  //int **b = &a;
	int **b;
	int (*c)[4] = &a;
	int *d = a;
	//b = &a;
	printf("sizeof(a) = %d\n",(int)sizeof(a));
	printf("sizeof(&a)= %d\n", (int)sizeof(&a));
	printf("sizeof(int  *) = %d\n",(int)sizeof(int *));
	printf("sizeof(a[0]) = %d\n", (int)sizeof(a[0]));
	// sizeof() return "long unsigned int" type
	fuc(&a);

}

