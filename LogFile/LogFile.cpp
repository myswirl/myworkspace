/************************************************************
 * Copyright (C), 2009-2011, ZhongMing Tech. Co., Ltd.
 * FileName:		// LogFile.cpp
 * Author:			// Lmy
 * Date:			// 2011年11月5日
 * Description:		// 写日志函数
 * Version:			// V1.0
 * History:			// 历史修改记录
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
//#include "stdafx.h"
#include "LogFile.h"

int				cfg_LogOn = 1;				//是否开启日志,1:开,0:关；客户根据需要进行更改
int				cfg_LogLevel = 0;			//输出日志级别,0,1,2,3,4,值为0时输出日志最多，客户根据需要进行更改
char			g_LogFileName[MAX_LOGFILEPATH_LEN]="LogFileName";	//追加日志文件名，用户在这里定义自己的日志文件名
PLOG_FILE_CFG	g_pLogCfg;					//日志配置变量

/***********************************************************
* Function:       // InitLogCfg
* Description:    // 日志初始化函数
* Return:         // 是否初始化成功，-1:fail; 1:success;
***********************************************************/
int InitLogCfg( void )
{
	char pBuf[MAX_LOGFILEPATH_LEN]	={0};				//存放路径的变量      
	struct tm *nowtime;
	time_t long_time;

	if( ( g_pLogCfg = (PLOG_FILE_CFG)malloc( sizeof( LOG_FILE_CFG ) ) ) != NULL )
	{
		memset( g_pLogCfg, 0, sizeof( LOG_FILE_CFG ) );	//空间分配成功
	}else
	{
		return -1;
	}

#ifdef WIN32
	GetCurrentDirectory(MAX_LOGFILEPATH_LEN,pBuf);		//Windows获取程序的当前目录
	strcat(pBuf,"\\");
#else
	memcpy( pBuf, "./", sizeof( "./" ) );               //Linux获取程序的当前目录
#endif
	memcpy( g_pLogCfg->s8LogPath, pBuf, sizeof( pBuf ) );	//日志文件路径
	g_pLogCfg->s32LogSize = MAX_LOGFILE_SIZE;				//日志最大大小，10M
	g_pLogCfg->s32LogLevel = cfg_LogLevel;					//日志级别，输出所有日志
	g_pLogCfg->s32LogOn = cfg_LogOn;						//是否开启日志

#ifdef WIN32
	InitializeCriticalSection( &g_pLogCfg->m_cs );
#else
	pthread_mutex_init( &g_pLogCfg->m_cs, NULL );
#endif

	time( &long_time );
	nowtime = localtime( &long_time );						//获取当前时间

#ifdef WIN32
	sprintf( g_pLogCfg->s8LogFileName, "%s%s-%04d%02d%02d%02d%02d%02d.txt",
	         g_pLogCfg->s8LogPath,g_LogFileName, nowtime->tm_year + 1900, nowtime->tm_mon + 1, nowtime->tm_mday, nowtime->tm_hour, nowtime->tm_min, nowtime->tm_sec );
#else
	sprintf( g_pLogCfg->s8LogFileName, "%s/%s-%04d%02d%02d%02d%02d%02d.txt",
	         g_pLogCfg->s8LogPath,g_LogFileName, nowtime->tm_year + 1900, nowtime->tm_mon + 1, nowtime->tm_mday, nowtime->tm_hour, nowtime->tm_min, nowtime->tm_sec );
#endif

	if( g_pLogCfg->s32LogOn == 1 && g_pLogCfg->s8LogFileName != NULL ) //如果开启了日志，创建日志文件
	{
		g_pLogCfg->pLogFileHandle = fopen( g_pLogCfg->s8LogFileName, "a+" );
		if( g_pLogCfg->pLogFileHandle == NULL )
		{
			return -1;
		}
	}
	return 1;
}

