#include <stdio.h>
#include <stdlib.h>

int main()
{
	char ch;
	printf("reboot your system(y/n)? \n");
	scanf("%c",&ch);
	if( ch =='y' )
	{
		system("reboot");
	}else
	{
		printf("the input char is: %c \n",ch);
	}
	return 0;
}

