#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define PORT 4401


void process_conn_client(int clnt)
{
	ssize_t size = 0;
	char buffer[1024];
	memset(buffer, 0 ,1024);

	while(1)
	{
		
		size = read(0,buffer,1024);
	//	fprintf(stdout,"%s\n",buffer);
		if(size > 0)
		{
	//		char send_info[1024];
		//	memset(send_info,0,1024);
	//		fgets(send_info,1024,stdin);
		
			write(clnt,buffer,size);
			size = read(clnt,buffer,1024);
			write(1,buffer,size);
		}
	}
}

int main(void)
{
	int sock_clnt;

	struct sockaddr_in server_addr;

	sock_clnt = socket(AF_INET,SOCK_STREAM,0);
	if(sock_clnt < 0)
	{
		fprintf(stdout,"socket() error.\n");
		return -1;
	}

	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);
	int tmp = inet_pton(AF_INET,"127.0.0.1",&server_addr.sin_addr);
	if(tmp < 0)
		puts("失败");
	int res = connect(sock_clnt,(struct sockaddr*)&server_addr,sizeof(struct sockaddr));
	if(res < 0)
	{
		puts("连接失败...\n");
		return 0;
	}
	else
	{
		puts("连接成功....\n");
	}

	process_conn_client(sock_clnt);
	close(sock_clnt);
	return 0;
}
