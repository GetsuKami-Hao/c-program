#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>

int main()
{
	pid_t pid,ppid;

	pid = getpid(); //获得当前进程id号
	ppid = getppid(); //获得当前进程父进程的id号

	printf("当前进程号为%d\n",pid);
	printf("当前进程的父进程号：%d\n",ppid);

	return 0;
}
