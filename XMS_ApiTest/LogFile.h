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

#define	MAX_LOGBUF_LEN			1024				//ÿ����־��Ϣ����󳤶�
#define MAX_LOGFILEPATH_LEN		256					//��־�ļ�·������󳤶�
#define MAX_LOGFILE_SIZE		(10*1024*1024)		//��־�ļ���С�����10M
// ��־�����ȼ�
typedef enum
{
	LEVEL_DEBUG = 0,	//����,�������,�����־��Ϣ,��ϵͳʵ�����й�����һ�㶼�ǲ������
	LEVEL_INFO  = 1,	//��Ϣ,����ϵͳ�ĵ�ǰ״̬�������û���
	LEVEL_WARN  = 2,	//����,��ʱ����һЩ�޸��ԵĹ���,ϵͳ�ܹ��ָ�,���Լ���������ȥ
	LEVEL_ERROR = 3,    //����,����һ���޷��޸��Ĵ���
	LEVEL_NONE  = 4		//��,  ����ӡ��־��Ϣ. 
		
} LEVEL_TYPE;
// ��ͬ��־�ȼ���Ӧ���ַ���
static const char LOG_ERROR[][8] = {
    "DEBUG",
	"INFO ",
	"WARN ",
	"ERROR",
	"NONE "
};
// ��־������
typedef struct LOG_FILE_CFG
{
	FILE	*pLogFileHandle;						//��־���
	char	s8LogBuf[MAX_LOGBUF_LEN];				//��־������
	char	s8LogFileName[MAX_LOGFILEPATH_LEN];		//��־�ļ�ȥ·������
	char	s8LogPath[MAX_LOGFILEPATH_LEN];			//��־�ļ�·��
	int		s32LogSize;								//��־��С
	int		s32LogOn;								//�Ƿ�����־��0:��������1:����;
	int		s32LogLevel;							//��־����

#ifdef WIN32
	CRITICAL_SECTION m_cs;							//�������������
#else
	pthread_mutex_t	m_cs;
#endif

}LOG_FILE_CFG, *PLOG_FILE_CFG;

// ���⺯������
int InitLogCfg( void );
int WriteLog( LEVEL_TYPE logLevel, char *pMsgStr );
void CloseLogFile(void);

// �����������
extern int cfg_LogOn;//�Ƿ�����־,1:��,0:��
extern int cfg_LogLevel;//�����־����,0,1,2,3,4,ֵΪ0ʱ�����־���

#endif

