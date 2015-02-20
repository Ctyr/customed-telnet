#include<stdio.h>
#include<netinet/in.h>		//定义了struct sockaddr_in
#include <arpa/inet.h>		//定义了inet_ntop函数
#include<stdlib.h>

int globalsock;
int ClientSockNumber=0;  //在线客户端数量
int ClientSockList[255]={0};  //在线客户端套接字列表

int main(int argc,char *argv[])
{
	print("\n欢迎访问Tyr.Chen自定义telnet服务器！",1);
	if(argc!=4)
	  {
		printf("使用方法:%s <端口> <用户名> <密码>\n",argv[0]);
		exit(0);
	  }
	int port=atoi(argv[1]);
	int servsock=CreateSocket(argc,port); //创建套接字
	
	set_password(argv[2],argv[3]);  //设置密码
	set_signal();  //设置信号

	while(1)
	{
	 puts("等待客户端响应...");
	 sleep(10);
   	}
}


