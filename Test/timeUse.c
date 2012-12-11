/*计算 函数耗时*/
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void function(void)
{
	unsigned int i,j;
	double y;
	for(i=0;i<1000;i++)
	{
		for(j=0;j<1000;j++)
		{
			y++;
		}
	}
}
int main(void)
{
	struct timeval tpstart, tpend;
	float timeuse;
	gettimeofday(&tpstart,NULL);
	function();
	gettimeofday(&tpend,NULL);

	timeuse = 1000000*(tpend.tv_sec - tpstart.tv_sec) + (tpend.tv_usec - tpstart.tv_usec);//单位us
	timeuse/=1000;//单位毫秒

	printf("Used time is:%f ms\n",timeuse);
	return 0;
	
}

