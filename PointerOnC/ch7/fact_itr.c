/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// �ļ���
 * Author:			// ����
 * Date:			// ����
 * Description:		// ģ������
 * Version:			// �汾��Ϣ
 * History:			// ��ʷ�޸ļ�¼
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>


/***********************************************************
* Function:       // factorial
* Description:    // ��������׳�
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
* Description:    // ������
***********************************************************/
int main( void )
{
	int n=4;
	printf("%d! = %ld\n", n, factorial(n) );
	return 0;
}
/************************************** The End Of File **************************************/
