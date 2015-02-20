#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define FILEMAX 655350

void sendfile(FILE *fp,int clntsock)
{
	char ch='a';
	char flg[50]="CHDFLAGOFFILE";
	char matrix[FILEMAX];
	encrypto(flg);
	send(clntsock,flg,strlen(flg),0);
	unsigned long int i=0;
	char filelen[20];
	char str[20];
	puts("读取文件中...");

	while(ch!=EOF)
	{
	 ch=fgetc(fp);
	 matrix[i]=ch;
	 i++;
	 if(i>=FILEMAX)
		massage("文件大小超出限制！\n");
	}
	matrix[--i]=matrix[i-1];
	sprintf(str,"%d",(int)strlen(matrix));
	strcpy(filelen,str);
	encrypto(filelen);
	send(clntsock,filelen,strlen(filelen),0);  //发送文件大小
	printf("文件大小为:%d字节\n",(int)strlen(matrix));
	while(strcmp(filelen,"OK"))
	recv(clntsock,filelen,sizeof(filelen),0);  //接收客户端的READY消息
	printf("文件发送中...\n");

	encrypto(matrix);  //加密内容并发送
	send(clntsock,matrix,strlen(matrix),0);
	sleep(2);
	printf("文件传送成功！\n");	
	exit(1);
}
