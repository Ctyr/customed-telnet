#include<stdio.h>
#include<memory.h>
#include<string.h>
#include<signal.h>
#include<fcntl.h>

#define SEND 655350
#define RECV 655350
struct servinfo{
	char servPS[200];
};
void clnt_set_signal();
void clntrecv();

int clngosock;

void process(int sock)
{
	int byteRecv,byteSend;
	char ComSend[SEND],ComRecv[RECV];
	const char com_flag[30]="CHD-COMMAND-FINISHED-FLAG";
	struct servinfo q;

	clnt_set_signal(sock);  //设置SIGIO信号
	clngosock=sock;
	recv(sock,&q,sizeof(q),0);  //接收来自服务器的命令提示符
	while(1)
	{
		memset(ComSend,0,SEND);
		memset(ComRecv,0,RECV);
		printf("%s",q.servPS);  //打印命令提示符
		gets(ComSend); 
		//scanf("%s",buffer);
		encrypto(ComSend);  //加密命令并发送给服务器
		byteSend= send(sock,ComSend,strlen(ComSend),0);
		if(byteSend<0)
			massage("send failed");
	}
}

void clnt_set_signal(int sock)  //设置IO信号
{
	struct sigaction clnt_SIGIO;
	clnt_SIGIO.sa_handler = clntrecv;
	sigfillset(&clnt_SIGIO.sa_mask);
	clnt_SIGIO.sa_flags=0;

	if(sigaction(SIGIO,&clnt_SIGIO,NULL)<0)
		massage("sigaction IO failed!");

	fcntl(sock,F_SETOWN,getpid());
	fcntl(sock,F_SETFL,O_NONBLOCK|FASYNC);
}

void clntrecv()  //SIGIO信号触发该函数
{

	int byteRecv,byteSend;
	char ComSend[SEND],ComRecv[RECV];
	memset(ComSend,0,SEND);
	memset(ComRecv,0,RECV);
	byteRecv= recv(clngosock,ComRecv,RECV,0);
	decrypto(ComRecv);
	if(byteRecv<0)
		puts("recv error");
	else
	if(!strcmp(ComRecv,"CHDFLAGOFSERVLEAVE"))  //如果接收到服务器leave消息，则退出
		massage("\n服务器已经下线.\n退出中...\n");
	putchar('\n');
	print(ComRecv,1);  //打印接收到的命令的执行结果
}
