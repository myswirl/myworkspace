/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// fib_iter.c
 * Author:			// 
 * Date:			// 
 * Description:		// 迭代计算斐波那契数，效率比递归提高几十万倍
 * Version:			// 
 * History:			// 
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>


/***********************************************************
* Function:       // fibonacci
* Description:    // 迭代调用fibonacci数
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
* Description:    // 主函数
***********************************************************/
int main( void )
{
	int n = 3;
	printf( "fibonacci(%d) = %ld\n", n, fibonacci( n ) );
	return 0;
}
/************************************** The End Of File **************************************/
