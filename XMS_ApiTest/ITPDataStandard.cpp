  //*****************************************************************************
//Copyright(c)  :  DONJIN CORPORTAION  All Rights Reserved                       
//FileName      :  ITPDataStandard.cpp                                                              
//Version       :  1.0                                                              
//Author        :  兰骏                                                             
//DateTime      :  2005-03-22 10:00                                           
//Description   :  实现ITP系统跨平台的函数实现                                                                 
//*****************************************************************************
#include "ITPDataDefine.h"
#include "ITPDataStandard.h"

#ifdef WIN32
#pragma  comment(lib, "Ws2_32.lib")

//#include "Iphlpapi.h"
#else
extern DJ_Void MD_Skip_Left_Space(DJ_S8 **str);
extern DJ_Void MD_Skip_Right_Space(DJ_S8 **str);
extern DJ_Void MD_Skip_lr_Space(DJ_S8 **str);
extern DJ_S32  MD_isapp(const DJ_S8 *linedata, const DJ_S8 *appname);
extern DJ_S32  MD_NotKeyLine(DJ_S8 *line, const DJ_S8 *keyname, DJ_S8 **val);
#endif

//共享内存函数定义
//*****************************************************************************
//Name        : djmemory_create
//Author      : Luo
//DateTime    : 2005-04-04 10:00
//Description : Create share memory
//Input       : pShareMemory:
//              Pointer for share memory data struct
//Output      : None
//Return      : 0 - Create share memory Failure(Windows and Linux)
//             >0 - Share memory Handle(Windows) or ID(Linux)
//*****************************************************************************
DJ_S32 djmemory_create(PITP_SHAREMEMORY_STRUCT pShareMemory)
{
	if (pShareMemory == NULL) ////////// Parameter verify//////////////////////
	{
		return 0;
	}

	/////////////////////////// WINDOWS PART //////////////////////////////////
#ifdef WIN32
	HANDLE hSM = NULL;
	
	hSM = CreateFileMapping( (DJ_Void*)pShareMemory->m_s32WinFile, // handle to file
		                     NULL,                               // security
							 pShareMemory->m_u32WinflProtect,      // protection
							 0,                                  // high-order size
							 pShareMemory->m_u32Size,              // low-order size
							 pShareMemory->m_s8pWinlpName          // object name
                           );

	return (DJ_S32)hSM;

	/////////////////////////// LINUX PART ////////////////////////////////////
#else
	DJ_S32 shm_id = 0;

	shm_id = shmget( pShareMemory->m_s32LinuxFile,  // Key
		             pShareMemory->m_u32Size,       // Size
					 pShareMemory->m_u32LinuxFlag   // Flag
		           );	
	
	return (DJ_S32)((shm_id==-1) ? 0 : shm_id);
#endif

}


//*****************************************************************************
//Name        : djmemory_maping
//Author      : Luo
//DateTime    : 2005-04-04 10:30
//Description : Map share memory to user space
//Input       : s32MemoryID:
//              Share memory Handle(Windows) Or ID(Linux) returned when create.
//              u32Offset:
//              Map start offset(Windows), It must be setted to 0.
//              u32Flag:
//              Access mode(Windows)
//Output      : None
//Return      : 0 - Map share memory Failure(Windows and Linux)
//             >0 - Share memory address(Windows and Linux)
//*****************************************************************************
DJ_Void* djmemory_maping(DJ_S32 s32MemoryID, DJ_U32 u32OffSet, DJ_U32 u32Flag)
{
	if (s32MemoryID == 0) ////////// Parameter verify//////////////////////////
	{
		return NULL;
	}

	/////////////////////////// WINDOWS PART //////////////////////////////////
#ifdef WIN32
	LPVOID pSM = NULL;

	pSM = MapViewOfFile( (DJ_Void*)s32MemoryID, // handle to file-mapping object
		                 u32Flag,               // access mode
						 0,                     // high-order offset
						 u32OffSet,             // low-order offset
						 0                      // number of bytes to map
                       );

	return (DJ_Void*)pSM;

	/////////////////////////// LINUX PART ////////////////////////////////////
#else
	DJ_Void *pSM = NULL;

	pSM = (DJ_Void*)shmat( s32MemoryID,         // share memory id
		                   0,                   // which address
						   0                    // flag
                         );

	return (DJ_Void*)((pSM==(DJ_Void*)-1) ? 0 : pSM);
#endif
}


//*****************************************************************************
//Name        : djmemory_unmaping
//Author      : Luo
//DateTime    : 2005-04-04 11:00
//Description : Unmap a mapped view of a file from the process's address space
//Input       : pBaseAddress:
//              Mapped share memory address.
//Output      : None
//Return      : 0 - Unmap Failure(Windows and Linux)
//              1 - unmap Success(Windows and Linux)
//*****************************************************************************
DJ_S32 djmemory_unmaping(DJ_Void *pBaseAddress)
{
	if (pBaseAddress == NULL) ////////// Parameter verify//////////////////////
	{
		return 0;
	}

	/////////////////////////// WINDOWS PART //////////////////////////////////
#ifdef WIN32
	DJ_S32 bRet = UnmapViewOfFile((DJ_Void *)pBaseAddress);

	return bRet;

	/////////////////////////// LINUX PART ////////////////////////////////////
#else
	DJ_S32 bRet = (DJ_S32)shmdt(pBaseAddress);
	
	return (DJ_S32)((bRet==(DJ_S32)-1) ? 0 : 1);
#endif

}


//*****************************************************************************
//Name        : djmemory_open
//Author      : Luo
//DateTime    : 2005-04-04 11:20
//Description : Open a exist share memory
//Input       : pShareMemory:
//              Pointer for share memory data struct
//Output      : None
//Return      : 0 - Open share memory Failure(Windows and Linux)
//             >0 - Share memory Handle(Windows) or ID(Linux)
//*****************************************************************************
DJ_S32 djmemory_open(PITP_SHAREMEMORY_STRUCT pShareMemory)
{
	if (pShareMemory == NULL) ////////// Parameter verify//////////////////////
	{
		return 0;
	}

	/////////////////////////// WINDOWS PART //////////////////////////////////
#ifdef WIN32
	HANDLE hSM = NULL;

	hSM = OpenFileMapping( pShareMemory->m_u32DesiredAccess, // access mode
		                   pShareMemory->m_s32InheritHandle, // inherit flag
						   pShareMemory->m_s8pWinlpName      // object name
		                 );

	return (DJ_S32)hSM;

	/////////////////////////// LINUX PART ////////////////////////////////////
#else
	DJ_S32 shm_id = 0;

	shm_id = shmget( pShareMemory->m_s32LinuxFile,  // Key
		             pShareMemory->m_u32Size,       // Size
					 pShareMemory->m_u32LinuxFlag   // Flag
		           );
	
	return (DJ_S32)((shm_id==-1) ? 0 : shm_id);
#endif
}


//*****************************************************************************
//Name        : djmemory_close
//Author      : Luo
//DateTime    : 2005-04-04 11:40
//Description : Close a exist share memory
//Input       : s32MemoryID:
//              Share memory Handle(Windows) or ID(Linux)
//Output      : None
//Return      : 0 - Close share memory Failure(Windows and Linux)
//              1 - Close share memory Success(Windows and Linux)
//*****************************************************************************
DJ_S32 djmemory_close(DJ_S32 s32MemoryID)
{
	if (s32MemoryID == 0) ////////// Parameter verify//////////////////////////
	{
		return 0;
	}

	/////////////////////////// WINDOWS PART //////////////////////////////////
#ifdef WIN32
	return CloseHandle((HANDLE)s32MemoryID);

	/////////////////////////// LINUX PART ////////////////////////////////////
#else
	DJ_S32 bRet = shmctl(s32MemoryID,IPC_RMID,0);

	return (DJ_S32)((bRet==-1) ? 0: 1);
#endif
}

//******************************

//同步机制函数定义
//*****************************************************************************
//Name        :  djcritical_Initialize                                                               
//Author      :  兰骏                                                               
//DateTime    :  2005-03-25 10:00                                        
//Description :  初始化关键段                                                               
//Input       :  PITP_CRITICAL_STRUCT pCritical                     
//Output      :  NONE                        
//Return      :  1 - Success
//               0 - Fail                              
//*****************************************************************************

DJ_S32 djcritical_Initialize(PITP_CRITICAL_STRUCT pCritical)
{
    if (pCritical!=NULL)
	{
#ifdef WIN32
        InitializeCriticalSection(&pCritical->WinlCriticalSection);

#else
        pthread_mutex_init(&pCritical->mutex,NULL);
#endif
		return 1;
	}
	return -1;
}

//*****************************************************************************
//Name        :  djcritical_Delete                                                               
//Author      :  兰骏                                                               
//DateTime    :  2005-03-25 10:00                                        
//Description :  删除关键段                                                                
//Input       :  PITP_CRITICAL_STRUCT pCritical                     
//Output      :  NONE                        
//Return      :  1 - Success
//               0 - Fail                             
//*****************************************************************************  
DJ_S32 djcritical_Delete(PITP_CRITICAL_STRUCT pCritical)
{
    if (pCritical!=NULL)
	{ 
#ifdef WIN32
        DeleteCriticalSection(&pCritical->WinlCriticalSection);

#else
		pthread_mutex_destroy(&pCritical->mutex);
#endif
		return 0;
	}
	return -1;
}

