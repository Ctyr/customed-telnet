#include<netinet/in.h>		//定义了struct sockaddr_in
#include <arpa/inet.h>		//定义了inet_ntop函数
#include<stdio.h>

#define servip "0.0.0.0"	//监听所有ip
extern globalsock;


int CreateSocket(int argc,int port)
{
	int servsock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);  //创建socket
	globalsock=servsock; 
	if(servsock<0)	
		massage("套接字创建失败!");
	else		
		print("套接字创建成功!",1);
	int recvbuff;
	//设置套接字选项使得可以绑定处于TIME_WAIT状态的端口
	struct sockaddr_in servAddr;
	if(setsockopt(servsock,SOL_SOCKET,SO_REUSEADDR,&recvbuff,sizeof(recvbuff))<0)
		massage("设置套接字选项失败!");
	
	servAddr.sin_family=AF_INET;
	servAddr.sin_port=htons(port);

	int flag=inet_pton(AF_INET,servip,&servAddr.sin_addr.s_addr);		
	if(!flag)	
		massage("错误的IP地址!");
	else if(flag<0)	
		massage("pton failed!");

	flag=bind(servsock,(struct sockaddr *)&servAddr,sizeof(servAddr));

	if(flag<0)	
		massage("地址绑定失败!");
	else		
		print("地址绑定成功!",1);

	flag=listen(servsock,SOMAXCONN);	//SOMAXCONN常量定义了最大连接队列

	if(flag<0)	
		massage("侦听失败!");
	else		
		print("侦听成功!",1);
	print("------------------",1);
	printf("侦听地址 %s:%d 等待客户端连接 ...\n",servip,port);
	return servsock;
}
