/*���λ������������λ������<<(��>>)���������� unsigned char ���� unsigned short ���͵Ĳ�����ʱ��
 *�м����㲽��Ľ������������ʽǿ��ת��ΪԤ�ڵĶ������������͡�*/ 
#include <stdio.h>
 
int main(void)
{
	unsigned char port = 0x5a;
    unsigned char result_8;
    result_8  = (~port)>>4;
    printf("Error, result=0x%02x \n",result_8);
    
	result_8  = ((unsigned char)(~port))>>4;
    printf("OK,    result=0x%02x \n",result_8);
    
	return 0;
}