//*****************************************************************************
//Name        :  djcritical_Enter                                                               
//Author      :  兰骏                                                               
//DateTime    :  2005-03-25 10:00                                        
//Description :  进入关键段                                                                
//Input       :  PITP_CRITICAL_STRUCT pCritical                     
//Output      :  NONE                        
//Return      :  1 - Success
//               0 - Fail                             
//*****************************************************************************
DJ_S32 djcritical_Enter(PITP_CRITICAL_STRUCT pCritical)
{
	if (pCritical!=NULL)
	{ 
#ifdef WIN32
	    EnterCriticalSection(&pCritical->WinlCriticalSection);
#else
        pthread_mutex_lock(&pCritical->mutex);
#endif
		return 1;
	}
	return -1;
}

//*****************************************************************************
//Name        :  djcritical_Leave                                                               
//Author      :  兰骏                                                               
//DateTime    :  2005-03-25 10:00                                        
//Description :  离开关键段                                                                
//Input       :  PITP_CRITICAL_STRUCT pCritical                     
//Output      :  NONE                        
//Return      :  1 - Success
//               0 - Fail                          
//*****************************************************************************
        
DJ_S32 djcritical_Leave(PITP_CRITICAL_STRUCT pCritical)
{
	if (pCritical!=NULL)
	{
#ifdef WIN32
        LeaveCriticalSection(&pCritical->WinlCriticalSection);
#else        
		pthread_mutex_unlock(&pCritical->mutex);
#endif
		return 1;
	}
	return -1;
}

//*****************************************************************************
//Name        :  djsemaphores_Create                                                               
//Author      :  兰骏                                                               
//DateTime    :  2005-03-25 10:00                                        
//Description :  创建一个信号量                                                                
//Input       :  ITP_SEMAPHORES_STRUCT pSemaphoress                     
//Output      :  NONE                        
//Return      :  1 - Success
//               0 - Fail                          
//*****************************************************************************
DJ_S32 djsemaphores_Create(PITP_SEMAPHORES_STRUCT pSemaphoress)
{
	if (pSemaphoress!=NULL)
	{
#ifdef WIN32
	    pSemaphoress->m_s32WinFile = (DJ_S32) CreateSemaphore(NULL,
			                                 pSemaphoress->m_s32WinlInitialCount,
						                     pSemaphoress->m_s32WinlMaximumCount,
						                     pSemaphoress->m_s8WinlpName);
		if (pSemaphoress->m_s32WinFile>0)
			return 1;
		else
			return 0;
       
#else
    return (DJ_S32) semget(pSemaphoress->m_s32LinuxFile,pSemaphoress->m_s32VxSems,pSemaphoress->m_s32VxFlag);
#endif
	}
	return 0;
}

//*****************************************************************************
//Name        :  djsemaphores_Free
//Author      :  Luo
//DateTime    :  2005-05-11 10:00
//Description :  Release a semaphores
//Input       :  ITP_SEMAPHORES_STRUCT pSemaphoress
//Output      :  NONE                        
//Return      :  1 - Success
//               0 - Fail                          
//*****************************************************************************
DJ_S32 djsemaphores_Free(PITP_SEMAPHORES_STRUCT pSemaphoress)
{
#ifdef WIN32
	DJ_S32 Ret = FALSE;

	if (pSemaphoress != NULL)
	{
		Ret = CloseHandle((DJ_Void*)pSemaphoress->m_s32WinFile);
	}

	return Ret;

#else
	return 0;
#endif
}


//*****************************************************************************
//Name        :  djsemaphores_Set
//Author      :  Luo
//DateTime    :  2005-05-11 10:20
//Description :  Increases the count of the specified semaphore object 
//Input       :  ITP_SEMAPHORES_STRUCT pSemaphoress
//Output      :  NONE
//Return      :  1 - Success
//               0 - Fail
//*****************************************************************************   
DJ_S32 djsemaphores_Set(PITP_SEMAPHORES_STRUCT pSemaphoress)
{
#ifdef WIN32
	DJ_S32 Ret = FALSE;

	if (pSemaphoress != NULL)
	{
		Ret = ReleaseSemaphore((DJ_Void*)pSemaphoress->m_s32WinFile, 1, NULL);
	}

	return Ret;
#else
	return 0;
#endif
}


//*****************************************************************************
//Name        :  djsemaphores_Wait
//Author      :  Luo
//DateTime    :  2005-05-11 10:30
//Description :  Wait a semaphoress
//Input       :  ITP_SEMAPHORES_STRUCT pSemaphoress
//               s32TimeOut: time-out interval
//Output      :  NONE
//Return      :  1 - Success
//               0 - Fail
//*****************************************************************************
DJ_S32 djsemaphores_Wait(PITP_SEMAPHORES_STRUCT pSemaphoress,DJ_S32 s32TimeOut)
{
#ifdef WIN32
	DJ_S32 Ret = FALSE;
	
	if (pSemaphoress != NULL)
	{
		Ret = WaitForSingleObject((DJ_Void*)pSemaphoress->m_s32WinFile, s32TimeOut);

		if (Ret == WAIT_OBJECT_0)
		{
			Ret = TRUE;
		}
		else
		{
			Ret = FALSE;
		}
	}

	return Ret;
#else
	return 0;
#endif
}

//*****************************************************************************
//Name        :  djsemaphores_Open                                                             
//Author      :  Luo
//DateTime    :  2005-05-17 16:00                                        
//Description :  Return a handle to an existing named semaphore object
//Input       :  ITP_SEMAPHORES_STRUCT pSemaphoress                     
//Output      :  NONE                        
//Return      :  1 - Success
//               0 - Fail                          
//*****************************************************************************
DJ_S32 djsemaphores_Open(PITP_SEMAPHORES_STRUCT pSemaphoress)
{
#ifdef WIN32
	DJ_S32 Ret = FALSE;

	if (pSemaphoress != NULL)
	{
		Ret = (DJ_S32) OpenSemaphore(pSemaphoress->m_u32DesiredAccess, 
			                         pSemaphoress->m_s32InheritHandle,
							         pSemaphoress->m_s8WinlpName
						            );
		
		pSemaphoress->m_s32WinFile = Ret;

		if (Ret != 0)
		{
			Ret = 1;
		}
	}

	return Ret;
#else
	return 0;
#endif
}

//******************************

//线程操作函数定义
//*****************************************************************************
//Name        :  djthread_Create                                                               
//Author      :  兰骏                                                               
//DateTime    :  2005-03-25 10:00                                        
//Description :  创建一个线程                                                                
//Input       :  PITP_THREAD_STRUCT pThread                  
//Output      :  NONE							
//Return      :  >0 ThreadID
//               0 Fail                          
//*****************************************************************************
DJ_U32 djthread_Create(PITP_THREAD_STRUCT pThread)
{
	if (pThread!=NULL)
	{
#ifdef WIN32
		DJ_U32 u32ThreadID;
	
		pThread->m_u32WinThreadId = (long)CreateThread(NULL,pThread->m_u32StackSize,
			                        (LPTHREAD_START_ROUTINE)pThread->m_u32ThreadProc,
									pThread->m_s8arg,
									pThread->m_s32WinFlag, 
									&u32ThreadID); 
         
		return pThread->m_u32WinThreadId;
#else
	pthread_attr_t thread_attr;
	pthread_attr_init(&thread_attr);
	DJ_S32 detachstate = PTHREAD_CREATE_DETACHED;	
	pthread_attr_setdetachstate(&thread_attr, detachstate);

	DJ_S32 Ret = pthread_create((pthread_t*)&pThread->m_u32LinuxThreadId, 
		                         &thread_attr,
							     (void *(*)(void *))pThread->m_u32ThreadProc,
							     (void *)pThread->m_s8arg);
	
	if (Ret == 0)
	{
		return pThread->m_u32LinuxThreadId;
	}
#endif
	}	
	return 0;
}


//结束一个线程
//*****************************************************************************
//Name        :  djthread_Terminate                                                               
//Author      :  兰骏                                                               
//DateTime    :  2005-03-25 10:00                                        
//Description :  创建一个线程                                                                
//Input       :  PITP_THREAD_STRUCT pThread                  
//Output      :  NONE                        
//Return      :  >0 success
//               <=0 Fail                          
//*****************************************************************************
DJ_S32 djthread_Terminate(PITP_THREAD_STRUCT pThread,DJ_S32 s32ExitCode)
{
	if (pThread!=NULL)
	{
#ifdef WIN32
		DJ_S32 Ret = TerminateThread((DJ_Void *)pThread->m_u32WinThreadId,s32ExitCode);
		return Ret;
#else
		if (pThread->m_u32LinuxThreadId)
		{
			DJ_S32 Ret = pthread_cancel(pThread->m_u32LinuxThreadId);

			if (Ret == 0)
			{
				return pThread->m_u32LinuxThreadId;
			}
		}
#endif
	}
	return -1;
}


