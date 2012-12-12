/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// fact_rec.c
 * Author:			//
 * Date:			//
 * Description:		// µÝ¹é¼ÆËã½×³Ë
 * Version:			//
 * History:			//
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>


/***********************************************************
* Function:       // factorial
* Description:    // µÝ¹é¼ÆËã½×³Ë
* Input:          // 1:n
* Return:         // nµÄ½×³Ë¼ÆËã½á¹û
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
* Description:    // Ö÷º¯Êý
***********************************************************/
int main( void )
{
	int n = 4;
	printf( "%d! = %ld\n", n, factorial( n ) );
	return 0;
}

/************************************** The End Of File **************************************/
