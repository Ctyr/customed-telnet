void encrypto(char *u)
{
	int i=0,j=0;
	while(u[i]!='\0')
        {
               	u[i]+=j-7;
       	        i++;j++;
		if(j==100)
		  j=0;
        }
}

void decrypto(char *u)
{
	int i=0,j=0;
        while(u[i]!='\0')
        {
               u[i]-=j-7;
               i++;j++;
		if(j==100)
		  j=0;
        }

}