//退出一个线程
//*****************************************************************************
//Name        :  djthread_Exit                                                               
//Author      :  兰骏                                                               
//DateTime    :  2005-03-25 10:00                                        
//Description :  创建一个线程                                                                
//Input       :  PITP_THREAD_STRUCT pThread                  
//Output      :  NONE                        
//Return      :  1 success
//               0 Fail                          
//*****************************************************************************
DJ_S32 djthread_Exit(PITP_THREAD_STRUCT pThread,DJ_S32 s32ExitCode)
{
#ifdef WIN32
	if(pThread!=NULL)
	{
	    CloseHandle((DJ_Void*)pThread->m_u32WinThreadId);
	}
	ExitThread(s32ExitCode);
	return 1;
#else
	pthread_exit((void*)&s32ExitCode);
	return 1;
#endif	
}
//*****************************************************************************
//Name        :  djthread_Resume                                                               
//Author      :  兰骏                                                               
//DateTime    :  2005-03-25 10:00                                        
//Description :  恢复一个线程                                                                
//Input       :  PITP_THREAD_STRUCT pThread                  
//Output      :  NONE                        
//Return      :  >0 Success
//               <=0 Fail                          
//*****************************************************************************   
DJ_S32 djthread_Resume(PITP_THREAD_STRUCT pThread)
{
	if (pThread!=NULL)
	{
#ifdef WIN32
	    DJ_S32 Ret = ResumeThread((DJ_Void *)pThread->m_u32WinThreadId);
	    return Ret;
#else
	return 1;
#endif
	}
	return -1;
}

//*****************************************************************************
//Name        :  djthread_Suspend                                                               
//Author      :  兰骏                                                               
//DateTime    :  2005-03-25 10:00                                        
//Description :  挂起一个线程                                                                
//Input       :  PITP_THREAD_STRUCT pThread                  
//Output      :  NONE                        
//Return      :  >0 Success
//               <=0 Fail                          
//*****************************************************************************
DJ_S32 djthread_Suspend(PITP_THREAD_STRUCT pThread)
{  
	if (pThread!=NULL)
	{
#ifdef WIN32
	    DJ_S32 Ret = SuspendThread((DJ_Void *)pThread->m_u32WinThreadId);
	    return Ret;
#else
		return 1;
#endif
	}
	return -1;
}


//*****************************************************************************
//Name        :  djthread_detach                                                               
//Author      :  兰骏                                                               
//DateTime    :  2005-03-25 10:00                                        
//Description :  剥离一个线程(linuxs用)                                                                
//Input       :  PITP_THREAD_STRUCT pThread                  
//Output      :  NONE                        
//Return      :  >0 Success
//               <=0 Fail                          
//*****************************************************************************
DJ_S32 djthread_detach(PITP_THREAD_STRUCT pThread)  
{  
	if (pThread!=NULL)
	{
#ifdef WIN32
	    return 0;
#else
       pthread_detach(pThread->m_u32LinuxThreadId);
#endif
	}	
	return -1;
}

//*****************************************************************************
//Name        : djprocess_Create                                                               
//Author      : 兰骏                                                               
//DateTime    : 2007-07-13 10:00                                        
//Description : 创建一个进程(linuxs用)                                                             
//Input       : PITP_PROCESS_STRUCT pProcess                  
//Output      : NONE                        
//Return      : >0 Success
//              <=0 Fail                          
//*****************************************************************************
DJ_S32 djprocess_Create(PITP_PROCESS_STRUCT pProcess)
{
    DJ_S32 s32Ret = -1;
    if(pProcess!=NULL)
    {
#ifdef WIN32
        memset(&pProcess->winsi,0,sizeof(STARTUPINFO));	
		pProcess->winsi.cb=sizeof(STARTUPINFO);
		pProcess->winsi.lpTitle = new DJ_S8[256];
		sprintf(pProcess->winsi.lpTitle,"%s",pProcess->m_s8Cmd);
  
		CreateProcess(NULL,(DJ_S8 *)pProcess->m_s8Cmd, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL,
		    NULL, &pProcess->winsi, &pProcess->winpi);
		if(pProcess->winsi.lpTitle != NULL)
		{
			delete []pProcess->winsi.lpTitle;
			pProcess->winsi.lpTitle = NULL;
		}
#else
       if(fork()==0)
       {
           for(DJ_S32 i=3;i<NOFILE;++i)
           {
              close(i);
           }
           printf("execl s8argv1 = %s,s8argv1=%s,s8Path =%s\n",pProcess->m_s8argv1,pProcess->m_s8argv2,pProcess->m_s8Path);
           if(strcmp(pProcess->m_s8argv1,""))
           {
               s32Ret = execl(pProcess->m_s8Path,pProcess->m_s8Path,pProcess->m_s8argv1,pProcess->m_s8argv2,(DJ_S8*)0);
           }
           else
           {
               s32Ret = execl(pProcess->m_s8Path,pProcess->m_s8Path,(DJ_S8*)0);
           }
           if(s32Ret ==-1) 
           {
              printf("err =%d\n" ,errno);
              exit(-1); 
           }
        }
    
#endif
  
    }
    return s32Ret;
}


//其他函数
//*****************************************************************************
//Name        :  DJ_Sleep                                                               
//Author      :  兰骏                                                               
//DateTime    :  2005-03-25 10:00                                        
//Description :  延时一段时间                                                                
//Input       :  DJ_S32 s32TimeOut   超时参数,单位:毫秒                  
//Output      :  NONE                        
//Return      :  NONE                          
//*****************************************************************************
DJ_Void DJ_Sleep(DJ_S32 s32TimeOut)
{
    if (s32TimeOut>=0)
	{
#ifdef WIN32
		Sleep(s32TimeOut);
#else
        usleep(s32TimeOut*1000); 
#endif
	}
}

//*****************************************************************************
//Name        :  DJ_DeleteFile                                                               
//Author      :  兰骏                                                               
//DateTime    :  2005-03-25 10:00                                        
//Description :  删除文件                                                                
//Input       :  DJ_S8 *s32FilePath   文件路径                  
//Output      :  NONE                        
//Return      :  0 - Success
//               1 - Fail                          
//*****************************************************************************
DJ_S32 DJ_DeleteFile(DJ_S8 *s32FilePath)
{
	if (s32FilePath!=NULL)
	{
#ifdef WIN32
		if(DeleteFile(s32FilePath) == TRUE)
		{
			return 0;
		}
		else
			return -1;
#else
		return remove(s32FilePath);
#endif
	}
	return -1;
}

//*****************************************************************************
//Name        :  DJ_GetLastError                                                               
//Author      :  兰骏                                                               
//DateTime    :  2005-04-25 10:00                                        
//Description :  获取最后的出错代码                                                                
//Input       :  NONE                  
//Output      :  NONE                        
//Return      :  error code                        
//*****************************************************************************
DJ_S32 DJ_GetLastError()
{
#ifdef WIN32
	return GetLastError();
#else
	return errno;
#endif     
}

//*****************************************************************************
//Name        :  DJ_GetCurrentDirectory                                                               
//Author      :  兰骏                                                               
//DateTime    :  2005-04-25 10:00                                        
//Description :  获取当前的目录路径                                                                
//Input       :  DJ_S8 *s8Path              //传入的字符指针
//               DJ_S32 s32PathSize         //字符串的空间大小         
//Output      :  DJ_S8 *s8Path              //当前目录路径                        
//Return      :  >0 成功
//            :  0  失败                        
//*****************************************************************************
DJ_S32 DJ_GetCurrentDirectory(DJ_S8 *s8Path,DJ_S32 s32PathSize)
{
#ifdef WIN32
	return GetCurrentDirectory(s32PathSize,s8Path);
#else
	s8Path = getcwd(s8Path,s32PathSize);
	return 1;
#endif     
}

//*****************************************************************************
//Name        :  DJ_CmdExecute                                                               
//Author      :  兰骏                                                               
//DateTime    :  2005-06-30 10:00                                        
//Description :  执行命令行语句                                                                
//Input       :  DJ_S8 *s8pCommand          //传入命令行
//               DJ_S32 s32param            //命令行参数         
//Output      :  无                        
//Return      :  >0 成功
//            :  0  失败                        
//*****************************************************************************
DJ_S32 DJ_CmdExecute(DJ_S8 *s8pCommand,DJ_S32 s32param)
{
#ifdef WIN32
	return WinExec(s8pCommand,0);
#else
	return system(s8pCommand);
#endif
}

