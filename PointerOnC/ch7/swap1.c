/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// swap1.c
 * Author:			// 
 * Date:			// 
 * Description:		// 
 * Version:			// 
 * History:			// 
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>


/***********************************************************
* Function:       // swap
* Description:    // 交换函数，交换整数x和y的值，此函数无效
* Others:         // 因为函数参数是传值调用
***********************************************************/
void swap( int x, int y )
{
	int temp;

	temp   = x;
	x	   = y;
	y	   = temp;
}

/***********************************************************
* Function:       // main
* Description:    // 主函数
* Others:         // 
***********************************************************/
int main( void )
{
	int x=1;
	int y=2;
	printf( "before swap x:%d, y:%d \n",x ,y );
	swap(x,y);
	printf( "after  swap x:%d, y:%d \n",x ,y );	
	return 0;
}

/************************************** The End Of File **************************************/
