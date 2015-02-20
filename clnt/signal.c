#include<signal.h>  //signal
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void clntexit();
int globalclntsock;
void set_signal(int clntsock)  //设置SIGINT信号
{
	globalclntsock=clntsock;
	struct sigaction userInt;
	userInt.sa_handler = clntexit;
	sigfillset(&userInt.sa_mask);
	userInt.sa_flags=0;
	if(sigaction(SIGINT,&userInt,NULL)<0)
		massage("sigaction failed!");

}

void clntexit()  //SIGINT信号到达时触发该函数
{
	 char fl[30]="CHDFLAGOFLEAVING";
	 encrypto(fl);
	 printf("\n正在终止与服务器的连接 ...\n");
	 send(globalclntsock,fl,strlen(fl),0);  //向服务器发送LEAVE消息
         sleep(1);
	 close(globalclntsock);
	 printf("已退出\n");
	 exit(0);
}
