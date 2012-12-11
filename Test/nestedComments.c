#include <stdio.h>

int main(void)
{
	int isNestedComments;
	isNestedComments = /*/*/0*/**/1;
	if( 1 == isNestedComments )
	{
		printf("ÔÊĞíÇ¶Ì××¢ÊÍ\n");
	}else
	{
		printf("²»ÔÊĞíÇ¶Ì××¢ÊÍ\n");
	}
	
	return 0;
}

