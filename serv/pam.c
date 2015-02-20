#include<string.h>	
#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>

struct auth
{
 char login_name[25];
 char login_pass[25];
};
 struct auth right; 

void set_password(char *u,char *p) //设置认证信息
{
	strcpy(right.login_name,u);
	strcpy(right.login_pass,p);
}

int compare(struct auth q)  //判断认证信息是否正确
{
	char *u=q.login_name,*p=q.login_pass;
	decrypto(u);
	decrypto(p);
	if(!strcmp(right.login_name,u))
		if(!strcmp(right.login_pass,p))
			return 1;
	return 0;
}

void serv_login_module(int sock)  //登录认证模块
{
	char banner[90]="欢迎访问Tyr.Chen自定义telnet服务器！";
	char buf[20]="\0";
	char succflag[20]="success";
	char failflag[20]="fail";
	encrypto(failflag);
	encrypto(succflag);
	struct auth q;
	puts(banner);
	send(sock,banner,strlen(banner),0);
	while(strcmp(buf,"success"))
	{
	 recv(sock,&q,sizeof(q),0);
	 if(compare(q))
	 {
 		 send(sock,succflag,strlen(succflag),0);
		 strcpy(buf,"success");
		 printf("认证成功.\n");
		 return;
	 }
	 else
	 {
		send(sock,failflag,strlen(failflag),0);
		printf("认证失败.该事件将会被报告.\n");
		pthread_exit(NULL);
	 }
	}
	
}


