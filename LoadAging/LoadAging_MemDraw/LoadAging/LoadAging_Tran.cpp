#include "stdafx.h"
#include "LoadAging.h"
#include "LoadAgingDlg.h"
#include "LoadAging_Tran.h"
#include "MyLog.h"
#include "NT77Api.h"
#include "LogFile.h"
#include "SLinkList.h"
#include "SerialPort.h"

extern CLoadAgingApp theApp;
CLoadAgingDlg	*pdlg;

char c='b';//链表，头结点数据

SLinkList<char> sLinkList1(c);			//接收串口数据链表
SLinkList<char> sLinkList2(c);			//接收串口数据链表
SLinkList<char> sLinkList3(c);			//接收串口数据链表
SLinkList<char> sLinkList4(c);			//接收串口数据链表
SLinkList<char> sLinkList5(c);			//接收串口数据链表
SLinkList<char> sLinkList6(c);			//接收串口数据链表
SLinkList<char> sLinkList7(c);			//接收串口数据链表
SLinkList<char> sLinkList8(c);			//接收串口数据链表
SLinkList<char> sLinkList9(c);			//接收串口数据链表
SLinkList<char> sLinkList10(c);			//接收串口数据链表
SLinkList<char> sLinkList11(c);			//接收串口数据链表
SLinkList<char> sLinkList12(c);			//接收串口数据链表
SLinkList<char> sLinkList13(c);			//接收串口数据链表
SLinkList<char> sLinkList14(c);			//接收串口数据链表
SLinkList<char> sLinkList15(c);			//接收串口数据链表
SLinkList<char> sLinkList16(c);			//接收串口数据链表

// var about LoadAging.ini
char			cfg_IniName[256] = "";
char			cfg_IniShortName[] = "\\LoadAging.ini";
char			cfg_NormalPassword[128]={0};				//技术人员密码
char			cfg_SuperPassword[128]={0};					//工程师密码
char			cfg_DataRecordPath[256]="";					//数据保存路径
char			cfg_Title[256]="电子负载老化系统";			//标题
int				cfg_EnableBeep;								//使能蜂鸣器
int				cfg_EnableLight;							//使能指示灯
//系统全局变量--------------------------------------------
int				g_LoginCheckOK=-1;							//登录校验是否通过,-1:校验失败；0:工程师登录成功；1:技术员登录成功
long			g_SystemRunTimeCounter=0;					//系统运行时间计数器
CAR_STRUCT		g_AllCar[MAX_CAR_COUNT];					//最重要的全局结构体
int				g_ComList[MAX_CAR_COUNT];					//可用串口列表
int				g_AvailableComCounter=0;					//可用串口数目
int				g_curSelTestCar=0;							//当前选中测试车
CMyLog			g_data[MAX_CAR_COUNT];						//记录测试数据日志类
CSerialPort		g_Ports[MAX_CAR_COUNT];						//串口类，处理每个测试车串口数据
float			g_SetCurrentError=0.2;						//设置电流值偏差范围
float			g_SetVoltageError=1.0;						//设置电压值偏差范围

/************************************************************************/
/* 读取配置文件ini                                                      */
/************************************************************************/
void	ReadFromConfig(void)
{
	GetCurrentDirectory ( 256-32, cfg_IniName );
	strcat ( cfg_IniName, cfg_IniShortName );
	
	cfg_LogOn = GetPrivateProfileInt ( "ConfigInfo", "LogOn", 1, cfg_IniName);					//是否开启日志
	cfg_LogLevel = GetPrivateProfileInt ( "ConfigInfo", "LogLevel", 0, cfg_IniName);			//日志级别
	
	cfg_EnableBeep = GetPrivateProfileInt ( "ConfigInfo", "EnableBeep", 1, cfg_IniName);		//是否使能蜂鸣器
	cfg_EnableLight = GetPrivateProfileInt ( "ConfigInfo", "EnableLight", 1, cfg_IniName);		//是否使能指示灯
	
	GetPrivateProfileString ( "ConfigInfo", "NormalPassword", "000000", cfg_NormalPassword, sizeof(cfg_NormalPassword), cfg_IniName);
	GetPrivateProfileString ( "ConfigInfo", "SuperPassword", "000000", cfg_SuperPassword, sizeof(cfg_SuperPassword), cfg_IniName);
	GetPrivateProfileString ( "ConfigInfo", "DataRecordPath", "..\\DataRecord\\", cfg_DataRecordPath, sizeof(cfg_DataRecordPath), cfg_IniName);
	GetPrivateProfileString ( "ConfigInfo", "Title", "", cfg_Title, sizeof(cfg_Title), cfg_IniName);
	
}

/************************************************************************/
/* 写配置文件ini                                                        */
/************************************************************************/
void	WriteToConfig(void)
{
	char	bufStr[20];		
	sprintf(bufStr, "%d", cfg_EnableBeep);
	WritePrivateProfileString ( "ConfigInfo", "EnableBeep", bufStr, cfg_IniName);
	
	sprintf(bufStr, "%d", cfg_EnableLight);
	WritePrivateProfileString ( "ConfigInfo", "EnableLight", bufStr, cfg_IniName);
	
	WritePrivateProfileString ( "ConfigInfo", "NormalPassword", cfg_NormalPassword, cfg_IniName);
	WritePrivateProfileString ( "ConfigInfo", "SuperPassword", cfg_SuperPassword, cfg_IniName);
	WritePrivateProfileString ( "ConfigInfo", "DataRecordPath", cfg_DataRecordPath, cfg_IniName);
}

/************************************************************************/
/* 系统初始化函数                                                       */
/************************************************************************/
bool InitSystem(void) 
{
	pdlg = (CLoadAgingDlg	*)theApp.m_pMainWnd;
	
	InitLogCfg();			//初始化系统日志
	GetAvailableComList();	//初始化系统当前可用串口列表
	
	for (int i=0;i<MAX_CAR_COUNT;i++)
	{
		InitializeCriticalSection(&g_AllCar[i].CriticalSectionClear); 
	}
	return true;
}

/************************************************************************/
/* 获取测试时间字符串                                                   */
/************************************************************************/
void DrawTotalTestTime(char* testTimeStr,int iCarIndex)
{
	char logBuf[256]={0};
	LONGLONG lTotlalTime = 0;
	int     iH = 0;
	int     iM = 0;
	int     iS = 0;
	if(iCarIndex>MAX_CAR_COUNT|| iCarIndex<0)
	{
		sprintf(logBuf,"Error! DrawTotalTestTime() fail, iCardIndex:%d.",iCarIndex);
		WriteLog(LEVEL_ERROR,logBuf);
		return;
	}
	//lTotlalTime = (GetTickCount() - g_AllCar[iCarIndex].iTestStartTime)/1000;
	lTotlalTime = g_AllCar[iCarIndex].m_TimeCounter*COMMAND_INTERVAL/1000;
	iH = (int)lTotlalTime/3600;
	iM = (int)(lTotlalTime - iH*3600)/60;
	iS = (int)(lTotlalTime - iH*3600 - iM * 60);
	sprintf(testTimeStr, "%02d:%02d:%02d", iH, iM, iS);
}
/************************************************************************/
/*获取测试车状态                                                        */
/************************************************************************/
char*	GetTestCarStateName(int carID)									
{
	if (carID<0||carID>MAX_CAR_COUNT)
	{
		return "ERRORID";
	}
	//测试车当前状态，以及测试时间、测试按钮显示
	switch(g_AllCar[carID].m_CarState)
	{
	case CAR_STATE_NOIMPORT:
		return "未导入";
	case CAR_STATE_IMPORTED:
		return "已导入";
	case CAR_STATE_TESTING:
		return "测试中";
	case CAR_STATE_TESTFINISH:
		return "测试完";
	}
	return "UNKNOWN";
}
/************************************************************************/
/* 刷新当前测试车状态信息                                               */
/************************************************************************/
void DrawCurrentCarInfo(void)
{
	char logBuf[256];
	char tmpStr[512];
	char comStr[6];
	char	testTimeStr[16]={0};
	char	testResultStr[128]={0};
	
	memset(logBuf,0,sizeof(logBuf));
	memset(tmpStr,0,sizeof(tmpStr));
	
	if(g_curSelTestCar>MAX_CAR_COUNT|| g_curSelTestCar<0)
	{
		sprintf(logBuf,"Error! DrawCurrentCarInfo() fail, carID:%d.",g_curSelTestCar);
		WriteLog(LEVEL_ERROR,logBuf);
		return;
	}
	//串口信息
	if(g_AllCar[g_curSelTestCar].m_ComID == 0)
	{
		sprintf(comStr,"无");
	}else
	{
		sprintf(comStr,"COM%d",g_AllCar[g_curSelTestCar].m_ComID);		
	}
	//测试运行时间
	if(	CAR_STATE_TESTING == g_AllCar[g_curSelTestCar].m_CarState )
	{
		DrawTotalTestTime(testTimeStr,g_curSelTestCar);	
	}else
	{
		strcpy(testTimeStr,"00:00:00");
	}
	
	//测试数据信息
	if(	CAR_STATE_NOIMPORT == g_AllCar[g_curSelTestCar].m_CarState )
	{
		strcpy(testResultStr, "总数:0,合格:0,不良:0,不良率:0.00\%");
		
	}else
	{
		int	errorLoadCounter=0;	 //出错负载数
		int	totalLoadCounter=g_AllCar[g_curSelTestCar].testParam.endLoadNum - g_AllCar[g_curSelTestCar].testParam.startLoadNum + 1;  //总共待测试负载数目
		float errorPercent; 
		for (int loadIndex=g_AllCar[g_curSelTestCar].testParam.startLoadNum; loadIndex<=g_AllCar[g_curSelTestCar].testParam.endLoadNum; loadIndex++)
		{
			if(g_AllCar[g_curSelTestCar].m_Load[loadIndex-1].m_LoadErrorCounter != 0 )
			{
				errorLoadCounter++;
			}
		}
		errorPercent = (float)errorLoadCounter/(float)totalLoadCounter*100;//不良率
		sprintf(testResultStr, "总数:%d,合格:%d,不良:%d,不良率:%2.2f%%",
			totalLoadCounter,totalLoadCounter-errorLoadCounter,errorLoadCounter,errorPercent);
	}		
	
	sprintf(tmpStr,"当前区:%s, 串口:%s, 状态:%s, 车型:%d*%d, 扫描地址:[%d,%d], 出错次数:%d, %s, 总时间:%d分钟, 测试时间:%s",
		GetTestCarName(g_curSelTestCar),
		comStr,
		GetTestCarStateName(g_curSelTestCar),
		g_AllCar[g_curSelTestCar].testParam.layerPerCar,
		g_AllCar[g_curSelTestCar].testParam.loadPerLayer,
		g_AllCar[g_curSelTestCar].testParam.startLoadNum,
		g_AllCar[g_curSelTestCar].testParam.endLoadNum,
		g_AllCar[g_curSelTestCar].m_ErrorCounter,
		testResultStr,
		g_AllCar[g_curSelTestCar].testParam.totalTestTime,
		testTimeStr);
	pdlg->SetDlgItemText(IDC_STATIC_CURCAR,tmpStr);
	
}

