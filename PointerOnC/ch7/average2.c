/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// average2.c
 * Author:			// 
 * Date:			// 
 * Description:		// 模块描述
 * Version:			// 
 * History:			// 
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>
#include <stdarg.h>

/***********************************************************
* Function:       // average
* Description:    // 可变参数列表函数处理
***********************************************************/
float average( int n_values, ... )
{
	va_list var_arg;
	int		count;
	float	sum = 0;


	/*
	** Prepare to access the variable arguments.
	*/
	va_start( var_arg, n_values );


	/*
	** Add the values from the variable argument list.
	*/
	for( count = 0; count < n_values; count += 1 )
	{
		sum += va_arg( var_arg, int );
	}


	/*
	** Done processing variable arguments.
	*/
	va_end( var_arg );

	return sum / n_values;
}

/***********************************************************
* Function:       // main
* Description:    // 主函数
***********************************************************/
int main( void )
{
	printf( "average:%f \n", average( 5, 1, 2, 3, 4, 5 ) );
	return 0;
}

/************************************** The End Of File **************************************/
