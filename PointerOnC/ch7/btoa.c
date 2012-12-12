/*
** Take an integer value (unsigned), convert it to characters, and
** print it.  Leading zeros are suppressed.
*/
#include <stdio.h>


/***********************************************************
* Function:       // binary_to_ascii
* Description:    // ��value�Զ�����Ascii����ʽ��ӡ
* Others:         // �ݹ麯��
***********************************************************/
void binary_to_ascii( unsigned int value )
{
	unsigned int quotient;

	quotient = value / 10;
	if( quotient != 0 )
	{
		binary_to_ascii( quotient );
	}
	putchar( value % 10 + '0' );
}

/***********************************************************
* Function:       // main
* Description:    // ������
***********************************************************/
int main( void )
{
	unsigned int val = 1234;
	binary_to_ascii( val);
	printf("\n");
	return 0;
}
/************************************** The End Of File **************************************/
