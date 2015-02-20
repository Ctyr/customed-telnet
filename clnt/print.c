#include<string.h>
#include<stdlib.h>  

void print(char nn[],int flag)
{
	unsigned int ni;
	for(ni=0;ni<strlen(nn);ni++)
		putchar(nn[ni]);

	if(flag)
		putchar('\n');
}

void massage(char mm[])
{
	print(mm,1);
	exit(1);
}
