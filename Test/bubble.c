#include <stdio.h>
#define MAX_RECORD_NUMBER 10

int record[MAX_RECORD_NUMBER] = {12,76,48,62,94,17,37,52,69,32};

void swap(int * x , int * y )
{
	int temp;
	temp = *x;
	*x = *y;
	*y = temp;
}

int main()
{
	int i,j;
	printf("---before sorting---\n");
	for( i = 0; i < MAX_RECORD_NUMBER -1; i++)
		printf("%d ",record[i]);

	for( i = 0 ; i < MAX_RECORD_NUMBER - 1; i++ )
	{
		for( j = MAX_RECORD_NUMBER - 1; j > i; j--)
			if( record[j] < record[j-1] )
				swap(&record[j],&record[j-1]);
	}

	printf("\n---after sorting---\n");

	for( i = 0; i < MAX_RECORD_NUMBER -1; i++)
		printf("%d ",record[i]);
	
	printf("\n");
	return 1;
}
