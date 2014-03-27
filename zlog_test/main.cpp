#include <stdio.h>
#include <unistd.h>
#include "zlog.h"

zlog_category_t *g_pMyCategory;
int main()
{
	int rc=zlog_init("zlog.conf");
	if(rc)
	{
		printf("init failed!\n");
		return -1;
	}
	g_pMyCategory = zlog_get_category("my_cat");
	if(!g_pMyCategory)
	{
		printf("get cat fail\n");
		zlog_fini();
		return -2;
	}
	for (int  i=0; i<10000; i++)
	{
		zlog_fatal(g_pMyCategory, "fatal, zlog, %d", i);
		zlog_error(g_pMyCategory, "error, zlog");
		zlog_warn(g_pMyCategory, "warn, zlog");
		zlog_notice(g_pMyCategory, "notice,zlog");
		zlog_info(g_pMyCategory, "info,zlog");
		zlog_debug(g_pMyCategory, "debug,zlog");
		sleep(1);
	}
	
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
	zlog_fini();
	return 0;
}

