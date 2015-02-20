#include<string.h>
#include<stdio.h>	
#include<stdlib.h>
#define BANNERMAX 500

struct auth
{
 char login_name[25];
 char login_pass[25];
};

void clnt_login_module (int sock,struct auth q)
{
	char buf[BANNERMAX]="\0";
	char buffer[30]="\0";
	recv(sock,buf,BANNERMAX,0);  //接收banner信息
	printf("%s\n",buf);

	while(strcmp(buffer,"success"))
	{
	 send(sock,&q,sizeof(q),0);  //发送加密后的用户名和密码
	 recv(sock,buffer,sizeof(buffer),0); //接收认证信息
	 decrypto(buffer);
	 if(!strcmp(buffer,"fail"))
	 { 
		printf("认证失败.该事件将会被报告.\n");
		exit(1);
	 }
	}
	 printf("正在对用户名和密码进行认证 请稍等...\n");
	 sleep(2);
       printf("登录成功!\n");
}
