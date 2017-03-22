#include<netdb.h>
#include<string.h>
#include<stdio.h>
#include<arpa/inet.h>

int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    printf("Please input hostname\n");
    return 0;
  }
  char *host = argv[1];
  char host2[] = "www.sina.com.cn";

  struct hostent *ht = NULL;
  struct hostent *ht2 =NULL;

  /*
   * struct hostent{
   *  char *h_name;       //主机的正式名称
   *  char **h_aliases;   //别名列表
   *  int h_addrtype;     //主机地址类型 IPV4 or IPV6
   *  int h_length;       //地址长度
   *  char **h_addr_list; //地址列表
   * }
   *
   * */
  char str[30];

  ht = gethostbyname(host);
//ht2 = gethostbyname(host2);  // 函数gethostbyname()不可重入，下面输出的都是关于www.sina.com.cn的信息。 
  if(ht)
  {
    int i = 0;
    printf("get the host addr: %s\n",host);
    printf("name: %s\n",ht->h_name);
    printf("type: %s\n",ht->h_addrtype == AF_INET? "IPV4":"IPV6");

    printf("length: %d\n",ht->h_length);
    for(i=0;;i++)
    {
      if(ht->h_addr_list[i] != NULL)
        printf("IP:%s\n",inet_ntop(ht->h_addrtype,ht->h_addr_list[i],str,30));
      else
        break;
    }

    for(i=0;;i++)
    {
      if(ht->h_aliases[i] != NULL)
        printf("alias-%d:%s\n",i+1,ht->h_aliases[i]);
      else
        break;
    }
  }
  else
  {
    printf("hostname invalid.\n");
    return 0;
  }

  ht2 = gethostbyname(host2);

  if(ht2)
  {
    int i = 0;
    printf("get the host addr: %s\n",host2);
    printf("name: %s\n",ht2->h_name);
    printf("type: %s\n",ht2->h_addrtype == AF_INET? "IPV4":"IPV6");

    printf("length: %d\n",ht2->h_length);
    for(i=0;;i++)
    {
      if(ht2->h_addr_list[i] != NULL)
        printf("IP:%s\n",inet_ntop(ht2->h_addrtype,ht2->h_addr_list[i],str,30));
      else
        break;
    }

    for(i=0;;i++)
    {
      if(ht2->h_aliases[i] != NULL)
        printf("alias-%d:%s\n",i+1,ht2->h_aliases[i]);
      else
        break;
    }
  }
  else
  {
    printf("hostname invalid.\n");
    return 0;
  }
  return 0;
}
