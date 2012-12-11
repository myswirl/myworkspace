#include <stdio.h>
#include <time.h>
#include <string.h>

int main( void )
{
	struct tm *localTime;		//����ʱ��
	struct tm *gmTime;			//��������ʱ��
	time_t calTime;
	
	calTime = time(NULL);					//����ʱ�䣬��һ����׼ʱ���1970��1��1��0�㵽��ʱ����������
	
	localTime = localtime(&calTime);	//������ʱ��ת��Ϊ����ʱ��
	printf("Local hour is:%d \n", localTime->tm_hour );
	
	gmTime = gmtime(&calTime);
	printf("GMT hour is:%d \n", gmTime->tm_hour );
	
	//��tm��ʽ�Ľṹת��Ϊ�ַ���
	printf(asctime(gmTime));
	
	//������ʱ��ת��Ϊ����ʱ����ַ�����ʽ
	printf(ctime(&calTime));
	
	return 0;	
}