/************************************************************************/
/* 获取系统可用设备列表                                                 */
/************************************************************************/
void GetAvailableComList(void)
{
	int nCom;
	CString strCom;
	HANDLE hCom;
	
	//初始化COM列表,最多打开16个串口,打开串口范围[1,99]------------------
	for( nCom=1; nCom<100&&g_AvailableComCounter<16; nCom++)
	{
		strCom.Format("COM%d", nCom);
		hCom = CreateFile(strCom, 0, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (INVALID_HANDLE_VALUE != hCom)
		{
			g_ComList[g_AvailableComCounter] = nCom;
			g_AvailableComCounter++;
		}		
		CloseHandle(hCom);
	}
}

/************************************************************************/
/*获取测试车名称                                                        */
/************************************************************************/
char*	GetTestCarName(int carID)									
{
	switch(carID)
	{
	case 0:		return "A1";
	case 1:		return "A2";
	case 2:		return "A3";
	case 3:		return "A4";
	case 4:		return "B1";
	case 5:		return "B2";
	case 6:		return "B3";
	case 7:		return "B4";
	case 8:		return "C1";
	case 9:		return "C2";
	case 10:	return "C3";
	case 11:	return "C4";
	case 12:	return "D1";
	case 13:	return "D2";
	case 14:	return "D3";
	case 15:	return "D4";
	}
	return "UNKNOWN";
}



/************************************************************************/
/*获取负载模式名称                                                      */
/************************************************************************/
char*	GetLoadModeName(int modeID)									
{
	switch(modeID)
	{
	case LOAD_MODE_CC:		return "CC模式";
	case LOAD_MODE_CV:		return "CV模式";
	}
	return "UNKNOWN";
}
/************************************************************************/
/*获取负载模式名称                                                      */
/************************************************************************/
char*	Led_GetLoadModeName(int modeID)									
{
	switch(modeID)
	{
	case LOAD_MODE_CC:		return "CC";
	case LOAD_MODE_CV:		return "CV";
	}
	return "UNKNOWN";
}
char*	Led_GetFixedValue(int modeID)									
{
	switch(modeID)
	{
	case LOAD_MODE_CC:		return "A";
	case LOAD_MODE_CV:		return "V";
	}
	return "UNKNOWN";
}
char*	Led_GetUnitName(int modeID)									
{
	switch(modeID)
	{
	case LOAD_MODE_CC:		return "V";
	case LOAD_MODE_CV:		return "A";
	}
	return "UNKNOWN";
}

/************************************************************************/
/*获取通道状态名称字符串                                                */
/************************************************************************/
char*   GetChnStateName(int chnMode)
{
	switch (chnMode)
	{
	case	CHN_STATE_NOLINK:	return	"未连接";
	case	CHN_STATE_OK:		return	"合格";
	case	CHN_STATE_LOWVOL:	return	"欠压";
	case	CHN_STATE_OVERVOL:	return	"过压";
	case	CHN_STATE_LOWCUR:	return	"欠流";
	case	CHN_STATE_OVERCUR:	return	"过流";
	}
	return "UNKNOWN";
}
/************************************************************************/
/*获取title                                                             */
/************************************************************************/
char*	GetChnTitleName(int modeID)									
{
	switch(modeID)
	{
	case LOAD_MODE_CC:		return "定电流值,电压上限,电压下限,产品型号";
	case LOAD_MODE_CV:		return "定电压值,电流上限,电流下限,产品型号";
	}
	return "UNKNOWN";
}
/************************************************************************/
/* 根据串口号查找测试车ID                                               */
/************************************************************************/
int GetCarIDByPortNo( int portNo)
{
	char logBuf[512];
	if (portNo<=0 || portNo>=MAX_PORT_NO)
	{
		sprintf(logBuf,"Error, portNo:%d is wrong!", portNo);
		WriteLog(LEVEL_ERROR, logBuf);
		return -1;
	}
	for(int i=0;i<MAX_CAR_COUNT;i++)
	{
		if ( g_AllCar[i].m_ComID == portNo )
		{
			return i;
		}		
	}
	return -1;
	
}
/************************************************************************/
/* 处理串口接收数据                                                     */
/************************************************************************/
void	ProcessReceiveData( char recvByte, int portNo)
{
	int carID;
	char cStartFlag = (char)0x6B;						//命令开始标记
	char cEndFlag	= (char)0x6E;						//命令结束标记
	
	char logBuf[512];
	memset(logBuf,0,sizeof(logBuf));
	
	if (portNo<=0 || portNo>=MAX_PORT_NO)
	{
		sprintf(logBuf,"Error, portNo:%d is wrong!", portNo);
		WriteLog(LEVEL_ERROR, logBuf);
		return;
	}
	
	//根据串口号定位测试车
	carID = GetCarIDByPortNo(portNo);	
	if ( carID < 0 )
	{
		sprintf(logBuf,"Error, carID:%d is wrong!", carID);
		WriteLog(LEVEL_ERROR, logBuf);
		return;
	}
				
	//接收字节插入到单链表，每次读取一个字节
	switch (carID)
	{
	case 0:
		if( sLinkList1.length==0 && recvByte!=cStartFlag )				
		{
			return;	
		}
		if(sLinkList1.length==39)
		{
			return;
		}
		sLinkList1.add(recvByte);		
		sLinkList1.printAllData(logBuf);
		break;
	case 1:
		if( sLinkList2.length==0 && recvByte!=cStartFlag )				
		{
			return;	
		}
		if(sLinkList2.length==39)
		{
			return;
		}
		sLinkList2.add(recvByte);		
		sLinkList2.printAllData(logBuf);
		break;
	case 2:
		if( sLinkList3.length==0 && recvByte!=cStartFlag )				
		{
			return;	
		}
		if(sLinkList3.length==39)
		{
			return;
		}
		sLinkList3.add(recvByte);		
		sLinkList3.printAllData(logBuf);
		break;
	case 3:
		if( sLinkList4.length==0 && recvByte!=cStartFlag )				
		{
			return;	
		}
		if(sLinkList4.length==39)
		{
			return;
		}
		sLinkList4.add(recvByte);		
		sLinkList4.printAllData(logBuf);
		break;
	case 4:
		if( sLinkList5.length==0 && recvByte!=cStartFlag )				
		{
			return;	
		}
		if(sLinkList5.length==39)
		{
			return;
		}
		sLinkList5.add(recvByte);		
		sLinkList5.printAllData(logBuf);
		break;
	case 5:
		if( sLinkList6.length==0 && recvByte!=cStartFlag )				
		{
			return;	
		}
		if(sLinkList6.length==39)
		{
			return;
		}
		sLinkList6.add(recvByte);		
		sLinkList6.printAllData(logBuf);
		break;
	case 6:
		if( sLinkList7.length==0 && recvByte!=cStartFlag )				
		{
			return;	
		}
		if(sLinkList7.length==39)
		{
			return;
		}
		sLinkList7.add(recvByte);		
		sLinkList7.printAllData(logBuf);
		break;
	case 7:
		if( sLinkList8.length==0 && recvByte!=cStartFlag )				
		{
			return;	
		}
		if(sLinkList8.length==39)
		{
			return;
		}
		sLinkList8.add(recvByte);		
		sLinkList8.printAllData(logBuf);
		break;
	case 8:
		if( sLinkList9.length==0 && recvByte!=cStartFlag )				
		{
			return;	
		}
		if(sLinkList9.length==39)
		{
			return;
		}
		sLinkList9.add(recvByte);		
		sLinkList9.printAllData(logBuf);
		break;
	case 9:
		if( sLinkList10.length==0 && recvByte!=cStartFlag )				
		{
			return;	
		}
		if(sLinkList10.length==39)
		{
			return;
		}
		sLinkList10.add(recvByte);		
		sLinkList10.printAllData(logBuf);
		break;
	case 10:
		if( sLinkList11.length==0 && recvByte!=cStartFlag )				
		{
			return;	
		}
		if(sLinkList11.length==39)
		{
			return;
		}
		sLinkList11.add(recvByte);		
		sLinkList11.printAllData(logBuf);
		break;
	case 11:
		if( sLinkList12.length==0 && recvByte!=cStartFlag )				
		{
			return;	
		}
		if(sLinkList12.length==39)
		{
			return;
		}
		sLinkList12.add(recvByte);		
		sLinkList12.printAllData(logBuf);
		break;
	case 12:
		if( sLinkList13.length==0 && recvByte!=cStartFlag )				
		{
			return;	
		}
		if(sLinkList13.length==39)
		{
			return;
		}
		sLinkList13.add(recvByte);		
		sLinkList13.printAllData(logBuf);
		break;
	case 13:
		if( sLinkList14.length==0 && recvByte!=cStartFlag )				
		{
			return;	
		}
		if(sLinkList14.length==39)
		{
			return;
		}
		sLinkList14.add(recvByte);		
		sLinkList14.printAllData(logBuf);
		break;
	case 14:
		if( sLinkList15.length==0 && recvByte!=cStartFlag )				
		{
			return;	
		}
		if(sLinkList15.length==39)
		{
			return;
		}
		sLinkList15.add(recvByte);		
		sLinkList15.printAllData(logBuf);
		break;
	case 15:
		if( sLinkList16.length==0 && recvByte!=cStartFlag )				
		{
			return;	
		}
		if(sLinkList16.length==39)
		{
			return;
		}
		sLinkList16.add(recvByte);		
		sLinkList16.printAllData(logBuf);
		break;
	}
	
	//输出日志
	WriteLog(LEVEL_DEBUG, logBuf);
	
}
/************************************************************************/
/* 刷新当个LED灯, curCarID:[0,15], loadIndex:[0,59], chnIndex:[0,3]     */
/************************************************************************/
void RefreshOneLED( int curCarID, int loadIndex, int chnIndex)
{
	CWnd*   pW;
	char	tmpStr[256];
	int		iPerLayer=0;//当前层的第几个负载

	int iLedIndex=loadIndex*4+chnIndex;	//这个灯的全局索引号,范围[0,239]
	if (iLedIndex<0 || iLedIndex>239)
	{
		return;
	}
	if(g_curSelTestCar != curCarID)
	{
		return;//如果此定时器不是当前选中区，不需要刷新LED
	}
	//刷新LED颜色，四种颜色
	if (g_AllCar[curCarID].m_CarState == CAR_STATE_TESTING && 
		loadIndex>=g_AllCar[curCarID].testParam.startLoadNum-1 && 
		loadIndex<=g_AllCar[curCarID].testParam.endLoadNum-1 )
	{
		
		if(g_AllCar[curCarID].testParam.loadPerLayer != 0)
		{
			iPerLayer=loadIndex%g_AllCar[curCarID].testParam.loadPerLayer+1;
		}
		switch (g_AllCar[curCarID].m_Load[loadIndex].m_LoadState)
		{
		case LOAD_STATE_SELECT:		//已经选中，灰色
			pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorSelect);
			sprintf(tmpStr,"位置:第%d层第%d个, 拨码:%d, 通道:%d \n状态:已选中\n设定:%s,%0.3f%s, %0.3f%s~%0.3f%s\n实时:功率= \n电流= , 电压=",
				g_AllCar[curCarID].m_Load[loadIndex].m_LayerID,
				iPerLayer,
				loadIndex+1,
				chnIndex+1,
				Led_GetLoadModeName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
				g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetValue,
				Led_GetFixedValue(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
				g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetMin,
				Led_GetUnitName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
				g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetMax,
				Led_GetUnitName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode)
				);
			break;
		case LOAD_STATE_NOLINK:		//未连接，也就是这个负载的四个通道都没有接电源，灰色
			pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorSelect);
			sprintf(tmpStr,"位置:第%d层第%d个, 拨码:%d, 通道:%d \n状态:未连接\n设定:%s,%0.3f%s, %0.3f%s~%0.3f%s\n实时:功率= \n电流= , 电压=",
				g_AllCar[curCarID].m_Load[loadIndex].m_LayerID,
				iPerLayer,
				loadIndex+1,
				chnIndex+1,
				Led_GetLoadModeName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
				g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetValue,
				Led_GetFixedValue(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
				g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetMin,
				Led_GetUnitName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
				g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetMax,
				Led_GetUnitName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode)
				);
			break;
		case LOAD_STATE_NOSETPARAM:	//未设置参数，需要重新设置参数，灰色
			pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorSelect);
			sprintf(tmpStr,"位置:第%d层第%d个, 拨码:%d, 通道:%d \n状态:未设置\n设定:%s,%0.3f%s, %0.3f%s~%0.3f%s\n实时:功率= \n电流= , 电压=",
				g_AllCar[curCarID].m_Load[loadIndex].m_LayerID,
				iPerLayer,
				loadIndex+1,
				chnIndex+1,
				Led_GetLoadModeName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
				g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetValue,
				Led_GetFixedValue(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
				g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetMin,
				Led_GetUnitName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
				g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetMax,
				Led_GetUnitName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode)
				);
			break;
		case LOAD_STATE_WORKING:	//工作中，此种状态可实时获取数据
			{
				char testResult[5]={0};
				switch (g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_ChnState)
				{
				case CHN_STATE_NOLINK:	//未连接，白色
					pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorSelect);//白色
					strcpy(testResult,"未连接");
					break;
				case CHN_STATE_OK:		//合格，绿色
					pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorGreen);//红色		
					strcpy(testResult,"合格");
					break;
				case CHN_STATE_LOWVOL:	//欠压，红色
					pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorRed);//红色						
					strcpy(testResult,"欠压");
					break;
				case CHN_STATE_OVERVOL:	//过压，红色
					pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorRed);//红色						
					strcpy(testResult,"过压");
					break;
				case CHN_STATE_LOWCUR:	//欠流，红色
					pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorRed);//红色						
					strcpy(testResult,"欠流");
					break;
				case CHN_STATE_OVERCUR:	//过流，红色
					pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorRed);//红色
					strcpy(testResult,"过流");
					break;
				}
				
				sprintf(tmpStr,"位置:第%d层第%d个, 拨码:%d, 通道:%d \n状态:%s\n设定:%s,%0.3f%s, %0.3f%s~%0.3f%s\n实时:功率=%0.2f \n电流=%0.3f , 电压=%0.3f",
					g_AllCar[curCarID].m_Load[loadIndex].m_LayerID,
					iPerLayer,
					loadIndex+1,
					chnIndex+1,
					testResult,
					Led_GetLoadModeName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
					g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetValue,
					Led_GetFixedValue(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
					g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetMin,
					Led_GetUnitName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
					g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetMax,
					Led_GetUnitName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
					g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_NowCurrent * g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_NowVoltage,
					g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_NowCurrent,
					g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_NowVoltage
					);
			}
			break;
		case LOAD_STATE_PROTECT:	//温度过高，系统处于保护状态，此时拉载停止
			pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorRed);
			sprintf(tmpStr,"位置:第%d层第%d个, 拨码:%d, 通道:%d \n状态:保护\n设定:%s,%0.3f%s, %0.3f%s~%0.3f%s\n实时:功率= \n电流= , 电压=",
				g_AllCar[curCarID].m_Load[loadIndex].m_LayerID,
				iPerLayer,
				loadIndex+1,
				chnIndex+1,
				Led_GetLoadModeName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
				g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetValue,
				Led_GetFixedValue(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
				g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetMin,
				Led_GetUnitName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
				g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetMax,
				Led_GetUnitName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode)
				);
			break;
			}
			
		}else
		{
			pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorNotSelect);//未选中
			sprintf(tmpStr,"位置:%d,  状态:未选中\n设定\n实时:功率= \n电流= , 电压=",iLedIndex+1);
			
		}
		
		//刷新提示框
		pW=pdlg->GetDlgItem(1400+1+iLedIndex);
		pdlg->m_tooltip.AddTool(pW,tmpStr); 	
		
}
/************************************************************************/
/* 刷新当前测试车的LED灯，curCarID范围[0,15]                            */
/************************************************************************/
void RefreshAllLED( int curCarID)
{
	CWnd*   pW;
	char	tmpStr[256];
	char	testTimeStr[16]={0};
	memset(tmpStr,0,sizeof(tmpStr));
	
	if (curCarID<0 || curCarID>MAX_CAR_COUNT)
	{
		sprintf(tmpStr,"Error, RefreshAllLED() fail curCarID:%d is wrong!",curCarID);
		WriteLog(LEVEL_ERROR, tmpStr);
		return;
	}
	if(pdlg == NULL)
	{
		return;
	}
		
	//循环更新LED灯
	for(int iLedIndex=0; iLedIndex<MAX_LED_COUNT; iLedIndex++)
	{
		int loadIndex = iLedIndex/4;	//此LED所属于的负载号, [0,49]
		int chnIndex = iLedIndex%4;		//此LED所属于的通道号, [0,3]
		int	iPerLayer=0;
		//刷新LED颜色，四种颜色
		if (g_AllCar[curCarID].m_CarState == CAR_STATE_TESTING && 
			loadIndex>=g_AllCar[curCarID].testParam.startLoadNum-1 && 
			loadIndex<=g_AllCar[curCarID].testParam.endLoadNum-1 )
		{
			
			if(g_AllCar[curCarID].testParam.loadPerLayer != 0)
			{
				iPerLayer=loadIndex%g_AllCar[curCarID].testParam.loadPerLayer+1;
			}
			switch (g_AllCar[curCarID].m_Load[loadIndex].m_LoadState)
			{
			case LOAD_STATE_SELECT:		//已经选中，灰色
				pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorSelect);
				sprintf(tmpStr,"位置:第%d层第%d个, 拨码:%d, 通道:%d \n状态:已选中\n设定:%s,%0.3f%s, %0.3f%s~%0.3f%s\n实时:功率= \n电流= , 电压=",
					g_AllCar[curCarID].m_Load[loadIndex].m_LayerID,
					iPerLayer,
					loadIndex+1,
					chnIndex+1,
					Led_GetLoadModeName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
					g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetValue,
					Led_GetFixedValue(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
					g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetMin,
					Led_GetUnitName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
					g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetMax,
					Led_GetUnitName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode)
					);
				break;
			case LOAD_STATE_NOLINK:		//未连接，也就是这个负载的四个通道都没有接电源，灰色
				pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorSelect);
				sprintf(tmpStr,"位置:第%d层第%d个, 拨码:%d, 通道:%d \n状态:未连接\n设定:%s,%0.3f%s, %0.3f%s~%0.3f%s\n实时:功率= \n电流= , 电压=",
					g_AllCar[curCarID].m_Load[loadIndex].m_LayerID,
					iPerLayer,
					loadIndex+1,
					chnIndex+1,
					Led_GetLoadModeName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
					g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetValue,
					Led_GetFixedValue(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
					g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetMin,
					Led_GetUnitName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
					g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetMax,
					Led_GetUnitName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode)
					);
				break;
			case LOAD_STATE_NOSETPARAM:	//未设置参数，需要重新设置参数，灰色
				pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorSelect);
				sprintf(tmpStr,"位置:第%d层第%d个, 拨码:%d, 通道:%d \n状态:未设置\n设定:%s,%0.3f%s, %0.3f%s~%0.3f%s\n实时:功率= \n电流= , 电压=",
					g_AllCar[curCarID].m_Load[loadIndex].m_LayerID,
					iPerLayer,
					loadIndex+1,
					chnIndex+1,
					Led_GetLoadModeName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
					g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetValue,
					Led_GetFixedValue(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
					g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetMin,
					Led_GetUnitName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
					g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetMax,
					Led_GetUnitName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode)
					);
				break;
			case LOAD_STATE_WORKING:	//工作中，此种状态可实时获取数据
				{
					char testResult[5]={0};
					switch (g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_ChnState)
					{
					case CHN_STATE_NOLINK:	//未连接，白色
						pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorSelect);//白色
						strcpy(testResult,"未连接");
						break;
					case CHN_STATE_OK:		//合格，绿色
						pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorGreen);//红色		
						strcpy(testResult,"合格");
						break;
					case CHN_STATE_LOWVOL:	//欠压，红色
						pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorRed);//红色						
						strcpy(testResult,"欠压");
						break;
					case CHN_STATE_OVERVOL:	//过压，红色
						pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorRed);//红色						
						strcpy(testResult,"过压");
						break;
					case CHN_STATE_LOWCUR:	//欠流，红色
						pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorRed);//红色						
						strcpy(testResult,"欠流");
						break;
					case CHN_STATE_OVERCUR:	//过流，红色
						pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorRed);//红色
						strcpy(testResult,"过流");
						break;
					}
					
					sprintf(tmpStr,"位置:第%d层第%d个, 拨码:%d, 通道:%d \n状态:%s\n设定:%s,%0.3f%s, %0.3f%s~%0.3f%s\n实时:功率=%0.2f \n电流=%0.3f , 电压=%0.3f",
						g_AllCar[curCarID].m_Load[loadIndex].m_LayerID,
						iPerLayer,
						loadIndex+1,
						chnIndex+1,
						testResult,
						Led_GetLoadModeName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
						g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetValue,
						Led_GetFixedValue(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
						g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetMin,
						Led_GetUnitName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
						g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetMax,
						Led_GetUnitName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
						g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_NowCurrent * g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_NowVoltage,
						g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_NowCurrent,
						g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_NowVoltage
						);
				}
				break;
			case LOAD_STATE_PROTECT:	//温度过高，系统处于保护状态，此时拉载停止
				pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorRed);
				sprintf(tmpStr,"位置:第%d层第%d个, 拨码:%d, 通道:%d \n状态:保护\n设定:%s,%0.3f%s, %0.3f%s~%0.3f%s\n实时:功率= \n电流= , 电压=",
					g_AllCar[curCarID].m_Load[loadIndex].m_LayerID,
					iPerLayer,
					loadIndex+1,
					chnIndex+1,
					Led_GetLoadModeName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
					g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetValue,
					Led_GetFixedValue(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
					g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetMin,
					Led_GetUnitName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
					g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetMax,
					Led_GetUnitName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode)
					);
				break;
			}
			
		}else
		{
			pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorNotSelect);//未选中
			sprintf(tmpStr,"位置:%d,  状态:未选中\n设定\n实时:功率= \n电流= , 电压=",iLedIndex+1);
			
		}
		
		//刷新提示框
		pW=pdlg->GetDlgItem(1400+1+iLedIndex);
		pdlg->m_tooltip.AddTool(pW,tmpStr); 	
		
	}
}
/************************************************************************/
/*根据测试时间在测试时序中的位置，判断当前测试命令类型，loadNum范围[1,63]   */
/************************************************************************/
int	GetSerialPortCommandType(int carID, int loadNum)
{
	
	if ( carID<0 || carID>MAX_CAR_COUNT )
	{
		TRACE("WARNING: GetSerialPortCommandType() error.. carID: %d\n", carID);
		return -1;
	}
	if( cfg_EnableBeep==1 && g_AllCar[carID].m_ErrorCounter>0 && g_AllCar[carID].m_CMDAlreadyBeepOn==0)
	{
		g_AllCar[carID].m_CMDAlreadyBeepOn=1;
		return LOAD_COMMAND_BEEPON;
	}
	if( cfg_EnableLight==1 && g_AllCar[carID].m_ErrorCounter>0 && g_AllCar[carID].m_CMDAlreadyLightOn==0)
	{
		g_AllCar[carID].m_CMDAlreadyLightOn=1;
		return LOAD_COMMAND_LIGHTON;
	}
	//判断当前测试时间是否到了插入时序中
	for(int testItemIndex=0;testItemIndex< g_AllCar[carID].testItemNum; testItemIndex++)
	{
		int testItem=g_AllCar[carID].pTimeSeires[testItemIndex].testItem;
		long startTime=g_AllCar[carID].pTimeSeires[testItemIndex].startTime;
		long testTime=g_AllCar[carID].pTimeSeires[testItemIndex].testTime;
		
		//插入时序的开始点
		if (g_AllCar[carID].m_TimeCounter*COMMAND_INTERVAL == startTime*1000) 
		{
			if(testItem == TEST_FLOW_PULSEON)
			{
				return LOAD_COMMAND_PULSEON;
				
			}else if (testItem == TEST_FLOW_PULSEOFF)
			{
				return LOAD_COMMAND_PULSEOFF;
				
			}else if (testItem == TEST_FLOW_110V)
			{
				return LOAD_COMMAND_110V;
			}else if (testItem == TEST_FLOW_220V)
			{
				return LOAD_COMMAND_220V;
			}
		}
		/*/插入时序的结束点
		if (g_AllCar[carID].m_TimeCounter*COMMAND_INTERVAL == startTime*1000+testTime*1000) 
		{
			if (testItem == TEST_FLOW_PULSEOFF)//脉开关测试时序结束，下发ON
			{
				return LOAD_COMMAND_PULSEON;				
			}
		}*/
		
		//插入时序的过程中
		if (g_AllCar[carID].m_TimeCounter*COMMAND_INTERVAL > startTime*1000 &&
			g_AllCar[carID].m_TimeCounter*COMMAND_INTERVAL < startTime*1000+testTime*1000 ) 
		{
			return 0;	//时序过程中，不下发Read命令
		}
		
	}	
	
	if ( loadNum<1 || loadNum>=MAX_LOAD_PERCAR )
	{
		TRACE("WARNING: GetSerialPortCommandType() error.. loadNum: %d\n", loadNum);
		return -1;
	}
	//当前负载状态为未设置参数，需要重新设置参数
	if (g_AllCar[carID].m_Load[loadNum-1].m_LoadState == LOAD_STATE_NOSETPARAM )
	{
		g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_SELECT;
		return LOAD_COMMAND_SET;
	}
	//对负载实时读取	
	return LOAD_COMMAND_READ;//不下发命令
	
}
/************************************************************************/
/*当前测试车，当前测试负载号[1,63]						                */
/************************************************************************/
int	GetCurLoadNum(int carID)
{
	
	if ( carID<0 || carID>MAX_CAR_COUNT )
	{
		TRACE("WARNING: GetCurLoadNum() error.. carID: %d\n", carID);
		return -1;
	}
	//超出测试范围,小于开始值
	if (g_AllCar[carID].m_curLoadNum < g_AllCar[carID].testParam.startLoadNum ||
		g_AllCar[carID].m_curLoadNum > g_AllCar[carID].testParam.endLoadNum)
	{
		g_AllCar[carID].m_curLoadNum = g_AllCar[carID].testParam.startLoadNum;	//当前测试负载号
	}
	
	return g_AllCar[carID].m_curLoadNum;
	
}
/************************************************************************/
/* 下发串口测试命令，carID范围[0,15]，curLoadNum范围[1,64], 64广播命令  */
/************************************************************************/
void WriteSerialPortCommand(int carID, int serialPortCommandType, int curLoadNum)
{
	
	char wBuffer[39];
	char logBuf[512];
	int	 setValue;
	char	mainCtrlBoardCommad=0x3f;//主控板拨码开关号
	memset(wBuffer, 0, sizeof(wBuffer));
	memset(logBuf, 0, sizeof(logBuf));
	
	if ( carID<0 || carID>MAX_CAR_COUNT )
	{
		return;
	}
	
	if ( curLoadNum<0 || curLoadNum>MAX_LOAD_PERCAR+1 )
	{
		return;
	}
	
	if ( serialPortCommandType<1  || serialPortCommandType>0x14) 
	{
		return;
	}

	wBuffer[0] = '\x6B';				//开始标记
	wBuffer[1] = curLoadNum;			//当前负载号
	wBuffer[38] = '\x6E';				//结束标记
	
	switch (serialPortCommandType)
	{
	case LOAD_COMMAND_SET:
		sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_SET     ,", carID, curLoadNum);
		wBuffer[2] = '\x0E';			//功能码
		wBuffer[3] = g_AllCar[carID].m_Load[curLoadNum-1].m_LoadMode;//拉载模式
		
		if (0 == g_AllCar[carID].m_Load[curLoadNum-1].m_LoadMode)//恒流
		{
			setValue = (int)(g_AllCar[carID].m_Load[curLoadNum-1].m_Channel[0].m_SetValue*25.5);
		}else if (1 == g_AllCar[carID].m_Load[curLoadNum-1].m_LoadMode)//恒压
		{
			setValue = (int)(g_AllCar[carID].m_Load[curLoadNum-1].m_Channel[0].m_SetValue*2.55);
		}		
		wBuffer[5] = (char)setValue;
		
		
		if (0 == g_AllCar[carID].m_Load[curLoadNum-1].m_LoadMode)//恒流
		{
			setValue = (int)(g_AllCar[carID].m_Load[curLoadNum-1].m_Channel[1].m_SetValue*25.5);
		}else if (1 == g_AllCar[carID].m_Load[curLoadNum-1].m_LoadMode)//恒压
		{
			setValue = (int)(g_AllCar[carID].m_Load[curLoadNum-1].m_Channel[1].m_SetValue*2.55);
		}		
		wBuffer[13] = (char)setValue;
		
		if (0 == g_AllCar[carID].m_Load[curLoadNum-1].m_LoadMode)//恒流
		{
			setValue = (int)(g_AllCar[carID].m_Load[curLoadNum-1].m_Channel[2].m_SetValue*25.5);	
		}else if (1 == g_AllCar[carID].m_Load[curLoadNum-1].m_LoadMode)//恒压
		{
			setValue = (int)(g_AllCar[carID].m_Load[curLoadNum-1].m_Channel[2].m_SetValue*2.55);
		}		
		wBuffer[21] = (char)setValue;
		
		if (0 == g_AllCar[carID].m_Load[curLoadNum-1].m_LoadMode)//恒流
		{
			setValue = (int)(g_AllCar[carID].m_Load[curLoadNum-1].m_Channel[3].m_SetValue*25.5);
		}else if (1 == g_AllCar[carID].m_Load[curLoadNum-1].m_LoadMode)//恒压
		{
			setValue = (int)(g_AllCar[carID].m_Load[curLoadNum-1].m_Channel[3].m_SetValue*2.55);	
		}		
		wBuffer[29] = (char)setValue;
		break;
	case LOAD_COMMAND_READ:
		wBuffer[2] = '\x0D';			//功能码
		sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_READ    ,", carID, curLoadNum);
		break;
	case LOAD_COMMAND_STOP:
		sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_STOP    ,", carID, curLoadNum);
		wBuffer[2] = '\x0C';			//功能码
		break;
	case LOAD_COMMAND_220V:				//220V
		TRACE("WriteSerialPortCommand(), LOAD_COMMAND_220V\n");
		sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_200V    ,", carID,mainCtrlBoardCommad);
		wBuffer[1] = mainCtrlBoardCommad;
		wBuffer[2] = '\x09';			//功能码
		break;
	case LOAD_COMMAND_110V:
		TRACE("WriteSerialPortCommand(), LOAD_COMMAND_110V\n");
		sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_110V    ,", carID,mainCtrlBoardCommad);
		wBuffer[1] = mainCtrlBoardCommad;
		wBuffer[2] = '\x08';			//功能码
		break;
	case LOAD_COMMAND_PULSEON:
		TRACE("WriteSerialPortCommand(), LOAD_COMMAND_PULSEON\n");
		sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_PULSEON ,", carID,mainCtrlBoardCommad);
		wBuffer[1] = mainCtrlBoardCommad;
		wBuffer[2] = '\x06';			//功能码
		break;
	case LOAD_COMMAND_PULSEOFF:
		TRACE("WriteSerialPortCommand(), LOAD_COMMAND_PULSEOFF\n");
		sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_PULSEOFF,", carID,mainCtrlBoardCommad);
		wBuffer[1] = mainCtrlBoardCommad;
		wBuffer[2] = '\x07';			//功能码
		break;
	case LOAD_COMMAND_BEEPON:
		TRACE("WriteSerialPortCommand(), LOAD_COMMAND_BEEPON\n");
		sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_BEEPON  ,", carID,mainCtrlBoardCommad);
		wBuffer[1] = mainCtrlBoardCommad;
		wBuffer[2] = '\x10';			//功能码
		break;
	case LOAD_COMMAND_BEEPOFF:
		TRACE("WriteSerialPortCommand(), LOAD_COMMAND_BEEPOFF\n");
		sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_BEEPOFF ,", carID,mainCtrlBoardCommad);
		wBuffer[1] = mainCtrlBoardCommad;
		wBuffer[2] = '\x11';			//功能码
		break;
	case LOAD_COMMAND_LIGHTON:
		TRACE("WriteSerialPortCommand(), LOAD_COMMAND_LIGHTON\n");
		sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_LIGHTON ,", carID,mainCtrlBoardCommad);
		wBuffer[1] = mainCtrlBoardCommad;
		wBuffer[2] = '\x12';			//功能码
		break;
	case LOAD_COMMAND_LIGHTOFF:
		TRACE("WriteSerialPortCommand(), LOAD_COMMAND_LIGHTOFF\n");
		sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_LIGHTOFF,", carID,mainCtrlBoardCommad);
		wBuffer[1] = mainCtrlBoardCommad;
		wBuffer[2] = '\x13';			//功能码
		break;
	default:
		return;
		
	}
	g_Ports[carID].WriteToPort(wBuffer);
	for (int i=0;i<39;i++)
	{
		sprintf(logBuf,"%s %02X",logBuf,wBuffer[i]);
	}
	WriteLog(LEVEL_DEBUG,logBuf);
	
}
/************************************************************************/
/* 处理串口接收到的数据, carID范围[0,15]                                */
/************************************************************************/
void	DealWithSerialPortData(int carID)				
{
	char logBuf[512];
	char cStartFlag = (char)0x6B;				//命令开始标记
	char cEndFlag	= (char)0x6E;				//命令结束标记
	
	memset(logBuf, 0, sizeof(logBuf));
	
	if ( carID<0 || carID>MAX_CAR_COUNT )
	{
		return;	
	}
	
	
	//只处理A1测试区的情况------------------------------------------------------------
	switch (carID)
	{
	case 0:
		{
			int	existErrorData = FALSE;							//是否存在出错数据
			int startPos = sLinkList1.GetPos(cStartFlag);		//开始位置
			int endPos = sLinkList1.GetPos(cEndFlag);			//结束位置
			
			if (sLinkList1.length < startPos+38)				//单链表长度，串口事件至少39个字节
			{
				return;
			}
			int loadNum = sLinkList1.GetElem(startPos+1);		//负载号
			int funNum = sLinkList1.GetElem(startPos+2);		//功能码
			char endElem = sLinkList1.GetElem(startPos+38);		//结束元素
			if (loadNum<1 || loadNum>=MAX_LOAD_PERCAR || funNum!=0x0D || endElem!= cEndFlag)
			{
				sprintf(logBuf,"Error, Serial Data is wrong! carID:%d, loadNum:%d, funNum:%X, endElem:%X", carID, loadNum, funNum, endElem);
				WriteLog(LEVEL_ERROR,logBuf);
				sLinkList1.DelElemEx(startPos+38);				//从单链表中endPos位置之前的元素
				return;
			}
			
			//底层返回的，负载状态
			int loadState = (int)sLinkList1.GetElem(startPos+37);
			if (loadState == 3)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOLINK;
				sLinkList1.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
				
			}else if (loadState == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_WORKING;
				
				
			}else if (loadState == 2)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_PROTECT;
				sLinkList1.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
				
			}else if (loadState == 0)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOSETPARAM;
				sLinkList1.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
			}
			
			
			//开始分析数据了---------------------------------------------------------
			BYTE A1,A2,B1,B2,D1,D2,E1,E2;
			int	 A,B,D,E;
			float C,F; 
			int iChnIndex;//通道ID,范围[0,3]
			
			iChnIndex = 0;
			if(g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError==0)
			{
				//通道1,电压计算
				A1 = sLinkList1.GetElem(startPos+3);			
				A2 = sLinkList1.GetElem(startPos+4);
				B1 = sLinkList1.GetElem(startPos+19);			
				B2 = sLinkList1.GetElem(startPos+20);
				A = A1*256 + A2;
				B = B1*256 + B2;
				if(B==0)
				{
					C=0;
				}else
				{
					
					C = (float)A/1024;
					C = C/B;
					C = (C/0.0174)*5;			
					
				}
				
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage = C;
				
				
				
				//通道1,电流计算
				D1 = sLinkList1.GetElem(startPos+5);			
				D2 = sLinkList1.GetElem(startPos+6);
				E1 = sLinkList1.GetElem(startPos+21);			
				E2 = sLinkList1.GetElem(startPos+22);
				D = D1*256 + D2;
				E = E1*256 + E2;
				if( E==0 )
				{
					F=0;			
				}else
				{
					F = (float)D/1024;
					F = F/E;
					F = (F/11)*1000;
					
				}
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent = F;
				
				
				//通道1, 状态, 错误统计			
				if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//恒流
				{	
					if( C == 0 && F== 0)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
						
					}else if ( g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage < g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin )
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;	//欠压
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else if ( g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage > g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax )
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;	//过压
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else if ( g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError  )
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;	//过流
						g_AllCar[carID].m_ErrorCounter++;	
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else if ( g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError  )
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;	//欠流
						g_AllCar[carID].m_ErrorCounter++;	
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;		//合格
					}
				}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//恒压
				{
					if( C == 0 && F== 0)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
						
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;	
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;	
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//欠压
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;	
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
					}
				}
			}
			iChnIndex=1;
			if(g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError==0)
			{
				
				//通道2,电压计算
				A1 = sLinkList1.GetElem(startPos+7);			
				A2 = sLinkList1.GetElem(startPos+8);
				B1 = sLinkList1.GetElem(startPos+23);			
				B2 = sLinkList1.GetElem(startPos+24);
				A = A1*256 + A2;
				B = B1*256 + B2;
				if(B==0)
				{
					C=0;
				}else
				{
					C = (float)A/1024;
					C = C/B;
					C = (C/0.0174)*5;			
					
				}
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage = C;
				
				//通道2,电流计算
				D1 = sLinkList1.GetElem(startPos+9);			
				D2 = sLinkList1.GetElem(startPos+10);
				E1 = sLinkList1.GetElem(startPos+25);			
				E2 = sLinkList1.GetElem(startPos+26);
				D = D1*256 + D2;
				E = E1*256 + E2;
				if( E==0 )
				{
					F=0;			
				}else
				{
					F = (float)D/1024;
					F = F/E;
					F = (F/11)*1000;
					
				}	
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent = F;
				//通道2, 状态, 错误统计			
				if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//恒流
				{	
					if( C == 0 && F== 0)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
						
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//欠压
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;	
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
					}
				}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//恒压
				{
					if( C == 0 && F== 0)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
						
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;	
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//欠压
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;	
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
					}
				}
			}
			iChnIndex=2;
			if(g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError==0)
			{
				
				//通道3,电压计算
				A1 = sLinkList1.GetElem(startPos+11);			
				A2 = sLinkList1.GetElem(startPos+12);
				B1 = sLinkList1.GetElem(startPos+27);			
				B2 = sLinkList1.GetElem(startPos+28);
				A = A1*256 + A2;
				B = B1*256 + B2;
				if(B==0)
				{
					C=0;
				}else
				{
					C = (float)A/1024;
					C = C/B;
					C = (C/0.0174)*5;			
					
				}
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage = C;
				
				//通道3,电流计算
				D1 = sLinkList1.GetElem(startPos+13);			
				D2 = sLinkList1.GetElem(startPos+14);
				E1 = sLinkList1.GetElem(startPos+29);			
				E2 = sLinkList1.GetElem(startPos+30);
				D = D1*256 + D2;
				E = E1*256 + E2;
				if( E==0 )
				{
					F=0;			
				}else
				{
					F = (float)D/1024;
					F = F/E;
					F = (F/11)*1000;
					
				}
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent = F;
				//通道3, 状态, 错误统计			
				if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//恒流
				{	
					if( C == 0 && F== 0)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
						
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//欠压
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;	
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;	
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;	
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
					}
				}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//恒压
				{
					if( C == 0 && F== 0)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
						
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//欠压
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
					}
				}
			}
			iChnIndex=3;
			if(g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError==0)
			{
				
				//通道4,电压计算
				A1 = sLinkList1.GetElem(startPos+15);			
				A2 = sLinkList1.GetElem(startPos+16);
				B1 = sLinkList1.GetElem(startPos+31);			
				B2 = sLinkList1.GetElem(startPos+32);
				A = A1*256 + A2;
				B = B1*256 + B2;
				if(B==0)
				{
					C=0;
				}else
				{
					C = (float)A/1024;
					C = C/B;
					C = (C/0.0174)*5;			
					
				}
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage = C;
				
				//通道4,电流计算
				D1 = sLinkList1.GetElem(startPos+17);			
				D2 = sLinkList1.GetElem(startPos+18);
				E1 = sLinkList1.GetElem(startPos+33);			
				E2 = sLinkList1.GetElem(startPos+34);
				D = D1*256 + D2;
				E = E1*256 + E2;
				if( E==0 )
				{
					F=0;			
				}else
				{
					F = (float)D/1024;
					F = F/E;
					F = (F/11)*1000;
					
				}
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent = F;
				//通道4, 状态, 错误统计			
				if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//恒流
				{	
					if( C == 0 && F== 0)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
						
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//欠压
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;	
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
					}
				}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//恒压
				{
					if( C == 0 && F== 0)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
						
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;	
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;	
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
					{
						
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//欠压
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;	
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//是否存在出错数据
					}else
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
					}
				}
			}
			
			sLinkList1.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
			//出错数据，写入测试数据文件
			if ( g_AllCar[carID].testParam.isSaveData == 1 && TRUE == existErrorData )
			{
				g_data[carID].WriteData_Ttime("");
				sprintf(logBuf,"%d,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s\n",
					loadNum,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_ChnState));
				
				g_data[carID].WriteData(logBuf);			
				
			}
		}
		break;
	case 1:
		{
			int	existErrorData = FALSE;							//是否存在出错数据
			int startPos = sLinkList2.GetPos(cStartFlag);		//开始位置
			int endPos = sLinkList2.GetPos(cEndFlag);			//结束位置
			
			if (sLinkList2.length < startPos+38)				//单链表长度，串口事件至少39个字节
			{
				return;
			}
			int loadNum = sLinkList2.GetElem(startPos+1);		//负载号
			int funNum = sLinkList2.GetElem(startPos+2);		//功能码
			char endElem = sLinkList2.GetElem(startPos+38);		//结束元素
			if (loadNum<1 || loadNum>=MAX_LOAD_PERCAR || funNum!=0x0D || endElem!= cEndFlag)
			{
				sprintf(logBuf,"Error, Serial Data is wrong! carID:%d, loadNum:%d, funNum:%X, endElem:%X", carID, loadNum, funNum, endElem);
				WriteLog(LEVEL_ERROR,logBuf);
				sLinkList2.DelElemEx(startPos+38);				//从单链表中endPos位置之前的元素
				return;
			}
			
			//底层返回的，负载状态
			int loadState = (int)sLinkList2.GetElem(startPos+37);
			if (loadState == 3)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOLINK;
				sLinkList2.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
				
			}else if (loadState == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_WORKING;
				
				
			}else if (loadState == 2)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_PROTECT;
				sLinkList2.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
				
			}else if (loadState == 0)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOSETPARAM;
				sLinkList2.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
			}
			
			//开始分析数据了---------------------------------------------------------
			BYTE A1,A2,B1,B2,D1,D2,E1,E2;
			int	 A,B,D,E;
			float C,F; 
			int iChnIndex;//通道ID,范围[0,3]
			//通道[0,3]的计算
			for (iChnIndex =0; iChnIndex<4; iChnIndex++)
			{
				if(g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError==0)
				{
					A1 = sLinkList2.GetElem((iChnIndex+1)*4-1+startPos);			
					A2 = sLinkList2.GetElem((iChnIndex+1)*4-0+startPos);
					B1 = sLinkList2.GetElem((iChnIndex+1)*4+15+startPos);			
					B2 = sLinkList2.GetElem((iChnIndex+1)*4+16+startPos);
					A = A1*256 + A2;
					B = B1*256 + B2;
					if(B==0)
					{
						C=0;
					}else
					{						
						C = (float)A/1024;
						C = C/B;
						C = (C/0.0174)*5;								
					}
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage = C;
					
					//通道,电流计算
					D1 = sLinkList2.GetElem((iChnIndex+1)*4+1+startPos);			
					D2 = sLinkList2.GetElem((iChnIndex+1)*4+2+startPos);
					E1 = sLinkList2.GetElem((iChnIndex+1)*4+17+startPos);			
					E2 = sLinkList2.GetElem((iChnIndex+1)*4+18+startPos);
					D = D1*256 + D2;
					E = E1*256 + E2;
					if( E==0 )
					{
						F=0;			
					}else
					{
						F = (float)D/1024;
						F = F/E;
						F = (F/11)*1000;						
					}			
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent = F;
					
					//通道状态, 错误统计			
					if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//恒流
					{	
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//欠压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
						}
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//恒压
					{
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//欠压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
						}
					}
				}
			}			
			sLinkList2.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
			//出错数据，写入测试数据文件
			if ( g_AllCar[carID].testParam.isSaveData == 1 && TRUE == existErrorData )
			{
				g_data[carID].WriteData_Ttime("");
				sprintf(logBuf,"%d,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s\n",
					loadNum,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_ChnState));
				
				g_data[carID].WriteData(logBuf);			
				
			}
		}
		break;
	case 2:
		{
			int	existErrorData = FALSE;							//是否存在出错数据
			int startPos = sLinkList3.GetPos(cStartFlag);		//开始位置
			int endPos = sLinkList3.GetPos(cEndFlag);			//结束位置
			
			if (sLinkList3.length < startPos+38)				//单链表长度，串口事件至少39个字节
			{
				return;
			}
			int loadNum = sLinkList3.GetElem(startPos+1);		//负载号
			int funNum = sLinkList3.GetElem(startPos+2);		//功能码
			char endElem = sLinkList3.GetElem(startPos+38);		//结束元素
			if (loadNum<1 || loadNum>=MAX_LOAD_PERCAR || funNum!=0x0D || endElem!= cEndFlag)
			{
				sprintf(logBuf,"Error, Serial Data is wrong! carID:%d, loadNum:%d, funNum:%X, endElem:%X", carID, loadNum, funNum, endElem);
				WriteLog(LEVEL_ERROR,logBuf);
				sLinkList3.DelElemEx(startPos+38);				//从单链表中endPos位置之前的元素
				return;
			}
			
			//底层返回的，负载状态
			int loadState = (int)sLinkList3.GetElem(startPos+37);
			if (loadState == 3)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOLINK;
				sLinkList3.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
				
			}else if (loadState == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_WORKING;
				
				
			}else if (loadState == 2)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_PROTECT;
				sLinkList3.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
				
			}else if (loadState == 0)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOSETPARAM;
				sLinkList3.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
			}
			
			//开始分析数据了---------------------------------------------------------
			BYTE A1,A2,B1,B2,D1,D2,E1,E2;
			int	 A,B,D,E;
			float C,F; 
			int iChnIndex;//通道ID,范围[0,3]
			//通道[0,3]的计算
			for (iChnIndex =0; iChnIndex<4; iChnIndex++)
			{
				if(g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError==0)
				{
					A1 = sLinkList3.GetElem((iChnIndex+1)*4-1+startPos);			
					A2 = sLinkList3.GetElem((iChnIndex+1)*4-0+startPos);
					B1 = sLinkList3.GetElem((iChnIndex+1)*4+15+startPos);			
					B2 = sLinkList3.GetElem((iChnIndex+1)*4+16+startPos);
					A = A1*256 + A2;
					B = B1*256 + B2;
					if(B==0)
					{
						C=0;
					}else
					{						
						C = (float)A/1024;
						C = C/B;
						C = (C/0.0174)*5;								
					}
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage = C;
					
					//通道,电流计算
					D1 = sLinkList3.GetElem((iChnIndex+1)*4+1+startPos);			
					D2 = sLinkList3.GetElem((iChnIndex+1)*4+2+startPos);
					E1 = sLinkList3.GetElem((iChnIndex+1)*4+17+startPos);			
					E2 = sLinkList3.GetElem((iChnIndex+1)*4+18+startPos);
					D = D1*256 + D2;
					E = E1*256 + E2;
					if( E==0 )
					{
						F=0;			
					}else
					{
						F = (float)D/1024;
						F = F/E;
						F = (F/11)*1000;						
					}			
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent = F;
					
					//通道状态, 错误统计			
					if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//恒流
					{	
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//欠压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
						}
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//恒压
					{
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//欠压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
						}
					}
				}
			}			
			sLinkList3.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
			//出错数据，写入测试数据文件
			if ( g_AllCar[carID].testParam.isSaveData == 1 && TRUE == existErrorData )
			{
				g_data[carID].WriteData_Ttime("");
				sprintf(logBuf,"%d,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s\n",
					loadNum,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_ChnState));
				
				g_data[carID].WriteData(logBuf);			
				
			}
		}
		break;
	case 3:
		{
			int	existErrorData = FALSE;							//是否存在出错数据
			int startPos = sLinkList4.GetPos(cStartFlag);		//开始位置
			int endPos = sLinkList4.GetPos(cEndFlag);			//结束位置
			
			if (sLinkList4.length < startPos+38)				//单链表长度，串口事件至少39个字节
			{
				return;
			}
			int loadNum = sLinkList4.GetElem(startPos+1);		//负载号
			int funNum = sLinkList4.GetElem(startPos+2);		//功能码
			char endElem = sLinkList4.GetElem(startPos+38);		//结束元素
			if (loadNum<1 || loadNum>=MAX_LOAD_PERCAR || funNum!=0x0D || endElem!= cEndFlag)
			{
				sprintf(logBuf,"Error, Serial Data is wrong! carID:%d, loadNum:%d, funNum:%X, endElem:%X", carID, loadNum, funNum, endElem);
				WriteLog(LEVEL_ERROR,logBuf);
				sLinkList4.DelElemEx(startPos+38);				//从单链表中endPos位置之前的元素
				return;
			}
			
			//底层返回的，负载状态
			int loadState = (int)sLinkList4.GetElem(startPos+37);
			if (loadState == 3)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOLINK;
				sLinkList4.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
				
			}else if (loadState == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_WORKING;
				
				
			}else if (loadState == 2)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_PROTECT;
				sLinkList4.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
				
			}else if (loadState == 0)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOSETPARAM;
				sLinkList4.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
			}
			
			//开始分析数据了---------------------------------------------------------
			BYTE A1,A2,B1,B2,D1,D2,E1,E2;
			int	 A,B,D,E;
			float C,F; 
			int iChnIndex;//通道ID,范围[0,3]
			//通道[0,3]的计算
			for (iChnIndex =0; iChnIndex<4; iChnIndex++)
			{
				if(g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError==0)
				{
					A1 = sLinkList4.GetElem((iChnIndex+1)*4-1+startPos);			
					A2 = sLinkList4.GetElem((iChnIndex+1)*4-0+startPos);
					B1 = sLinkList4.GetElem((iChnIndex+1)*4+15+startPos);			
					B2 = sLinkList4.GetElem((iChnIndex+1)*4+16+startPos);
					A = A1*256 + A2;
					B = B1*256 + B2;
					if(B==0)
					{
						C=0;
					}else
					{						
						C = (float)A/1024;
						C = C/B;
						C = (C/0.0174)*5;								
					}
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage = C;
					
					//通道,电流计算
					D1 = sLinkList4.GetElem((iChnIndex+1)*4+1+startPos);			
					D2 = sLinkList4.GetElem((iChnIndex+1)*4+2+startPos);
					E1 = sLinkList4.GetElem((iChnIndex+1)*4+17+startPos);			
					E2 = sLinkList4.GetElem((iChnIndex+1)*4+18+startPos);
					D = D1*256 + D2;
					E = E1*256 + E2;
					if( E==0 )
					{
						F=0;			
					}else
					{
						F = (float)D/1024;
						F = F/E;
						F = (F/11)*1000;						
					}			
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent = F;
					
					//通道状态, 错误统计			
					if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//恒流
					{	
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//欠压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
						}
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//恒压
					{
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//欠压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
						}
					}
					
				}
			}			
			sLinkList4.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
			//出错数据，写入测试数据文件
			if ( g_AllCar[carID].testParam.isSaveData == 1 && TRUE == existErrorData )
			{
				g_data[carID].WriteData_Ttime("");
				sprintf(logBuf,"%d,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s\n",
					loadNum,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_ChnState));
				
				g_data[carID].WriteData(logBuf);			
				
			}
		}
		break;
	case 4:
		{
			int	existErrorData = FALSE;							//是否存在出错数据
			int startPos = sLinkList5.GetPos(cStartFlag);		//开始位置
			int endPos = sLinkList5.GetPos(cEndFlag);			//结束位置
			
			if (sLinkList5.length < startPos+38)				//单链表长度，串口事件至少39个字节
			{
				return;
			}
			int loadNum = sLinkList5.GetElem(startPos+1);		//负载号
			int funNum = sLinkList5.GetElem(startPos+2);		//功能码
			char endElem = sLinkList5.GetElem(startPos+38);		//结束元素
			if (loadNum<1 || loadNum>=MAX_LOAD_PERCAR || funNum!=0x0D || endElem!= cEndFlag)
			{
				sprintf(logBuf,"Error, Serial Data is wrong! carID:%d, loadNum:%d, funNum:%X, endElem:%X", carID, loadNum, funNum, endElem);
				WriteLog(LEVEL_ERROR,logBuf);
				sLinkList5.DelElemEx(startPos+38);				//从单链表中endPos位置之前的元素
				return;
			}
			
			//底层返回的，负载状态
			int loadState = (int)sLinkList5.GetElem(startPos+37);
			if (loadState == 3)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOLINK;
				sLinkList5.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
				
			}else if (loadState == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_WORKING;
				
				
			}else if (loadState == 2)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_PROTECT;
				sLinkList5.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
				
			}else if (loadState == 0)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOSETPARAM;
				sLinkList5.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
			}
			
			//开始分析数据了---------------------------------------------------------
			BYTE A1,A2,B1,B2,D1,D2,E1,E2;
			int	 A,B,D,E;
			float C,F; 
			int iChnIndex;//通道ID,范围[0,3]
			//通道[0,3]的计算
			for (iChnIndex =0; iChnIndex<4; iChnIndex++)
			{
				if(g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError==0)
				{
					A1 = sLinkList5.GetElem((iChnIndex+1)*4-1+startPos);			
					A2 = sLinkList5.GetElem((iChnIndex+1)*4-0+startPos);
					B1 = sLinkList5.GetElem((iChnIndex+1)*4+15+startPos);			
					B2 = sLinkList5.GetElem((iChnIndex+1)*4+16+startPos);
					A = A1*256 + A2;
					B = B1*256 + B2;
					if(B==0)
					{
						C=0;
					}else
					{						
						C = (float)A/1024;
						C = C/B;
						C = (C/0.0174)*5;								
					}
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage = C;
					
					//通道,电流计算
					D1 = sLinkList5.GetElem((iChnIndex+1)*4+1+startPos);			
					D2 = sLinkList5.GetElem((iChnIndex+1)*4+2+startPos);
					E1 = sLinkList5.GetElem((iChnIndex+1)*4+17+startPos);			
					E2 = sLinkList5.GetElem((iChnIndex+1)*4+18+startPos);
					D = D1*256 + D2;
					E = E1*256 + E2;
					if( E==0 )
					{
						F=0;			
					}else
					{
						F = (float)D/1024;
						F = F/E;
						F = (F/11)*1000;						
					}			
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent = F;
					
					//通道状态, 错误统计			
					if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//恒流
					{	
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//欠压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
						}
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//恒压
					{
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//欠压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
						}
					}
					
				}
			}			
			sLinkList5.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
			//出错数据，写入测试数据文件
			if ( g_AllCar[carID].testParam.isSaveData == 1 && TRUE == existErrorData )
			{
				g_data[carID].WriteData_Ttime("");
				sprintf(logBuf,"%d,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s\n",
					loadNum,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_ChnState));
				
				g_data[carID].WriteData(logBuf);			
				
			}
		}
		break;
	case 5:
		{
			int	existErrorData = FALSE;							//是否存在出错数据
			int startPos = sLinkList6.GetPos(cStartFlag);		//开始位置
			int endPos = sLinkList6.GetPos(cEndFlag);			//结束位置
			
			if (sLinkList6.length < startPos+38)				//单链表长度，串口事件至少39个字节
			{
				return;
			}
			int loadNum = sLinkList6.GetElem(startPos+1);		//负载号
			int funNum = sLinkList6.GetElem(startPos+2);		//功能码
			char endElem = sLinkList6.GetElem(startPos+38);		//结束元素
			if (loadNum<1 || loadNum>=MAX_LOAD_PERCAR || funNum!=0x0D || endElem!= cEndFlag)
			{
				sprintf(logBuf,"Error, Serial Data is wrong! carID:%d, loadNum:%d, funNum:%X, endElem:%X", carID, loadNum, funNum, endElem);
				WriteLog(LEVEL_ERROR,logBuf);
				sLinkList6.DelElemEx(startPos+38);				//从单链表中endPos位置之前的元素
				return;
			}
			
			//底层返回的，负载状态
			int loadState = (int)sLinkList6.GetElem(startPos+37);
			if (loadState == 3)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOLINK;
				sLinkList6.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
				
			}else if (loadState == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_WORKING;
				
				
			}else if (loadState == 2)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_PROTECT;
				sLinkList6.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
				
			}else if (loadState == 0)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOSETPARAM;
				sLinkList6.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
			}
			
			//开始分析数据了---------------------------------------------------------
			BYTE A1,A2,B1,B2,D1,D2,E1,E2;
			int	 A,B,D,E;
			float C,F; 
			int iChnIndex;//通道ID,范围[0,3]
			//通道[0,3]的计算
			for (iChnIndex =0; iChnIndex<4; iChnIndex++)
			{
				if(g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError==0)
				{
					A1 = sLinkList6.GetElem((iChnIndex+1)*4-1+startPos);			
					A2 = sLinkList6.GetElem((iChnIndex+1)*4-0+startPos);
					B1 = sLinkList6.GetElem((iChnIndex+1)*4+15+startPos);			
					B2 = sLinkList6.GetElem((iChnIndex+1)*4+16+startPos);
					A = A1*256 + A2;
					B = B1*256 + B2;
					if(B==0)
					{
						C=0;
					}else
					{						
						C = (float)A/1024;
						C = C/B;
						C = (C/0.0174)*5;								
					}
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage = C;
					
					//通道,电流计算
					D1 = sLinkList6.GetElem((iChnIndex+1)*4+1+startPos);			
					D2 = sLinkList6.GetElem((iChnIndex+1)*4+2+startPos);
					E1 = sLinkList6.GetElem((iChnIndex+1)*4+17+startPos);			
					E2 = sLinkList6.GetElem((iChnIndex+1)*4+18+startPos);
					D = D1*256 + D2;
					E = E1*256 + E2;
					if( E==0 )
					{
						F=0;			
					}else
					{
						F = (float)D/1024;
						F = F/E;
						F = (F/11)*1000;						
					}			
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent = F;
					
					//通道状态, 错误统计			
					if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//恒流
					{	
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//欠压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
						}
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//恒压
					{
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//欠压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
						}
					}
					
				}
			}			
			sLinkList6.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
			//出错数据，写入测试数据文件
			if ( g_AllCar[carID].testParam.isSaveData == 1 && TRUE == existErrorData )
			{
				g_data[carID].WriteData_Ttime("");
				sprintf(logBuf,"%d,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s\n",
					loadNum,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_ChnState));
				
				g_data[carID].WriteData(logBuf);			
				
			}
		}
		break;
	case 6:
		{
			int	existErrorData = FALSE;							//是否存在出错数据
			int startPos = sLinkList7.GetPos(cStartFlag);		//开始位置
			int endPos = sLinkList7.GetPos(cEndFlag);			//结束位置
			
			if (sLinkList7.length < startPos+38)				//单链表长度，串口事件至少39个字节
			{
				return;
			}
			int loadNum = sLinkList7.GetElem(startPos+1);		//负载号
			int funNum = sLinkList7.GetElem(startPos+2);		//功能码
			char endElem = sLinkList7.GetElem(startPos+38);		//结束元素
			if (loadNum<1 || loadNum>=MAX_LOAD_PERCAR || funNum!=0x0D || endElem!= cEndFlag)
			{
				sprintf(logBuf,"Error, Serial Data is wrong! carID:%d, loadNum:%d, funNum:%X, endElem:%X", carID, loadNum, funNum, endElem);
				WriteLog(LEVEL_ERROR,logBuf);
				sLinkList7.DelElemEx(startPos+38);				//从单链表中endPos位置之前的元素
				return;
			}
			
			//底层返回的，负载状态
			int loadState = (int)sLinkList7.GetElem(startPos+37);
			if (loadState == 3)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOLINK;
				sLinkList7.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
				
			}else if (loadState == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_WORKING;
				
				
			}else if (loadState == 2)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_PROTECT;
				sLinkList7.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
				
			}else if (loadState == 0)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOSETPARAM;
				sLinkList7.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
			}
			
			//开始分析数据了---------------------------------------------------------
			BYTE A1,A2,B1,B2,D1,D2,E1,E2;
			int	 A,B,D,E;
			float C,F; 
			int iChnIndex;//通道ID,范围[0,3]
			//通道[0,3]的计算
			for (iChnIndex =0; iChnIndex<4; iChnIndex++)
			{
				if(g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError==0)
				{
					A1 = sLinkList7.GetElem((iChnIndex+1)*4-1+startPos);			
					A2 = sLinkList7.GetElem((iChnIndex+1)*4-0+startPos);
					B1 = sLinkList7.GetElem((iChnIndex+1)*4+15+startPos);			
					B2 = sLinkList7.GetElem((iChnIndex+1)*4+16+startPos);
					A = A1*256 + A2;
					B = B1*256 + B2;
					if(B==0)
					{
						C=0;
					}else
					{						
						C = (float)A/1024;
						C = C/B;
						C = (C/0.0174)*5;								
					}
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage = C;
					
					//通道,电流计算
					D1 = sLinkList7.GetElem((iChnIndex+1)*4+1+startPos);			
					D2 = sLinkList7.GetElem((iChnIndex+1)*4+2+startPos);
					E1 = sLinkList7.GetElem((iChnIndex+1)*4+17+startPos);			
					E2 = sLinkList7.GetElem((iChnIndex+1)*4+18+startPos);
					D = D1*256 + D2;
					E = E1*256 + E2;
					if( E==0 )
					{
						F=0;			
					}else
					{
						F = (float)D/1024;
						F = F/E;
						F = (F/11)*1000;						
					}			
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent = F;
					
					//通道状态, 错误统计			
					if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//恒流
					{	
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//欠压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
						}
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//恒压
					{
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//欠压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
						}
					}
					
				}
			}			
			sLinkList7.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
			//出错数据，写入测试数据文件
			if ( g_AllCar[carID].testParam.isSaveData == 1 && TRUE == existErrorData )
			{
				g_data[carID].WriteData_Ttime("");
				sprintf(logBuf,"%d,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s\n",
					loadNum,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_ChnState));
				
				g_data[carID].WriteData(logBuf);			
				
			}
		}
		break;
	case 7:
		{
			int	existErrorData = FALSE;							//是否存在出错数据
			int startPos = sLinkList8.GetPos(cStartFlag);		//开始位置
			int endPos = sLinkList8.GetPos(cEndFlag);			//结束位置
			
			if (sLinkList8.length < startPos+38)				//单链表长度，串口事件至少39个字节
			{
				return;
			}
			int loadNum = sLinkList8.GetElem(startPos+1);		//负载号
			int funNum = sLinkList8.GetElem(startPos+2);		//功能码
			char endElem = sLinkList8.GetElem(startPos+38);		//结束元素
			if (loadNum<1 || loadNum>=MAX_LOAD_PERCAR || funNum!=0x0D || endElem!= cEndFlag)
			{
				sprintf(logBuf,"Error, Serial Data is wrong! carID:%d, loadNum:%d, funNum:%X, endElem:%X", carID, loadNum, funNum, endElem);
				WriteLog(LEVEL_ERROR,logBuf);
				sLinkList8.DelElemEx(startPos+38);				//从单链表中endPos位置之前的元素
				return;
			}
			
			//底层返回的，负载状态
			int loadState = (int)sLinkList8.GetElem(startPos+37);
			if (loadState == 3)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOLINK;
				sLinkList8.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
				
			}else if (loadState == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_WORKING;
				
				
			}else if (loadState == 2)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_PROTECT;
				sLinkList8.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
				
			}else if (loadState == 0)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOSETPARAM;
				sLinkList8.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
			}
			
			//开始分析数据了---------------------------------------------------------
			BYTE A1,A2,B1,B2,D1,D2,E1,E2;
			int	 A,B,D,E;
			float C,F; 
			int iChnIndex;//通道ID,范围[0,3]
			//通道[0,3]的计算
			for (iChnIndex =0; iChnIndex<4; iChnIndex++)
			{
				if(g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError==0)
				{
					A1 = sLinkList8.GetElem((iChnIndex+1)*4-1+startPos);			
					A2 = sLinkList8.GetElem((iChnIndex+1)*4-0+startPos);
					B1 = sLinkList8.GetElem((iChnIndex+1)*4+15+startPos);			
					B2 = sLinkList8.GetElem((iChnIndex+1)*4+16+startPos);
					A = A1*256 + A2;
					B = B1*256 + B2;
					if(B==0)
					{
						C=0;
					}else
					{						
						C = (float)A/1024;
						C = C/B;
						C = (C/0.0174)*5;								
					}
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage = C;
					
					//通道,电流计算
					D1 = sLinkList8.GetElem((iChnIndex+1)*4+1+startPos);			
					D2 = sLinkList8.GetElem((iChnIndex+1)*4+2+startPos);
					E1 = sLinkList8.GetElem((iChnIndex+1)*4+17+startPos);			
					E2 = sLinkList8.GetElem((iChnIndex+1)*4+18+startPos);
					D = D1*256 + D2;
					E = E1*256 + E2;
					if( E==0 )
					{
						F=0;			
					}else
					{
						F = (float)D/1024;
						F = F/E;
						F = (F/11)*1000;						
					}			
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent = F;
					
					//通道状态, 错误统计			
					if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//恒流
					{	
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//欠压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
						}
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//恒压
					{
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//欠压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
						}
					}
					
				}
			}			
			sLinkList8.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
			//出错数据，写入测试数据文件
			if ( g_AllCar[carID].testParam.isSaveData == 1 && TRUE == existErrorData )
			{
				g_data[carID].WriteData_Ttime("");
				sprintf(logBuf,"%d,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s\n",
					loadNum,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_ChnState));
				
				g_data[carID].WriteData(logBuf);			
				
			}
		}
		break;
	case 8:
		{
			int	existErrorData = FALSE;							//是否存在出错数据
			int startPos = sLinkList9.GetPos(cStartFlag);		//开始位置
			int endPos = sLinkList9.GetPos(cEndFlag);			//结束位置
			
			if (sLinkList9.length < startPos+38)				//单链表长度，串口事件至少39个字节
			{
				return;
			}
			int loadNum = sLinkList9.GetElem(startPos+1);		//负载号
			int funNum = sLinkList9.GetElem(startPos+2);		//功能码
			char endElem = sLinkList9.GetElem(startPos+38);		//结束元素
			if (loadNum<1 || loadNum>=MAX_LOAD_PERCAR || funNum!=0x0D || endElem!= cEndFlag)
			{
				sprintf(logBuf,"Error, Serial Data is wrong! carID:%d, loadNum:%d, funNum:%X, endElem:%X", carID, loadNum, funNum, endElem);
				WriteLog(LEVEL_ERROR,logBuf);
				sLinkList9.DelElemEx(startPos+38);				//从单链表中endPos位置之前的元素
				return;
			}
			
			//底层返回的，负载状态
			int loadState = (int)sLinkList9.GetElem(startPos+37);
			if (loadState == 3)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOLINK;
				sLinkList9.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
				
			}else if (loadState == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_WORKING;
				
				
			}else if (loadState == 2)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_PROTECT;
				sLinkList9.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
				
			}else if (loadState == 0)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOSETPARAM;
				sLinkList9.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
			}
			
			//开始分析数据了---------------------------------------------------------
			BYTE A1,A2,B1,B2,D1,D2,E1,E2;
			int	 A,B,D,E;
			float C,F; 
			int iChnIndex;//通道ID,范围[0,3]
			//通道[0,3]的计算
			for (iChnIndex =0; iChnIndex<4; iChnIndex++)
			{
				if(g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError==0)
				{
					A1 = sLinkList9.GetElem((iChnIndex+1)*4-1+startPos);			
					A2 = sLinkList9.GetElem((iChnIndex+1)*4-0+startPos);
					B1 = sLinkList9.GetElem((iChnIndex+1)*4+15+startPos);			
					B2 = sLinkList9.GetElem((iChnIndex+1)*4+16+startPos);
					A = A1*256 + A2;
					B = B1*256 + B2;
					if(B==0)
					{
						C=0;
					}else
					{						
						C = (float)A/1024;
						C = C/B;
						C = (C/0.0174)*5;								
					}
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage = C;
					
					//通道,电流计算
					D1 = sLinkList9.GetElem((iChnIndex+1)*4+1+startPos);			
					D2 = sLinkList9.GetElem((iChnIndex+1)*4+2+startPos);
					E1 = sLinkList9.GetElem((iChnIndex+1)*4+17+startPos);			
					E2 = sLinkList9.GetElem((iChnIndex+1)*4+18+startPos);
					D = D1*256 + D2;
					E = E1*256 + E2;
					if( E==0 )
					{
						F=0;			
					}else
					{
						F = (float)D/1024;
						F = F/E;
						F = (F/11)*1000;						
					}			
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent = F;
					
					//通道状态, 错误统计			
					if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//恒流
					{	
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//欠压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
						}
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//恒压
					{
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//欠压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
						}
					}
					
				}
			}			
			sLinkList9.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
			//出错数据，写入测试数据文件
			if ( g_AllCar[carID].testParam.isSaveData == 1 && TRUE == existErrorData )
			{
				g_data[carID].WriteData_Ttime("");
				sprintf(logBuf,"%d,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s\n",
					loadNum,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_ChnState));
				
				g_data[carID].WriteData(logBuf);			
				
			}
		}
		break;
	case 9:
		{
			int	existErrorData = FALSE;							//是否存在出错数据
			int startPos = sLinkList10.GetPos(cStartFlag);		//开始位置
			int endPos = sLinkList10.GetPos(cEndFlag);			//结束位置
			
			if (sLinkList10.length < startPos+38)				//单链表长度，串口事件至少39个字节
			{
				return;
			}
			int loadNum = sLinkList10.GetElem(startPos+1);		//负载号
			int funNum = sLinkList10.GetElem(startPos+2);		//功能码
			char endElem = sLinkList10.GetElem(startPos+38);		//结束元素
			if (loadNum<1 || loadNum>=MAX_LOAD_PERCAR || funNum!=0x0D || endElem!= cEndFlag)
			{
				sprintf(logBuf,"Error, Serial Data is wrong! carID:%d, loadNum:%d, funNum:%X, endElem:%X", carID, loadNum, funNum, endElem);
				WriteLog(LEVEL_ERROR,logBuf);
				sLinkList10.DelElemEx(startPos+38);				//从单链表中endPos位置之前的元素
				return;
			}
			
			//底层返回的，负载状态
			int loadState = (int)sLinkList10.GetElem(startPos+37);
			if (loadState == 3)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOLINK;
				sLinkList10.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
				
			}else if (loadState == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_WORKING;
				
				
			}else if (loadState == 2)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_PROTECT;
				sLinkList10.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
				
			}else if (loadState == 0)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOSETPARAM;
				sLinkList10.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
			}
			
			//开始分析数据了---------------------------------------------------------
			BYTE A1,A2,B1,B2,D1,D2,E1,E2;
			int	 A,B,D,E;
			float C,F; 
			int iChnIndex;//通道ID,范围[0,3]
			//通道[0,3]的计算
			for (iChnIndex =0; iChnIndex<4; iChnIndex++)
			{
				if(g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError==0)
				{
					A1 = sLinkList10.GetElem((iChnIndex+1)*4-1+startPos);			
					A2 = sLinkList10.GetElem((iChnIndex+1)*4-0+startPos);
					B1 = sLinkList10.GetElem((iChnIndex+1)*4+15+startPos);			
					B2 = sLinkList10.GetElem((iChnIndex+1)*4+16+startPos);
					A = A1*256 + A2;
					B = B1*256 + B2;
					if(B==0)
					{
						C=0;
					}else
					{						
						C = (float)A/1024;
						C = C/B;
						C = (C/0.0174)*5;								
					}
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage = C;
					
					//通道,电流计算
					D1 = sLinkList10.GetElem((iChnIndex+1)*4+1+startPos);			
					D2 = sLinkList10.GetElem((iChnIndex+1)*4+2+startPos);
					E1 = sLinkList10.GetElem((iChnIndex+1)*4+17+startPos);			
					E2 = sLinkList10.GetElem((iChnIndex+1)*4+18+startPos);
					D = D1*256 + D2;
					E = E1*256 + E2;
					if( E==0 )
					{
						F=0;			
					}else
					{
						F = (float)D/1024;
						F = F/E;
						F = (F/11)*1000;						
					}			
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent = F;
					
					//通道状态, 错误统计			
					if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//恒流
					{	
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//欠压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
						}
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//恒压
					{
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//欠压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
						}
					}
					
				}
			}			
			sLinkList10.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
			//出错数据，写入测试数据文件
			if ( g_AllCar[carID].testParam.isSaveData == 1 && TRUE == existErrorData )
			{
				g_data[carID].WriteData_Ttime("");
				sprintf(logBuf,"%d,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s\n",
					loadNum,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_ChnState));
				
				g_data[carID].WriteData(logBuf);			
				
			}
		}
		break;
	case 10:
		{
			int	existErrorData = FALSE;							//是否存在出错数据
			int startPos = sLinkList11.GetPos(cStartFlag);		//开始位置
			int endPos = sLinkList11.GetPos(cEndFlag);			//结束位置
			
			if (sLinkList11.length < startPos+38)				//单链表长度，串口事件至少39个字节
			{
				return;
			}
			int loadNum = sLinkList11.GetElem(startPos+1);		//负载号
			int funNum = sLinkList11.GetElem(startPos+2);		//功能码
			char endElem = sLinkList11.GetElem(startPos+38);		//结束元素
			if (loadNum<1 || loadNum>=MAX_LOAD_PERCAR || funNum!=0x0D || endElem!= cEndFlag)
			{
				sprintf(logBuf,"Error, Serial Data is wrong! carID:%d, loadNum:%d, funNum:%X, endElem:%X", carID, loadNum, funNum, endElem);
				WriteLog(LEVEL_ERROR,logBuf);
				sLinkList11.DelElemEx(startPos+38);				//从单链表中endPos位置之前的元素
				return;
			}
			
			//底层返回的，负载状态
			int loadState = (int)sLinkList11.GetElem(startPos+37);
			if (loadState == 3)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOLINK;
				sLinkList11.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
				
			}else if (loadState == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_WORKING;
				
				
			}else if (loadState == 2)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_PROTECT;
				sLinkList11.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
				
			}else if (loadState == 0)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOSETPARAM;
				sLinkList11.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
			}
			
			//开始分析数据了---------------------------------------------------------
			BYTE A1,A2,B1,B2,D1,D2,E1,E2;
			int	 A,B,D,E;
			float C,F; 
			int iChnIndex;//通道ID,范围[0,3]
			//通道[0,3]的计算
			for (iChnIndex =0; iChnIndex<4; iChnIndex++)
			{
				if(g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError==0)
				{
					A1 = sLinkList11.GetElem((iChnIndex+1)*4-1+startPos);			
					A2 = sLinkList11.GetElem((iChnIndex+1)*4-0+startPos);
					B1 = sLinkList11.GetElem((iChnIndex+1)*4+15+startPos);			
					B2 = sLinkList11.GetElem((iChnIndex+1)*4+16+startPos);
					A = A1*256 + A2;
					B = B1*256 + B2;
					if(B==0)
					{
						C=0;
					}else
					{						
						C = (float)A/1024;
						C = C/B;
						C = (C/0.0174)*5;								
					}
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage = C;
					
					//通道,电流计算
					D1 = sLinkList11.GetElem((iChnIndex+1)*4+1+startPos);			
					D2 = sLinkList11.GetElem((iChnIndex+1)*4+2+startPos);
					E1 = sLinkList11.GetElem((iChnIndex+1)*4+17+startPos);			
					E2 = sLinkList11.GetElem((iChnIndex+1)*4+18+startPos);
					D = D1*256 + D2;
					E = E1*256 + E2;
					if( E==0 )
					{
						F=0;			
					}else
					{
						F = (float)D/1024;
						F = F/E;
						F = (F/11)*1000;						
					}			
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent = F;
					
					//通道状态, 错误统计			
					if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//恒流
					{	
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//欠压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
						}
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//恒压
					{
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//欠压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
						}
					}
					
				}
			}			
			sLinkList11.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
			//出错数据，写入测试数据文件
			if ( g_AllCar[carID].testParam.isSaveData == 1 && TRUE == existErrorData )
			{
				g_data[carID].WriteData_Ttime("");
				sprintf(logBuf,"%d,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s\n",
					loadNum,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_ChnState));
				
				g_data[carID].WriteData(logBuf);			
				
			}
		}
		break;
	case 12:
		{
			int	existErrorData = FALSE;							//是否存在出错数据
			int startPos = sLinkList13.GetPos(cStartFlag);		//开始位置
			int endPos = sLinkList13.GetPos(cEndFlag);			//结束位置
			
			if (sLinkList13.length < startPos+38)				//单链表长度，串口事件至少39个字节
			{
				return;
			}
			int loadNum = sLinkList13.GetElem(startPos+1);		//负载号
			int funNum = sLinkList13.GetElem(startPos+2);		//功能码
			char endElem = sLinkList13.GetElem(startPos+38);		//结束元素
			if (loadNum<1 || loadNum>=MAX_LOAD_PERCAR || funNum!=0x0D || endElem!= cEndFlag)
			{
				sprintf(logBuf,"Error, Serial Data is wrong! carID:%d, loadNum:%d, funNum:%X, endElem:%X", carID, loadNum, funNum, endElem);
				WriteLog(LEVEL_ERROR,logBuf);
				sLinkList13.DelElemEx(startPos+38);				//从单链表中endPos位置之前的元素
				return;
			}
			
			//底层返回的，负载状态
			int loadState = (int)sLinkList13.GetElem(startPos+37);
			if (loadState == 3)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOLINK;
				sLinkList13.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
				
			}else if (loadState == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_WORKING;
				
				
			}else if (loadState == 2)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_PROTECT;
				sLinkList13.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
				
			}else if (loadState == 0)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOSETPARAM;
				sLinkList13.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
			}
			
			//开始分析数据了---------------------------------------------------------
			BYTE A1,A2,B1,B2,D1,D2,E1,E2;
			int	 A,B,D,E;
			float C,F; 
			int iChnIndex;//通道ID,范围[0,3]
			//通道[0,3]的计算
			for (iChnIndex =0; iChnIndex<4; iChnIndex++)
			{
				if(g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError==0)
				{
					A1 = sLinkList13.GetElem((iChnIndex+1)*4-1+startPos);			
					A2 = sLinkList13.GetElem((iChnIndex+1)*4-0+startPos);
					B1 = sLinkList13.GetElem((iChnIndex+1)*4+15+startPos);			
					B2 = sLinkList13.GetElem((iChnIndex+1)*4+16+startPos);
					A = A1*256 + A2;
					B = B1*256 + B2;
					if(B==0)
					{
						C=0;
					}else
					{						
						C = (float)A/1024;
						C = C/B;
						C = (C/0.0174)*5;								
					}
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage = C;
					
					//通道,电流计算
					D1 = sLinkList13.GetElem((iChnIndex+1)*4+1+startPos);			
					D2 = sLinkList13.GetElem((iChnIndex+1)*4+2+startPos);
					E1 = sLinkList13.GetElem((iChnIndex+1)*4+17+startPos);			
					E2 = sLinkList13.GetElem((iChnIndex+1)*4+18+startPos);
					D = D1*256 + D2;
					E = E1*256 + E2;
					if( E==0 )
					{
						F=0;			
					}else
					{
						F = (float)D/1024;
						F = F/E;
						F = (F/11)*1000;						
					}			
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent = F;
					
					//通道状态, 错误统计			
					if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//恒流
					{	
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//欠压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
						}
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//恒压
					{
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//欠压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
						}
					}
					
				}
			}			
			sLinkList13.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
			//出错数据，写入测试数据文件
			if ( g_AllCar[carID].testParam.isSaveData == 1 && TRUE == existErrorData )
			{
				g_data[carID].WriteData_Ttime("");
				sprintf(logBuf,"%d,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s\n",
					loadNum,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_ChnState));
				
				g_data[carID].WriteData(logBuf);			
				
			}
		}
		break;
	case 13:
		{
			int	existErrorData = FALSE;							//是否存在出错数据
			int startPos = sLinkList14.GetPos(cStartFlag);		//开始位置
			int endPos = sLinkList14.GetPos(cEndFlag);			//结束位置
			
			if (sLinkList14.length < startPos+38)				//单链表长度，串口事件至少39个字节
			{
				return;
			}
			int loadNum = sLinkList14.GetElem(startPos+1);		//负载号
			int funNum = sLinkList14.GetElem(startPos+2);		//功能码
			char endElem = sLinkList14.GetElem(startPos+38);		//结束元素
			if (loadNum<1 || loadNum>=MAX_LOAD_PERCAR || funNum!=0x0D || endElem!= cEndFlag)
			{
				sprintf(logBuf,"Error, Serial Data is wrong! carID:%d, loadNum:%d, funNum:%X, endElem:%X", carID, loadNum, funNum, endElem);
				WriteLog(LEVEL_ERROR,logBuf);
				sLinkList14.DelElemEx(startPos+38);				//从单链表中endPos位置之前的元素
				return;
			}
			
			//底层返回的，负载状态
			int loadState = (int)sLinkList14.GetElem(startPos+37);
			if (loadState == 3)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOLINK;
				sLinkList14.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
				
			}else if (loadState == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_WORKING;
				
				
			}else if (loadState == 2)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_PROTECT;
				sLinkList14.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
				
			}else if (loadState == 0)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOSETPARAM;
				sLinkList14.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
			}
			
			//开始分析数据了---------------------------------------------------------
			BYTE A1,A2,B1,B2,D1,D2,E1,E2;
			int	 A,B,D,E;
			float C,F; 
			int iChnIndex;//通道ID,范围[0,3]
			//通道[0,3]的计算
			for (iChnIndex =0; iChnIndex<4; iChnIndex++)
			{
				if(g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError==0)
				{
					A1 = sLinkList14.GetElem((iChnIndex+1)*4-1+startPos);			
					A2 = sLinkList14.GetElem((iChnIndex+1)*4-0+startPos);
					B1 = sLinkList14.GetElem((iChnIndex+1)*4+15+startPos);			
					B2 = sLinkList14.GetElem((iChnIndex+1)*4+16+startPos);
					A = A1*256 + A2;
					B = B1*256 + B2;
					if(B==0)
					{
						C=0;
					}else
					{						
						C = (float)A/1024;
						C = C/B;
						C = (C/0.0174)*5;								
					}
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage = C;
					
					//通道,电流计算
					D1 = sLinkList14.GetElem((iChnIndex+1)*4+1+startPos);			
					D2 = sLinkList14.GetElem((iChnIndex+1)*4+2+startPos);
					E1 = sLinkList14.GetElem((iChnIndex+1)*4+17+startPos);			
					E2 = sLinkList14.GetElem((iChnIndex+1)*4+18+startPos);
					D = D1*256 + D2;
					E = E1*256 + E2;
					if( E==0 )
					{
						F=0;			
					}else
					{
						F = (float)D/1024;
						F = F/E;
						F = (F/11)*1000;						
					}			
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent = F;
					
					//通道状态, 错误统计			
					if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//恒流
					{	
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//欠压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
						}
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//恒压
					{
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//欠压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
						}
					}
					
				}
			}			
			sLinkList14.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
			//出错数据，写入测试数据文件
			if ( g_AllCar[carID].testParam.isSaveData == 1 && TRUE == existErrorData )
			{
				g_data[carID].WriteData_Ttime("");
				sprintf(logBuf,"%d,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s\n",
					loadNum,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_ChnState));
				
				g_data[carID].WriteData(logBuf);			
				
			}
		}
		break;
	case 14:
		{
			int	existErrorData = FALSE;							//是否存在出错数据
			int startPos = sLinkList15.GetPos(cStartFlag);		//开始位置
			int endPos = sLinkList15.GetPos(cEndFlag);			//结束位置
			
			if (sLinkList15.length < startPos+38)				//单链表长度，串口事件至少39个字节
			{
				return;
			}
			int loadNum = sLinkList15.GetElem(startPos+1);		//负载号
			int funNum = sLinkList15.GetElem(startPos+2);		//功能码
			char endElem = sLinkList15.GetElem(startPos+38);		//结束元素
			if (loadNum<1 || loadNum>=MAX_LOAD_PERCAR || funNum!=0x0D || endElem!= cEndFlag)
			{
				sprintf(logBuf,"Error, Serial Data is wrong! carID:%d, loadNum:%d, funNum:%X, endElem:%X", carID, loadNum, funNum, endElem);
				WriteLog(LEVEL_ERROR,logBuf);
				sLinkList15.DelElemEx(startPos+38);				//从单链表中endPos位置之前的元素
				return;
			}
			
			//底层返回的，负载状态
			int loadState = (int)sLinkList15.GetElem(startPos+37);
			if (loadState == 3)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOLINK;
				sLinkList15.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
				
			}else if (loadState == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_WORKING;
				
				
			}else if (loadState == 2)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_PROTECT;
				sLinkList15.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
				
			}else if (loadState == 0)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOSETPARAM;
				sLinkList15.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
			}
			
			//开始分析数据了---------------------------------------------------------
			BYTE A1,A2,B1,B2,D1,D2,E1,E2;
			int	 A,B,D,E;
			float C,F; 
			int iChnIndex;//通道ID,范围[0,3]
			//通道[0,3]的计算
			for (iChnIndex =0; iChnIndex<4; iChnIndex++)
			{
				if(g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError==0)
				{
					A1 = sLinkList15.GetElem((iChnIndex+1)*4-1+startPos);			
					A2 = sLinkList15.GetElem((iChnIndex+1)*4-0+startPos);
					B1 = sLinkList15.GetElem((iChnIndex+1)*4+15+startPos);			
					B2 = sLinkList15.GetElem((iChnIndex+1)*4+16+startPos);
					A = A1*256 + A2;
					B = B1*256 + B2;
					if(B==0)
					{
						C=0;
					}else
					{						
						C = (float)A/1024;
						C = C/B;
						C = (C/0.0174)*5;								
					}
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage = C;
					
					//通道,电流计算
					D1 = sLinkList15.GetElem((iChnIndex+1)*4+1+startPos);			
					D2 = sLinkList15.GetElem((iChnIndex+1)*4+2+startPos);
					E1 = sLinkList15.GetElem((iChnIndex+1)*4+17+startPos);			
					E2 = sLinkList15.GetElem((iChnIndex+1)*4+18+startPos);
					D = D1*256 + D2;
					E = E1*256 + E2;
					if( E==0 )
					{
						F=0;			
					}else
					{
						F = (float)D/1024;
						F = F/E;
						F = (F/11)*1000;						
					}			
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent = F;
					
					//通道状态, 错误统计			
					if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//恒流
					{	
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//欠压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
						}
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//恒压
					{
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//欠压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
						}
					}
					
				}
			}			
			sLinkList15.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
			//出错数据，写入测试数据文件
			if ( g_AllCar[carID].testParam.isSaveData == 1 && TRUE == existErrorData )
			{
				g_data[carID].WriteData_Ttime("");
				sprintf(logBuf,"%d,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s\n",
					loadNum,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_ChnState));
				
				g_data[carID].WriteData(logBuf);			
				
			}
		}
		break;
	case 15:
		{
			int	existErrorData = FALSE;							//是否存在出错数据
			int startPos = sLinkList16.GetPos(cStartFlag);		//开始位置
			int endPos = sLinkList16.GetPos(cEndFlag);			//结束位置
			
			if (sLinkList16.length < startPos+38)				//单链表长度，串口事件至少39个字节
			{
				return;
			}
			int loadNum = sLinkList16.GetElem(startPos+1);		//负载号
			int funNum = sLinkList16.GetElem(startPos+2);		//功能码
			char endElem = sLinkList16.GetElem(startPos+38);		//结束元素
			if (loadNum<1 || loadNum>=MAX_LOAD_PERCAR || funNum!=0x0D || endElem!= cEndFlag)
			{
				sprintf(logBuf,"Error, Serial Data is wrong! carID:%d, loadNum:%d, funNum:%X, endElem:%X", carID, loadNum, funNum, endElem);
				WriteLog(LEVEL_ERROR,logBuf);
				sLinkList16.DelElemEx(startPos+38);				//从单链表中endPos位置之前的元素
				return;
			}
			
			//底层返回的，负载状态
			int loadState = (int)sLinkList16.GetElem(startPos+37);
			if (loadState == 3)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOLINK;
				sLinkList16.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
				
			}else if (loadState == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_WORKING;
				
				
			}else if (loadState == 2)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_PROTECT;
				sLinkList16.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
				
			}else if (loadState == 0)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOSETPARAM;
				sLinkList16.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
				return;
			}
			
			//开始分析数据了---------------------------------------------------------
			BYTE A1,A2,B1,B2,D1,D2,E1,E2;
			int	 A,B,D,E;
			float C,F; 
			int iChnIndex;//通道ID,范围[0,3]
			//通道[0,3]的计算
			for (iChnIndex =0; iChnIndex<4; iChnIndex++)
			{
				if(g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError==0)
				{
					A1 = sLinkList16.GetElem((iChnIndex+1)*4-1+startPos);			
					A2 = sLinkList16.GetElem((iChnIndex+1)*4-0+startPos);
					B1 = sLinkList16.GetElem((iChnIndex+1)*4+15+startPos);			
					B2 = sLinkList16.GetElem((iChnIndex+1)*4+16+startPos);
					A = A1*256 + A2;
					B = B1*256 + B2;
					if(B==0)
					{
						C=0;
					}else
					{						
						C = (float)A/1024;
						C = C/B;
						C = (C/0.0174)*5;								
					}
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage = C;
					
					//通道,电流计算
					D1 = sLinkList16.GetElem((iChnIndex+1)*4+1+startPos);			
					D2 = sLinkList16.GetElem((iChnIndex+1)*4+2+startPos);
					E1 = sLinkList16.GetElem((iChnIndex+1)*4+17+startPos);			
					E2 = sLinkList16.GetElem((iChnIndex+1)*4+18+startPos);
					D = D1*256 + D2;
					E = E1*256 + E2;
					if( E==0 )
					{
						F=0;			
					}else
					{
						F = (float)D/1024;
						F = F/E;
						F = (F/11)*1000;						
					}			
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent = F;
					
					//通道状态, 错误统计			
					if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//恒流
					{	
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//欠压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
						}
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//恒压
					{
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//欠压
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//是否存在出错数据
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
						}
					}
					
				}
			}			
			sLinkList16.DelElemEx(startPos + 38 );//从单链表中endPos位置之前的元素
			//出错数据，写入测试数据文件
			if ( g_AllCar[carID].testParam.isSaveData == 1 && TRUE == existErrorData )
			{
				g_data[carID].WriteData_Ttime("");
				sprintf(logBuf,"%d,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s\n",
					loadNum,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_ChnState),
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowVoltage,
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_ChnState));
				
				g_data[carID].WriteData(logBuf);			
				
			}
		}
		break;
	}
	
	
}

