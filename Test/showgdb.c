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
* Function:       // 函数名称
* Description:    // 函数功能、性能等的描述
* Input:          // 1.输入参数1，说明，包括每个参数的作用、取值说明及参数间关系
* Input:          // 2.输入参数2，说明，包括每个参数的作用、取值说明及参数间关系
* Output:         // 1.输出参数1，说明
* Return:         // 函数返回值的说明
* Others:         // 其它说明
***********************************************************/
void doloop( int n )
{
	int i = 0;
	printf( "loop is going start>>>\n" );
	for( i = 0; i < n; i++ )
	{
		printf( "i=%d\n", i );
	}
	printf( "loop is donw <<<\n" );
}

/***********************************************************
* Function:       // 函数名称
* Description:    // 函数功能、性能等的描述
* Input:          // 1.输入参数1，说明，包括每个参数的作用、取值说明及参数间关系
* Input:          // 2.输入参数2，说明，包括每个参数的作用、取值说明及参数间关系
* Output:         // 1.输出参数1，说明
* Return:         // 函数返回值的说明
* Others:         // 其它说明
***********************************************************/
int main( int argc, char *argv[] )
{
	int		n	   = 10;
	char	*str   = "hello,world";
	if( argc > 1 )
	{
		printf( "args:%s\n", argv[1] );
	}
	printf( "say hello:%s\n", str );
	doloop( n );
	printf( "exit now!\n" );
	return 0;
}
/************************************** The End Of File **************************************/
