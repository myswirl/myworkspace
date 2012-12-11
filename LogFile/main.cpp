#include <stdio.h>
#include "LogFile.h"

int main()
{
	int i;
	char logMsg[MAX_LOGBUF_LEN]={0};
	InitLogCfg();

	sprintf(logMsg, "Debug, Test Log Message Message the word is word very long long time age ageo:%d.", i );
	WriteLog(LEVEL_DEBUG, logMsg );

	printf("please input 11111111\n");
	scanf("%d",&i);

	sprintf(logMsg, "i:%d ",i);
	WriteLog(LEVEL_DEBUG, logMsg );
	
	printf("please input \n");
	scanf("%d",&i);

	sprintf(logMsg, "i:%d ",i);
	WriteLog(LEVEL_DEBUG, logMsg );
	
/*
	for ( i=0; i<10000; i++)
	{
		sprintf(logMsg, "Debug, Test Log Message Message the word is word very long long time age ageo:%d.", i );
		WriteLog(LEVEL_DEBUG, logMsg );
		printf("%s\n", logMsg);
	}

	for ( i=0; i<60000; i++)
	{
		sprintf(logMsg, "Error, Test Log Message Message the word is word very long long time age ageo:%d.", i );
		WriteLog(LEVEL_ERROR, logMsg );
		printf("%s\n", logMsg);
	}

	for ( i=0; i<80000; i++)
	{
		sprintf(logMsg, "INFO, Test Log Message Message the word is word very long long time age ageo:%d.", i );
		WriteLog(LEVEL_INFO, logMsg );
		printf("%s\n", logMsg);
	}
*/
	CloseLogFile();
	
	return 0;
}