/************************************************************************/
/* 保存测试数据, [0,15]                                                 */
/************************************************************************/
void	SaveTestData(int carID)				
{
	if ( carID<0 || carID>MAX_CAR_COUNT )
	{
		return;
	}

	if (g_AllCar[carID].testParam.isSaveData == 0)//导入参数，选中了保存文件
	{
		return;
	}

	if (g_AllCar[carID].m_CarState != CAR_STATE_TESTING)
	{
		return;
	}

	long testTime = g_AllCar[carID].m_TimeCounter*COMMAND_INTERVAL;				//测试运行了多长时间，单位ms
	long saveDataInterval = g_AllCar[carID].testParam.saveDataInterval*1000;	//保存数据时间间隔，单位ms
	if (testTime%saveDataInterval == 0)
	{
		
		//保存当前数据
		for (int loadIndex=0; loadIndex < MAX_LOAD_PERCAR; loadIndex++ )
		{
			char    logBuf[256];
			memset(logBuf,0,sizeof(logBuf));
			
			if (loadIndex >= g_AllCar[carID].testParam.startLoadNum-1 && 
				loadIndex <= g_AllCar[carID].testParam.endLoadNum-1 )//&&LOAD_STATE_WORKING == g_AllCar[carID].m_Load[loadIndex].m_LoadState)
			{
				g_data[carID].WriteData_Ttime("");
				sprintf(logBuf,"%d,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s,%0.3f,%0.3f,%s\n",
					loadIndex+1,
					g_AllCar[carID].m_Load[loadIndex].m_Channel[0].m_NowVoltage,
					g_AllCar[carID].m_Load[loadIndex].m_Channel[0].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadIndex].m_Channel[0].m_ChnState),
					g_AllCar[carID].m_Load[loadIndex].m_Channel[1].m_NowVoltage,
					g_AllCar[carID].m_Load[loadIndex].m_Channel[1].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadIndex].m_Channel[1].m_ChnState),
					g_AllCar[carID].m_Load[loadIndex].m_Channel[2].m_NowVoltage,
					g_AllCar[carID].m_Load[loadIndex].m_Channel[2].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadIndex].m_Channel[2].m_ChnState),
					g_AllCar[carID].m_Load[loadIndex].m_Channel[3].m_NowVoltage,
					g_AllCar[carID].m_Load[loadIndex].m_Channel[3].m_NowCurrent,
					GetChnStateName(g_AllCar[carID].m_Load[loadIndex].m_Channel[3].m_ChnState));
				
				g_data[carID].WriteData(logBuf);

			}
		}
	}


}