//*****************************************************************************
//Name        :  DJ_CheckProcess                                                               
//Author      :  兰骏                                                               
//DateTime    :  2007-06-30 10:00                                        
//Description :  判断进程是否存在                                                               
//Input       :  DJ_S8 *s8KeyFileName           //进程名.
//Output      :  无                       
//Return      :   0 进程未存在.
//            :  -1 进程已经存在                       
//*****************************************************************************
DJ_S32 DJ_CheckProcess(DJ_S8 *s8KeyFileName)
{
#ifdef WIN32

     HANDLE hEvent = CreateEvent(NULL,FALSE,FALSE,s8KeyFileName);
     if(GetLastError() == ERROR_ALREADY_EXISTS)
     {
        CloseHandle(hEvent);
        return -1;
     }
	 return 0;
#else
   DJ_S8  s8Cmd[120];
   DJ_S8  s8bufcmd[120];
   DJ_S8  s8Buf[2]={'"',0};
   
   sprintf(s8Cmd,"ps aux|grep -v grep|grep %s%s%s |wc -l",s8Buf,s8KeyFileName,s8Buf);
   printf("DJ_CheckProcess s8Cmd=%s",s8Cmd);
   FILE *fread = popen(s8Cmd,"r");
   if(fread!=NULL)
   {
      while(fgets(s8bufcmd,100,fread)!=NULL) 
      {
         printf("%s\n",s8bufcmd);  
         if(atoi(s8bufcmd)>0)
         {
            return atoi(s8bufcmd);
         }
         return 0;
      } 
   }
   
/*    key_t unitkey;
    int id;
    unitkey = ftok(s8KeyFileName,1);
    if(unitkey!=-1)
    {
       id = semget(unitkey,1,IPC_CREAT | IPC_EXCL| 0666);
       if((id == -1) & (errno == EEXIST))
       {
          return -1;  
       }
   }*/
   return 0;
#endif
}
DJ_S32 DJ_GetProcessID(DJ_S8 *s8KeyFileName)
{
#ifdef WIN32

#else
   DJ_S8  s8Cmd[120];
   DJ_S8  s8bufcmd[120];
   DJ_S8  s8Buf[2]={'"',0};
   
   sprintf(s8Cmd,"ps aux|grep -v grep|grep %s%s%s |awk '{ print $2 }'",s8Buf,s8KeyFileName,s8Buf);
   printf("DJ_GetProcessID s8Cmd=%s\n",s8Cmd);
   FILE *fread = popen(s8Cmd,"r");
   if(fread!=NULL)
   {
      while(fgets(s8bufcmd,100,fread)!=NULL) 
      {
         printf("%s\n",s8bufcmd);  
         if(atoi(s8bufcmd)>0)
         {
            fclose(fread);
            return atoi(s8bufcmd);
         }
		 fclose(fread);
         return 0;
      } 
   }
   fclose(fread);
#endif
   return 0;
   
}
//*****************************************************************************
//Name         :  DJ_GetLocalIPAddress                                                               
//Author       :  兰骏                                                               
//DateTime     :  2007-07-30 10:00                                        
//Description  :  获取本地IP地址                                                              
//Input        :  无         
//Output       :  DJ_S8 *s8IPAddress      //本地IP地址                   
//Return       :  无
//*****************************************************************************

DJ_Void DJ_GetLocalIPAddress(DJ_S8* s8IPAddress)
{
#ifdef WIN32
/*	DJ_S8     szHostname[100]; 
	HOSTENT   *pHostEnt;   
    in_addr   inAddr;   
    gethostname(   szHostname,   sizeof(   szHostname   ));   
    pHostEnt   =   gethostbyname(   szHostname   );   
    memcpy(&inAddr.S_un,   pHostEnt->h_addr_list[0],   pHostEnt->h_length);  
    strcpy(s8IPAddress,inet_ntoa(inAddr));*/
#else
    struct ifconf conf;
    struct ifreq *ifr;
    char buff[BUFSIZ];
    int num;
    int i;

    int s = socket(PF_INET, SOCK_DGRAM, 0);
    conf.ifc_len = BUFSIZ;
    conf.ifc_buf = buff;

    ioctl(s, SIOCGIFCONF, &conf);
    num = conf.ifc_len / sizeof(struct ifreq);
    ifr = conf.ifc_req;

    printf("num=%d\n",num);

    for(i=0;i<num;i++)
    {
       struct sockaddr_in *sin = (struct sockaddr_in *)(&ifr->ifr_addr);

       ioctl(s, SIOCGIFFLAGS, ifr);
//       printf("sin->sin_addr =0x%x\n",sin->sin_addr);
       if(((ifr->ifr_flags & IFF_LOOPBACK) == 0) && (ifr->ifr_flags & IFF_UP))
       {
          strcpy(s8IPAddress,inet_ntoa(sin->sin_addr));
//          printf("s8IPAddress=%s,sin_addr =0x%x\n",s8IPAddress, sin->sin_addr);
          break;
       }
       ifr++;
    }
#endif 
  
}
//*****************************************************************************
//Name         :  DJ_GetRegFilePath                                                               
//Author       :  兰骏                                                               
//DateTime     :  2007-07-30 10:00                                        
//Description  :  获取XMS安装目录                                                                
//Input        :  无         
//Output       :  DJ_S8 *s8FilePath      //XMS安装目录                   
//Return       :  0  获取成功
//                -1  获取失败
//*****************************************************************************

DJ_S32 DJ_GetRegFilePath(DJ_S8 *s8FilePath)
{
#ifdef WIN32
  /*  HKEY key;
    REGSAM mask = KEY_READ;
    DJ_S8 s8SubKey[MAX_PATH];
    DJ_S8 s8MainModPath[MAX_PATH];

    strcpy(s8SubKey, "SOFTWARE\\DJXMS\\");

    if(::RegOpenKeyEx(HKEY_LOCAL_MACHINE, s8SubKey,NULL, mask, &key) == ERROR_SUCCESS)
    {
	    DJ_U32 dataType = REG_SZ;
	    DJ_U32 dataLen = MAX_PATH;
	    if(::RegQueryValueEx(key, "InstallDir", NULL, &dataType, (LPBYTE)s8MainModPath, &dataLen) != ERROR_SUCCESS)
		{
	        return -1;
		}
	}
    strcpy(s8FilePath,s8MainModPath);
    strcat(s8FilePath,"\\bin\\djitpmainmod.exe");*/
    return 0;

#else
    DJ_S8 *s8temp = getenv("XMSPATH");
    if(s8temp !=NULL)
    {
       strcpy(s8FilePath,strtok(s8temp,":"));
    }
    else
    {
       strcpy(s8FilePath,"/usr/keygoe");
    }
    strcat(s8FilePath,"/bin/DJXMSMainMod");
    return 0;

#endif
    
}
//*****************************************************************************
//Name        : SplitStr2Int
//Author      : hewei
//DateTime    : 2005-08-01 10:00
//Description : 拆分字符串
//Input       : str: 原始字符串
//              sep: 分隔符
//              buf: 拆分后的整形数组结果值
//Output      : None
//Return      : -1 - failed
//             >0 - success
//*****************************************************************************
DJ_S32 Split2Int(DJ_S8 * str,DJ_S8 * sep,DJ_U32 buf[])
{
	DJ_U32 i = 0;
	DJ_S8 * p = NULL;
	DJ_S8  s8Buf[2048];
	
	if(str == NULL || strlen(str) == 0 || sep == NULL || strlen(sep) == 0)
	{
		return -1;
	}
	strcpy(s8Buf,str);
	p=strtok(s8Buf,sep);
	
	while(p!=NULL)
	{
		buf[i++] = atoi(p);
		p=strtok(NULL,sep);
	}
	return i;
}

//*****************************************************************************
//Name        : ITP_IP_Int_to_String
//Author      : Luo
//DateTime    : 2005-08-29 15:40
//Description : Convert a integer IP to dot IP:0x12345678 -> 12.34.56.78 (pos)
//Input       : IP, string
//Output      : None
//Return      : Pointer to string
//*****************************************************************************
DJ_S8* ITP_IP_Int_to_String(DJ_S32 IP, DJ_S8* string)
{
	DJ_U8  FIP1 = 0, FIP2 = 0, FIP3 = 0, FIP4 = 0;

	FIP1 = (IP >> 24) & 0xFF;
	FIP2 = (IP >> 16) & 0xFF;
	FIP3 = (IP >>  8) & 0xFF;
	FIP4 = IP & 0xFF;

	sprintf(string, "%d.%d.%d.%d", 
		    FIP4, 
		    FIP3, 
		    FIP2, 
		    FIP1
		   );

	return string;
}


//*****************************************************************************
//Name        : ITP_IP_String_to_Int
//Author      : Luo
//DateTime    : 2005-08-29 15:50
//Description : Convert a dot IP to int IP:0x12345678 <- 12.34.56.78 (pos)
//Input       : string
//Output      : None
//Return      : IP as a integer
//*****************************************************************************
DJ_U32  ITP_IP_String_to_Int(DJ_S8* string)
{
	DJ_U32 FIP[4];
	DJ_U32 UIP = 0;

	if (string)
	{
		Split2Int(string,".",FIP);
		UIP = FIP[3] << 24 | 
			  FIP[2] << 16 |
			  FIP[1] << 8  | 
			  FIP[0];		
	}

	return UIP;
}
//*****************************************************************************
//Name        :  DJNetEntry_SetIP                                                               
//Author      :  兰骏                                                               
//DateTime    :  2005-11-17 10:00                                        
//Description :  绑定网卡地址和IP到ARP表中                                                                
//Input       :  DJ_U32 u32IP      //IP地址
//               DJ_S8 *MACAddr    //网卡地址
//               DJ_U32 u32Index   //ARP表中的位置
//Output      :  无                        
//Return      :  0 成功 -1失败
//*****************************************************************************
DJ_S32  DJNetEntry_SetIP(DJ_U32 u32IP,DJ_S8 *MACAddr,DJ_U32 u32Index)
{
#ifdef WIN32
//	MIB_IPNETROW ArpEntry;
	return 0;
#else
	return 0;
#endif
}

