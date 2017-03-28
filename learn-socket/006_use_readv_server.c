#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<malloc.h>
#include<sys/uio.h>
#include<signal.h>
#include<netinet/in.h>

#define PORT 4401
#define MAXPENDING 2

static struct iovec *vs=NULL;

void process_conn_server(int s);
void sig_pipe(int signo);
void sig_proccess(int signo);

int main(void)
{
	int sock_server, sock_clnt;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	pid_t pid;

  signal(SIGINT,sig_proccess);
  signal(SIGPIPE,sig_pipe);

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

  int ret,on = 1;
  ret = setsockopt(sock_server,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
  //  允许地址重用,取消程序ctrl + c结束后的TIME_WAIT状态。
  if(ret < 0)
  {
    fprintf(stdout,"setsockopt() error.\n");
    return -1;
  }
  else
  {
    puts("setsockopt() success...\n");
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
		// 将accept()注释之后，client　connect()依然可以连接成功。
		// 代表accept() 发生在TCP三次握手之后。

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
		}
		else
		{
			close(sock_clnt);
		}
	}
}

void sig_pipe(int signo)
{
  printf("Catch a SIGPIPE signal\n");

  free(vs);
  _exit(0);
}

void sig_proccess(int signo)
{
  printf("Catch a exit signal\n");

  free(vs);
  _exit(0);
}

void process_conn_server(int s)
{
	ssize_t size = 0;
	char buffer[30];

	memset(buffer,0,30);

  struct iovec* v = (struct iovec *)malloc(3*sizeof(struct iovec));
  if(!v)
  {
    puts("Not enough memory....\n");
    return ;
  }

  vs = v;

  v[0].iov_base = buffer;
  v[1].iov_base = buffer + 10;
  v[2].iov_base = buffer + 20;
  v[0].iov_len = v[1].iov_len = v[2].iov_len = 10;

	while(1)
	{
		size = readv(s,v,3);
		if(size == 0)
		{
      printf("未接收到数据......\n");
			return ;
		}
    sprintf(v[0].iov_base,"%d ",(int)size);
    sprintf(v[1].iov_base,"bytes alt");
    sprintf(v[2].iov_base,"ogetther\n");

    v[0].iov_len = strlen(v[0].iov_base);
    v[1].iov_len = strlen(v[1].iov_base);
    v[2].iov_len = strlen(v[2].iov_base);

    writev(s,v,3);
	}
}



