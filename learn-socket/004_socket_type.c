#include<sys/socket.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdio.h>

int is_socket_type(int fd)
{
  struct stat st;
  int err = fstat(fd,&st);

  if(err < 0)
  {
    return -1;
  }

  if((st.st_mode & S_IFMT) == S_IFSOCK)
    return 1;
  else
    return 0;
}

void main(void)
{
  int ret = is_socket_type(0); //判断标准输入是否是套接字描述符
  printf("value %d\n",ret);

  int s = socket(AF_INET,SOCK_STREAM,0);
  ret = is_socket_type(s);
  printf("value %d\n",ret);

  return ;
}

