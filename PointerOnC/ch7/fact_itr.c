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
* Function:       // factorial
* Description:    // 迭代计算阶乘
***********************************************************/
long factorial( int n )
{
	int result = 1;

	while( n > 1 )
	{
		result	  *= n;
		n		  -= 1;
	}

	return result;
}

/***********************************************************
* Function:       // main
* Description:    // 主函数
***********************************************************/
int main( void )
{
	int n=4;
	printf("%d! = %ld\n", n, factorial(n) );
	return 0;
}
/************************************** The End Of File **************************************/
