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
//	GetCurrentDirectory(LOG_PATH_LEN_MAX,logPath1);	//Windows��ȡ����ĵ�ǰĿ¼
	strcat(logPath1,"./A1");	

	//��ʼ����־����,��־����:WARN,��־���:20M
	if (cml.InitLogCfg(LEVEL_WARN, logPath1, 20*1024*1024) < 0)	
	{
		printf("InitLogCfg() Fail!\n");
	}

	//ʱ�� [�ȼ�] ��Ϣ����,����:20101220-16:32:18:453 [Error] d:\mingyangworkspace\mylog\main.cpp 27
	cml.WriteData("����ʱ��,������ֵ,��ѹ����,��ѹ����,��Ʒ�ͺ�,������ֵ,��ѹ����,��ѹ����,��Ʒ�ͺ�,������ֵ");
	for(i=0;i<400;i++)
	{		
		cml.WriteData_Ttime("%d,1,2,3,4,5,6",i);
	}

	//GetCurrentDirectory(LOG_PATH_LEN_MAX,logPath2);	//Windows��ȡ����ĵ�ǰĿ¼
	strcat(logPath2,"./A2");
	//��ʼ����־����,��־����:WARN,��־���:20M
	if (cml2.InitLogCfg(LEVEL_WARN, logPath2, 20*1024*1024) < 0)	
	{
		printf("InitLogCfg() Fail!\n");
	}
	//ʱ�� [�ȼ�] ��Ϣ����,����:20101220-16:32:18:453 [Error] d:\mingyangworkspace\mylog\main.cpp 27
	cml2.WriteData("����ʱ��,������ֵ,��ѹ����,��ѹ����,��Ʒ�ͺ�,������ֵ,��ѹ����,��ѹ����,��Ʒ�ͺ�,������ֵ");
	for(i=0;i<400;i++)
	{		
		cml2.WriteData_Ttime("%d,1,2,3,4,5,6",i);
	}

	return 0;
}
