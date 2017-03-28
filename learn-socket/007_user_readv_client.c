#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<malloc.h>
#include<sys/uio.h>
#include<signal.h>
#include<netinet/in.h>


#define PORT 4401

static struct iovec* vr=NULL;



void sig_proccess(int signo);
void sig_pipe(int signo);
void process_conn_client(int clnt);

int main(void)
{
	int sock_clnt;

	struct sockaddr_in server_addr;

  signal(SIGINT,sig_proccess);
  signal(SIGPIPE,sig_pipe);

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

void sig_proccess(int signo)
{
  printf("Catch a exit signal!\n");

  free(vr);
  _exit(0);
}
void sig_pipe(int signo)
{
  printf("Catch a SIGPIPE signal!\n");

  free(vr);
  _exit(0);
}
void process_conn_client(int clnt)
{
	ssize_t size = 0;
	char buffer[30];
	memset(buffer, 0 ,30);

  struct iovec *r = (struct iovec *)malloc(3*sizeof(struct iovec));
  if(!r)
  {
    puts("Not enough memory\n");
    return ;
  }

  vr = r;
  r[0].iov_base = buffer;
  r[1].iov_base = buffer + 10;
  r[2].iov_base = buffer + 20;

  r[0].iov_len = r[1].iov_len = r[2].iov_len = 10;

  int i =0;
	while(1)
	{
		
		size = read(0,r[0].iov_base,10);
		if(size > 0)
		{
		 
      r[0].iov_len = size;
      ssize_t a = writev(clnt,r,1);
      printf("发送数据......%d\n",(int)a);
      r[0].iov_len = r[1].iov_len = r[2].iov_len = 10;

      size = readv(clnt,r,3);

      for(i=0;i<3;i++)
      {
        if(r[i].iov_len > 0)
          write(1,r[i].iov_base,r[i].iov_len);
      }
		}
    
	}
}
