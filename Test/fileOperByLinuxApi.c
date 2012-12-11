/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// fileOperByLinuxApi.c
 * Author:			// lmy
 * Date:			// 2011年5月27日
 * Description:		// Linux下用Api操作文件例子
 * Version:			// 
 * History:			// 
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define MAX_BUF_LEN 100


/***********************************************************
* Function:       // main
* Description:    // 主函数
***********************************************************/
int main( )
{
	int		fd, len;
	char	str[MAX_BUF_LEN];
	fd = open( "hello.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR );
	if( fd )
	{
		write( fd, "Hello World", strlen( "Hello World" ) );
		close( fd );
	}
	fd		   = open( "hello.txt", O_RDWR );
	len		   = read( fd, str, MAX_BUF_LEN );
	str[len]   = '\0';
	printf( "%s\n", str );
	close( fd );
	return 0;
}
/************************************** The End Of File **************************************/
