#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<malloc.h>
#include<mysql/mysql.h>

#define server_port "5000"
#define MAXPENDING 1000

void checkOperator(int clntSocket);
int login(int clntSocket);
int openMysql(MYSQL **conn);
int isExistUser(char *user);
int isExistPhone(char *phone);
int enroll(int clntSocket);
int modify_passwd(int clntSocket);
void recv_question(int clntSocket);
void send_question(int clntSocket);
void send_answer(int clntSocket);

void send_answer(int clntSocket)
{
	MYSQL *conn;
	if(openMysql(&conn))
	{
		char buffer[10] = {0};
		recv(clntSocket,buffer,10,0);
		char sql[BUFSIZ] = {0};

		strcpy(sql,"select * from user_answer where id = ");
		strcat(sql,buffer);
		puts(sql);
		int stat = mysql_query(conn,sql);
		if(stat)
		{
			printf("select error %s\n",mysql_error(conn));
			send(clntSocket,"send_answer_false",sizeof("send_answer_false"),0);
		}
		else
		{
			MYSQL_RES *res;
			MYSQL_ROW row;
			res = mysql_store_result(conn);
			if(res)
			{
				int t;
				while(row = mysql_fetch_row(res))
				{
					for(t = 0;t<mysql_num_fields(res);t++)
					{
						send(clntSocket,row[t],strlen(row[t]),0);
					}
				}
			}
			mysql_free_result(res);
		}
		mysql_close(conn);

	}
}
void send_question(int clntSocket)
{
	MYSQL *conn;
	if(openMysql(&conn))
	{
		char buffer[10] = {0};
		recv(clntSocket,buffer,10,0);
		int mysql_index = atoi(buffer);
		mysql_index += 5;
		char tmp[10] = {0};
		sprintf(tmp,"%d",mysql_index);
		char sql[BUFSIZ] = {0};
		strcpy(sql,"select * from user_question order by time desc,id desc limit ");
		strcat(sql,buffer);
		strcat(sql,",");
		strcat(sql,tmp);
		puts(sql);

		//res = mysql_query(conn,"select * from user_question order by time DESC ,id desc limit 0,5");

		int stat = mysql_query(conn,sql);
		if(stat)
		{
			printf("select error %s\n",mysql_error(conn));
			send(clntSocket,"send_question_false",sizeof("send_question_false"),0);
		}
		else
		{
			MYSQL_RES *res;
			MYSQL_ROW row;
			
			res = mysql_store_result(conn);
			if(res)
			{
				int t;
				while(row = mysql_fetch_row(res))
				{
					for(t=0;t<mysql_num_fields(res);t++)
					{
						send(clntSocket,row[t],strlen(row[t]),0);
					}
				}
			}
			mysql_free_result(res);
		}
		mysql_close(conn);
	}
}

void recv_question(int clntSocket)
{
	MYSQL *conn;
	if(openMysql(&conn))
	{
		char buffer[BUFSIZ];
		memset(buffer,sizeof(char)*BUFSIZ,0);

		ssize_t numBytesRecv = recv(clntSocket,buffer,BUFSIZ,0);

		if(numBytesRecv < 0 )
			puts("recv() failed!\n");
		char *result_gather[4];
		int i = 0;
		char *tmp;
		tmp = buffer;
		result_gather[i] = strsep(&tmp,"|");
		while(result_gather[i])
		{
			i++;
			result_gather[i] = strsep(&tmp,"|");
		}

		char sql[BUFSIZ] = {0};
		strcpy(sql,"insert into user_question(account,title,content)values('");
		strcat(sql,result_gather[0]);
		strcat(sql,"','");
		strcat(sql,result_gather[1]);
		strcat(sql,"','");
		strcat(sql,result_gather[2]);
		strcat(sql,"')");
		puts(sql);

		int res = mysql_query(conn,sql);
		if(res)
		{
			printf("insert error : %s\n",mysql_error(conn));
			send(clntSocket,"recv_question_false",sizeof("recv_question_false"),0);
		}
		else
		{
			int ok = mysql_affected_rows(conn);
			mysql_commit(conn);
			mysql_close(conn);

			if(ok >= 1)
			{
				send(clntSocket,"recv_question_true",sizeof("recv_question_true"),0);
			}
			else
				send(clntSocket,"recv_question_false",sizeof("recv_question_false"),0);

		}
	}
	else
		puts("open mysql faile!");
}
int isExistPhone(char *phone)
{
	int result = -1;
	char sql[50] = {0};

	strcpy(sql,"select * from user where phone = '");
	strcat(sql,phone);
	strcat(sql,"'");
	MYSQL *conn;
	int status = openMysql(&conn);

	if(status >= 1)
	{
		MYSQL_RES *res_ptr = NULL;
		MYSQL_ROW sqlrow = 0;
		int res = mysql_query(conn,sql);
		if(!res)
		{
			res_ptr = mysql_store_result(conn);
			if(res_ptr)
			{
				result = (int)mysql_num_rows(res_ptr);
			}
			mysql_free_result(res_ptr);
		}
	}
	mysql_commit(conn);
	mysql_close(conn);
	return result;
}

