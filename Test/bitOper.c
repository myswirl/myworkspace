/*如果位操作符～和移位操作符<<(或>>)联合作用于 unsigned char 或者 unsigned short 类型的操作数时，
 *中间运算步骤的结果必须立刻显式强制转换为预期的短整型数据类型。*/ 
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

