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
* Description:    // żУ�麯����Ҳ�����ж�value�Ķ�����Ϊģʽ��1�ĸ����Ƿ�Ϊż����
* Return:         // true: ��ż����;false:����ż����;
* Others:         // 
***********************************************************/
int even_parity( int value, int n_bits )
{
	int parity = 0;
	//ͳ��1�ĸ���
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
* Description:    // ������
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
