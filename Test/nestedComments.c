#include <stdio.h>

int main(void)
{
	int isNestedComments;
	isNestedComments = /*/*/0*/**/1;
	if( 1 == isNestedComments )
	{
		printf("����Ƕ��ע��\n");
	}else
	{
		printf("������Ƕ��ע��\n");
	}
	
	return 0;
}