//*****************************************************************************
//Name        : Closesocket
//Author      : Luo
//DateTime    : 2006-03-20 15:00
//Description : close a specify socket
//Input       : s - socket handle
//Output      : None
//Return      : 0 - Success
//             -1 - Failure
//*****************************************************************************
DJ_S32 Closesocket(DJ_SOCKET s)
{
#ifdef WIN32
	
	return closesocket(s);

#else
	
	return close(s);

#endif
}


#ifndef WIN32
//*****************************************************************************
//Name        :  LoadLibrary                                                               
//Author      :  hewei                                                               
//DateTime    :  2005-08-30 10:00                                        
//Description :  加载动态库                                                                
//Input       :  DJ_S8 *pDllName          //动态库名
//Output      :  无                        
//Return      :  句柄
//*****************************************************************************
DJ_U32 * LoadLibrary(DJ_S8 * pDllName)
{
    void *handle = NULL;
   
    if(pDllName == NULL || strlen(pDllName) == 0)
    {
        return NULL;
    }
 
    handle = dlopen(pDllName,RTLD_NOW);
    if(handle == NULL)
    {
       return NULL;
    }
        
    return (DJ_U32 *)handle;
}

//*****************************************************************************
//Name        :  LoadLibrary                                                               
//Author      :  hewei                                                               
//DateTime    :  2005-08-30 10:00                                        
//Description :  获取动态库函数地址                                                                
//Input       :  hHandle          //动态库句柄
//               pFuncName        //函数名
//Output      :  无                        
//Return      :  句柄
//*****************************************************************************
DJ_U32 * GetProcAddress(DJ_Void *hHandle,DJ_S8 *pFuncName)
{
   DJ_U32 *pFuncHandle = NULL;

   if(hHandle == NULL || pFuncName == NULL || strlen(pFuncName) == 0)
   {
       return NULL;
   }
   pFuncHandle =(DJ_U32*) dlsym(hHandle,pFuncName);

   return pFuncHandle;
}

//*****************************************************************************
//Name        :  LoadLibrary                                                               
//Author      :  hewei                                                               
//DateTime    :  2005-08-30 10:00                                        
//Description :  释放动态库句柄
//Input       :  hHandle          //动态库句柄
//Output      :  无                        
//Return      :  句柄
//*****************************************************************************
DJ_S32 FreeLibrary(DJ_Void *hHandle)
{
    if(hHandle == NULL)
    {
        return 0;
    }

    dlclose(hHandle);
    return 1;
}

//*****************************************************************************
//Name        :  Skip_Left_Space                                                               
//Author      :  hewei                                                               
//DateTime    :  2005-08-30 10:00                                        
//Description :  去掉字符串左边的空格                                                                
//Input       :  str          //字符串
//Output      :  无                        
//Return      :  
//***************************************************************************** 
DJ_Void Skip_Left_Space(DJ_S8 **str)
{
	while ((*(*str) == ' ') || (*(*str) == '\t'))
	{
		(*str)++;
	}
}

//*****************************************************************************
//Name        :  Skip_Right_Space                                                               
//Author      :  hewei                                                               
//DateTime    :  2005-08-30 10:00                                        
//Description :  去掉字符串右边的空格                                                                
//Input       :  str          //字符串
//Output      :  无                        
//Return      :  
//***************************************************************************** 
DJ_Void Skip_Right_Space(DJ_S8 **str)
{
	DJ_S32 i;
	
	i = strlen(*str);
        DJ_S32 s32oldlen = i;
  	
	while(i >=0 && (*(*str + i-1) ==' ' || *(*str + i-1) =='\t'))
	{
		i--;
	}
        if(s32oldlen >i)
        {
  	    *(*str+i) =0;
        }
}

//*****************************************************************************
//Name        :  Skip_lr_Space                                                               
//Author      :  hewei                                                               
//DateTime    :  2005-08-30 10:00                                        
//Description :  去掉字符串左右边的空格                                                                
//Input       :  str          //字符串
//Output      :  无                        
//Return      :  
//***************************************************************************** 
DJ_Void Skip_lr_Space(DJ_S8 **str)
{
	Skip_Left_Space(str);
	Skip_Right_Space(str);
}

//*****************************************************************************
//Name        :  isapp                                                               
//Author      :  hewei                                                               
//DateTime    :  2005-08-30 10:00                                        
//Description :  判断是否字符串为appname
//Input       :  str          //字符串
//Output      :  无                        
//Return      :  1 为'[]'类型但不是需要的appname
//               0 为需要的appname
//               -1 为数据
//               -2 为注释
//***************************************************************************** 
DJ_S32 isapp(const DJ_S8 *linedata, const DJ_S8 *appname)
{
	DJ_S8 line[LINELEN];
	DJ_S8 *str = line;
	DJ_S8 *right;

	memcpy(line, linedata, LINELEN);

	Skip_lr_Space(&str);
	if (*str != '[')
	{
		return -1;
	}
	else if(*str =='#')
	{
		return -2;
	}
	else 
	{
		str++;
		right = str;
		while (*right != '\0') 
		{
			if (*right == ']')
			{
				break;
			}
			right++;
		}
		if (*right != '\0')
		{
			*right = '\0';
		}
	}
	Skip_lr_Space(&str);
	if(strcasecmp(str, appname) == 0)
	{
		return 0;
	}
	return 1;
}

//*****************************************************************************
//Name        :  gotoapp                                                               
//Author      :  hewei                                                               
//DateTime    :  2005-08-30 10:00                                        
//Description :  找到appname所在行
//Input       :  str          //字符串
//Output      :  无                        
//Return      :  
//***************************************************************************** 
DJ_S32 gotoapp(FILE * file, DJ_S8 *appname)
{
	DJ_S8 line[LINELEN];
	DJ_S8 *str = NULL;

	Skip_lr_Space(&appname);
	while ((str = fgets(line, LINELEN, file)) != NULL)
	{
		Skip_lr_Space(&str);
		if (isapp(str, appname) == 0)
		{
			return 0;
		}
	}

	return -1;
}

//*****************************************************************************
//Name        :  NotKeyLine                                                               
//Author      :  hewei                                                               
//DateTime    :  2005-08-30 10:00                                        
//Description :  判断是否为key行
//Input       :  str          //字符串
//Output      :  无                        
//Return      :  
// 0 : 表示为key行且相等
// 1 : 表示表示为key行,但不相等
// -1: 表示为非key行
//***************************************************************************** 
DJ_S32 NotKeyLine(DJ_S8 *line, const DJ_S8 *keyname, DJ_S8 **val)
{
	DJ_S8 *str = line;
	DJ_S8 *right;

	Skip_lr_Space(&str);

	if (*str == '[')
	{
		return -1;
	}

	right = str;
	while (*right != '\0') 
	{
		if (('=' == *right))
		{
			break;
		}
		right++;
	}
	if (*right != '\0') 
	{
		*right = '\0';
	}
	else
	{
		return 1;
	}
	
	Skip_lr_Space(&str);
	
	if (strcasecmp(str, keyname) == 0 ) 
	{
		str = right + 1;
		Skip_lr_Space(&str);
		*val = str;
		return 0;
	}

	return -1;
}

//*****************************************************************************
//Name        :  getkey
//Author      :  hewei                                                               
//DateTime    :  2005-08-30 10:00                                        
//Description :  判断是否为key行
//Input       :  str          //字符串
//Output      :  无                        
//Return      :  
//***************************************************************************** 
DJ_S32 getkey(FILE * file, const DJ_S8 *keyname, DJ_S8 *buf, DJ_S32 len)
{
	DJ_S8 line[LINELEN];
	DJ_S8 *str = NULL;
//*******!!!!	
	if(len<=LINELEN)
	{
   	    while (NULL != (str = fgets(line, LINELEN, file))) 
		{
		    Skip_lr_Space(&str);
		    DJ_S32 len = strlen(line);
		    while (line[len - 1] == '\n' || line[len - 1] == '\r')
			{
			    len--;
			}
		    line[len] = '\0';
		    if (!NotKeyLine(line, keyname, &str)) 
			{
			    strncpy(buf, str, len);
			    return 0;
			}
		}
	}
	else
	{
	   DJ_S8 *s8pStr = NULL;
	   s8pStr = (DJ_S8*) malloc(len+10);

	   if ( NULL == s8pStr ) 
	   		return -1;
	   
	   while (NULL != (str = fgets(s8pStr, len, file))) 
	   {
		  Skip_lr_Space(&str);
		  DJ_S32 len = strlen(s8pStr);
		  while (s8pStr[len - 1] == '\n' || s8pStr[len - 1] == '\r')
		  {
			 len--;
		  }
		  s8pStr[len] = '\0';
		  if (!NotKeyLine(s8pStr, keyname, &str)) 
		  {
			 strncpy(buf, str, len);
			 free(s8pStr);
			 return 0;
		  }
	   }
	   free(s8pStr);
	}

	return -1;
}

