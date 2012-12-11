#include <stdio.h>
#include <time.h> 
#include "MyLog.h"

CMyLog cml;
CMyLog cml2;
int main()
{
	char logPath1[256];
	char logPath2[256];
	int i;
//	GetCurrentDirectory(LOG_PATH_LEN_MAX,logPath1);	//Windows获取程序的当前目录
	strcat(logPath1,"./A1");	

	//初始化日志配置,日志级别:WARN,日志最大:20M
	if (cml.InitLogCfg(LEVEL_WARN, logPath1, 20*1024*1024) < 0)	
	{
		printf("InitLogCfg() Fail!\n");
	}

	//时间 [等级] 信息内容,例如:20101220-16:32:18:453 [Error] d:\mingyangworkspace\mylog\main.cpp 27
	cml.WriteData("测试时间,定电流值,电压上限,电压下限,产品型号,定电流值,电压上限,电压下限,产品型号,定电流值");
	for(i=0;i<400;i++)
	{		
		cml.WriteData_Ttime("%d,1,2,3,4,5,6",i);
	}

	//GetCurrentDirectory(LOG_PATH_LEN_MAX,logPath2);	//Windows获取程序的当前目录
	strcat(logPath2,"./A2");
	//初始化日志配置,日志级别:WARN,日志最大:20M
	if (cml2.InitLogCfg(LEVEL_WARN, logPath2, 20*1024*1024) < 0)	
	{
		printf("InitLogCfg() Fail!\n");
	}
	//时间 [等级] 信息内容,例如:20101220-16:32:18:453 [Error] d:\mingyangworkspace\mylog\main.cpp 27
	cml2.WriteData("测试时间,定电流值,电压上限,电压下限,产品型号,定电流值,电压上限,电压下限,产品型号,定电流值");
	for(i=0;i<400;i++)
	{		
		cml2.WriteData_Ttime("%d,1,2,3,4,5,6",i);
	}

	return 0;
}
