/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// fib_rec.c
 * Author:			// 
 * Date:			// 
 * Description:		// �ݹ麯�������ɶ��Ժã����Ƕ��⿪��̫��
 * Version:			// 
 * History:			// 
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>


/***********************************************************
* Function:       // fibonacci
* Description:    // �ݹ����쳲�������
***********************************************************/
long fibonacci( int n )
{
	if( n <= 2 )
	{
		return 1;
	}

	return fibonacci( n - 1 ) + fibonacci( n - 2 );
}

/***********************************************************
* Function:       // main
* Description:    // ������
***********************************************************/
int main( void )
{
	int n = 3;
	printf("fibonacci(%d) = %ld\n", n, fibonacci(n) );
	return 0;
}
/************************************** The End Of File **************************************/