//*****************************************************************************
//Name        :  AddAppKey
//Author      :  hewei                                                               
//DateTime    :  2005-08-30 10:00                                        
//Description :  加入Appname值
//Input       :  str          //字符串
//Output      :  无                        
//Return      :  
//***************************************************************************** 
DJ_Void AddAppKey(FILE *file, const DJ_S8 *appname, const DJ_S8 *key, const DJ_S8 *val)
{
	DJ_S8 buf[LINELEN] ="[";
//******!!!!
	strncat(buf, appname, LINELEN-1);
	strcat(buf, "]");
	fprintf(file, "%s\n", buf);
//	snprintf(buf, LINELEN, "%s=%s", key, val);
	fprintf(file, "%s=%s\n", key, val);
}

//*****************************************************************************
//Name        :  GetPrivateProfileString
//Author      :  hewei                                                               
//DateTime    :  2005-08-30 10:00                                        
//Description :  windows兼容函数
//Input       :  str          //字符串
//Output      :  无                        
//Return      :  
//***************************************************************************** 
DJ_S32 GetPrivateProfileString(DJ_S8 *appname, DJ_S8 *keyname,DJ_S8 *defval, DJ_S8 *buf, DJ_U32 len,DJ_S8 *filename)
{
	FILE *file = fopen(filename, "r+");

	if (file == NULL || buf == NULL) 
	{
		return -1;
	}

	if (gotoapp(file, appname) < 0) 
	{
		if(defval != NULL)
		{
			strncpy(buf, defval, len);
		}
		fclose(file);
		return -1;
	}
	else
	{
		if (getkey(file, keyname, buf, len) < 0)
		{
			if(defval != NULL)
			{				
				strncpy(buf, defval, len);
			}
			else
			{
				buf[0] = 0;
			}
			fclose(file);
			return -1;
		}
	}

	fclose(file);

	return 0;
}

//*****************************************************************************
//Name        :  GetPrivateProfileInt
//Author      :  hewei                                                               
//DateTime    :  2005-08-30 10:00                                        
//Description :  windows兼容函数
//Input       :  str          //字符串
//Output      :  无                        
//Return      :  
//***************************************************************************** 
DJ_S32 GetPrivateProfileInt(DJ_S8 *appname,DJ_S8 *keyname,DJ_S32 defval,DJ_S8 *filename)
{
	DJ_S8 buf[LINELEN];

	GetPrivateProfileString(appname,keyname, "",buf,LINELEN,filename);
	if(buf[0] != 0)
	{
	   return (atoi(buf));
	}
	else
	{
	   return defval;
	}

	return 0;
}

//*****************************************************************************
//Name        :  WritePrivateProfileString
//Author      :  hewei                                                               
//DateTime    :  2005-08-30 10:00                                        
//Description :  windows兼容函数
//Input       :  str          //字符串
//Output      :  无                        
//Return      :  
//***************************************************************************** 
DJ_S32 WritePrivateProfileString(DJ_S8 *appname,DJ_S8 *keyname,DJ_S8 *val,DJ_S8 *filename)
{
	DJ_S32 app,ret,key;
	FILE *file,*newfile;
	DJ_S8 *str,tmpname[513];
	DJ_S8 *line =NULL;
	DJ_S8 linecpy[LINELEN];

	if ((file = fopen(filename, "r+")) == NULL) 
	{
		if((file = fopen(filename, "w+"))  == NULL)
		{
			return -1;
		}

		AddAppKey(file, appname, keyname, val);
		fclose(file);
	}
	else
	{
		line = (DJ_S8 *) malloc(MAX_LINELEN);

		sprintf(tmpname, "%s~", filename);
		app = 0;
		key = 0;
		newfile = fopen(tmpname, "w+");
		while (fgets(line, MAX_LINELEN, file) != NULL)
		{
			ret = isapp(line, appname);
			//if find appname
			if (ret == 0)
			{
			   app =1;
               fprintf(newfile, "%s", line);
			}
			else if((ret ==-1)&& (app ==1))
			{
				memcpy(linecpy, line, LINELEN);
				//if find keyname
				if ((ret = NotKeyLine(linecpy, keyname, &str)) == 0)
				{
					fprintf(newfile, "%s=%s\n", keyname, val);
					app = -1;
					key = 1;
				}
				else 
				{
					fprintf(newfile, "%s", line);
				}
			}
			else if (ret ==1)
			{
                if(app==1)
				{
					if(key == 0)
					{
						fprintf(newfile, "%s=%s\n", keyname, val);
						key = 1;
					}
					app = -1;
				}
				fprintf(newfile, "%s", line);
			}
			else
			{
				fprintf(newfile, "%s", line);
			}
		}
		if (app == 0)
		{
			AddAppKey(newfile, appname, keyname, val);
		}
		else if(key == 0)
		{
			fprintf(newfile, "%s=%s\n", keyname, val);
			key = 1;
		}
		free(line);
		fclose(newfile);
		fclose(file);
		remove(filename);
		rename(tmpname, filename);
	}

	return 0;
}

//*****************************************************************************
//Name        :  GetPrivateProfileStruct
//Author      :  hewei                                                               
//DateTime    :  2005-08-30 10:00                                        
//Description :  windows兼容函数,读取一个结构
//Input       :  
//Output      :  无                        
//Return      :  
//***************************************************************************** 
DJ_S32 GetPrivateProfileStruct(DJ_S8* appname,DJ_S8* keyname,DJ_Void * pstruct,
							   DJ_U32 u32size,DJ_S8* filename)
{
	DJ_S8 buf[LINELEN] = {0};
	
	if(pstruct == NULL)
	{
		return -1;
	}
	if(GetPrivateProfileString(appname,keyname,"",buf,u32size,filename))
	{
		return -1;
	}
	if(strlen((DJ_S8*)pstruct) == 0)
	{
		return -1;
	}
	strncpy((DJ_S8*)pstruct,buf,2*u32size);

	return 0;
}

//*****************************************************************************
//Name        :  WritePrivateProfileStruct
//Author      :  hewei                                                               
//DateTime    :  2005-08-30 10:00                                        
//Description :  windows兼容函数,写入一个结构
//Input       :  
//Output      :  无                        
//Return      :  
//***************************************************************************** 
DJ_S32 WritePrivateProfileStruct(DJ_S8* appname,DJ_S8* keyname,
							   DJ_Void* pstruct,DJ_U32 u32size,DJ_S8* filename)
{
	DJ_S8  *p = NULL;
	DJ_S8  buf[LINELEN] = {0};
	DJ_S8  *pbuf = buf;
	DJ_S8  high=0,low=0;	
	DJ_U32 sum = 0;
	DJ_U32 i = 0;
	
	if(pstruct == NULL)
	{
		return -1;
	}

	p = (DJ_S8 *)&pstruct;
	for(i=0;i<u32size;i++)
	{
		sum += ((*p) & 0xFF);
		low = (*p) & 0xF;
		high = ((*(p++)) >> 4) & 0xF;
		sprintf(pbuf++,"%0x%0x",high,low);
		++pbuf;		
	}
	sum &= 0xFF;
	sprintf(pbuf,"%0x", (int)sum);

	return WritePrivateProfileString(appname,keyname,buf,filename);
}

//*****************************************************************************
//Name        :  DeletePrivateProfileString
//Author      :  fuyunshi                                                               
//DateTime    :  2007-04-05                                        
//Description :  windows兼容函数,删除配置文件中指定的keyname条目
//Input       :  str          //字符串
//Output      :  无                        
//Return      :  
//***************************************************************************** 
DJ_S32 DeletePrivateProfileString( const DJ_S8 *appname,
                                   const DJ_S8 *keyname,
                                   const DJ_S8 *val,
                                   const DJ_S8 *filename )
{
	DJ_S32 app,ret,IsReserveSpace = 0;
	FILE *file,*newfile;
	DJ_S8 *str,tmpname[513];
	DJ_S8 line[LINELEN];
	DJ_S8 linecpy[LINELEN];

	if ((file = fopen(filename, "r+")) == NULL) 
	{
		return -1;
	}
	else
	{
		sprintf(tmpname, "%s~", filename);
		app = 0;
		newfile = fopen(tmpname, "w+");
		while (fgets(line, LINELEN, file) != NULL)
		{
			if (app <= 0)
			{
			    if (isapp(line, appname) == 0)
				{
				   app =1;
				}
				fprintf(newfile, "%s", line);
			}
			else
			{
				memcpy(linecpy, line, LINELEN);
				if ((ret = NotKeyLine(line, keyname, &str)) == 0)
				{
					//fprintf(newfile, "%s=%s\n", keyname, val);
				//	app = -1;
					IsReserveSpace = 0;
				}
				else if (ret == 1)
				{
					if ( 0 == IsReserveSpace )
					{
					    fprintf(newfile, "%s", linecpy);
						IsReserveSpace++;
					}
				}
				else
				{
					IsReserveSpace = 0;
					//fprintf(newfile, "%s=%s\n", keyname, val);
					fprintf(newfile, "%s", linecpy);
                    str = linecpy;
                    Skip_Left_Space(&str);
                    if ( '[' == *str ) app = -1;
					
				}
			}
		}
		/*if (app == 0)
		{
			AddAppKey(newfile, appname, keyname, val);
		}*/
		fclose(newfile);
		fclose(file);
		remove(filename);
		rename(tmpname, filename);
	}

	return 0;
}

