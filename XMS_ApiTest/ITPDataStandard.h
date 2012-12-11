/*****************************************************************************
//Copyright(c)  :  DONJIN CORPORTAION  All Rights Reserved                       
//FileName      :  ITPDataStandard.h                                                              
//Version       :  1.0                                                              
//Author        :  兰骏                                                             
//DateTime      :  2005-03-22 10:00                                           
//Description   :  实现ITP系统跨平台数据结构和函数定义                                                                 
*****************************************************************************
*****************************************************************************
//Version       :  1.0.1
//mender        :  兰骏
//DateTime      :  2005-11-04 11:00  
//Description  :   在djthread_Exit函数中加入了关闭句柄操作                                                          
*****************************************************************************
*****************************************************************************
//Author        :  黎荣满
//DateTime      :  2005-11-29 15:16                                           
//Description   :  宏定义PTHREAD_MUTEX_INITIALIZER与MTP3协议栈有冲突。因目前
//                 ITPDataStandard.h里没用到该宏定义，暂时先注释
*****************************************************************************
*****************************************************************************
//Version       :  1.0.2
//mender        :  兰骏
//DateTime      :  2006-02-07 15:00  
//Description  :   修改数据结构定义，使其符合定义标准                                                          
******************************************************************************/
#ifndef _ITPDATASTANDARD_H
#define _ITPDATASTANDARD_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <sys/types.h>
#include <sys/stat.h>

#ifdef WIN32
#include "windows.h"
#include "io.h"
#else
#include <unistd.h>
#include <dirent.h>
#include <dlfcn.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/param.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <pthread.h>
#include <errno.h>
#include <syslog.h>

#include<fcntl.h>

#endif

#include "ITPDataDefine.h"

#define MAX_MUTEX_NUM    0x40
#define MAX_THREAD_NUM   0x100
#define MAX_SHAREMEM_NUM 0x100
#define MAX_SEMAPHORES_NUM 0x40

#define MAX_LPNAME_SIZE  0x20

#ifdef WIN32 
#define __LT_SPINLOCK_INIT 0
#define __LOCK_INITIALIZER { 0, __LT_SPINLOCK_INIT }
/*#define PTHREAD_MUTEX_INITIALIZER {0, 0, 0, 0, __LOCK_INITIALIZER}*/
#define DLLEXPORT __declspec (dllexport)

typedef struct
{
    DJ_S8 mutex_t[24];
}pthread_mutex_t;

#else
#ifndef MAX_PATH
#define MAX_PATH         256
#endif
#define MAX_FILE         256
typedef DJ_Void *        HMODULE;
#define WINAPI
#define __cdecl
#define DLLEXPORT

#endif

/*end */


/*/////////////////////////////////////修改后的数据结构和原型定义////////////////////////////////////////////////////*/
typedef struct
{
    DJ_S32 m_s32WinFile;      /* hFile, For Windows*/
    DJ_S32 m_s32LinuxFile;    /* key, For Linux */           
    DJ_U32 m_u32WinflProtect; /* windows only*/

    DJ_U32 m_u32DesiredAccess;/* access mode,  For Windows OpenFileMapping*/
    DJ_S32 m_s32InheritHandle;/* inherit flag, For windows OpenFileMapping*/

    DJ_U32 m_u32LinuxFlag;    /* flag , For Linux*/
    DJ_U32 m_u32Size;         /* dwMaximumSizeLow,size (win+linux)*/

    DJ_S8  m_s8pWinlpName[MAX_LPNAME_SIZE];   /* windows only*/

}ITP_SHAREMEMORY_STRUCT,*PITP_SHAREMEMORY_STRUCT;


typedef struct
{
#ifdef WIN32 
    DJ_U32 m_u32WinThreadId;
#else       
    DJ_U32 m_u32LinuxThreadId; 
#endif
    DJ_U32 m_u32WinPriority;  /*windows only*/
    DJ_U32 m_u32VxPriority;   /*vxWorks only*/
    
    DJ_U32 (* m_u32ThreadProc)(DJ_Void *); 
    
    DJ_U32 m_u32StackSize;    /*vxWorks+win+linux*/
    DJ_S32 m_s32WinFlag;    /*windows*/
    DJ_S8 *m_s8arg;         
   
    DJ_U32 m_u32VxOptions;/*vxWorks*/
    DJ_U8 *m_u8pVxTaskName;/*vxWorks*/

}ITP_THREAD_STRUCT,*PITP_THREAD_STRUCT;

typedef struct
{

#ifdef WIN32
    STARTUPINFO            winsi;
    PROCESS_INFORMATION  winpi;
#else
#endif
    DJ_S8 m_s8Cmd[100];
    DJ_S8 m_s8argv1[100];
    DJ_S8 m_s8argv2[100];
    DJ_S8 m_s8Path[128];

}ITP_PROCESS_STRUCT,*PITP_PROCESS_STRUCT;


