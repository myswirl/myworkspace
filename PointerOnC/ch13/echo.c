/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// echo.c
 * Author:			// 
 * Date:			// 
 * Description:		// ��ӡ�����в���
 * Version:			// 
 * History:			// 
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>
#include <stdlib.h>

/***********************************************************
* Function:       // main
* Description:    // ������
* Input:          // 1.argc:�����д����������,����������
* Input:          // 2.argv:ָ���ַ�������ĵ�һ��Ԫ��
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
