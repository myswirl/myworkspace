/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// fib_iter.c
 * Author:			// 
 * Date:			// 
 * Description:		// ��������쳲���������Ч�ʱȵݹ���߼�ʮ��
 * Version:			// 
 * History:			// 
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>


/***********************************************************
* Function:       // fibonacci
* Description:    // ��������fibonacci��
***********************************************************/
long fibonacci( int n )
{
	long	result;
	long	previous_result;
	long	next_older_result;

	result = previous_result = 1;

	while( n > 2 )
	{
		n				  -= 1;
		next_older_result  = previous_result;
		previous_result	   = result;
		result			   = previous_result + next_older_result;
	}
	return result;
}

/***********************************************************
* Function:       // main
* Description:    // ������
***********************************************************/
int main( void )
{
	int n = 3;
	printf( "fibonacci(%d) = %ld\n", n, fibonacci( n ) );
	return 0;
}
/************************************** The End Of File **************************************/
