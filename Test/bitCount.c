/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// 文件名
 * Author:			// 作者
 * Date:			// 日期
 * Description:		// 模块描述
 * Version:			// 版本信息
 * History:			// 历史修改记录
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>


/***********************************************************
* Function:       // count_one_bits
* Description:    // 统计value中bit 1的个数
* Return:         // 个数
***********************************************************/
int count_one_bits( unsigned value )
{
	int ones;
	for( ones = 0; value != 0;  value = value >> 1 )
	{
		if( value % 2 != 0 )
		{
			ones = ones + 1;
		}
	}
	return ones;
}

/***********************************************************
* Function:       // count_one_bits_V2
* Description:    // 第二种方法
* Return:         // 个数
***********************************************************/
int count_one_bits_V2( unsigned value )
{
	int ones;
	for( ones = 0; value != 0; value = value >> 1 )
	{
		if( ( value & 1 ) != 0 )
		{
			ones += 1;
		}
	}
	return ones;
}

/***********************************************************
* Function:       // main
* Description:    // 主函数
***********************************************************/
int main( void )
{
	unsigned int v = 0xAA;

	printf( "0x%X one bits count is:%d. \n", v, count_one_bits( v ) );
	return 0;
}

/************************************** The End Of File **************************************/

