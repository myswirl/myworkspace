/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// fork1.c
 * Author:			// 
 * Date:			// 
 * Description:		// fork�����ӽ��̣�������룬��������
 					�ӽ��̵����ݿռ䡢��ջ�ռ䶼��Ӹ����̵õ�һ�������������ǹ���
 * Version:			// 
 * History:			// 
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

/***********************************************************
* Function:       // main
***********************************************************/
int main( void )
{
	pid_t pid;
	int count = 0;
	//��ʱ����һ������
	pid = fork( );

	count++;
	
	//��ʱ�Ѿ�������������ͬʱ����
	printf( "count = %d \n", count );
	
	return 0;
}
/************************************** The End Of File **************************************/