/***********************************************************
* Function:       // WriteLog
* Description:    // 写日志函数
* Input:          // 1.logLevel:日志输出级别；2.pMsgStr:日志消息
* Return:         // 写日志是否成功, -1:fail; 1:success;
***********************************************************/
int  WriteLog( LEVEL_TYPE logLevel, char *pMsgStr )
{
	long		logMsgLen  = 0;                                                     //每条日志消息长度
	long		s32Pos	   = 0;                                                     //日志位置
	struct tm	*nowtime;
	time_t		long_time;

	
	if( g_pLogCfg == NULL )                                                         //日志变量未初始化
	{
		return -1;
	}

	if(access(g_pLogCfg->s8LogFileName, F_OK) == -1)
	{
		printf("%s not exist!\n",g_pLogCfg->s8LogFileName);
	}
	// 等级控制，例如选择日志级别LEVEL_ERROR，那么WARN信息不会输出日志
	if( logLevel < g_pLogCfg->s32LogLevel || g_pLogCfg->pLogFileHandle == NULL )    //是否记日志
	{
		return -1;
	}
#ifdef WIN32
	SYSTEMTIME sysTime;
	GetLocalTime( &sysTime );                                                       //获得系统当前时间
	sprintf( g_pLogCfg->s8LogBuf, "%04d%02d%02d-%02d:%02d:%02d:%03d [%5s] ",
	         sysTime.wYear, sysTime.wMonth, sysTime.wDay,
	         sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds, LOG_ERROR[logLevel] );
#else
	struct timeval t;
	gettimeofday( &t, NULL );
	time( &long_time );
	nowtime = localtime( &long_time ); //获得系统当前时间
	sprintf( g_pLogCfg->s8LogBuf, "%04d%02d%02d-%02d:%02d:%02d:%03ld [%5s] ",
	         nowtime->tm_year + 1900, nowtime->tm_mon + 1, nowtime->tm_mday,
	         nowtime->tm_hour, nowtime->tm_min, nowtime->tm_sec, ( t.tv_usec / 1000 ), LOG_ERROR[logLevel] );
#endif

	if( pMsgStr != NULL )
	{	
		logMsgLen = strlen( pMsgStr ) + 32;		//32为时间等信息的长度
		if (logMsgLen > MAX_LOGBUF_LEN)			//日志消息太长了
		{
			return -1;
		}
		strcat( g_pLogCfg->s8LogBuf, pMsgStr );
	}
	strcat( g_pLogCfg->s8LogBuf, "\n" );

#ifdef WIN32
	EnterCriticalSection( &g_pLogCfg->m_cs );
#else
	pthread_mutex_lock( &g_pLogCfg->m_cs );
#endif

	int ret = fseek( g_pLogCfg->pLogFileHandle, 0, SEEK_END );
	s32Pos = ftell( g_pLogCfg->pLogFileHandle );

	printf("g_pLogCfg->pLogFileHandle:%p, s32Pos:%d ,ret:%d\n", g_pLogCfg->pLogFileHandle,s32Pos, ret);

	if( s32Pos >= g_pLogCfg->s32LogSize )       //超过日志文件大小
	{
		fclose( g_pLogCfg->pLogFileHandle );    //关闭文件句柄
		memset( g_pLogCfg->s8LogFileName, 0, sizeof( g_pLogCfg->s8LogFileName ) );
		time( &long_time );
		nowtime = localtime( &long_time );      //获取当前时间

#ifdef WIN32
		sprintf( g_pLogCfg->s8LogFileName, "%s%s-%04d%02d%02d%02d%02d%02d.txt",
		         g_pLogCfg->s8LogPath,  g_LogFileName,nowtime->tm_year + 1900, nowtime->tm_mon + 1, nowtime->tm_mday, nowtime->tm_hour, nowtime->tm_min, nowtime->tm_sec );
#else
		sprintf( g_pLogCfg->s8LogFileName, "%s/%s-%04d%02d%02d%02d%02d%02d.txt",
		         g_pLogCfg->s8LogPath,  g_LogFileName, nowtime->tm_year + 1900, nowtime->tm_mon + 1, nowtime->tm_mday, nowtime->tm_hour, nowtime->tm_min, nowtime->tm_sec );
#endif

		if( g_pLogCfg->s32LogOn == 1 && g_pLogCfg->s8LogFileName != NULL ) //如果开启了日志，创建日志文件
		{
			g_pLogCfg->pLogFileHandle = fopen( g_pLogCfg->s8LogFileName, "a+" );
			if( g_pLogCfg->pLogFileHandle == NULL )
			{
				return -1;
			}
		}
	}

	fprintf( g_pLogCfg->pLogFileHandle, "%s", g_pLogCfg->s8LogBuf );
	fflush( g_pLogCfg->pLogFileHandle );
#ifdef WIN32
	LeaveCriticalSection( &g_pLogCfg->m_cs );
#else
	pthread_mutex_unlock( &g_pLogCfg->m_cs );
#endif
	return 0;
}

void CloseLogFile(void)
{
	if ( g_pLogCfg != NULL && g_pLogCfg->pLogFileHandle != NULL)
	{
		fclose( g_pLogCfg->pLogFileHandle );//关闭文件句柄
	}
}