int isExistUser(char *user)
{
	int result = -1;
	char sql[50] = {0};

	strcpy(sql,"select * from user where account = '");
	strcat(sql,user);
	strcat(sql,"'");
	MYSQL * conn;
	int status = openMysql(&conn);
	//puts(sql);
	//printf("status = %d\n",status);
	if(status >= 1)
	{
		MYSQL_RES *res_ptr = NULL;
		MYSQL_ROW sqlrow = 0;
		int res = mysql_query(conn,sql);
	//	puts("msyql\n");
		if(!res)
		{
			res_ptr = mysql_store_result(conn);
			if(res_ptr)
			{
				result = (int)mysql_num_rows(res_ptr);
			}
			mysql_free_result(res_ptr);
		}
	}
	mysql_commit(conn);
	mysql_close(conn);
	//printf("result = %d\n",result);
	return result;
	
}
int openMysql(MYSQL **conn)
{
	*conn = mysql_init(NULL);

	if(!(*conn))
		return -1;

	*conn = mysql_real_connect(*conn,"localhost","root","123456","user_information_system",0,NULL,0);
	if(mysql_set_character_set(*conn,"utf8"))
	{
		fprintf(stderr,"error %s\n",mysql_error(*conn));
	}

	if(*conn)
	{
		return 1;
	}
	else
		return -1;
}
int modify_passwd(int clntSocket)
{
	char buffer[BUFSIZ];
	memset(buffer,0,BUFSIZ);
	ssize_t numBytesRcvd = recv(clntSocket,buffer,BUFSIZ,0);
	if(numBytesRcvd < 0)
		puts("recv() failed!\n");
	char *result_gather[4] = {0};
	int i = 0;
	char * tmp = NULL;
	tmp = buffer;

	result_gather[i] = strsep(&tmp,"|");
	while(result_gather[i])
	{
		i++;
		result_gather[i] = strsep(&tmp,"|");
	}
	int result = isExistUser(result_gather[0]);
	if(result <= 0)
	{
		send(clntSocket,"user_no_exist",sizeof("user_no_exist"),0);
		return -1;
	}
	result = isExistPhone(result_gather[2]);
	if(result <= 0)
	{
		send(clntSocket,"phone_no_exist",sizeof("phone_no_exist"),0);
		return -1;
	}
	MYSQL *conn_ptr;
	int res = openMysql(&conn_ptr);
	if(res == 1)
	{
		char sql[100] = {0};
		strcpy(sql,"update user set passwd = '");
		strcat(sql,result_gather[1]);
		strcat(sql,"' where account = '");
		strcat(sql,result_gather[0]);
		strcat(sql,"' and phone = '");
		strcat(sql,result_gather[2]);
		strcat(sql,"'");
		puts(sql);
		mysql_query(conn_ptr,sql);
		int ok = mysql_affected_rows(conn_ptr);
		mysql_commit(conn_ptr);
		mysql_close(conn_ptr);
		if(ok >= 1)
		{
			send(clntSocket,"modify_passwd_true",sizeof("modify_passwd_true"),0);
			return ok;
		}
		else
		{
			send(clntSocket,"modify_passwd_false",sizeof("modify_passwd_false"),0);
			return -1;
		}
		
	}
	else
	{
		send(clntSocket,"modify_passwd_false",sizeof("modify_passwd_false"),0);
		puts("opneMysql() failed!\n");
		return -1;
	}
}
int enroll(int clntSocket)
{
	char buffer[BUFSIZ];
	memset(buffer,0,BUFSIZ);
	ssize_t numBytesRcvd = recv(clntSocket,buffer,BUFSIZ,0);
	if(numBytesRcvd < 0)
		puts("recv() failed!\n");

	char *result_gather[5];
	int i=0;
	char *tmp;
	tmp = buffer;
	result_gather[i] = strsep(&tmp,"|");
	while(result_gather[i])
	{
		i++;
		result_gather[i] = strsep(&tmp,"|");
	}
	int result = isExistUser(result_gather[0]);
	if(result > 0)
	{
		send(clntSocket,"user_exist",sizeof("user_exist"),0);
		return -1;
	}
	result = isExistPhone(result_gather[2]);
	if(result > 0)
	{
		send(clntSocket,"phone_exist",sizeof("phone_exist"),0);
		return -1;
	}

	MYSQL *conn_ptr;
	int res = openMysql(&conn_ptr);
	if(res == 1)
	{
		char sql[100] = {0};
		strcpy(sql,"insert into user(account,passwd,phone,bus_number)values('");
		strcat(sql,result_gather[0]);
		strcat(sql,"','");
		strcat(sql,result_gather[1]);
		strcat(sql,"','");
		strcat(sql,result_gather[2]);
		strcat(sql,"','");
		strcat(sql,result_gather[3]);
		strcat(sql,"')");
		int res = mysql_query(conn_ptr,sql);
		if(res)
		{
			printf("insert error : %s\n",mysql_error(conn_ptr));
		}
		else
		
		{
			int ok = mysql_affected_rows(conn_ptr);
			mysql_commit(conn_ptr);
			mysql_close(conn_ptr);
			if(ok >= 1)
			{
				send(clntSocket,"enroll_true",sizeof("enroll_true"),0);
				return 1;
			}
			else
			{
				send(clntSocket,"enroll_false",sizeof("enroll_false"),0);
				return -1;
			}

		}
	}
	else
	{
		puts("openMysql() failed!\n");
		return -1;
	}
}

