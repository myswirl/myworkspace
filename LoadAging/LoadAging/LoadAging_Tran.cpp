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

// var about LoadAging.ini
char			cfg_SoftwareVersion[32]="V1.10";					//软件版本标识
char			cfg_IniName[256] = "";
char			cfg_IniShortName[] = "\\LoadAging.ini";
char			cfg_NormalPassword[128]={0};				//技术人员密码
char			cfg_SuperPassword[128]={0};					//工程师密码
char			cfg_DataRecordPath[256]="";					//数据保存路径
char			cfg_Title[256]="电子负载老化系统";			//标题
int				cfg_EnableBeep;								//使能蜂鸣器
int				cfg_EnableLight;							//使能指示灯
int				cfg_DisplayTestInfo;						//是否统计测试信息
int				cfg_PowerMax;								//功率上限值
int				cfg_CurrentErrorPercent=10;					//电流偏差百分比 在设定值的基础上正负百分之十
int				cfg_VoltageErrorPercent=10;					//电压偏差百分比 在设定值的基础上正负百分之十
int				cfg_IsDealwithRecvData=1;					//是否使能数据处理
int				cfg_FixDataPercent=3;						//定值的偏差范围千分之1---3
int				cfg_NoFixDataPercent=3;						//非定值的偏差范围千分之1---3

//系统全局变量--------------------------------------------
int				g_LoginCheckOK=-1;							//登录校验是否通过,-1:校验失败；0:工程师登录成功；1:技术员登录成功
long			g_SystemRunTimeCounter=0;					//系统运行时间计数器
CAR_STRUCT		g_AllCar[MAX_CAR_COUNT];					//最重要的全局结构体
int				g_ComList[MAX_CAR_COUNT];					//可用串口列表
int				g_AvailableComCounter=0;					//可用串口数目
int				g_curSelTestCar=0;							//当前选中测试车
int				g_curImportCar=0;							//当前选中导入数据的测试车
CMyLog			g_data[MAX_CAR_COUNT];						//记录测试数据日志类
CSerialPort		g_Ports[MAX_CAR_COUNT];						//串口类，处理每个测试车串口数据
float			g_SetCurrentError=0.2;						//设置电流值偏差范围
float			g_SetVoltageError=2.0;						//设置电压值偏差范围
int				g_LastTimeSeriesCMD=LOAD_COMMAND_220V_ON;		//上一个时序命令，110V还是220V，决定脉冲开关的命令类型			
int				g_ValidHour=0;								//加密狗
const float EPSINON	= 0.000000	;	

