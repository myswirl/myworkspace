/*
** A program that uses the less error-prone memory allocator.
*/
#include <stdio.h>
#include "alloc.h"

int main(void)
{
	int	*new_memory;

	// Get space for a bunch of integers
	new_memory = MALLOC( 25, int );

	printf("MALLOC OK! \n");
	//�ͷŶ�̬������ڴ�ռ�
	free(new_memory);
	return 0;

}