typedef struct
{
         
#ifdef WIN32 
    DJ_S32 m_s32WinFile;
#else       
    DJ_S32 m_s32LinuxFile;  
#endif

    DJ_S8  m_s8WinlpName[20];         /*windows only */
    DJ_S32 m_s32WinlInitialCount;  /*windows only */
    DJ_S32 m_s32WinlMaximumCount;  /*windows only*/
    DJ_U32 m_u32DesiredAccess;     /*windows only*/
    DJ_S32 m_s32InheritHandle;     /*windows only*/
	
    DJ_S32 m_s32VxSems;	    /*for Linux,信号灯的个数*/
    DJ_S32 m_s32VxFlag;	    /*for Linux*/
	
}ITP_SEMAPHORES_STRUCT,*PITP_SEMAPHORES_STRUCT;


typedef struct _tagITP_CRITICAL_STRUCT
{
#ifdef WIN32         
	CRITICAL_SECTION WinlCriticalSection; /*windows only*/
#else
    pthread_mutex_t mutex;/*for Linux*/
#endif
}ITP_CRITICAL_STRUCT,*PITP_CRITICAL_STRUCT;

#ifdef __cplusplus
extern "C" {
#endif
/********************************/

/*Linux兼容windows函数定义*/
#ifndef WIN32
    #define LINELEN 1024

    #define MAX_LINELEN 32768

	typedef struct
	{ 
		DJ_U16 wYear; 
		DJ_U16 wMonth; 
		DJ_U16 wDayOfWeek; 
		DJ_U16 wDay; 
		DJ_U16 wHour; 
		DJ_U16 wMinute; 
		DJ_U16 wSecond; 
		DJ_U16 wMilliseconds; 
	}SYSTEMTIME, *PSYSTEMTIME,*LPSYSTEMTIME; 

	DJ_U32 * LoadLibrary(DJ_S8 * pDllName);
	DJ_U32 * GetProcAddress(DJ_Void *hHandle,DJ_S8 *pFuncName);
	DJ_S32 FreeLibrary(DJ_Void *hHandle);
	DJ_S32 GetPrivateProfileString(DJ_S8 *appname, DJ_S8 *keyname,DJ_S8 *defval, DJ_S8 *buf, DJ_U32 len,DJ_S8 *filename);
	DJ_S32 GetPrivateProfileInt(DJ_S8 *appname,DJ_S8 *keyname,DJ_S32 defval,DJ_S8 *filename);
	DJ_S32 WritePrivateProfileString(DJ_S8 *filename,DJ_S8 *appname,DJ_S8 *keyname,DJ_S8 *val);
	DJ_S32 GetPrivateProfileStruct(DJ_S8* appname,DJ_S8* keyname,DJ_Void * pstruct,DJ_U32 u32size,DJ_S8* filename);
	DJ_S32 WritePrivateProfileStruct(DJ_S8* appname,DJ_S8* keyname,DJ_Void* pstruct,DJ_U32 u32size,DJ_S8* filename);
	DJ_Void GetLocalTime(LPSYSTEMTIME lpSystemTime);
	DJ_U32 GetTickCount();
        /*DJ_Void _strtime(DJ_S8 *strtime);*/
    /*added by fuyunshi on 070405*/
  /*  DJ_S32 DeletePrivateProfileString( const DJ_S8 * appname,
                                const DJ_S8 * keyname,
                                const DJ_S8 * val,
                                const DJ_S8 * filename);*/
#endif

/*共享内存函数定义*/
DJ_S32   djmemory_create(PITP_SHAREMEMORY_STRUCT pShareMemory);
DJ_Void* djmemory_maping(DJ_S32 s32MemoryID, DJ_U32 u32OffSet, DJ_U32 u32Flag);
DJ_S32   djmemory_unmaping(DJ_Void *pBaseAddress);
DJ_S32   djmemory_open(PITP_SHAREMEMORY_STRUCT pShareMemory);
DJ_S32   djmemory_close(DJ_S32 s32MemoryID);

/********************************/

/*同步机制函数定义*/

DJ_S32 djcritical_Initialize(PITP_CRITICAL_STRUCT pCritical);
DJ_S32 djcritical_Delete(PITP_CRITICAL_STRUCT pCritical);
DJ_S32 djcritical_Enter(PITP_CRITICAL_STRUCT pCritical); 
DJ_S32 djcritical_Leave(PITP_CRITICAL_STRUCT pCritical);          


/*信号量函数*/
DJ_S32 djsemaphores_Create(PITP_SEMAPHORES_STRUCT pSemaphoress);
DJ_S32 djsemaphores_Free(PITP_SEMAPHORES_STRUCT pSemaphoress);
DJ_S32 djsemaphores_Set(PITP_SEMAPHORES_STRUCT pSemaphoress);    
DJ_S32 djsemaphores_Wait(PITP_SEMAPHORES_STRUCT pSemaphoress,DJ_S32 s32TimeOut);
DJ_S32 djsemaphores_Open(PITP_SEMAPHORES_STRUCT pSemaphoress);

/********************************/

/*线程操作函数定义*/

DJ_U32 djthread_Create(PITP_THREAD_STRUCT pThread);
DJ_S32 djthread_Terminate(PITP_THREAD_STRUCT pThread,DJ_S32 s32ExitCode);
DJ_S32 djthread_Exit(PITP_THREAD_STRUCT pThread,DJ_S32 s32ExitCode); 
DJ_S32 djthread_Resume(PITP_THREAD_STRUCT pThread);
DJ_S32 djthread_Suspend(PITP_THREAD_STRUCT pThread);   
/*(linux)*/
DJ_S32 djthread_detach(PITP_THREAD_STRUCT pThread); 


/*进程操作函数定义*/
DJ_S32 djprocess_Create(PITP_PROCESS_STRUCT pProcess);

/*其他函数*/

DJ_Void DJ_Sleep(DJ_S32 s32TimeOut);
DJ_S32 DJ_DeleteFile(DJ_S8 *s32FilePath);
DJ_S32 DJ_GetLastError();
DJ_S32 DJ_GetCurrentDirectory(DJ_S8 *s8Path,DJ_S32 s32PathSize);
DJ_S32 DJ_CmdExecute(DJ_S8 *s8pCommand,DJ_S32 s32param);
DJ_U32 ITP_IP_String_to_Int(DJ_S8* string);
DJ_S8* ITP_IP_Int_to_String(DJ_S32 IP, DJ_S8* string);

DJ_Void DJ_GetLocalIPAddress(DJ_S8* s8IPAddress);
DJ_S32 DJ_CheckProcess(DJ_S8 *s8KeyFileName);
DJ_S32 DJ_GetRegFilePath(DJ_S8 *s8FilePath);
DJ_S32 DJ_GetProcessID(DJ_S8 *s8KeyFileName);

DJ_S32  DJNetEntry_SetIP(DJ_U32 u32IP,DJ_S8 *MACAddr,DJ_U32 u32Index);

DJ_S32 Closesocket(DJ_SOCKET s);

/*一组读写文件的函数(Added by ZJM)*/
enum ITP_OpenFlags
{
	mode_Read =          0x0000,
	mode_Write =         0x0001,
	mode_ReadWrite =     0x0002,
	share_Compat =       0x0000,
	share_Exclusive =    0x0010,
	share_DenyWrite =    0x0020,
	share_DenyRead =     0x0030,
	share_DenyNone =     0x0040,
	mode_NoInherit =     0x0080,
	mode_Normal     =    0x0000,    /*add by fuys*/
	mode_Sync		=	 0x0100,	/*add by fuys*/
	mode_Async		=	 0x0180,	/*add by fuys*/
	mode_Create =        0x1000,
	mode_NoTruncate =    0x2000,
	type_Text =          0x4000, /*typeText and typeBinary are used in*/
	type_Binary =   (int)0x8000  /*derived classes only*/
};

/*enum ITP_FileAttrType
{
	attr_normal =    0x00,
	attr_readOnly =  0x01,
	attr_hidden =    0x02,
	attr_system =    0x04,
	attr_volume =    0x08,
	attr_directory = 0x10,
	attr_archive =   0x20
};*/

typedef enum ITP_SeekPosition { sp_begin = 0x0, sp_current = 0x1, sp_end = 0x2 } ITP_SeekPosition;

#if 0
DJ_HANDLE  DJ_OpenFile( DJ_S8* szFileName, DJ_S32 nOpenFlags );
DJ_Void    DJ_CloseFile( DJ_HANDLE hFile );
DJ_Void    DJ_FlushFile( DJ_HANDLE hFile );
DJ_U32     DJ_SeekFile( DJ_HANDLE hFile, DJ_S32 lOff, ITP_SeekPosition nFrom );
DJ_U8      DJ_SetFileLength( DJ_HANDLE hFile, DJ_U32 u32NewLen );
DJ_U32     DJ_GetFileLength( DJ_HANDLE hFile );
DJ_U32     DJ_SeekFileToEnd( DJ_HANDLE hFile );
DJ_U32     DJ_SeekFileToBegin( DJ_HANDLE hFile );
DJ_U32     DJ_GetFilePosition( DJ_HANDLE hFile );
DJ_U32     DJ_ReadFile( DJ_HANDLE hFile, DJ_Void* lpBuf, DJ_U32 nCount );
DJ_U32     DJ_WriteFile( DJ_HANDLE hFile, const DJ_Void* lpBuf, DJ_U32 nCount );

DJ_S8      DJ_CreateDirectory( DJ_S8* szPath );    /*参数szPath是一个全路径名，本函数创建一个多层目录*/
DJ_S8      DJ_CreateFullFilePath( DJ_S8* szFile ); /*创建一个全路径文件名中的路径*/
#endif

#ifdef __cplusplus
}
#endif

#endif


