/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// echo.c
 * Author:			// 
 * Date:			// 
 * Description:		// 打印命令行参数
 * Version:			// 
 * History:			// 
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>
#include <stdlib.h>

/***********************************************************
* Function:       // main
* Description:    // 主函数
* Input:          // 1.argc:命令行传入参数个数,包括程序名
* Input:          // 2.argv:指向字符串数组的第一个元素
***********************************************************/
int main( int argc, char **argv )
{
	int i=0;
	for( i=0; i<argc; i++ )
	{
		printf("argv[%d]=%s \n",i, argv[i]);
	}
	return EXIT_SUCCESS;
}
/************************************** The End Of File **************************************/
