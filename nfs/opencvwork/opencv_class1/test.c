#include<stdio.h>
main()
{
	int i;
	char str[10];
	int a = 1231231222;
	sprintf(str,"%d",a);
	for(i=0;i<10;i++)
	{
		printf("%c",str[i]);
	}
	printf("\n");
}
