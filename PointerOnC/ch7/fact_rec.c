/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// fact_rec.c
 * Author:			//
 * Date:			//
 * Description:		// �ݹ����׳�
 * Version:			//
 * History:			//
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>


/***********************************************************
* Function:       // factorial
* Description:    // �ݹ����׳�
* Input:          // 1:n
* Return:         // n�Ľ׳˼�����
***********************************************************/
long factorial( int n )
{
	if( n <= 0 )
	{
		return 1;
	} else
	{
		return n * factorial( n - 1 );
	}
}

/***********************************************************
* Function:       // main
* Description:    // ������
***********************************************************/
int main( void )
{
	int n = 4;
	printf( "%d! = %ld\n", n, factorial( n ) );
	return 0;
}

/************************************** The End Of File **************************************/
