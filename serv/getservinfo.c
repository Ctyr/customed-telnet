#include<stdio.h>
#include<string.h>
struct servinfo{
	char servPS  [200];
};

//获得服务器用户名、主机名和目录等信息以设置shell的命令提示符PS1变量
void getservinfo(struct servinfo *p)  
{
	FILE *stream,*pr;
	char servhost[35]="\0",servuser[35]="\0",servdir[100]="\0";	
	stream=popen("echo -n $USER","r"); //取得当前用户名
	fread(servuser,sizeof(char),sizeof(servuser),stream);
	
	gethostname(servhost,sizeof(servhost));  //取得主机名
	pr=popen("echo -n $PWD","r");
	fread(servdir,sizeof(char),sizeof(servdir),pr);
	memset(p->servPS,0,sizeof(p->servPS));
	strcat(p->servPS,servuser);
	strcat(p->servPS,"@");
	strcat(p->servPS,servhost);
	strcat(p->servPS,":");
	strcat(p->servPS,servdir);

	if(strcmp(servuser,"root"))
		strcat(p->servPS,"#");
	else
		strcat(p->servPS,"$");
	strcat(p->servPS,"\0");
	pclose(stream);	
	pclose(pr);
}

