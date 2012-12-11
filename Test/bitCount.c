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
* Function:       // count_one_bits
* Description:    // ͳ��value��bit 1�ĸ���
* Return:         // ����
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
* Description:    // �ڶ��ַ���
* Return:         // ����
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
* Description:    // ������
***********************************************************/
int main( void )
{
	unsigned int v = 0xAA;

	printf( "0x%X one bits count is:%d. \n", v, count_one_bits( v ) );
	return 0;
}

/************************************** The End Of File **************************************/

