/************************************************************
 * Copyright (C), 2009-2011, ZhongMing Tech. Co., Ltd.
 * FileName:		// LogFile.cpp
 * Author:			// Lmy
 * Date:			// 2011��11��5��
 * Description:		// д��־����
 * Version:			// V1.0
 * History:			// ��ʷ�޸ļ�¼
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
//#include "stdafx.h"
#include "LogFile.h"

int				cfg_LogOn = 1;				//�Ƿ�����־,1:��,0:�أ��ͻ�������Ҫ���и���
int				cfg_LogLevel = 0;			//�����־����,0,1,2,3,4,ֵΪ0ʱ�����־��࣬�ͻ�������Ҫ���и���
char			g_LogFileName[MAX_LOGFILEPATH_LEN]="LogFileName";	//׷����־�ļ������û������ﶨ���Լ�����־�ļ���
PLOG_FILE_CFG	g_pLogCfg;					//��־���ñ���

/***********************************************************
* Function:       // InitLogCfg
* Description:    // ��־��ʼ������
* Return:         // �Ƿ��ʼ���ɹ���-1:fail; 1:success;
***********************************************************/
int InitLogCfg( void )
{
	char pBuf[MAX_LOGFILEPATH_LEN]	={0};				//���·���ı���      
	struct tm *nowtime;
	time_t long_time;

	if( ( g_pLogCfg = (PLOG_FILE_CFG)malloc( sizeof( LOG_FILE_CFG ) ) ) != NULL )
	{
		memset( g_pLogCfg, 0, sizeof( LOG_FILE_CFG ) );	//�ռ����ɹ�
	}else
	{
		return -1;
	}

#ifdef WIN32
	GetCurrentDirectory(MAX_LOGFILEPATH_LEN,pBuf);		//Windows��ȡ����ĵ�ǰĿ¼
	strcat(pBuf,"\\");
#else
	memcpy( pBuf, "./", sizeof( "./" ) );               //Linux��ȡ����ĵ�ǰĿ¼
#endif
	memcpy( g_pLogCfg->s8LogPath, pBuf, sizeof( pBuf ) );	//��־�ļ�·��
	g_pLogCfg->s32LogSize = MAX_LOGFILE_SIZE;				//��־����С��10M
	g_pLogCfg->s32LogLevel = cfg_LogLevel;					//��־�������������־
	g_pLogCfg->s32LogOn = cfg_LogOn;						//�Ƿ�����־

#ifdef WIN32
	InitializeCriticalSection( &g_pLogCfg->m_cs );
#else
	pthread_mutex_init( &g_pLogCfg->m_cs, NULL );
#endif

	time( &long_time );
	nowtime = localtime( &long_time );						//��ȡ��ǰʱ��

#ifdef WIN32
	sprintf( g_pLogCfg->s8LogFileName, "%s%s-%04d%02d%02d%02d%02d%02d.txt",
	         g_pLogCfg->s8LogPath,g_LogFileName, nowtime->tm_year + 1900, nowtime->tm_mon + 1, nowtime->tm_mday, nowtime->tm_hour, nowtime->tm_min, nowtime->tm_sec );
#else
	sprintf( g_pLogCfg->s8LogFileName, "%s/%s-%04d%02d%02d%02d%02d%02d.txt",
	         g_pLogCfg->s8LogPath,g_LogFileName, nowtime->tm_year + 1900, nowtime->tm_mon + 1, nowtime->tm_mday, nowtime->tm_hour, nowtime->tm_min, nowtime->tm_sec );
#endif

	if( g_pLogCfg->s32LogOn == 1 && g_pLogCfg->s8LogFileName != NULL ) //�����������־��������־�ļ�
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
* Description:    // д��־����
* Input:          // 1.logLevel:��־�������2.pMsgStr:��־��Ϣ
* Return:         // д��־�Ƿ�ɹ�, -1:fail; 1:success;
***********************************************************/
int  WriteLog( LEVEL_TYPE logLevel, char *pMsgStr )
{
	long		logMsgLen  = 0;                                                     //ÿ����־��Ϣ����
	long		s32Pos	   = 0;                                                     //��־λ��
	struct tm	*nowtime;
	time_t		long_time;

	
	if( g_pLogCfg == NULL )                                                         //��־����δ��ʼ��
	{
		return -1;
	}

	if(access(g_pLogCfg->s8LogFileName, F_OK) == -1)
	{
		printf("%s not exist!\n",g_pLogCfg->s8LogFileName);
	}
	// �ȼ����ƣ�����ѡ����־����LEVEL_ERROR����ôWARN��Ϣ���������־
	if( logLevel < g_pLogCfg->s32LogLevel || g_pLogCfg->pLogFileHandle == NULL )    //�Ƿ����־
	{
		return -1;
	}
#ifdef WIN32
	SYSTEMTIME sysTime;
	GetLocalTime( &sysTime );                                                       //���ϵͳ��ǰʱ��
	sprintf( g_pLogCfg->s8LogBuf, "%04d%02d%02d-%02d:%02d:%02d:%03d [%5s] ",
	         sysTime.wYear, sysTime.wMonth, sysTime.wDay,
	         sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds, LOG_ERROR[logLevel] );
#else
	struct timeval t;
	gettimeofday( &t, NULL );
	time( &long_time );
	nowtime = localtime( &long_time ); //���ϵͳ��ǰʱ��
	sprintf( g_pLogCfg->s8LogBuf, "%04d%02d%02d-%02d:%02d:%02d:%03ld [%5s] ",
	         nowtime->tm_year + 1900, nowtime->tm_mon + 1, nowtime->tm_mday,
	         nowtime->tm_hour, nowtime->tm_min, nowtime->tm_sec, ( t.tv_usec / 1000 ), LOG_ERROR[logLevel] );
#endif

	if( pMsgStr != NULL )
	{	
		logMsgLen = strlen( pMsgStr ) + 32;		//32Ϊʱ�����Ϣ�ĳ���
		if (logMsgLen > MAX_LOGBUF_LEN)			//��־��Ϣ̫����
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

	if( s32Pos >= g_pLogCfg->s32LogSize )       //������־�ļ���С
	{
		fclose( g_pLogCfg->pLogFileHandle );    //�ر��ļ����
		memset( g_pLogCfg->s8LogFileName, 0, sizeof( g_pLogCfg->s8LogFileName ) );
		time( &long_time );
		nowtime = localtime( &long_time );      //��ȡ��ǰʱ��

#ifdef WIN32
		sprintf( g_pLogCfg->s8LogFileName, "%s%s-%04d%02d%02d%02d%02d%02d.txt",
		         g_pLogCfg->s8LogPath,  g_LogFileName,nowtime->tm_year + 1900, nowtime->tm_mon + 1, nowtime->tm_mday, nowtime->tm_hour, nowtime->tm_min, nowtime->tm_sec );
#else
		sprintf( g_pLogCfg->s8LogFileName, "%s/%s-%04d%02d%02d%02d%02d%02d.txt",
		         g_pLogCfg->s8LogPath,  g_LogFileName, nowtime->tm_year + 1900, nowtime->tm_mon + 1, nowtime->tm_mday, nowtime->tm_hour, nowtime->tm_min, nowtime->tm_sec );
#endif

		if( g_pLogCfg->s32LogOn == 1 && g_pLogCfg->s8LogFileName != NULL ) //�����������־��������־�ļ�
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
		fclose( g_pLogCfg->pLogFileHandle );//�ر��ļ����
	}
}
