#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>		//定义了struct sockaddr_in
#include <arpa/inet.h>		//定义了inet_ntop函数
#include<netdb.h>   		// 定义了 NI_MAXHOST & NI_MAXSERV
#include<time.h>

#define BUFSIZE	655350
#define COMRET 655350

FILE *fp;
extern int ClientSockNumber;
extern int ClientSockList[255];

//服务器返回的命令提示符，格式为:username@hostname:dirname[$|#]
struct servinfo{  
	char servPS  [200];
};
char clntip[16];

void process(int sock,struct sockaddr_in clntAddr)
{
	int byteRecv,byteSend;
	char buffer[BUFSIZE];

	FILE *stream;
	char hostName[NI_MAXHOST]; 
	char servName[NI_MAXSERV];
	char ComRTN[COMRET];
	time_t timep;
  	time(&timep);
	char timeo[255];
	strcpy(timeo,ctime(&timep));  //记录命令执行时间
	timeo[strlen(timeo)-1]='\0';
	if(inet_ntop(AF_INET,&clntAddr.sin_addr.s_addr,clntip,sizeof(clntip))!=NULL)
	 {

	 	int rtnVal=getnameinfo((struct sockaddr *)&clntAddr,sizeof(clntAddr),hostName,sizeof(hostName),servName,sizeof(servName),0);
		//getnameinfo反向解析客户端hostname
		printf("与客户端%s(%s)开始会话\n",hostName,clntip);
	  	fp=fopen("./server.log","a+");
		//记录连接到日志
		fprintf(fp,"%s\n--------------------------------------\n",ctime(&timep));
	  	fprintf(fp,"与客户端%s(%s)的会话\n",hostName,clntip);
		fclose(fp);
	}
	else
		massage("无法获得客户端地址!");
	struct servinfo local;
	int ii=1;
	getservinfo(&local);
	send(sock,&local,sizeof(local),0);  //发送PS1值
	while(1)
	{
	 fp=fopen("./server.log","a+");
	
	 memset(buffer,0,BUFSIZE);
	 memset(ComRTN,0,COMRET);
	 byteRecv=recv(sock,buffer,BUFSIZE,0);	
	 decrypto(buffer);
	 if (byteRecv<0)
	 	massage("接收失败");
	 if(!strcmp(buffer,"CHDFLAGOFLEAVING"))  //此为客户端离开消息
	 {
		printf("客户端'%s' 已经下线 ...\n",clntip);
		
		printf("当前客户端数量:%d\n",--ClientSockNumber);
		fprintf(fp,"客户端'%s'已下线 ...\n",clntip);
		int j=0;
		while(ClientSockList[j]!=sock)
			j++;
		while(ClientSockList[j]!=0)  //不再维护该客户端套接字
		{
			ClientSockList[j]=ClientSockList[j+1];
			j++;
		}
		pthread_exit(NULL);
	 }
	 if(!strcmp(buffer,"CHDFLAGOFFILE"))  //客户端进入文件传输模式
	 {
		recvfile(sock);
	 }
	 printf("[%s] %s\n",timeo,buffer);  //记录时间和命令
	 fprintf(fp,"%d\t%s\n",ii++,buffer);
	 stream=popen(buffer,"r");
	 fread(ComRTN,sizeof(char),COMRET,stream);

	 if(ComRTN[0]!='\0') 
	 {
		encrypto(ComRTN);  //加密消息并发送
	 	byteSend=send(sock,ComRTN,strlen(ComRTN),0);
	 }
	 else
		continue;  //如果命令返回值为空，则继续向下执行
	 pclose(stream);
	 fclose(fp);
	}
	

}