/************************************************************************/
/* 串口数据测试线程,处理函数                                            */
/************************************************************************/
DWORD WINAPI ThreadFunc(VOID) 
{ 
	int sleepTime=200;
	int i=0;
	while(1)
	{
		Sleep(sleepTime);
		i++;
		
		if(i%2==0 )
		{
			if (i==2)
			{
				Sleep(sleepTime);
				WriteSerialPortCommand(g_curSelTestCar,LOAD_COMMAND_LIGHTON,0);
			}else if (i==4)
			{
				Sleep(sleepTime);
				WriteSerialPortCommand(g_curSelTestCar,LOAD_COMMAND_LIGHTOFF,0);
			}else if (i==6)
			{
				Sleep(sleepTime);
				WriteSerialPortCommand(g_curSelTestCar,LOAD_COMMAND_BEEPON,0);
			}else if (i==8)
			{
				Sleep(sleepTime);
				WriteSerialPortCommand(g_curSelTestCar,LOAD_COMMAND_BEEPOFF,0);
			}else if (i==10)
			{
				Sleep(sleepTime);
				WriteSerialPortCommand(g_curSelTestCar,LOAD_COMMAND_110V,0);
			}else if (i==12)
			{
				Sleep(sleepTime);
				WriteSerialPortCommand(g_curSelTestCar,LOAD_COMMAND_220V,0);
			}
		}else
		{
			Sleep(sleepTime);
			WriteSerialPortCommand(g_curSelTestCar,LOAD_COMMAND_SET,1);
			Sleep(sleepTime);
			WriteSerialPortCommand(g_curSelTestCar,LOAD_COMMAND_READ,1);		
		}
		if (i==14)
		{
			i=0;
		}
	}
}
/************************************************************************/
/* 串口数据测试线程,启动函数                                            */
/************************************************************************/
void	SerialPortFuncTestThread(void)					
{
	DWORD dwThreadID = 0;
	HANDLE hThread;
	
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ThreadFunc, NULL, 0, &dwThreadID);  

	if (hThread != NULL)
	{		
		CloseHandle(hThread);
	}
}