/************************************************************************/
/* 读取配置文件ini                                                      */
/************************************************************************/
void	ReadFromConfig(void)
{
	GetCurrentDirectory ( 256-32, cfg_IniName );
	strcat ( cfg_IniName, cfg_IniShortName );
	
	cfg_LogOn = GetPrivateProfileInt ( "ConfigInfo", "LogOn", 1, cfg_IniName);					//是否开启日志
	cfg_LogLevel = GetPrivateProfileInt ( "ConfigInfo", "LogLevel", 0, cfg_IniName);			//日志级别
	cfg_DisplayTestInfo = GetPrivateProfileInt ( "ConfigInfo", "DisplayTestInfo", 0, cfg_IniName);		//是否显示测试统计信息
	cfg_EnableBeep = GetPrivateProfileInt ( "ConfigInfo", "EnableBeep", 1, cfg_IniName);		//是否使能蜂鸣器
	cfg_EnableLight = GetPrivateProfileInt ( "ConfigInfo", "EnableLight", 1, cfg_IniName);		//是否使能指示灯
	
	cfg_PowerMax = GetPrivateProfileInt ( "ConfigInfo", "PowerMax", 75, cfg_IniName);		//功率上限值，add by lmy 20120613

	cfg_CurrentErrorPercent = GetPrivateProfileInt ( "ConfigInfo", "CurrentErrorPercent", 10, cfg_IniName);	
	cfg_VoltageErrorPercent = GetPrivateProfileInt ( "ConfigInfo", "VoltageErrorPercent", 10, cfg_IniName);	
	cfg_IsDealwithRecvData= GetPrivateProfileInt ( "ConfigInfo", "IsDealwithRecvData", 1, cfg_IniName);					//是否使能数据处理
	cfg_FixDataPercent = GetPrivateProfileInt ( "ConfigInfo", "FixDataPercent", 3, cfg_IniName);						//定值的偏差范围千分之1---3
	cfg_NoFixDataPercent= GetPrivateProfileInt ( "ConfigInfo", "NoFixDataPercent", 3, cfg_IniName);						//非定值的偏差范围千分之1---3

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
	WritePrivateProfileString ( "ConfigInfo", "SoftwareVersion", cfg_SoftwareVersion, cfg_IniName);

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
		InitializeCriticalSection(&g_AllCar[i].CriticalSectionRecvBuffer); 
	}
	if(cfg_DisplayTestInfo == 1)
	{
		pdlg->GetDlgItem(IDC_STATIC_SERPORT_TEST)->ShowWindow(SW_SHOW);	
		pdlg->SetDlgItemText(IDC_STATIC_SERPORT_TEST,cfg_SoftwareVersion);

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
		if(g_AllCar[carID].m_CarTestingState == CAR_STATE_TESTING_READ)
		{
			return "测试中";
		}else if(g_AllCar[carID].m_CarTestingState == CAR_STATE_TESTING_110VOFF)
		{
			return "110V_OFF";
		}else if(g_AllCar[carID].m_CarTestingState == CAR_STATE_TESTING_110VON)
		{
			return "110V_ON";
		}else if(g_AllCar[carID].m_CarTestingState == CAR_STATE_TESTING_220VOFF)
		{
			return "220V_OFF";
		}else if(g_AllCar[carID].m_CarTestingState == CAR_STATE_TESTING_220VON)
		{
			return "220V_ON";
		}else
		{
			return "测试中";
		}
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
	if(	CAR_STATE_TESTING == g_AllCar[g_curSelTestCar].m_CarState ||
		cfg_DisplayTestInfo==1)
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
		strCom.Format("\\\\.\\COM%d", nCom);
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
/*获取并联模式名称                                                      */
/************************************************************************/
char*	GetParaModeName(int modeID)									
{
	if (modeID == 0)
	{
		return "无并联";
	}else if (modeID == 1)
	{
		return "两路并联";
	}else if (modeID == 2)
	{
		return "四路并联";
	}
	return "UNKNOWN";
}
char* GetParamSetMode(int paramSetModID)
{
	switch(paramSetModID)
	{
	case 0:		return "整区设置";
	case 1:		return "分层设置";
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
void	ProcessReceiveData( unsigned char* pRecvData, int portNo)
{
	int carID;
	char tmpStr[64]={0};
	char logBuf[512];
	memset(logBuf,0,sizeof(logBuf));

	//根据串口号定位测试车
	carID = GetCarIDByPortNo(portNo);	
	if ( carID < 0 )
	{
		sprintf(tmpStr,"Error, carID:%d is wrong!", carID);
		WriteLog(LEVEL_ERROR, tmpStr);
		return;
	}
	
	//将收取到的数据copy到recvbuffer缓冲区
	EnterCriticalSection(&g_AllCar[g_curSelTestCar].CriticalSectionRecvBuffer);
	memcpy(g_AllCar[carID].recvData,pRecvData,SERIALPORT_BUFSIZE);
	LeaveCriticalSection(&g_AllCar[g_curSelTestCar].CriticalSectionRecvBuffer);	

	sprintf(logBuf,"carID:%02d,                     RecvData[%d,%d,%d]    ,",carID,g_AllCar[carID].m_ReadCommandCounter, g_AllCar[carID].m_ReadEventCounter,g_AllCar[carID].m_LostReadEventCounter);
	for(int i=0;i<SERIALPORT_BUFSIZE;i++)
	{
		sprintf(logBuf,"%s %02X",logBuf, g_AllCar[carID].recvData[i]);
	}
	WriteLog(LEVEL_DEBUG, logBuf);
	if (g_AllCar[carID].recvData[2] == LOAD_COMMAND_READ)
	{
		g_AllCar[carID].m_ReadEventCounter++;//每台车收到的事件统计
		if(cfg_DisplayTestInfo == 1)//界面实时显示串口数据统计信息	
		{	
			sprintf(tmpStr,"[%d/%d],%d",g_AllCar[g_curSelTestCar].m_ReadCommandCounter,g_AllCar[g_curSelTestCar].m_ReadEventCounter,
				g_AllCar[g_curSelTestCar].m_ReadCommandCounter-g_AllCar[g_curSelTestCar].m_ReadEventCounter);
			pdlg->SetDlgItemText(IDC_STATIC_SERPORT_TEST,tmpStr);
		}

	}

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
			pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorBlack);
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
// 	if (g_AllCar[curCarID].m_CarState == CAR_STATE_NOIMPORT )
// 	{
// 		return;
// 	}
// 	if (g_AllCar[curCarID].m_CarState == CAR_STATE_IMPORTED)
// 	{
// 		return;
// 	}
	 
	//循环更新LED灯
	for(int iLedIndex=0; iLedIndex<MAX_LED_COUNT; iLedIndex++)
	{
		int loadIndex = iLedIndex/4;	//此LED所属于的负载号, [0,49]
		int chnIndex = iLedIndex%4;		//此LED所属于的通道号, [0,3]
		int	iPerLayer=0;
		
		//刷新LED颜色，四种颜色
		if ( (g_AllCar[curCarID].m_CarState == CAR_STATE_TESTING || g_AllCar[curCarID].m_CarState == CAR_STATE_TESTFINISH)
			&& loadIndex>=g_AllCar[curCarID].testParam.startLoadNum-1 
			&& loadIndex<=g_AllCar[curCarID].testParam.endLoadNum-1 )
		{
			
			if(g_AllCar[curCarID].testParam.loadPerLayer != 0)
			{
				iPerLayer=loadIndex%g_AllCar[curCarID].testParam.loadPerLayer+1;
			}
			switch (g_AllCar[curCarID].m_Load[loadIndex].m_LoadState)
			{
			case LOAD_STATE_SELECT:		//已经选中，黄色
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
// 			case LOAD_STATE_NOLINK:		//未连接，也就是这个负载的四个通道都没有接电源，黄色
// 				pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorSelect);
// 				sprintf(tmpStr,"位置:第%d层第%d个, 拨码:%d, 通道:%d \n状态:未连接\n设定:%s,%0.3f%s, %0.3f%s~%0.3f%s\n实时:功率= \n电流= , 电压=",
// 					g_AllCar[curCarID].m_Load[loadIndex].m_LayerID,
// 					iPerLayer,
// 					loadIndex+1,
// 					chnIndex+1,
// 					Led_GetLoadModeName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
// 					g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetValue,
// 					Led_GetFixedValue(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
// 					g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetMin,
// 					Led_GetUnitName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
// 					g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetMax,
// 					Led_GetUnitName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode)
// 					);
// 				break;
// 			case LOAD_STATE_NOSETPARAM:	//未设置参数，需要重新设置参数
// 				pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorSelect);
// 				sprintf(tmpStr,"位置:第%d层第%d个, 拨码:%d, 通道:%d \n状态:未设置\n设定:%s,%0.3f%s, %0.3f%s~%0.3f%s\n实时:功率= \n电流= , 电压=",
// 					g_AllCar[curCarID].m_Load[loadIndex].m_LayerID,
// 					iPerLayer,
// 					loadIndex+1,
// 					chnIndex+1,
// 					Led_GetLoadModeName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
// 					g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetValue,
// 					Led_GetFixedValue(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
// 					g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetMin,
// 					Led_GetUnitName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode),
// 					g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_SetMax,
// 					Led_GetUnitName(g_AllCar[curCarID].m_Load[loadIndex].m_LoadMode)
// 					);
// 				break;
			case LOAD_STATE_NOLINK:
			case LOAD_STATE_WORKING:	//工作中，此种状态可实时获取数据
				{
					char testResult[5]={0};
					switch (g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_ChnState)
					{
					case CHN_STATE_NOLINK:	//未连接，黄色
						if (g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_ChnHaveBeenRight == 0)
						{
							pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorSelect);//黄色
							strcpy(testResult,"未连接");
						}else
						{
							pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorRed);//
							strcpy(testResult,"无输出");
						}
						break;
					case CHN_STATE_OK:		//合格，绿色
						pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorGreen);		
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
					case CHN_STATE_2_PARALELL:
						pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorNotSelect);
						strcpy(testResult,"两路并联");
						break;
					case CHN_STATE_4_PARALELL:
						pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorNotSelect);
						strcpy(testResult,"四路并联");
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
				pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorBlack);
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
			//刷新提示框
			pW=pdlg->GetDlgItem(1400+1+iLedIndex);
			pdlg->m_tooltip.AddTool(pW,tmpStr); 		
		}else
		{
			pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorNotSelect);
			sprintf(tmpStr,"位置:%d,  状态:未选中\n设定\n实时:功率= \n电流= , 电压=",iLedIndex+1);
			//刷新提示框
			pW=pdlg->GetDlgItem(1400+1+iLedIndex);
			pdlg->m_tooltip.AddTool(pW,tmpStr); 

		}
	}
}
/************************************************************************/
/*根据测试时间在测试时序中的位置，判断当前测试命令类型，loadNum范围[1,63]   */
/************************************************************************/
int	GetSerialPortCommandType(int carID, int loadNum)
{
	int i;
	int iLoad;
	if ( carID<0 || carID>MAX_CAR_COUNT || g_AllCar[carID].m_Load[loadNum-1].m_LoadState == LOAD_STATE_PROTECT )
	{
		return -1;
	}
	if(g_AllCar[carID].m_CMDBeepClear == 1 || g_AllCar[carID].m_CMDLightClear == 1)
	{
		return -1;
	}
	if( cfg_EnableBeep==1 && g_AllCar[carID].m_CMDAlreadyBeepOn==0 )
	{
		if(g_AllCar[carID].m_CMDBeepOnCounter<7)
		{			
		
			for(iLoad=g_AllCar[g_curSelTestCar].testParam.startLoadNum-1; iLoad<g_AllCar[g_curSelTestCar].testParam.endLoadNum; iLoad++)
			{
				for( i=0;i<4;i++)//此负载的四个通道有一个出现错误，则下发告警
				{
					if (1 == g_AllCar[carID].m_Load[iLoad].m_Channel[i].m_ChnHaveBeenError )
					{
						g_AllCar[carID].m_CMDBeepOnCounter++;
						return LOAD_COMMAND_BEEPON;
					}
				}
			}
			
		}else
		{
			g_AllCar[carID].m_CMDAlreadyBeepOn=1;//重发超过7次,则认为蜂鸣器响了
			g_AllCar[carID].m_CMDBeepOnCounter=0;
		}
	}
	if( cfg_EnableLight==1 && g_AllCar[carID].m_CMDAlreadyLightOn==0 )
	{
		if(g_AllCar[carID].m_CMDLightOnCounter<7)
		{	
			for(iLoad=g_AllCar[g_curSelTestCar].testParam.startLoadNum-1; iLoad<g_AllCar[g_curSelTestCar].testParam.endLoadNum; iLoad++)
			{
				for( i=0;i<4;i++)//此负载的四个通道有一个出现错误，则下发告警
				{
					if (1 == g_AllCar[carID].m_Load[iLoad].m_Channel[i].m_ChnHaveBeenError )
					{
						g_AllCar[carID].m_CMDLightOnCounter++;
						return LOAD_COMMAND_LIGHTON;
					}
				}
			}
		}else
		{
			g_AllCar[carID].m_CMDAlreadyLightOn=1;//重发超过7次则认为告警灯亮了
			g_AllCar[carID].m_CMDLightOnCounter=0;
		}
	}


	//判断当前测试时间是否到了插入时序中
	for(int testItemIndex=0;testItemIndex< g_AllCar[carID].testItemNum; testItemIndex++)
	{
		int testItem=g_AllCar[carID].pTimeSeires[testItemIndex].testItem;
		long startTime=g_AllCar[carID].pTimeSeires[testItemIndex].startTime;
		long testTime=g_AllCar[carID].pTimeSeires[testItemIndex].testTime;
//		TRACE("carID:%d, timerCounter:%d, testItemIndex:%d, startTime:%d, testTime:%d, testItem:%d\n", 
//			carID,g_AllCar[carID].m_TimeCounter,testItemIndex, startTime,testTime,testItem);

// 		//插入时序的开始点
// 		if (g_AllCar[carID].m_TimeCounter*COMMAND_INTERVAL == startTime*1000) 
// 		{
// 			return testItem;			
// 		}
		
		//插入时序的过程中
		if (g_AllCar[carID].m_TimeCounter*COMMAND_INTERVAL >= startTime*1000 &&
			g_AllCar[carID].m_TimeCounter*COMMAND_INTERVAL < startTime*1000+testTime*1000 ) 
		{
			if(g_AllCar[carID].pTimeSeires[testItemIndex].recvEvt == 0)//如果未回主控板命令
			{
				return testItem;//时序过程中，判断是否单片机回事件，校验机制，方式主板数据丢失
			}else
			{
				return 0;	
			}			
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
		//二路并联
		if (1 == g_AllCar[carID].m_Load[curLoadNum-1].m_ParaMode && 0 == g_AllCar[carID].m_Load[curLoadNum-1].m_LoadMode)
		{
			setValue = (int)(g_AllCar[carID].m_Load[curLoadNum-1].m_Channel[0].m_SetValue*25.5/2);
			wBuffer[5] = (char)setValue;
			wBuffer[13] = (char)setValue;
			
			setValue = (int)(g_AllCar[carID].m_Load[curLoadNum-1].m_Channel[2].m_SetValue*25.5/2);
			wBuffer[21] = (char)setValue;
			wBuffer[29] = (char)setValue;
			break;
		}
		//四路并联
		if (2 == g_AllCar[carID].m_Load[curLoadNum-1].m_ParaMode && 0 == g_AllCar[carID].m_Load[curLoadNum-1].m_LoadMode)
		{
			setValue = (int)(g_AllCar[carID].m_Load[curLoadNum-1].m_Channel[0].m_SetValue*25.5/4);
			wBuffer[5] = (char)setValue;
			wBuffer[13] = (char)setValue;
			wBuffer[21] = (char)setValue;
			wBuffer[29] = (char)setValue;
			break;
		}
		//无并联
		if (0 == g_AllCar[carID].m_Load[curLoadNum-1].m_ParaMode)
		{
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
		}
		
		break;
		case LOAD_COMMAND_READ:
			g_AllCar[carID].m_CarTestingState = CAR_STATE_TESTING_READ;//测试中状态，Read
			wBuffer[2] = '\x0D';			//功能码
			sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_READ[%d,%d,%d]    ,",
				carID, curLoadNum,g_AllCar[carID].m_ReadCommandCounter,g_AllCar[carID].m_ReadEventCounter,g_AllCar[carID].m_LostReadEventCounter);
			break;
		case LOAD_COMMAND_STOP:
			sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_STOP    ,", carID, curLoadNum);
			wBuffer[2] = '\x0C';			//功能码
			break;
		case LOAD_COMMAND_110V_OFF:			//
			g_AllCar[carID].m_CarTestingState = CAR_STATE_TESTING_110VOFF;//测试中状态
			sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_110V_OFF    ,", carID,mainCtrlBoardCommad);
			wBuffer[1] = mainCtrlBoardCommad;
			wBuffer[2] = '\x09';			//功能码
			break;
		case LOAD_COMMAND_110V_ON:
			g_AllCar[carID].m_CarTestingState = CAR_STATE_TESTING_110VON;//测试中状态
			sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_110V_ON    ,", carID,mainCtrlBoardCommad);
			wBuffer[1] = mainCtrlBoardCommad;
			wBuffer[2] = '\x08';			//功能码
			break;
		case LOAD_COMMAND_220V_ON:
			g_AllCar[carID].m_CarTestingState = CAR_STATE_TESTING_220VON;//测试中状态
			sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_220V_ON ,", carID,mainCtrlBoardCommad);
			wBuffer[1] = mainCtrlBoardCommad;
			wBuffer[2] = '\x06';			//功能码
			break;
		case LOAD_COMMAND_220V_OFF:
			g_AllCar[carID].m_CarTestingState = CAR_STATE_TESTING_220VOFF;//测试中状态
			sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_220V_OFF,", carID,mainCtrlBoardCommad);
			wBuffer[1] = mainCtrlBoardCommad;
			wBuffer[2] = '\x07';			//功能码
			break;
		case LOAD_COMMAND_BEEPON:
			sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_BEEPON  ,", carID,mainCtrlBoardCommad);
			wBuffer[1] = mainCtrlBoardCommad;
			wBuffer[2] = '\x10';			//功能码
			break;
		case LOAD_COMMAND_BEEPOFF:
			sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_BEEPOFF ,", carID,mainCtrlBoardCommad);
			wBuffer[1] = mainCtrlBoardCommad;
			wBuffer[2] = LOAD_COMMAND_BEEPOFF;			//功能码
			break;
		case LOAD_COMMAND_LIGHTON:
			sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_LIGHTON ,", carID,mainCtrlBoardCommad);
			wBuffer[1] = mainCtrlBoardCommad;
			wBuffer[2] = '\x12';			//功能码
			break;
		case LOAD_COMMAND_LIGHTOFF:
			sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_LIGHTOFF,", carID,mainCtrlBoardCommad);
			wBuffer[1] = mainCtrlBoardCommad;
			wBuffer[2] = LOAD_COMMAND_LIGHTOFF;			//功能码
			break;
		default:
			return;
			
	}
	//拨码开关0、1、2、10、13、14同时存在时，存在收不到的情况
	if (serialPortCommandType ==LOAD_COMMAND_SET || 
		serialPortCommandType == LOAD_COMMAND_READ ||
		serialPortCommandType == LOAD_COMMAND_STOP)
	{
		if (curLoadNum==0)
		{
			curLoadNum=63;
			wBuffer[1] = curLoadNum;			//主板负载号
		}else if (curLoadNum==1)
		{
			curLoadNum=61;
			wBuffer[1] = curLoadNum;			//负载号1
		}else if (curLoadNum==2)
		{
			curLoadNum=62;
			wBuffer[1] = curLoadNum;			//负载号2
		}else if (curLoadNum==3)
		{
				curLoadNum=65;
				wBuffer[1] = curLoadNum;			//负载号3, add by lmy 20120613
			}else if (curLoadNum==10)
		{
			curLoadNum=70;
			wBuffer[1] = curLoadNum;			
		}else if (curLoadNum==13)
		{
			curLoadNum=73;
			wBuffer[1] = curLoadNum;			
		}else if (curLoadNum==14)
		{
			curLoadNum=74;
			wBuffer[1] = curLoadNum;			
		}

	}
	//串口数据返回超时处理
	if(serialPortCommandType==LOAD_COMMAND_READ && 
		g_AllCar[carID].m_ReadCommandCounter > g_AllCar[carID].m_ReadEventCounter+g_AllCar[carID].m_LostReadEventCounter)
	{
		g_AllCar[carID].m_LostEventTimer++;
		if(g_AllCar[carID].m_LostEventTimer<7)//5s没有数据返回则下发下一个命令
		{
			g_AllCar[carID].m_curLoadNum--;//命令停留在当前负载，
			return;
		}
		g_AllCar[carID].m_LostReadEventCounter++;//确定数据丢失了，下发下一个命令

	}
	g_AllCar[carID].m_LostEventTimer=0;//超时未返回数据计数器
	g_Ports[carID].WriteToPort(wBuffer);

	if(serialPortCommandType==LOAD_COMMAND_READ)
	{
		g_AllCar[carID].m_ReadCommandCounter++;//每台车命令个数统计
	}
	for (int i=0;i<39;i++)
	{
		sprintf(logBuf,"%s %02X",logBuf,wBuffer[i]);
	}
	WriteLog(LEVEL_DEBUG,logBuf);
	
}
/************************************************************************/
/* 指定通道测试结果比较                                                 */
/************************************************************************/
bool ChnTestDataCompare(int carID, int loadNum, int iChnIndex)
{
	bool	existErrorData = FALSE;							//是否存在出错数据
	char	tmpStr[100]={0};
	if(g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError==1) //出现过错误，则不进行计算
	{
		return existErrorData;
	}

	//通道状态, 错误统计			
	if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//恒流
	{	
		if( g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage == 0 && g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent== 0)
		{			
			if (g_AllCar[carID].m_TimeCounter>200)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接				
				if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenRight == 1)
				{
					g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
					existErrorData = TRUE;	
				}
			}
			
		}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <	g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
		{
			if (g_AllCar[carID].m_TimeCounter>200)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//欠压
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
				g_AllCar[carID].m_ErrorCounter++;
				g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
				existErrorData = TRUE;							//是否存在出错数据
				sprintf(tmpStr,"Error, CHN_STATE_LOWVOL, carID:%d, loadNum:%d, iChnIndex:%d", carID,loadNum, iChnIndex);
				WriteLog(LEVEL_DEBUG, tmpStr);
			}
		}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >	g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
		{
			if (g_AllCar[carID].m_TimeCounter>200)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
				g_AllCar[carID].m_ErrorCounter++;
				g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
				existErrorData = TRUE;							//是否存在出错数据
				sprintf(tmpStr,"Error, CHN_STATE_OVEROWVOL, carID:%d, loadNum:%d, iChnIndex:%d", carID,loadNum, iChnIndex);
				WriteLog(LEVEL_DEBUG, tmpStr);
			}
		}else if ((g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent - g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue * (( 100.0 +cfg_CurrentErrorPercent )/100.0) ) > EPSINON)
		{
			if (g_AllCar[carID].m_TimeCounter>200)
			{				
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;	//过流
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
				g_AllCar[carID].m_ErrorCounter++;	
				g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
				existErrorData = TRUE;							//是否存在出错数据
				sprintf(tmpStr,"Error, CHN_STATE_OVERCUR, carID:%d, loadNum:%d, iChnIndex:%d", carID,loadNum, iChnIndex);
				WriteLog(LEVEL_DEBUG, tmpStr);
			}
			
		}else if ((g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent -g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue * (( 100.0 - cfg_CurrentErrorPercent )/100.0) ) < EPSINON)
		{
			if (g_AllCar[carID].m_TimeCounter>200)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
				g_AllCar[carID].m_ErrorCounter++;	
				g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
				existErrorData = TRUE;							//是否存在出错数据
				sprintf(tmpStr,"Error, CHN_STATE_LOWCUR, carID:%d, loadNum:%d, iChnIndex:%d", carID,loadNum, iChnIndex);
				WriteLog(LEVEL_DEBUG, tmpStr);	
			}
		}else
		{
			g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
			g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenRight =1;
		}
	}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//恒压
	{
		if( g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage == 0 && g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent== 0)
		{
			g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//未连接
			if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenRight == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
				existErrorData = TRUE;	
			}
		}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <	g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
		{
			if (g_AllCar[carID].m_TimeCounter>200)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//欠流
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
				g_AllCar[carID].m_ErrorCounter++;
				g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
				existErrorData = TRUE;							//是否存在出错数据
				sprintf(tmpStr,"Error, CHN_STATE_LOWCUR, carID:%d, loadNum:%d, iChnIndex:%d", carID,loadNum, iChnIndex);
				WriteLog(LEVEL_DEBUG, tmpStr);	
			}
			
		}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >	g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
		{
			if (g_AllCar[carID].m_TimeCounter>200)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//过流
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
				g_AllCar[carID].m_ErrorCounter++;	
				g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
				existErrorData = TRUE;							//是否存在出错数据
				sprintf(tmpStr,"Error, CHN_STATE_OVERCUR, carID:%d, loadNum:%d, iChnIndex:%d", carID,loadNum, iChnIndex);
				WriteLog(LEVEL_DEBUG, tmpStr);
			}
			
		}else if ((g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage - g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue*((100.0+cfg_VoltageErrorPercent)/100.0) ) > EPSINON)
		{
			if (g_AllCar[carID].m_TimeCounter>200)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//过压
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
				g_AllCar[carID].m_ErrorCounter++;	
				g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
				existErrorData = TRUE;							//是否存在出错数据
				sprintf(tmpStr,"Error, CHN_STATE_OVERVOL, carID:%d, loadNum:%d, iChnIndex:%d", carID,loadNum, iChnIndex);
				WriteLog(LEVEL_DEBUG, tmpStr);	
			}		
			
		}else if ((g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage - g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue*((100-cfg_VoltageErrorPercent)/100.0) ) < EPSINON)
		{
			if (g_AllCar[carID].m_TimeCounter>200)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//欠压
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//出现过错误
				g_AllCar[carID].m_ErrorCounter++;	
				g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
				existErrorData = TRUE;							//是否存在出错数据
				sprintf(tmpStr,"Error, CHN_STATE_OVERVOL, carID:%d, loadNum:%d, iChnIndex:%d", carID,loadNum, iChnIndex);
				WriteLog(LEVEL_DEBUG, tmpStr);	
			}		
			
		}else
		{
			g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//合格
			g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenRight =1;

		}
	}
	return existErrorData;
}

