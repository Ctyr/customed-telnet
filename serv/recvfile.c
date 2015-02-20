#include<stdio.h>
#include<string.h>
#define FILEMAX 655350

extern int ClientSockNumber;
extern int ClientSockList[255];

void recvfile(int sock)
{
	FILE *filerecv;
	printf("进入文件传输模式！\n正在接收文件...\n");
	char matrix[FILEMAX];
	char filelen[255];
	int bytesrecv=0;
	int totoalbytes=0;
	recv(sock,filelen,sizeof(filelen),0); //接收文件大小
	decrypto(filelen);
	printf("文件大小为:%s字节\n",filelen);
	send(sock,"OK",strlen("OK"),0);  //准备接收
	while(totoalbytes<atoi(filelen))  //直到完全接收完文件为止
	{
		bytesrecv=recv(sock,matrix+bytesrecv,sizeof(matrix),0);
		totoalbytes+=bytesrecv;
	}
	decrypto(matrix);
	printf("请输入保存的文件名或者完整路径:");
	char buf[255];	
	scanf("%s",buf);
	filerecv=fopen(buf,"w");
	if(filerecv==NULL)
		massage("文件创建失败!\n");
	fprintf(filerecv,"%s",matrix);
	puts("文件写入成功!");
	fclose(filerecv);
	ClientSockNumber--;  //客户端数量减1
	int j=0;
	while(ClientSockList[j]!=sock)
		j++;
	while(ClientSockList[j]!=0) //从当前维护的客户端列表中删除该客户端
	{
		ClientSockList[j]=ClientSockList[j+1];
		j++;
	}
	pthread_exit();
}
