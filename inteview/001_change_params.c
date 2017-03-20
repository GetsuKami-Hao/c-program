#include<stdio.h>
#include<stdarg.h>
void my_printf(char *fmt,...);
int my_sum(int number,...);


void main(void)
{
	my_printf("我的天%d\n,%s",12,"asd");
	my_printf("---------%d\n",my_sum(3,10,20,30));
}

//printf函数可变参数的个数不能轻易的得到，而可变参数的类型也不是固定的，
//需由格式字符串进行识别（由%f、%d、%s等确定），因此则涉及到可变参数表的更复杂应
void my_printf(char *fmt,...)
{
	va_list ap;
	char *p,*sval;
	int ival;
	double dval;

	va_start(ap,fmt);
	for(p=fmt;*p;p++)  //遍历fmt
	{
		if(*p != '%')
		{
			putchar(*p);
			continue;  //进行下一次循环
		}
		switch(*++p)
		{
			case 'd':
				ival = va_arg(ap,int);
				printf("%d",ival);
				break;
			case 'f':
				dval = va_arg(ap,double);
				printf("%f",dval);
			case 's':
				for(sval = va_arg(ap,char *);*sval;sval++)
					putchar(*sval);
				break;
			default:
				putchar(*p);
				break;
		}
	}
	va_end(ap);
}

int my_sum(int number ,...)
{
	va_list pArgs;
	int times =0;
	int sum = 0;
	va_start(pArgs,number);
	for(;times < number ; times++)
	{
		sum += va_arg(pArgs,int);
	}
	va_end(pArgs);
	return sum;
}
