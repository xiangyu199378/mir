#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>

void sendstr(char **pstr,int *a,int *b);

void main()
{
	int a=-90,b=-320;
	char *pstr;
	

	sendstr(&pstr,&a,&b);

	write(pstr);


}

void sendstr(char **pstr,int *a,int *b)
{
	char i=0;
	char len1=0,len2=0;
	char *str1 = (char *)malloc(5),*str2 = (char *)malloc(7);

	sprintf(str1,"%d",*a);
	sprintf(str2,"%d",*b);

	puts(str1);
	puts(str2);

	len1 = strlen(str1);
	len2 = strlen(str2);
	printf("len1 = %d\tlen2 = %d\n",len1,len2);
	
	for(i = len1;i >= 0;i--)
		str1[i+1] = str1[i];
	str1[0] = 'S';

	printf("str1 = %s\n",str1);


	for(i = len2-1;i >= 0;i--)
		str2[i+1] = str2[i];
	str2[0] = 'P';
	str2[len2+1] = 'E';
	str2[len2+2] = '\0';
	printf("str2 = %s\n",str2);

	*pstr = strcat(str1,str2);	
	printf("*pstr = %s\n",*pstr);
}
