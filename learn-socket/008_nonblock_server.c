#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<fcntl.h>

#define PORT 4401
#define MAXPENDING 40

int main(int argc, char *argv[])
{
  struct sockaddr_in local,remot;
  int len;
  int serv_sock,clnt_sock;

  local.sin_family = AF_INET;
  local.sin_port = htons(PORT);
  local.sin_addr.s_addr = htonl(INADDR_ANY);

  serv_sock = socket(AF_INET,SOCK_STREAM,0);
  fcntl(serv_sock,F_SETFL,O_NONBLOCK);

  int err = bind(serv_sock,(struct sockaddr *)&local,sizeof(local));
  if(err < 0)
  {
    puts("bind() faile...\n");
    return -1;
  }

  err = listen(serv_sock,MAXPENDING);
  if(err < 0)
  {
    puts("listen() faile...\n");
    return -1;
  }
  else
  {
    puts("侦听中......\n");
  }
  while(1)
  {
    while(serv_sock < 0){
      char buffer[1024];
      memset(buffer,0,sizeof(buffer));

      clnt_sock = accept(serv_sock,(struct sockaddr*)&remot,&len);

      while(recv(serv_sock,buffer,1024,MSG_DONTWAIT) <= 0)
        ;
      if(strcmp(buffer,"HELLO") == 0){
        send(serv_sock,"OK",3,0);
        close(clnt_sock);
        continue;
      }

      if(strcmp(buffer,"SHUTDOWN") == 0){
        send(serv_sock,"BYE",3,0);
        close(clnt_sock);
        break;
      }
    }
  }
  close(serv_sock);
  return 0;
}
