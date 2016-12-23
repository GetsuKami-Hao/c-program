#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<malloc.h>
#include<mysql/mysql.h>


#include"socket_server.h"

#define server_port "5000"
#define MAXPENDING 1000


void checkOperator(int clntSocket);

void checkOperator(int clntSocket)
{
	char operator[20];
	memset(operator,0,sizeof(operator));

	ssize_t numBytesRcvd =  recv(clntSocket,operator,20,0);
	
	if(numBytesRcvd < 0)
		puts("recv() failed!\n");
	else
	{
		send(clntSocket,"get_operator",sizeof("get_operator"),0);
	}

	int status = strcmp(operator,"user_login");
	if(status == 0)
	{
		send(clntSocket,"recv_operator",sizeof("recv_operator"),0);
		login(clntSocket);
	}
	else if(!strcmp(operator,"user_enroll"))
	{
		//int res = strcmp(operator,"user_enroll");
		send(clntSocket,"recv_operator",sizeof("recv_operator"),0);
		enroll(clntSocket);
	}
	else if(!strcmp(operator,"user_modify_passwd"))
	{
		send(clntSocket,"recv_operator",sizeof("recv_operator"),0);
		modify_passwd(clntSocket);
	}
	else if(!strcmp(operator,"send_question"))
	{
		send(clntSocket,"recv_operator",sizeof("recv_operator"),0);
		recv_question(clntSocket);
	}
	else if(!strcmp(operator,"get_question"))
	{
		send(clntSocket,"recv_operator",sizeof("recv_operator"),0);
		send_question(clntSocket);
	}
	else if(!strcmp(operator,"get_answer"))
	{
		send(clntSocket,"recv_operator",sizeof("recv_operator"),0);
		send_answer(clntSocket);
	}
	else if(!strcmp(operator,"send_answer"))
	{
		send(clntSocket,"recv_operator",sizeof("recv_operator"),0);
		get_answer(clntSocket);
	}
}

void main()
{
	in_port_t servPort = atoi(server_port);
	int servSock;
	if((servSock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0)
		puts("socket() failed!\n");

	struct sockaddr_in servAddr;
	memset(&servAddr,0,sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(servPort);

	if(bind(servSock,(struct sockaddr *)&servAddr,sizeof(servAddr))< 0)
		puts("bind() failed\n");
	if(listen(servSock,MAXPENDING) < 0)
		puts("listen() failed!\n");

	while(1)
	{
		struct sockaddr_in clientAddr;
		socklen_t clientAddrLen = sizeof(clientAddr);
		int clientSock = accept(servSock,(struct sockaddr *)&clientAddr,&clientAddrLen);
		if(clientSock < 0)
			puts("accept() failed!\n");

		char clientName[INET_ADDRSTRLEN];
		if(inet_ntop(AF_INET,&clientAddr.sin_addr.s_addr,clientName,sizeof(clientName))  == NULL)
			puts("Unable to get client addr.\n");

		checkOperator(clientSock);
	}
}
