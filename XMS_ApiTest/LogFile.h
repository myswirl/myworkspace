#ifndef __LOG_FILE_H__
#define __LOG_FILE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include  <unistd.h>

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#include <pthread.h>
#endif

#define	MAX_LOGBUF_LEN			1024				//每条日志消息的最大长度
#define MAX_LOGFILEPATH_LEN		256					//日志文件路径的最大长度
#define MAX_LOGFILE_SIZE		(10*1024*1024)		//日志文件大小，最大10M
// 日志报警等级
typedef enum
{
	LEVEL_DEBUG = 0,	//调试,级别最低,最多日志消息,在系统实际运行过程中一般都是不输出的
	LEVEL_INFO  = 1,	//信息,反馈系统的当前状态给最终用户的
	LEVEL_WARN  = 2,	//警告,这时进行一些修复性的工作,系统能够恢复,可以继续运行下去
	LEVEL_ERROR = 3,    //错误,导致一个无法修复的错误
	LEVEL_NONE  = 4		//无,  不打印日志消息. 
		
} LEVEL_TYPE;
// 不同日志等级对应的字符串
static const char LOG_ERROR[][8] = {
    "DEBUG",
	"INFO ",
	"WARN ",
	"ERROR",
	"NONE "
};
// 日志配置项
typedef struct LOG_FILE_CFG
{
	FILE	*pLogFileHandle;						//日志句柄
	char	s8LogBuf[MAX_LOGBUF_LEN];				//日志缓存区
	char	s8LogFileName[MAX_LOGFILEPATH_LEN];		//日志文件去路径名称
	char	s8LogPath[MAX_LOGFILEPATH_LEN];			//日志文件路经
	int		s32LogSize;								//日志大小
	int		s32LogOn;								//是否开启日志，0:不开启；1:开启;
	int		s32LogLevel;							//日志级别

#ifdef WIN32
	CRITICAL_SECTION m_cs;							//函数的重入控制
#else
	pthread_mutex_t	m_cs;
#endif

}LOG_FILE_CFG, *PLOG_FILE_CFG;

// 对外函数声明
int InitLogCfg( void );
int WriteLog( LEVEL_TYPE logLevel, char *pMsgStr );
void CloseLogFile(void);

// 对外变量声明
extern int cfg_LogOn;//是否开启日志,1:开,0:关
extern int cfg_LogLevel;//输出日志级别,0,1,2,3,4,值为0时输出日志最多

#endif