//*****************************************************************************
//Name        :  GetLocalTime
//Author      :  hewei                                                               
//DateTime    :  2005-08-30 10:00                                        
//Description :  windows兼容函数，取当前时间
//Input       :  str          //字符串
//Output      :  无                        
//Return      :  
//***************************************************************************** 
DJ_Void GetLocalTime(LPSYSTEMTIME lpSystemTime)
{
	/*
	struct timeval tmv;	
	struct tm ptm;


	gettimeofday(&tmv,NULL);
    localtime_r(&tmv.tv_sec,&ptm);

	lpSystemTime->wYear = ptm.tm_year;
	lpSystemTime->wMonth = ptm.tm_mon;
	lpSystemTime->wDayOfWeek = 0;
	lpSystemTime->wDay = ptm.tm_wday;
	lpSystemTime->wHour = ptm.tm_hour;
	lpSystemTime->wMinute = ptm.tm_min;
	lpSystemTime->wSecond = ptm.tm_sec;
	lpSystemTime->wMilliseconds = tmv.tv_usec/1000;
	*/
	struct timeval tmv;	
	struct tm ptm;
	
	gettimeofday(&tmv,NULL);
    localtime_r(&tmv.tv_sec,&ptm);

	lpSystemTime->wYear = (DJ_U16)(1900+ptm.tm_year);
	lpSystemTime->wMonth = (DJ_U16)(1+ptm.tm_mon);
	lpSystemTime->wDayOfWeek = (DJ_U16)ptm.tm_wday;
	lpSystemTime->wDay = (DJ_U16)ptm.tm_mday;
	lpSystemTime->wHour = ptm.tm_hour;
	lpSystemTime->wMinute = ptm.tm_min;
	lpSystemTime->wSecond = ptm.tm_sec;
	lpSystemTime->wMilliseconds = tmv.tv_usec/1000;
}
/*
DJ_Void _strtime(DJ_S8 *strtime)
{
    SYSTEMTIME pSystemTime;
    GetLocalTime(&pSystemTime);
    sprintf(strtime,"%d:%d:%d",pSystemTime.wHour,pSystemTime.wMinute,pSystemTime.wSecond);
}
*/

//*****************************************************************************
//Name        :  GetLocalTime
//Author      :  hewei                                                               
//DateTime    :  2005-08-30 10:00                                        
//Description :  windows兼容函数，取当前时间
//Input       :  str          //字符串
//Output      :  无                        
//Return      :  
//***************************************************************************** 
DJ_U32 GetTickCount()
{
    struct timeval tv;
	
	gettimeofday(&tv,NULL);
	return (tv.tv_sec*1000 + tv.tv_usec/1000);
}

#endif



#if 0
//一组读写文件的函数(Added by ZJM)，这组函数参照CFile类编写，它们不能操作大于4G的文件
DJ_HANDLE DJ_OpenFile( DJ_S8* szFileName, DJ_S32 nOpenFlags )
{
	DJ_U32 u32Access = 0, u32ShareMode = 0, u32CreateFlag;
	DJ_HANDLE hFile;

#ifdef WIN32
	SECURITY_ATTRIBUTES sa;
	//ASSERT((mode_Read|mode_Write|mode_ReadWrite) == 3);
	switch( nOpenFlags & 3 )
	{
	case mode_Read:
		u32Access = GENERIC_READ;
		break;
	case mode_Write:
		u32Access = GENERIC_WRITE;
		break;
	case mode_ReadWrite:
		u32Access = GENERIC_READ|GENERIC_WRITE;
		break;
	default:
		return NULL;
	}

	switch( nOpenFlags & 0x70 )    // map compatibility mode to exclusive
	{	
	case share_Compat:
	case share_Exclusive:
		u32ShareMode = 0;
		break;
	case share_DenyWrite:
		u32ShareMode = FILE_SHARE_READ;
		break;
	case share_DenyRead:
		u32ShareMode = FILE_SHARE_WRITE;
		break;
	case share_DenyNone:
		u32ShareMode = FILE_SHARE_WRITE|FILE_SHARE_READ;
		break;
	default:
		return NULL;
	}
	
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = (nOpenFlags & mode_NoInherit) == 0;

	if( nOpenFlags & mode_Create )
	{
		if( nOpenFlags & mode_NoTruncate )
			u32CreateFlag = OPEN_ALWAYS;
		else
			u32CreateFlag = CREATE_ALWAYS;
	}
	else
		u32CreateFlag = OPEN_EXISTING;

	hFile = ::CreateFile( szFileName, u32Access, u32ShareMode, &sa,
		u32CreateFlag, FILE_ATTRIBUTE_NORMAL, NULL );
	return (hFile == INVALID_HANDLE_VALUE)?NULL:hFile;
#else
	//ASSERT((mode_Read|mode_Write|mode_ReadWrite) == 3);
	struct stat m_stState;
	int iRet = 0;
	int errno; 

	if ( mode_Read == (nOpenFlags & 3) )
	{
		if ( -1 == stat( szFileName, &m_stState ) )
		{
			g_stLog.LogOut(Log_Level_Warn,"               Open File <%s> Failed, ErrorCode: %s!\n", szFileName, strerror(errno) );
			return NULL;
		}
		
		if ( !S_ISREG( m_stState.st_mode ) )
		{
			g_stLog.LogOut(Log_Level_Warn,"               Open File <%s> Failed Because It is not a File!\n", szFileName );
			return NULL;
		}
		
		if ( S_ISDIR( m_stState.st_mode ) )
		{
			g_stLog.LogOut(Log_Level_Warn,"                Open File <%s> Failed Because It is a Folder!\n", szFileName );
			return NULL;
		}
	}

	switch( nOpenFlags & 3 )
	{
	case mode_Read:
		u32Access = O_RDONLY;
		printf("O_RDONLY \n");
		break;
	case mode_Write:
		u32Access = O_WRONLY;
		printf("O_WRONLY \n");
		break;
	case mode_ReadWrite:
		u32Access = O_RDWR;
		printf("O_RDWR \n");
		break;
	default:
		return NULL;
	}


	if ( O_RDONLY == u32Access )
	{
		if ( 0 != access( szFileName, F_OK ))
		{
			g_stLog.LogOut(Log_Level_Warn,"                Open File <%s> Failed, because File is not exist. \n");
			return NULL;
		}
	}
	else
	{
		if ( 0 != access( szFileName, F_OK ))
		{
			printf("File Is Not Exist and O_CREAT \n");
			u32Access |= O_CREAT;
		}
		else
		{
			if( nOpenFlags & mode_Create )
			{
				printf("O_TRUNC \n");
				u32Access |= O_TRUNC;
			}
			else 
			{
				printf("O_APPEND \n");
				u32Access |= O_APPEND;
			}
		}
	}
	
	iRet = open( szFileName, u32Access );

	if ( -1 == iRet )
	{
		printf("Open File Failed, iRet: %d, ErrCode: %s\n", iRet, strerror(errno));
		return NULL;
	}

/*
    if ( (u32Access & O_WRONLY) || (u32Access & O_RDWR) )
    {
		struct flock stFlock = {0};

		stFlock.l_type  = F_WRLCK;
		stFlock.l_whence = SEEK_END;
		stFlock.l_start = 0;
	    stFlock.l_len   = 0;

		if ( -1 == fcntl(iRet, F_SETLK, &stFlock ) )
		{
			printf("33333333333333 \n");
			close(iRet);
			return NULL;
		}
	}
	*/
	
	hFile = (DJ_HANDLE)iRet;
	
	return (DJ_HANDLE)hFile;
#endif

}

DJ_Void DJ_CloseFile( DJ_HANDLE hFile )
{
#ifdef WIN32
	if( hFile != NULL )
		::CloseHandle( hFile );
#else
	//Unix版本的代码
	if ( NULL != hFile )
	{
		struct flock stFlock = {0};
	
		stFlock.l_type	= F_UNLCK;
		stFlock.l_whence = SEEK_END;
		stFlock.l_start = 0;
		stFlock.l_len	= 0;
	
		fcntl((DJ_S32)hFile, F_SETLK, &stFlock );

		close( (DJ_S32)hFile);
	}
#endif
}

DJ_Void DJ_FlushFile( DJ_HANDLE hFile )
{
#ifdef WIN32
	::FlushFileBuffers( hFile );
#else
	//Unix版本的代码
	//fflush((FILE*)hFile);
#endif
}

//失败，返回-1，成功返回新的文件位置
DJ_U32 DJ_SeekFile( DJ_HANDLE hFile, DJ_S32 lOff, ITP_SeekPosition nFrom )
{
#ifdef WIN32
	return ::SetFilePointer( hFile, lOff, NULL, (DJ_U32)nFrom );
#else
	//Unix版本的代码
	//return fseek( (FILE*)hFile, lOff, (DJ_S32)nFrom );
	return lseek((DJ_S32)hFile, lOff, (DJ_S32)nFrom );
#endif
}

