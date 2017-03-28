#include<stdio.h>

#define my_method1(m,n,x) my_method2(m,n,x)


#define WARN_IF(EXP) \
       do { if (EXP) \
                      fprintf (stdout,"Warning: " #EXP " = %d\n",EXP); } \
     while (0)


//#define COMMAND(NAME) { #NAME , NAME##_command }

#define MIN(A,B) ((B) <= (A) ? (A):(B))

struct command{
  char *name;
  void (*function)(void);
};  

int my_method2(int m,int n,int x)
{
  return m*n*x;
}


void main(void)
{
//  struct command commands[] = {COMMAND(quit), COMMAND(help)};
  WARN_IF(my_method1(1,3,6));
  int *c,*p,a,b;
  a = 33;
  b = 12;
  p = &a;
  c = &a;
  c++;
  *c = 101010; //将a地址后面的一个整型大小的地址赋值。
  int t = MIN(*p++,b);
  p = &a;
  printf("t= %d, *p = %d , MIN = %d ....\n",t,*p,MIN(*p++,b)); //=>t=101010, *p=101010, MIN=101010 指针指向改变
  p = &a;
  printf("*p = %d , *p++ = %d .....\n",*p,*p++);  //=> *p=101010, *p++ = 33 指针指向改变
  printf("%d\n", my_method1(1,2,3) );
}

