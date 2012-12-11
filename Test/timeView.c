#include <stdio.h>
#include <time.h>
#include <string.h>

int main( void )
{
	struct tm *localTime;		//本地时间
	struct tm *gmTime;			//格林威治时间
	time_t calTime;
	
	calTime = time(NULL);					//日历时间，从一个标准时间点1970年1月1日0点到此时经过的秒数
	
	localTime = localtime(&calTime);	//将日历时间转化为本地时间
	printf("Local hour is:%d \n", localTime->tm_hour );
	
	gmTime = gmtime(&calTime);
	printf("GMT hour is:%d \n", gmTime->tm_hour );
	
	//将tm格式的结构转化为字符串
	printf(asctime(gmTime));
	
	//把日历时间转化为本地时间的字符串形式
	printf(ctime(&calTime));
	
	return 0;	
}
