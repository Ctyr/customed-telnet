#include<stdlib.h>
#include<stdio.h>
#include<memory.h>
#include<netdb.h>  		//定义了getaddrinfo函数以及struct addrinfo 
#include<netinet/in.h>  //定义了struct sockaddr_in

char servip[33];
int port;
int Domain(char hostName[],char servName[])
{	
	
	int value;
	struct addrinfo *domain;
	value=getaddrinfo(hostName,servName,NULL,&domain); //进行域名解析
	struct addrinfo *list=domain;
	if(value)
	{
		puts("解析失败!");
		puts(gai_strerror(value));
		exit(1);
	}

	printAddr(hostName,list);  
	int clntsock;
	for(;list!=NULL;list=list->ai_next)  //for循环寻找可以使用的地址
	{
	 clntsock=socket(list->ai_family,list->ai_socktype,list->ai_protocol);
	 if(clntsock<0)	
	 	continue;	
	 break;
	}
	if(clntsock<0)	
		massage("socket failed!");
	printf("正在连接到服务器 %s:%d ...\n",servip,port);
	int flag=connect(clntsock,list->ai_addr,list->ai_addrlen);

	if(flag<0)
		massage("连接失败!");
	else
		print("连接已建立.",1);
	printf("使用'Ctrl+C'退出连接.\n");
	freeaddrinfo(domain);
	return clntsock;
}


int printAddr(char hostName[],struct addrinfo *list)  //打印域名解析和结果
{

	void *binaryip;
	
	struct sockaddr *servaddr=list->ai_addr;
	memset(servip,0,sizeof(servip));
	if(servaddr->sa_family==AF_INET)
	{
	 port=ntohs(((struct sockaddr_in *)servaddr)->sin_port);
	 binaryip=&(((struct sockaddr_in *)servaddr)->sin_addr);
	 inet_ntop(AF_INET,binaryip,servip,sizeof(servip));
	}
	else
	{
	 port=ntohs(((struct sockaddr_in6 *)servaddr)->sin6_port);
	 binaryip=&(((struct sockaddr_in6 *)servaddr)->sin6_addr);
	 inet_ntop(AF_INET6,binaryip,servip,sizeof(servip));
	}
	printf("\n正在对'%s'进行域名解析...\n",hostName);
	printf("主机'%s'DNS解析到:%s.\n",hostName,servip);
	
}