/************************************************************************/
/* 是否主控板命令                                                       */
/************************************************************************/
bool IsMainBoardCommand(int funcNum)
{
	switch (funcNum)
	{
	case LOAD_COMMAND_220V_ON://		= 0x09,	//主控板命令，220V老化测试命令
	case LOAD_COMMAND_110V_ON://		= 0x08,	//主控板命令，110V老化测试命令
	case LOAD_COMMAND_220V_OFF://	= 0x06,	//主控板命令，脉冲开
	case LOAD_COMMAND_110V_OFF://	= 0x07,	//主控板命令，脉冲关
	case LOAD_COMMAND_BEEPON://		= 0x10,	//主控板命令，蜂鸣器开
	case LOAD_COMMAND_BEEPOFF://	= 0x11,	//主控板命令，蜂鸣器关
	case LOAD_COMMAND_LIGHTON://	= 0x12,	//主控板命令，LED开
	case LOAD_COMMAND_LIGHTOFF://	= 0x13,	//主控板命令，LED关
		return true;
	}
	return false;
}

//对当前获取到的电压数据进行处理，返回值-1表示不处理，返回多少就显示多少
float DealWithSerialPortData_NowVoltage(int carID, int loadNum, int iChnIndex, float nowVoltage)
{
	//当 恒压测试， 电压定值在10%波动，电流在上下限15%波动，这时显示的值更改为 电压值在正负千分之1-千分之3波动，电流值在正负1%-3%波动；
	//当 恒流测试， 电流定值在10%波动，电压在上下限15%波动，这时显示的值更改为 电流值在正负千分之1-千分之3波动，电压值在正负1%-3%波动；

    int randomNum_percent_begin = 1;
	int randomNum_percent=1;//随机产生的偏差百分比，1-3%
	
	int randomNum_oddeven_begin = 1;//随机产生的odd even，1 or 2
	int randomNum_oddeven_end = 2;//随机产生的odd even，1 or 2
	int randomNum_oddeven = 1;//随机产生的odd even，1 or 2

	char tmpStr[256];
	memset(tmpStr, 0, sizeof(tmpStr));

	sprintf(tmpStr,"NowVoltage(), cfg_IsDealwithRecvData:%d, carID:%d, loadNum:%d, iChnIndex:%d, nowVoltage:%f, m_ParaMode:%d, m_LoadMode:%d, m_SetValue:%f, m_SetMin:%f, m_SetMax:%f\n ", 
		cfg_IsDealwithRecvData,
		carID,
		loadNum,
		iChnIndex,
		nowVoltage,
		g_AllCar[carID].m_Load[loadNum-1].m_ParaMode,
		g_AllCar[carID].m_Load[loadNum-1].m_LoadMode,
		g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue,
		g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin,
		g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax);
	WriteLog(LEVEL_DEBUG,tmpStr);
	
	if( nowVoltage == 0)
	{
		return -1;
	}

	if (cfg_IsDealwithRecvData == 0)
	{
		return -1;
	}
	
	
	if (g_AllCar[carID].m_Load[loadNum-1].m_ParaMode == 0 && g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//当前传入电压值，恒流测试,单路测试
	{	
		if ((nowVoltage - g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin*0.85 ) < EPSINON)
		{
			return -1;
		}
		if ((nowVoltage - g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax*1.15 ) > EPSINON)
		{
			return -1;
		}
		//如果当前电压值在当前上下限的 15%误差内
		//未超出设定值，则在 设定值的 正负千分之1-10 波动
		srand((unsigned)time(NULL));
		randomNum_percent = randomNum_percent_begin+rand()%cfg_NoFixDataPercent;
		
		srand((unsigned)time(NULL));
		randomNum_oddeven = randomNum_oddeven_begin+rand()%randomNum_oddeven_end;
		
		if (randomNum_oddeven == 1)
		{
			return (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin+g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)/2*(1000.0+randomNum_percent)/1000.0;
		}else
		{
			return (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin+g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)/2*(1000.0-randomNum_percent)/1000.0;
		}

	}else if (g_AllCar[carID].m_Load[loadNum-1].m_ParaMode == 0 && g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//当前传入电压值，恒压，单路测试
	{
		if ( (nowVoltage - g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue*1.1) > EPSINON )
		{
			return -1; //超出设定值的百分之十
		}
		if ( (nowVoltage - g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue*0.9) < EPSINON )
		{
			return -1;//超出设定值的百分之十
		}
		//未超出设定值，则在 设定值的 正负千分之1-3 波动
		srand((unsigned)time(NULL));
		randomNum_percent = randomNum_percent_begin+rand()%cfg_FixDataPercent;

		srand((unsigned)time(NULL));
		randomNum_oddeven = randomNum_oddeven_begin+rand()%randomNum_oddeven_end;

		if (randomNum_oddeven == 1)
		{
			return g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue*(1000.0+randomNum_percent)/1000.0;
		}else
		{
			return g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue*(1000.0-randomNum_percent)/1000.0;
		}

	}
	return -1;
}
//对当前获取到的电流数据进行处理，返回值-1表示异常，
float DealWithSerialPortData_NowCurrent(int carID, int loadNum, int iChnIndex, float nowCurrent)
{
	//产生1到3之间的随机数
    int randomNum_percent_begin = 1;
	int randomNum_percent=1;//随机产生的偏差百分比，1-3%
	int randomNum_oddeven_begin = 1;//随机产生的odd even，1 or 2
	int randomNum_oddeven_end = 2;//随机产生的odd even，1 or 2
	int randomNum_oddeven = 1;//随机产生的odd even，1 or 2
	
	char tmpStr[256];
	memset(tmpStr, 0, sizeof(tmpStr));
	
	sprintf(tmpStr,"NowVoltage(), cfg_IsDealwithRecvData:%d, carID:%d, loadNum:%d, iChnIndex:%d, nowCurrent:%f, m_ParaMode:%d, m_LoadMode:%d, m_SetValue:%f, m_SetMin:%f, m_SetMax:%f\n ", 
		cfg_IsDealwithRecvData,
		carID,
		loadNum,
		iChnIndex,
		nowCurrent,
		g_AllCar[carID].m_Load[loadNum-1].m_ParaMode,
		g_AllCar[carID].m_Load[loadNum-1].m_LoadMode,
		g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue,
		g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin,
		g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax);
	WriteLog(LEVEL_DEBUG,tmpStr);
		
	if( nowCurrent == 0)
	{
		return -1;
	}
	if (cfg_IsDealwithRecvData == 0)
	{
		return -1;
	}
	if (g_AllCar[carID].m_Load[loadNum-1].m_ParaMode == 0 && g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//当前传入电流值，恒压， 单路测试
	{	
		if ((nowCurrent - g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin*0.85 ) < EPSINON)
		{
			return -1;
		}
		if ((nowCurrent - g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax*1.15 ) > EPSINON)
		{
			return -1;
		}
		//如果当前电压值在当前上下限的 15%误差内
		//未超出设定值，则在 设定值的 正负千分之1-10 波动
		srand((unsigned)time(NULL));
		randomNum_percent = randomNum_percent_begin+rand()%cfg_NoFixDataPercent;
		
		srand((unsigned)time(NULL));
		randomNum_oddeven = randomNum_oddeven_begin+rand()%randomNum_oddeven_end;
		
		if (randomNum_oddeven == 1)
		{
			return (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin+g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)/2*(1000.0+randomNum_percent)/1000.0;
		}else
		{
			return (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin+g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)/2*(1000.0-randomNum_percent)/1000.0;
		}
		
	}else if (g_AllCar[carID].m_Load[loadNum-1].m_ParaMode == 0 && g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//当前传入电流值，恒流,单路测试
	{
		if ( (nowCurrent - g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue*1.1) > EPSINON )
		{
			return -1; //超出设定值的百分之十
		}
		if ( (nowCurrent - g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue*0.9) < EPSINON )
		{
			return -1;//超出设定值的百分之十
		}
		//未超出设定值，则在 设定值的 正负千分之1-3 波动
		srand((unsigned)time(NULL));
		randomNum_percent = randomNum_percent_begin+rand()%cfg_FixDataPercent;
		
		srand((unsigned)time(NULL));
		randomNum_oddeven = randomNum_oddeven_begin+rand()%randomNum_oddeven_end;
		
		if (randomNum_oddeven == 1)
		{
			return g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue*(1000.0+randomNum_percent)/1000.0;
		}else
		{
			return g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue*(1000.0-randomNum_percent)/1000.0;
		}
		
	}
	return -1;
}
/************************************************************************/
/* 处理串口接收到的数据, carID范围[0,15]                                */
/************************************************************************/
void	DealWithSerialPortData(int carID)				
{
	char logBuf[512];
	char cStartFlag = (char)0x6B;				//命令开始标记
	char cEndFlag	= (char)0x6E;				//命令结束标记
	float ret = -1;
	memset(logBuf, 0, sizeof(logBuf));
	
	if ( carID<0 || carID>MAX_CAR_COUNT )
	{
		return;	
	}
	int	existErrorData = FALSE;							//是否存在出错数据
	int startElem = g_AllCar[carID].recvData[0];			//开始位置	
	int loadNum = g_AllCar[carID].recvData[1];			//负载号
	int funNum = g_AllCar[carID].recvData[2];			//功能码
	char endElem = g_AllCar[carID].recvData[38];		//结束元素
	if (loadNum==0 && loadNum==0 && funNum==0 && startElem==0 && endElem ==0)
	{
		return;//此时未收到串口数据
	}
	if (loadNum<0 || loadNum>=MAX_LOAD_PERCAR ||funNum > 0x18 || startElem!=cStartFlag || endElem!= cEndFlag)
	{
		EnterCriticalSection(&g_AllCar[g_curSelTestCar].CriticalSectionRecvBuffer);
		memset(g_AllCar[carID].recvData,0,sizeof(g_AllCar[carID].recvData));
		LeaveCriticalSection(&g_AllCar[g_curSelTestCar].CriticalSectionRecvBuffer);
		sprintf(logBuf,"Error, Serial Data is wrong! carID:%d, loadNum:%d, funNum:%X, startElem:%X, endElem:%X", carID, loadNum, funNum, startElem, endElem);
		WriteLog(LEVEL_ERROR,logBuf);
		return;
	}
	if(LOAD_COMMAND_BEEPON == funNum )
	{
		g_AllCar[carID].m_CMDAlreadyBeepOn=1;//此时底层蜂鸣器响了, modify by lmy 20120415
		g_AllCar[carID].m_CMDBeepOnCounter=0;
		EnterCriticalSection(&g_AllCar[g_curSelTestCar].CriticalSectionRecvBuffer);
		memset(g_AllCar[carID].recvData,0,sizeof(g_AllCar[carID].recvData));
		LeaveCriticalSection(&g_AllCar[g_curSelTestCar].CriticalSectionRecvBuffer);
		return;
	}
	if(LOAD_COMMAND_LIGHTON == funNum )
	{
		g_AllCar[carID].m_CMDAlreadyLightOn=1;//此时底层告警灯亮了, modify by lmy 20120415
		g_AllCar[carID].m_CMDLightOnCounter=0;
		EnterCriticalSection(&g_AllCar[g_curSelTestCar].CriticalSectionRecvBuffer);
		memset(g_AllCar[carID].recvData,0,sizeof(g_AllCar[carID].recvData));
		LeaveCriticalSection(&g_AllCar[g_curSelTestCar].CriticalSectionRecvBuffer);
		return;
	}

	//判断是否主板命令
	if(IsMainBoardCommand(funNum))
	{		
		//当前测试时间位于插入时序的位置,判断当前测试时间是否到了插入时序中
		for(int testItemIndex=0;testItemIndex< g_AllCar[carID].testItemNum; testItemIndex++)
		{
			int testItem=g_AllCar[carID].pTimeSeires[testItemIndex].testItem;
			long startTime=g_AllCar[carID].pTimeSeires[testItemIndex].startTime;
			long testTime=g_AllCar[carID].pTimeSeires[testItemIndex].testTime;				
			
			//插入时序的过程中
			if (g_AllCar[carID].m_TimeCounter*COMMAND_INTERVAL >= startTime*1000 &&
				g_AllCar[carID].m_TimeCounter*COMMAND_INTERVAL < startTime*1000+testTime*1000 ) 
			{
				//在插入时序结构体中标识已回数据
				g_AllCar[carID].pTimeSeires[testItemIndex].recvEvt=1;
				EnterCriticalSection(&g_AllCar[g_curSelTestCar].CriticalSectionRecvBuffer);
				memset(g_AllCar[carID].recvData,0,sizeof(g_AllCar[carID].recvData));
				LeaveCriticalSection(&g_AllCar[g_curSelTestCar].CriticalSectionRecvBuffer);
				return;
			}						
		}	
	}
	//底层返回的，负载状态
	int loadState = (int)g_AllCar[carID].recvData[37];
	if (loadState == 1)
	{
		g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_WORKING;				
		
	}else if (loadState == 2)
	{
		g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_PROTECT;
		g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_ChnHaveBeenError=1;
		g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_ChnHaveBeenError=1;
		g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_ChnHaveBeenError=1;
		g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_ChnHaveBeenError=1;
		EnterCriticalSection(&g_AllCar[g_curSelTestCar].CriticalSectionRecvBuffer);
		memset(g_AllCar[carID].recvData,0,sizeof(g_AllCar[carID].recvData));
		LeaveCriticalSection(&g_AllCar[g_curSelTestCar].CriticalSectionRecvBuffer);
		return;
		
	}if (loadState == 3)
	{
		g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOLINK;
		//return;		
	}else if (loadState == 0)
	{
		g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOSETPARAM;
		EnterCriticalSection(&g_AllCar[g_curSelTestCar].CriticalSectionRecvBuffer);
		memset(g_AllCar[carID].recvData,0,sizeof(g_AllCar[carID].recvData));
		LeaveCriticalSection(&g_AllCar[g_curSelTestCar].CriticalSectionRecvBuffer);
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
			A1 = g_AllCar[carID].recvData[(iChnIndex+1)*4-1];			
			A2 = g_AllCar[carID].recvData[(iChnIndex+1)*4-0];
			B1 = g_AllCar[carID].recvData[(iChnIndex+1)*4+15];			
			B2 = g_AllCar[carID].recvData[(iChnIndex+1)*4+16];
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
			
			//通道,电流计算
			D1 = g_AllCar[carID].recvData[(iChnIndex+1)*4+1];			
			D2 = g_AllCar[carID].recvData[(iChnIndex+1)*4+2];
			E1 = g_AllCar[carID].recvData[(iChnIndex+1)*4+17];			
			E2 = g_AllCar[carID].recvData[(iChnIndex+1)*4+18];
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
			
			//对当前获取到的电压数据进行处理
			ret = -1;
			ret = DealWithSerialPortData_NowVoltage(carID, loadNum, iChnIndex, C);
			sprintf(logBuf,"NowVoltage, cardID:%d, loadNum:%d, iChnIndex:%d, C:%f, ret:%f", carID, loadNum, iChnIndex, C, ret);
			WriteLog(LEVEL_DEBUG,logBuf);
			if (ret == -1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage = C;
			}else
			{
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage = ret;
			}			
			//对当前获取到的电流数据进行处理
			ret = -1;
			ret = DealWithSerialPortData_NowCurrent(carID, loadNum, iChnIndex, F);
			sprintf(logBuf,"NowCurrent, cardID:%d, loadNum:%d, iChnIndex:%d, F:%f, ret:%f", carID, loadNum, iChnIndex, F, ret);
			WriteLog(LEVEL_DEBUG,logBuf);
			if (ret == -1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent = F;
			}else
			{
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent = ret;
			}
						
		}
	}
	//通道[0,3]的错误状态统计
	if (g_AllCar[carID].m_Load[loadNum-1].m_ParaMode==1 && g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//恒流 2路并联 
	{
		//通道0
		iChnIndex=0;
		if(g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError==0)
		{
			g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowCurrent += g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowCurrent;
			g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowVoltage += g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowVoltage;
			g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowVoltage = g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowVoltage/2;
			if ( ChnTestDataCompare(carID, loadNum, 0)==TRUE )
			{
				existErrorData = TRUE;
			}
		}
		//通道1
		g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_ChnState = CHN_STATE_2_PARALELL;
		//通道2
		iChnIndex=2;
		if(g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError==0)
		{
			g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowCurrent += g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowCurrent;
			g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowVoltage += g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowVoltage;
			g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowVoltage = g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowVoltage/2;
			if ( ChnTestDataCompare(carID, loadNum, 2)==TRUE )
			{
				existErrorData = TRUE;
			}
		}
		//通道3
		g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_ChnState = CHN_STATE_2_PARALELL;
		
	}else if(g_AllCar[carID].m_Load[loadNum-1].m_ParaMode==2 && g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )////恒流 4路并联 
	{
		//通道0
		iChnIndex=0;
		if(g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError==0)
		{
			g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowCurrent += g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowCurrent;
			g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowCurrent += g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowCurrent;
			g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowCurrent += g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowCurrent;
			
			g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowVoltage += g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_NowVoltage;
			g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowVoltage += g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowVoltage;
			g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowVoltage += g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_NowVoltage;
			g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowVoltage = g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowVoltage/4;
			if ( ChnTestDataCompare(carID, loadNum, 0)==TRUE )
			{
				existErrorData = TRUE;
			}
		}
		//通道1
		g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_ChnState = CHN_STATE_4_PARALELL;
		//通道2
		g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_ChnState = CHN_STATE_4_PARALELL;
		//通道3
		g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_ChnState = CHN_STATE_4_PARALELL;
		
	}else
	{
		iChnIndex =0;
		if ( ChnTestDataCompare(carID, loadNum, iChnIndex)==TRUE )
		{
			existErrorData = TRUE;
		}
		iChnIndex =1;
		if ( ChnTestDataCompare(carID, loadNum, iChnIndex)==TRUE )
		{
			existErrorData = TRUE;
		}
		iChnIndex =2;
		if ( ChnTestDataCompare(carID, loadNum, iChnIndex)==TRUE )
		{
			existErrorData = TRUE;
		}
		iChnIndex =3;
		if ( ChnTestDataCompare(carID, loadNum, iChnIndex)==TRUE )
		{
			existErrorData = TRUE;
		}
	}			
	
	//从单链表中endPos位置之前的元素
	//出错数据，写入测试数据文件
	if ( g_AllCar[carID].testParam.isSaveData == 1 && TRUE == existErrorData )
	{
		SaveCurrentLoadTestData(carID,loadNum);					
	}	
	
	EnterCriticalSection(&g_AllCar[g_curSelTestCar].CriticalSectionRecvBuffer);
	memset(g_AllCar[carID].recvData,0,sizeof(g_AllCar[carID].recvData));
	LeaveCriticalSection(&g_AllCar[g_curSelTestCar].CriticalSectionRecvBuffer);

}

/************************************************************************/
/* 保存指定负载通道数据                                                 */
/************************************************************************/
void	SaveCurrentLoadTestData(int carID, int loadNum)				
{
	char logBuf[256]={0};
	memset(logBuf,0,sizeof(logBuf));
	

	g_data[carID].WriteData_Ttime("");
	if (g_AllCar[carID].m_Load[loadNum-1].m_ParaMode == 0)
	{
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
	
	}else if (g_AllCar[carID].m_Load[loadNum-1].m_ParaMode == 1)
	{
		sprintf(logBuf,"%d,%0.3f,%0.3f,%s,,,,%0.3f,%0.3f,%s,,, \n",
			loadNum,
			g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowVoltage,
			g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowCurrent,
			GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_ChnState),
			g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowVoltage,
			g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_NowCurrent,
			GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_ChnState));
	

	}else if (g_AllCar[carID].m_Load[loadNum-1].m_ParaMode == 2)
	{
		sprintf(logBuf,"%d,%0.3f,%0.3f,%s,,,,,,,,,\n",
			loadNum,
			g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowVoltage,
			g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_NowCurrent,
			GetChnStateName(g_AllCar[carID].m_Load[loadNum-1].m_Channel[0].m_ChnState));
	
	}
				
	g_data[carID].WriteData(logBuf);
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
	
	if(g_AllCar[carID].m_TimeCounter < 600 ) // 1分钟之后开始记录数据
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
				SaveCurrentLoadTestData(carID,loadIndex+1);				
			}
		}
	}	
}

