#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>


#define PORT 4401
#define MAXPENDING 2


void process_conn_server(int s)
{
	ssize_t size = 0;
	char buffer[1024];

	memset(buffer,0,1024);


	while(1)
	{
		size = read(s,buffer,1024);
		if(size == 0)
		{
			return ;
		}
		sprintf(buffer, "%d bytes altogether\n", (int)size);
		write(s,buffer,strlen(buffer)+1);
		puts(buffer);
	}
}
int main(void)
{
	int sock_server, sock_clnt;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	pid_t pid;

	sock_server = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	
	if (sock_server < 0)
	{
		fprintf(stdout,"socket() error\n");
		return -1;
	}
	else
	{
		puts("创建socket 描述符成功.....");
	}

	memset(&server_addr,0,sizeof(struct sockaddr_in));
	memset(&server_addr,0,sizeof(struct sockaddr_in));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);

	int err = bind(sock_server,(struct sockaddr*)&server_addr,sizeof(server_addr));
	if(err < 0)
	{
		fprintf(stdout,"bind() error\n");
		return -1;
	}
	else
	{
		puts("绑定端口成功......");
	}

	err = listen(sock_server,MAXPENDING);
	if(err < 0)
	{
		fprintf(stdout,"listen() error\n");
		return -1;
	}
	else
	{
		puts("侦听中........");
	}

	while(1)
	{
		socklen_t addrlen = sizeof(struct sockaddr);
		sock_clnt = accept(sock_server,(struct sockaddr *)&client_addr, &addrlen);

		if(sock_clnt < 0)
		{
			fprintf(stdout,"accept() error\n");
			sleep(10);
			continue;
		}

		fprintf(stdout,"连接成功...\n");
		
		pid = fork();
		if(0 == pid)
		{
			process_conn_server(sock_clnt);
			close(sock_server);
			printf("退出子线程成功......\n");
		}
		else
		{
			close(sock_clnt);
			printf("退出主线程成功......\n");
		}

	}
}
