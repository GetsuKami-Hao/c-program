#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(void)
{
	int ret;
	
	printf("系统分配的进程号是：%d\n",getpid());
	
	/* 
	 * 执行system()函数时会调用fork(),execve()、waitpid()等函数，任何一个失败将导致system()失败
	 *
	 * 失败返回　-1;
	 * 当sh不能执行时　返回 127;
	 * 成功，返回进程状态值。
	 *
	 * */
	ret = system("ping www.baidu.com -c 2");

	printf("返回值为：%d\n",ret);
	return 0;
}