int login(int clntSocket)
{
	MYSQL *conn_ptr;
	int res = openMysql(&conn_ptr);
	if(res == 1)
	{
		char buffer[BUFSIZ];
		memset(buffer,0,BUFSIZ);

		ssize_t numBytesRcvd = recv(clntSocket,buffer,BUFSIZ,0);

		if(numBytesRcvd < 0)
			puts("recv() failed!!\n");
		char *result_gather[3];
		int i = 0;
		char * tmp;
		tmp = buffer;

		result_gather[i] = strsep(&tmp,"|");
		while(result_gather[i])
		{
			//puts(result_gather[i]);
			i++;
			result_gather[i] = strsep(&tmp,"|");
		}
		int result = isExistUser(result_gather[0]);

		if(result <= 0)
		{
			mysql_close(conn_ptr);
			send(clntSocket,"user_no_exist",sizeof("user_no_exist"),0);
			return -1;
		}
		else
		{
			char sql[100]= "select * from user where account = '";
			strcat(sql,result_gather[0]);
			strcat(sql,"' and passwd = '");
			strcat(sql,result_gather[1]);
			strcat(sql,"'");
			puts(sql);
			int res = mysql_query(conn_ptr,sql);
			if(res)
			{
				printf("select error:%s\n",mysql_error(conn_ptr));
			}
			else
			{
				MYSQL_RES * res_ptr ;
				res_ptr = mysql_store_result(conn_ptr);
				int status;
				if(res_ptr)
				{
					status = (int)mysql_num_rows(res_ptr);
				}
				mysql_free_result(res_ptr);
				if(status >= 1)
				{
					send(clntSocket,"login_true",sizeof("login_true"),0);
					MYSQL_ROW row;
					char *buff[5];
					int t;
					while(row=mysql_fetch_row(res_ptr))
					{
						for(t = 0;t<mysql_num_fields(res_ptr);t++)
						{
							buff[t] = row[t];
							puts(buff[t]);
						}
					}
				}
				else
					send(clntSocket,"login_false",sizeof("login_false"),0);
			}
		}
		mysql_commit(conn_ptr);
		mysql_close(conn_ptr);
		return 1;		
	}
	else
	{
		//mysql_close(conn_ptr);
		puts("openMysql() failed!\n ");
		return -1;
	}

}
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