//返回BOOL，成功返回TRUE，失败返回FALSE
DJ_U8 DJ_SetFileLength( DJ_HANDLE hFile, DJ_U32 u32NewLen )
{
#ifdef WIN32
	if( DJ_SeekFile( hFile, (DJ_S32)u32NewLen, sp_begin ) == (DJ_U32)-1 )
		return FALSE;
	return ::SetEndOfFile( hFile )?TRUE:FALSE;
#else
	//Unix版本的代码
	DJ_U8 Ret = FALSE;

    if ( NULL != hFile ) 
    {
        if ( lseek( (DJ_S32)hFile, u32NewLen, SEEK_SET ) )
        {
            lseek( (DJ_S32)hFile, 0, SEEK_END );
        }
        else
        {
            Ret = TRUE;
        }
    }

    return Ret;
#endif
}

//失败，返回-1，成功返回文件的长度
DJ_U32 DJ_GetFileLength( DJ_HANDLE hFile )
{
#ifdef WIN32
	return GetFileSize( hFile, NULL );
#else
	//Unix版本的代码
	DJ_U32 /*curr_position = 0, */ file_length = 0;
	struct stat stStat;

    if ( NULL != hFile ) 
    {
    /*
        curr_position = ftell((FILE*)hFile);
        if ( fseek( (FILE*)hFile, 0, SEEK_END ) )
        {
            iRet = -1;
            return iRet;
        }
        file_length = ftell((FILE*)hFile);
        fseek( (FILE*)hFile, curr_position, SEEK_SET );
        */
        if ( -1 == fstat( (DJ_S32)hFile, &stStat ) )
        {
			file_length = 0;
		}
		else
		{
			file_length = stStat.st_size;
		}
    }

    return file_length;
#endif
}

//失败，返回-1，成功返回新的文件位置
DJ_U32 DJ_SeekFileToEnd( DJ_HANDLE hFile )
{
#ifdef WIN32
	return DJ_SeekFile( hFile, 0, sp_end );
#else
	//Unix版本的代码
    //return fseek( (FILE*)hFile, 0, SEEK_END );
    return lseek( (DJ_S32)hFile, 0, SEEK_END );
#endif
}

//失败，返回-1，成功返回新的文件位置
DJ_U32 DJ_SeekFileToBegin( DJ_HANDLE hFile )
{
#ifdef WIN32
	return DJ_SeekFile( hFile, 0, sp_begin );
#else
	//Unix版本的代码
	//return fseek( (FILE*)hFile, 0, SEEK_SET );
	return lseek( (DJ_S32)hFile, 0, SEEK_SET );
#endif
}

//失败，返回-1，成功返回当前文件位置
DJ_U32 DJ_GetFilePosition( DJ_HANDLE hFile )
{
#ifdef WIN32
	return ::SetFilePointer( hFile, 0, NULL, FILE_CURRENT );
#else
	//Unix版本的代码,这段代码有问题，目前因为尚未使用，故暂不支持 on 091230
	//return ftell( (FILE*)hFile );
	DJ_U32 file_length = 0;
	struct stat stStat;

    if ( NULL != hFile ) 
    {
        if ( 0 != fstat( (DJ_S32)hFile, &stStat ) )
        {
			file_length = 0;
		}
		else
		{
			file_length = stStat.st_size;
		}
    }

    return file_length;

#endif
}

//失败，返回-1，成功返回读取的字节数
DJ_U32 DJ_ReadFile( DJ_HANDLE hFile, DJ_Void* lpBuf, DJ_U32 nCount )
{
#ifdef WIN32
	if( nCount == 0 )
		return 0;
	DJ_U32 dwRead;
	if( !::ReadFile( hFile, lpBuf, nCount, &dwRead, NULL ) )
		return (DJ_U32)-1;
	return dwRead;
#else
	//Unix版本的代码
	/*
	if( nCount == 0 )
		return 0;
	DJ_U32 dwRead;

    dwRead = fread( lpBuf, 1, nCount, (FILE*)hFile );
    if ( dwRead != nCount )
    {
       if ( 0 == feof( (FILE*)hFile ) )
          return 0;

	   if ( dwRead > nCount )
	   {
	       return 0;
	   }
    }
*/
	if( nCount == 0 )
		return 0;
	

    return read( (DJ_S32)hFile, lpBuf, nCount );

#endif
}

//失败，返回-1，成功返回写入的字节数
DJ_U32 DJ_WriteFile( DJ_HANDLE hFile, const DJ_Void* lpBuf, DJ_U32 nCount )
{
#ifdef WIN32
	DJ_U32 nWritten;

	if( nCount == 0 )
		return 0;

	if( !::WriteFile( hFile, lpBuf, nCount, &nWritten, NULL ) )
		return -1;

	return nWritten;
#else
	//Unix版本的代码
	/*
	if( nCount == 0 )
		return 0;
	DJ_U32 nWritten;

    nWritten = fwrite( lpBuf, 1, nCount, (FILE*)hFile );
    if ( nWritten != nCount )
		return (DJ_U32)-1;
    */
	DJ_U32 nWritten;
    
	if( nCount == 0 )
		return 0;	

    nWritten = write( (DJ_S32)hFile, lpBuf, nCount );
	
    if ( nWritten != nCount )
		return (DJ_U32)-1;
	
	return nWritten;
#endif
}

//失败，返回-1，成功返回0
//参数szPath是一个全路径名，本函数创建一个多层目录
DJ_S8 DJ_CreateDirectory( DJ_S8* szPath )
{
#ifdef WIN32
	DJ_S8*   pTmpPath;
	DJ_S8    szBackPath[MAX_PATH];
	DJ_S32   nLen = strlen( szPath );
	if( nLen == 0 )
		return -1;
	strcpy( szBackPath, szPath );
	if( szBackPath[nLen-1] != '\\' )
		strcat( szBackPath, "\\" );
	pTmpPath = strchr( szBackPath, '\\' );
	if( pTmpPath == szBackPath )  //第一个字符就是'\'
	{
		if( szBackPath[1] != '\\' )  //那么第二个字符也一定是'\'
			return -1;
		pTmpPath = strchr( szBackPath + 2, '\\' );
	}
	pTmpPath = strchr( pTmpPath+1, '\\' );
	
	while( pTmpPath != NULL )
	{
		*pTmpPath = NULL;
		if( !CreateDirectory( szBackPath, NULL ) && GetLastError() != ERROR_ALREADY_EXISTS )
		{
			*pTmpPath = '\\';
			return -1;
		}
		*pTmpPath = '\\';
		pTmpPath = strchr( pTmpPath+1, '\\' );
	}
	return 0;
#else
	//Unix版本的代码
	DJ_S32   iRetVal = 0;
    DJ_S8*   pTmpPath;
	DJ_S8    szBackPath[MAX_PATH];
	DJ_S32   nLen = strlen( szPath );
    //printf("szPath: %s\r\n", szPath);
	if( nLen == 0 )
		return -1;
	strcpy( szBackPath, szPath );
    //printf("szBackPath: %s\r\n", szBackPath);
    
	if( szBackPath[nLen-1] != '/' )
//        szBackPath[nLen] = '/';
		strcat( szBackPath, "/" );
    //printf("szBackPath1: %s\r\n", szBackPath);
    
	pTmpPath = strchr( szBackPath, '/' );
	if( pTmpPath == szBackPath )  //第一个字符就是'\'
	{
		if( szBackPath[1] == '/' )  //那么第二个字符一定不是'\'
			return -1;
		pTmpPath = strchr( szBackPath + 1, '/' );
	}
    //printf("szBackPath2: %s\r\n", szBackPath);
	pTmpPath = strchr( pTmpPath+1, '/' );
	
	while( pTmpPath != NULL )
	{
		//*pTmpPath = (DJ_S8)NULL;
        iRetVal = mkdir( szBackPath, 1|2|4|7 );
        //printf("mkdir szBackPath: %s\n", szBackPath);
		if( 0 != iRetVal && EEXIST == iRetVal )
		{
        	printf("szBackPath: %s is EXIST\n", szBackPath);
			*pTmpPath = '/';
			return -1;
		}
        
        chmod(szBackPath, S_IRWXU );
        
		*pTmpPath = '/';
		pTmpPath = strchr( pTmpPath+1, '/' );
	}
	return 0;
#endif
}

DJ_S8 DJ_CreateFullFilePath( DJ_S8* szFile )  //创建一个全路径文件名中的路径
{
	DJ_S8   nRet = -1;
    #ifdef WIN32
	DJ_S8*  pChar = strrchr( szFile, '\\' );
	if( pChar != NULL )
	{
		*pChar = 0;
		nRet = DJ_CreateDirectory( szFile );
		*pChar = '\\';
	}
    #else
	DJ_S8*  pChar = strrchr( szFile, '/' );
	if( pChar != NULL )
	{
		*pChar = 0;
		nRet = DJ_CreateDirectory( szFile );
		*pChar = '/';
	}
    #endif
	return nRet;
}
#endif

