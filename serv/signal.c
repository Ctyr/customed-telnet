#include<signal.h>  //signal
#include<stdlib.h>
#include<stdio.h>
#include<netinet/in.h>		//定义了struct sockaddr_in
#include <arpa/inet.h>		//定义了inet_ntop函数
#include<fcntl.h>   //fcntl
#include<pthread.h>
#include<string.h>

struct ThreadArgs{  //定义参数结构体，传递给线程处理函数
	int clntsock;
	struct sockaddr_in clntAddr;
};
extern int ClientSockNumber;
extern int ClientSockList[255];

void pipdetect();
void exitServ();
void ServProcess();
extern globalsock;

void* ThreadMain(void *threadArgs);

void set_signal()
{
	/*******************************signal INT*******************************/
	struct sigaction userInt;
	userInt.sa_handler = exitServ;
	sigfillset(&userInt.sa_mask);
	userInt.sa_flags=0;
	if(sigaction(SIGINT,&userInt,NULL)<0)
		massage("sigaction failed!");


	/*******************************signal PIPE*******************************/
	struct sigaction userPIP;
	userPIP.sa_handler = pipdetect;
	sigfillset(&userPIP.sa_mask);
	userPIP.sa_flags=0;
	if(sigaction(SIGPIPE,&userPIP,NULL)<0)
		massage("sigaction failed!");
	
	/*******************************signal IO********************************/
	struct sigaction SIGIOhandler;
	SIGIOhandler.sa_handler = ServProcess;
	sigfillset(&SIGIOhandler.sa_mask);
	SIGIOhandler.sa_flags=0;

	if(sigaction(SIGIO,&SIGIOhandler,NULL)<0)
		massage("sigaction IO failed!");

	fcntl(globalsock,F_SETOWN,getpid());  
		//F_SETOWN标识要为这个套接字接受SIGIO的进程
	fcntl(globalsock,F_SETFL,O_NONBLOCK|FASYNC);
		//FASYNC标识使用异步IO，在对端分组到达时递交IO信号
		//设置O_NONBLOCK标志表示不希望再recvfrom中阻塞
	
}

void ServProcess()  //当SIGIO信号到达时执行该函数
{
 	  struct sockaddr_in clntAddr;
	  int clntAddrLen=sizeof(clntAddr);
	  int clntsock=accept(globalsock,(struct sockaddr *)&clntAddr,&clntAddrLen);
	  ClientSockList[ClientSockNumber]=clntsock;
	  
	  struct ThreadArgs *threadArgs=(struct ThreadArgs*)malloc(sizeof(struct ThreadArgs));  
	  threadArgs->clntsock=clntsock;//传递参数
	  threadArgs->clntAddr=clntAddr;

	  pthread_t threadID;
	  int rtnval=pthread_create(&threadID,NULL,ThreadMain,threadArgs);//创建线程
	  if (rtnval!=0)
	  	massage("pthread_create() failed!");

	  printf("发现新客户端！线程ID:%lu\n",threadID);
	  printf("当前客户端数量:%d\n",++ClientSockNumber);
	  int j=0;
	  printf("当前客户端套接字列表为:");
	  while(ClientSockList[j]!=0)  //维护在线客户端套接字列表
	  {
		printf("%d-->",ClientSockList[j]);
		j++;
	  }
	  putchar('\n');
}

void pipdetect()
{
	puts("pip exit...");
	pthread_exit(NULL);
}

void exitServ()  //SIGINT信号触发该函数，服务器下线并发送LEAVE消息给所有在线客户端
{
	 char slflag[100]="CHDFLAGOFSERVLEAVE";
	 encrypto(slflag);
	 printf("\n正在退出服务器 ...\n");
	 sleep(1);
	 while(ClientSockNumber>=0)
	 {	 
	 	send(ClientSockList[ClientSockNumber],slflag,strlen(slflag),0);
		ClientSockNumber--;
	 }
	 close(globalsock);
	 printf("已成功退出\n");
	 exit(0);
}

void* ThreadMain(void *threadArgs)
{
	pthread_detach(pthread_self()); //线程分离，pthread_self()用于获取当前线程ID
	int clntsock=((struct ThreadArgs *)threadArgs)->clntsock; //获取套接字
	struct sockaddr_in clntAddr=((struct ThreadArgs *)threadArgs)->clntAddr;
	free(threadArgs); //不再需要该结构，可以释放
	serv_login_module(clntsock);
	process(clntsock,clntAddr);  //处理客户端

	return NULL;
}
