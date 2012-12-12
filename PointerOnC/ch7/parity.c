/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// parity.c
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
* Function:       // even_parity
* Description:    // 偶校验函数，也就是判断value的二进制为模式中1的个数是否为偶数个
* Return:         // true: 是偶数个;false:不是偶数个;
* Others:         // 
***********************************************************/
int even_parity( int value, int n_bits )
{
	int parity = 0;
	//统计1的个数
	while( n_bits > 0 )
	{
		parity	  += value & 1;
		value	 >>= 1;
		n_bits	  -= 1;
	}
	/*
	** Return TRUE if the low order bit of the count is zero
	** (which means that there were an even number of 1's).
	*/
	printf("the number of bit 1 is : %d \n", parity);
	return ( parity % 2 ) == 0;
}

/***********************************************************
* Function:       // main
* Description:    // 主函数
* Others:         // 
***********************************************************/
int main( void )
{
	int tmp = 3;
	if( even_parity( tmp, sizeof( int ) * 8 ) )
	{
		printf( "0x%x parity success.\n", tmp );
	}else
	{
		printf("0x%x parity failed.\n",tmp);
	}

	return 0;
}

/************************************** The End Of File **************************************/