/************************************************************************/
/* 多媒体定时器处理函数，通用      ,100ms                               */
/************************************************************************/
void	Proc_MutilMediaTimer(int carID,DWORD dwUser)	
{
	int		serialPortCommandType;	//串口命令类型
	int     curLoadNum;				//当前测试负载号, [1,63]
	char	testTimeStr[16]={0};
	char	tmpStr[64];
	char	logBuf[512];
	
	CLoadAgingDlg* pDlg = (CLoadAgingDlg*)dwUser;
	
	memset(tmpStr,0,sizeof(tmpStr));
	memset(logBuf,0,sizeof(logBuf));


	
	
	//分析串口数据
	DealWithSerialPortData(carID);
	
	//保存测试数据
	SaveTestData(carID);
	
		//点击CLEAR后,下发BEEPOFF
	if(g_AllCar[carID].m_CMDBeepClear==1)
	{
		g_AllCar[g_curSelTestCar].m_CMDBeepOffCounter++;
		if(g_AllCar[g_curSelTestCar].m_CMDBeepOffCounter<2)
		{
			return;
		}
		EnterCriticalSection(&g_AllCar[carID].CriticalSectionClear);
		g_AllCar[carID].m_CMDBeepClear=0;//这里要互斥
		g_AllCar[g_curSelTestCar].m_CMDBeepOffCounter=0;
		LeaveCriticalSection(&g_AllCar[carID].CriticalSectionClear);
		WriteSerialPortCommand(carID, LOAD_COMMAND_BEEPOFF, 0);			
		//WriteSerialPortCommand(carID, LOAD_COMMAND_BEEPOFF, 0);			
		return;	
		
	}
	//点击CLEAR后,下发LIGHTOFF
	if(g_AllCar[carID].m_CMDLightClear==1)
	{
		g_AllCar[g_curSelTestCar].m_CMDLightOffCounter++;
		if(g_AllCar[g_curSelTestCar].m_CMDLightOffCounter<2)
		{
			return;
		}
		for(int iLoad=g_AllCar[carID].testParam.startLoadNum-1; iLoad<g_AllCar[carID].testParam.endLoadNum; iLoad++)
		{
			g_AllCar[carID].m_Load[iLoad].m_Channel[0].m_ChnHaveBeenError=0;
			g_AllCar[carID].m_Load[iLoad].m_Channel[1].m_ChnHaveBeenError=0;
			g_AllCar[carID].m_Load[iLoad].m_Channel[2].m_ChnHaveBeenError=0;
			g_AllCar[carID].m_Load[iLoad].m_Channel[3].m_ChnHaveBeenError=0;
			g_AllCar[carID].m_Load[iLoad].m_Channel[0].m_ChnHaveBeenRight=0;
			g_AllCar[carID].m_Load[iLoad].m_Channel[1].m_ChnHaveBeenRight=0;
			g_AllCar[carID].m_Load[iLoad].m_Channel[2].m_ChnHaveBeenRight=0;
			g_AllCar[carID].m_Load[iLoad].m_Channel[3].m_ChnHaveBeenRight=0;
			g_AllCar[carID].m_CMDAlreadyBeepOn=0;
			g_AllCar[carID].m_CMDAlreadyLightOn=0;
		}
		EnterCriticalSection(&g_AllCar[carID].CriticalSectionClear);
		g_AllCar[carID].m_CMDLightClear=0;//这里要互斥
		g_AllCar[g_curSelTestCar].m_CMDLightOffCounter=0;
		LeaveCriticalSection(&g_AllCar[carID].CriticalSectionClear);
		WriteSerialPortCommand(carID, LOAD_COMMAND_LIGHTOFF, 0);
		//WriteSerialPortCommand(carID, LOAD_COMMAND_LIGHTOFF, 0);
		return;
	}
	//当前测试车状态，停止测试
	if(	g_AllCar[carID].m_CarState == CAR_STATE_TESTFINISH || g_AllCar[carID].m_CarState == CAR_STATE_IMPORTED )
	{	
		//关闭定时器
		switch(carID)
		{
		case 0:
			pDlg->m_MutilTimer_Car1.Stop();			
			break;
		case 1:
			pDlg->m_MutilTimer_Car2.Stop();			
			break;
		case 2:
			pDlg->m_MutilTimer_Car3.Stop();			
			break;
		case 3:
			pDlg->m_MutilTimer_Car4.Stop();			
			break;
		case 4:
			pDlg->m_MutilTimer_Car5.Stop();			
			break;
		case 5:
			pDlg->m_MutilTimer_Car6.Stop();			
			break;
		case 6:
			pDlg->m_MutilTimer_Car7.Stop();			
			break;
		case 7:
			pDlg->m_MutilTimer_Car8.Stop();			
			break;
		case 8:
			pDlg->m_MutilTimer_Car9.Stop();			
			break;
		case 9:
			pDlg->m_MutilTimer_Car10.Stop();			
			break;
		case 10:
			pDlg->m_MutilTimer_Car11.Stop();			
			break;
		case 11:
			pDlg->m_MutilTimer_Car12.Stop();			
			break;
		case 12:
			pDlg->m_MutilTimer_Car13.Stop();			
			break;
		case 13:
			pDlg->m_MutilTimer_Car14.Stop();			
			break;
		case 14:
			pDlg->m_MutilTimer_Car15.Stop();			
			break;
		case 15:
			pDlg->m_MutilTimer_Car16.Stop();			
			break;
		}
	
		WriteSerialPortCommand(carID, LOAD_COMMAND_STOP, 64);	//下发串口命令，停止测试
		WriteSerialPortCommand(carID, LOAD_COMMAND_STOP, 64);	//下发串口命令，停止测试
		Sleep(10);//此命令不需要返回
		WriteSerialPortCommand(carID, LOAD_COMMAND_STOP, 64);	//下发串口命令，停止测试
		
		pDlg->GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(TRUE);		
		if(g_curSelTestCar == carID)		//如果主界面是当前测试车
		{
			sprintf(tmpStr,"%s开始测试",GetTestCarName(carID));
			pDlg->SetDlgItemText(IDC_BUTTON_TEST, tmpStr);
			pDlg->m_BtnTest.SetIcon(IDI_ICON_STARTTEST);
			if(	g_AllCar[carID].m_CarState == CAR_STATE_TESTFINISH )
			{
				sprintf(tmpStr,"%s测试完成",GetTestCarName(carID));
				AfxMessageBox(tmpStr);
			}
		}
		sprintf(logBuf,"carID:%d, 停止测试.",carID);
		WriteLog(LEVEL_DEBUG,logBuf);
		
		return;//停止测试
		
	}else if ( g_AllCar[carID].m_CarState == CAR_STATE_TESTING )
	{
		//测试流程，下发命令
		curLoadNum = GetCurLoadNum(carID);										//获取当前待测试负载号
		serialPortCommandType = GetSerialPortCommandType(carID,curLoadNum);		//当前测试车，当前测试负载，获取应下发命令类型
		WriteSerialPortCommand(carID, serialPortCommandType, curLoadNum);		//下发串口命令
	}
	
	g_AllCar[carID].m_TimeCounter++;							//测试时间统计
	g_AllCar[carID].m_curLoadNum++;							//当前测试负载号，循环测试负载
	
	
	//总的测试时间到了,单位ms
	LONGLONG lTotlalTime = g_AllCar[carID].m_TimeCounter*COMMAND_INTERVAL;//运行了多少ms
	if ( lTotlalTime>= g_AllCar[carID].testParam.totalTestTime*60*1000 )
	{	
		sprintf(tmpStr,"%s:停止测试, 时间到了.",GetTestCarName(carID));
		WriteLog(LEVEL_DEBUG,tmpStr);	
		g_AllCar[carID].m_CarState = CAR_STATE_TESTFINISH;		//测试状态置为停止
		
	}
}			
