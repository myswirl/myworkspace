/*
fork �� vfork
1.fork:�ӽ��̿��������̵����ݶ�
  vfork:�ӽ����븸���̹������ݶ�
2.fork:�����ӽ��̵�ִ�д���ȷ��
  vfork:�ӽ��������С������̺�����
*/

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
	pid = vfork( );

	count++;
	
	//��ʱ�Ѿ�������������ͬʱ����
	printf( "count = %d \n", count );
	
	return 0;
}
