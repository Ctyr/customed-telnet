#include<stdio.h>
#include<netinet/in.h>  //定义了struct sockaddr_in
#include<netdb.h>  //getaddrinfo  & struct addrinfo 
#include<string.h>
#include<stdlib.h>

struct auth
{
 char login_name[25];
 char login_pass[25];
};

int main(int argc,char *argv[])
{
	FILE *fileto=NULL;  //发送给服务器的文件的指针
	struct addrinfo *domain;
	struct auth q;
	if(argc<5||argc>6)
	{
	printf("使用方法:%s <域名/ip> <端口> <用户名> <密码> [需要发送文件的路径]\n",argv[0]);
	exit(1);
	}	
	char *port=argv[2];
	int clntsock=Domain(argv[1],port);
	char *u=argv[3];
	encrypto(u);  //加密和设置认证信息
	strcpy(q.login_name,u);
	u=argv[4];
	encrypto(u);
	strcpy(q.login_pass,u);
	if(argc==6)  //如果参数数量等于6，则进入文件传输模式
	{
	 	fileto=fopen(argv[5],"r");
	 	if(fileto==NULL)
	 	{	
 	  	 	printf("文件%s似乎不存在,请重试\n",argv[5]);
	  	 	exit(1);
		}	
		else	 
		{
			clnt_login_module(clntsock,q);	//调用登录模块
			sendfile(fileto,clntsock);  //进行文件传输
		}
		
	}
	set_signal(clntsock);  //设置信号
	clnt_login_module(clntsock,q);  //登录验证
	process(clntsock);				
}
