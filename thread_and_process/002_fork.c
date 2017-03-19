#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

int main(void)
{
	pid_t pid;
	pid = fork(); //执行一次返回两次。子进程中返回０,父进程中返回子进程的ID

	if(pid < 0)
	{
		printf("进程创建失败！\n");
		return -1;
	}
	else if(pid == 0)
	{
		printf("子进程,fork返回值： %d, ID: %d,　父进程 ID: %d\n",pid,getpid(),getppid());
	}
	else
	{
		printf("父进程，fork返回值：%d, ID: %d,　父进程 ID: %d\n",pid, getpid(),getppid());
	}

	return 0;
}
