#include "stdafx.h"
#include "XMS_TestDial.h"
#include "XMS_TestDialDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CALLED_RECORD_TIME	8000L*10	//被叫侧录音时长，10秒
#define CALLER_RECORD_TIME	8000L*15	//主叫侧录音时长，多录制15秒

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"
#include "DJAcsDevState.h"

#include "XMS_Dial_Sub.H"
#include "XMS_Dial_String.H"
#include "XMS_Dial_Event.H"
#include "Result.h"
#include "LogFile.h"
// Special code for CAS(SS1)
#include "C:\\DJKeygoe\\Samples\\CAS_Common_Code\\CAS_Common_Cfg.h"		
CmdParamData_CAS_t g_Param_CAS;
// end of code for CAS(SS1)

// -----------------------------------------------------------------------
// variable declaration
extern CXMS_TestDialApp theApp;
CXMS_TestDialDlg	*pdlg;
CResult             *pdlgResult;
DWORD g_dwThreadID = 0;
bool g_threadRunning = TRUE;	//定义的全局变量，用于控制线程的运行；
// var about XMS_Dial.INI
char			cfg_IniName[MAX_FILE_NAME_LEN] = "";
char			cfg_IniShortName[] = "\\K200_LoopTest_DTMF.ini";

ServerID_t		cfg_ServerID;
int				cfg_iCalledLen = 5;
char			cfg_CallingNum[32] = "83636988";
char			cfg_VocPath[128] = "";	
int				cfg_iDispChnl = 32;		// always disp 32 channel
int				cfg_iVoiceRule;		// search voice in Fix Relationship
int				cfg_iPartWorkModuleID[256]={0};
char            cfg_chPartWorkModuleID[256] = {0};
char			cfg_s8CallerE1[10]={0};//主叫E1号
char			cfg_s8CalledE1[10]={0};//被叫E1号
int				cfg_iCallerE1[MAX_E1_NUM]={0};//主叫E1号
int				cfg_iCalledE1[MAX_E1_NUM]={0};//被叫E1号char				
char			cfg_SimCalledNum[32] = "12345";
int             cfg_iVoiceCheck = 1;
float           cfg_iTestTime = 1.0;
int				cfg_LogOn=1;			//是否开启日志
int				cfg_LogLevel=0;			//日志级别 
int				cfg_AppUnitID=28;
long             g_iTestStartTime = 0;
int				 g_iElapseTime = 100000;  //100 个 1000 毫秒

LONGLONG    g_iTotalCnt = 0; 
LONGLONG    g_iTotalCallErr = 0;
LONGLONG    g_iTotalCheckErr = 0;
LONGLONG    g_iEstablishSum = 0;
LONGLONG    g_iCallSuc = 0;

// var about work
ACSHandle_t		g_acsHandle = -1;


// var in XMS_Dail_Event.CPP
extern TYPE_XMS_DSP_DEVICE_RES_DEMO	AllDeviceRes[MAX_DSP_MODULE_NUMBER_OF_XMS];

extern int						g_iTotalPcm;
extern int						g_iTotalPcmOpened;
extern TYPE_CHANNEL_MAP_TABLE	MapTable_Pcm[MAX_PCM_NUM_IN_THIS_DEMO];

extern int						g_iTotalTrunk;
extern int						g_iTotalTrunkOpened;
extern TYPE_CHANNEL_MAP_TABLE	MapTable_Trunk[MAX_TRUNK_NUM_IN_THIS_DEMO];

extern int						g_iTotalUser;
extern int						g_iTotalUserOpened;
extern TYPE_CHANNEL_MAP_TABLE	MapTable_User[MAX_TRUNK_NUM_IN_THIS_DEMO];

extern int						g_iTotalVoice;
extern int						g_iTotalVoiceOpened;
extern int						g_iTotalVoiceFree;
extern TYPE_CHANNEL_MAP_TABLE	MapTable_Voice[MAX_TRUNK_NUM_IN_THIS_DEMO];

extern int						g_iTotalModule;
extern DJ_S8					MapTable_Module[MAX_DSP_MODULE_NUMBER_OF_XMS];

CmdParamData_AnalogUser_t    g_AnalogUser;
CmdParamData_AnalogTrunk_t   g_AnalogTrunk;
// -----------------------------------------------------------------------
// local function decalration
void	WriteToConfig(void);
void	ReadFromConfig(void);
void	InitTextBox(void);
void	FetchFromText();

void	InitListMain(void);
void	InitListMsg(void);
bool    GetFilePathName(TRUNK_STRUCT *pOneTrunk, char *pchFilePath);

char dtmf_table[16] = { 'D', '1','2','3',
'4','5','6','7',
'8','9','0','*',
'#', 'A','B','C' };

/************************************************************************/
/* User先放音，判断收到的DTMF是否正确                                   */
/************************************************************************/
bool IsCorrectDtmf(TRUNK_STRUCT *pOneTrunk)
{
	int i;
	
	if (pOneTrunk == NULL)
		return false;
	
	if (pOneTrunk->DtmfCount != 10)
	{
		return false;
	}
	
	for (i=0; i<10; i++)
	{
		if (pOneTrunk->DtmfBuf[i] != dtmf_table[(pOneTrunk->iSeqID%15)])
			return false;
	}
	
	return true;
}


/************************************************************************/
/* 写测试结果                                                           */
/************************************************************************/
void WriteTestResult()
{
	char   TmpStr[256] = {0};

	sprintf(TmpStr, "\n*****************Test End**********\n");
	WriteLog(LEVEL_INFO, TmpStr);
	
	pdlg->GetDlgItem(IDC_STATIC_Count)->GetWindowText(TmpStr, sizeof(TmpStr));
	sprintf(TmpStr, "**呼叫次数：%s                            \n", TmpStr);
	WriteLog(LEVEL_INFO, TmpStr);

	pdlg->GetDlgItem(IDC_STATIC_Fail)->GetWindowText(TmpStr, sizeof(TmpStr));
	sprintf(TmpStr, "**呼损：%s                                \n", TmpStr);
	WriteLog(LEVEL_INFO, TmpStr);
	
	pdlg->GetDlgItem(IDC_STATIC_DtmfErr)->GetWindowText(TmpStr, sizeof(TmpStr));
	sprintf(TmpStr, "**误码次数: %s                            \n", TmpStr);
	WriteLog(LEVEL_INFO, TmpStr);

	pdlg->GetDlgItem(IDC_STATIC_SucRate)->GetWindowText(TmpStr, sizeof(TmpStr));
	sprintf(TmpStr, "**失败率(呼损\\误码): %s                  \n", TmpStr);
	WriteLog(LEVEL_INFO, TmpStr);

	pdlg->GetDlgItem(IDC_STATIC_TestTimer)->GetWindowText(TmpStr, sizeof(TmpStr));
	sprintf(TmpStr, "**测试时间: %s                            \n", TmpStr);
	WriteLog(LEVEL_INFO, TmpStr);

	sprintf(TmpStr, "******************************************************\n");
	WriteLog(LEVEL_INFO, TmpStr);
	
}

// -----------------------------------------------------------------------
int    SplitStr2Int(DJ_S8 *str, DJ_S8 *sep, int buf[])
{
	DJ_U32   i = 0;
	DJ_S8    *p = NULL;
	DJ_S8    *pBuf = NULL;
	
	if ( (str==NULL)||(strlen(str)==0)||(sep==NULL)||(strlen(sep)==0) )
	{
		return -1;
	}
	
	pBuf = str;
	p = strtok(pBuf, sep);
	
	while ( p != NULL )
	{
		buf[i++] = atoi(p);
		p = strtok(NULL, sep);
	}
	
	return i;
}

// -----------------------------------------------------------------------
void	ReadFromConfig(void)
{
	char TmpStr[200] = {0};
	
	GetCurrentDirectory ( MAX_FILE_NAME_LEN-32, cfg_IniName );
	strcat ( cfg_IniName, cfg_IniShortName );
	
	GetPrivateProfileString("ConfigInfo", "TestTime", "0.00", TmpStr, sizeof(TmpStr), cfg_IniName);
	sscanf(TmpStr, "%f", &cfg_iTestTime);
	
	GetPrivateProfileString ( "ConfigInfo", "IpAddr", "192.168.0.8", cfg_ServerID.m_s8ServerIp, sizeof(cfg_ServerID.m_s8ServerIp), cfg_IniName);
	
	cfg_ServerID.m_u32ServerPort = GetPrivateProfileInt ( "ConfigInfo", "Port", 9001, cfg_IniName);
	
	GetPrivateProfileString ( "ConfigInfo", "VocPath", "..\\VOC\\", cfg_VocPath, sizeof(cfg_VocPath), cfg_IniName);

	cfg_AppUnitID = GetPrivateProfileInt ( "ConfigInfo", "AppUnitID", 28, cfg_IniName);

	cfg_iCalledLen = GetPrivateProfileInt ( "ConfigInfo", "CalledLen", 5, cfg_IniName);
	if ( cfg_iCalledLen > 20 )
		cfg_iCalledLen = 20;
	
	GetPrivateProfileString ( "ConfigInfo", "CallingNum", "83636988", cfg_CallingNum, sizeof(cfg_CallingNum), cfg_IniName);
	
	GetPrivateProfileString ( "ConfigInfo", "SimCalledRNum", "12345", cfg_SimCalledNum, sizeof(cfg_SimCalledNum), cfg_IniName);	
	
	cfg_iVoiceRule = GetPrivateProfileInt ( "ConfigInfo", "VoiceRule", 0, cfg_IniName);
	cfg_LogOn = GetPrivateProfileInt ( "ConfigInfo", "LogOn", 0, cfg_IniName);
	cfg_LogLevel = GetPrivateProfileInt ( "ConfigInfo", "LogLevel", 0, cfg_IniName);
	
	GetPrivateProfileString("ConfigInfo", "PartWorkModuleID","",cfg_chPartWorkModuleID, sizeof(cfg_chPartWorkModuleID), cfg_IniName); 
	strncpy(TmpStr, cfg_chPartWorkModuleID, sizeof(TmpStr));	
	SplitStr2Int(TmpStr, ",", cfg_iPartWorkModuleID);
	//主叫E1号
	GetPrivateProfileString("ConfigInfo", "CallerE1","",cfg_s8CallerE1, sizeof(cfg_s8CallerE1), cfg_IniName); 
	strncpy(TmpStr, cfg_s8CallerE1, sizeof(TmpStr));	
	SplitStr2Int(TmpStr, ",", cfg_iCallerE1);
	//被叫E1号
	GetPrivateProfileString("ConfigInfo", "CalledE1","",cfg_s8CalledE1, sizeof(cfg_s8CalledE1), cfg_IniName); 
	strncpy(TmpStr, cfg_s8CalledE1, sizeof(TmpStr));	
	SplitStr2Int(TmpStr, ",", cfg_iCalledE1);

	cfg_iVoiceCheck = GetPrivateProfileInt ("ConfigInfo", "VoiceCheck", 0, cfg_IniName);
}

void	WriteToConfig(void)
{
	char	TmpStr[256]={0};
	
	sprintf(TmpStr, "%f", cfg_iTestTime);
	WritePrivateProfileString("ConfigInfo", "TestTime", TmpStr, cfg_IniName);
	
	sprintf(TmpStr, "%s", cfg_chPartWorkModuleID);
	WritePrivateProfileString("ConfigInfo", "PartWorkModuleID", TmpStr, cfg_IniName);
		
	sprintf(TmpStr, "%s", cfg_VocPath);
	WritePrivateProfileString("ConfigInfo", "VocPath", TmpStr, cfg_IniName);
	
	sprintf(TmpStr, "%s", cfg_ServerID.m_s8ServerIp);
	WritePrivateProfileString("ConfigInfo", "IpAddr", TmpStr, cfg_IniName);

	sprintf(TmpStr, "%s", cfg_s8CallerE1);
	WritePrivateProfileString("ConfigInfo", "CallerE1", TmpStr, cfg_IniName);

	sprintf(TmpStr, "%s", cfg_s8CalledE1);
	WritePrivateProfileString("ConfigInfo", "CalledE1", TmpStr, cfg_IniName);

}

void	InitTextBox(void)
{
	char	TmpStr[256];
	
	sprintf(TmpStr, "%f", cfg_iTestTime);
	pdlg->GetDlgItem(IDC_EDIT_TestTime)->SetWindowText(TmpStr);
	
	sprintf(TmpStr, "%s", cfg_chPartWorkModuleID);
	pdlg->GetDlgItem(IDC_EDIT_ModuleID)->SetWindowText(TmpStr);
	
	sprintf(TmpStr, "%s", cfg_VocPath);
	pdlg->GetDlgItem(IDC_EDIT_VOCPATH)->SetWindowText(TmpStr);
	
	sprintf(TmpStr, "%s", cfg_ServerID.m_s8ServerIp);
	pdlg->GetDlgItem(IDC_EDIT_IP)->SetWindowText(TmpStr);

	sprintf(TmpStr, "%s", cfg_s8CallerE1);
	pdlg->GetDlgItem(IDC_EDIT_CALLERE1)->SetWindowText(TmpStr);

	sprintf(TmpStr, "%s", cfg_s8CalledE1);
	pdlg->GetDlgItem(IDC_EDIT_CALLEDE1)->SetWindowText(TmpStr);

}

void	FetchFromText(void)
{
	char	TmpStr[256];
	
	pdlg->GetDlgItem(IDC_EDIT_TestTime)->GetWindowText(TmpStr, sizeof(TmpStr));
	sscanf(TmpStr, "%f", &cfg_iTestTime);
	
	pdlg->GetDlgItem(IDC_EDIT_ModuleID)->GetWindowText(cfg_chPartWorkModuleID, sizeof(cfg_chPartWorkModuleID));

	pdlg->GetDlgItem(IDC_EDIT_VOCPATH)->GetWindowText(cfg_VocPath, sizeof(cfg_VocPath));

	pdlg->GetDlgItem(IDC_EDIT_IP)->GetWindowText(cfg_ServerID.m_s8ServerIp, sizeof(cfg_ServerID.m_s8ServerIp));

	pdlg->GetDlgItem(IDC_EDIT_CALLERE1)->GetWindowText(cfg_s8CallerE1, sizeof(cfg_s8CallerE1));

	pdlg->GetDlgItem(IDC_EDIT_CALLEDE1)->GetWindowText(cfg_s8CalledE1, sizeof(cfg_s8CalledE1));
	
}

void InitListMsg( void )
{
	int i = pdlg->m_ListMsg.GetHorizontalExtent( );
	pdlg->m_ListMsg.SetHorizontalExtent( i + 1000 );
	pdlg->m_ListMsg.ResetContent( );
}
// --------------------------------------------------------------------------------
void	InitListMain(void)
{
	LV_COLUMN	lvc;
	DWORD dwExStyle;
	
	if (pdlg == NULL)
		return;
	
	dwExStyle = LVS_EX_FULLROWSELECT | /*LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES |*/
		LVS_EX_HEADERDRAGDROP;// | LVS_EX_TRACKSELECT;
	pdlg->m_ListMain.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(dwExStyle));	
	
	lvc.mask =  LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	
	lvc.iSubItem = 0;
	lvc.pszText = "ID" ;
	lvc.cx = 30;
	pdlg->m_ListMain.InsertColumn ( 0, &lvc ); 
	
	lvc.iSubItem = 1;
	lvc.pszText = "Mod,Ch" ;
	lvc.cx = 50;
	pdlg->m_ListMain.InsertColumn ( 1, &lvc ); 
	
	lvc.iSubItem = 2;
	lvc.pszText = "Type" ;
	lvc.cx = 60;
	pdlg->m_ListMain.InsertColumn ( 2, &lvc ); 
	
	lvc.iSubItem = 3;
	lvc.pszText = "LineState" ;
	lvc.cx = 70;
	pdlg->m_ListMain.InsertColumn ( 3, &lvc ); 
	
	lvc.iSubItem = 4;
	lvc.pszText = "State" ;
	lvc.cx = 70;
	pdlg->m_ListMain.InsertColumn ( 4, &lvc ); 
	
	lvc.iSubItem = 5;
	lvc.pszText = "收DTMF码" ;
	lvc.cx = 80;
	pdlg->m_ListMain.InsertColumn ( 5, &lvc ); 
	
	lvc.iSubItem = 6;
	lvc.pszText = "Total/CallFail/CheckFail";
	lvc.cx = 80;
	pdlg->m_ListMain.InsertColumn(6, &lvc);

	lvc.iSubItem = 7;
	lvc.pszText = "VocDev" ;
	lvc.cx = 60;
	pdlg->m_ListMain.InsertColumn ( 7, &lvc ); 

	lvc.iSubItem = 8;
	lvc.pszText = "FailReason" ;
	lvc.cx = 50;
	pdlg->m_ListMain.InsertColumn ( 8, &lvc ); 
	
	lvc.iSubItem = 9;
	lvc.pszText = "Establish";
	lvc.cx = 50;
	pdlg->m_ListMain.InsertColumn(9, &lvc);
	
	lvc.iSubItem = 10;
	lvc.pszText = "Called" ;
	lvc.cx = 60;
	pdlg->m_ListMain.InsertColumn (10, &lvc ); 
	
	lvc.iSubItem = 11;
	lvc.pszText = "Calling";
	lvc.cx = 50;
	pdlg->m_ListMain.InsertColumn(11, &lvc); 
}

int		CalDispRow ( int iSeqID )
{
	int	iRet;
	
	if ( cfg_iDispChnl == 30 )
	{
		if ( (iSeqID % 16) == 0 )
			return -1;
		
		iRet = (iSeqID / 32) * 30 + (iSeqID % 32);
		iRet --;
		if ( (iSeqID % 32) > 16 )
			iRet --;
		return iRet;
		
	}
	
	if ( cfg_iDispChnl == 31 )
	{
		if ( (iSeqID % 32) == 0 )
			return -1;
		
		iRet = (iSeqID / 32) * 31 + (iSeqID % 32);
		iRet --;
		return iRet;
	}
	
	return iSeqID;
}

void DrawMain_LineState( TRUNK_STRUCT *pOneTrunk )
{
	char StateStr[100];
	int	 iDispRow;
	
	if (pOneTrunk == NULL || pdlg == NULL)
		return;
	
	iDispRow = CalDispRow(pOneTrunk->iSeqID); 
	if ( iDispRow < 0 )	return;
	
	// 
	GetString_LineState ( StateStr, pOneTrunk->iLineState );
	pdlg->m_ListMain.SetItemText ( iDispRow, 3, StateStr ); 
}

void DrawMain_State( TRUNK_STRUCT *pOneTrunk )
{
	char StateStr[100];
	int	 iDispRow;
	
	if (pOneTrunk == NULL || pdlg == NULL)
		return;
	
	iDispRow = CalDispRow(pOneTrunk->iSeqID); 
	if ( iDispRow < 0 )	return;
	//
	switch( pOneTrunk->State ) 
	{
	case TRK_WAITOPEN:
		strcpy(StateStr,"Wait Open"); 
		break;
		
	case TRK_FREE:		
		strcpy(StateStr,"空闲"); 
		break ;

	case TRK_RING:		
		strcpy(StateStr,"振铃"); 
		break ;		
				
	case TRK_NOTHANDLE:
		strcpy(StateStr,"不处理");
		break ;
		
	case TRK_CALLOUT:
		strcpy(StateStr,"呼出");
		break;
		
	case TRK_CALLOUT1:
		strcpy(StateStr, "呼出");
		break;
		
	case TRK_WAIT_LINKOK:
		strcpy(StateStr,"等Link完成");
		break;
		
	case TRK_PLAY:
		strcpy(StateStr, "放音");
		break;
		
	case TRK_GETDTMF:
		strcpy(StateStr, "收DTMF码");
		break;
		
	case TRK_FAIL:
		strcpy(StateStr,"呼出失败");
		break;
		
	case TRK_WAITHANGUP:
		strcpy(StateStr, "等待挂机");
		break;
		
	case TRK_WAITSTOP:
		strcpy(StateStr, "测试停止");
		break;
		
	case TRK_RECORDFILE:
		strcpy(StateStr, "录音&收码");
		break;
		
	case TRK_PLAYRECORD:	
		strcpy(StateStr, "放音");
		break;
		// new add for XMS
	case TRK_WAIT_ANSWERCALL:
		strcpy(StateStr, "等待应答");
		break;
		
	default:
		strcpy(StateStr,"other");
		break;
	}
	
	pdlg->m_ListMain.SetItemText ( iDispRow, 4, StateStr ); 
}


void DrawMain_VocInfo( TRUNK_STRUCT *pOneTrunk )
{
	char StateStr[100];
	int	 iDispRow;
	DeviceID_t	*pDev;
	
	if (pOneTrunk == NULL || pdlg == NULL)
		return;
	
	iDispRow = CalDispRow(pOneTrunk->iSeqID); 
	if ( iDispRow < 0 )	return;
	
	if ( pOneTrunk->VocDevID.m_s16DeviceMain != 0 )
	{
		pDev = &pOneTrunk->VocDevID;
		sprintf ( StateStr, "%d,%d", pDev->m_s8ModuleID, pDev->m_s16ChannelID ); 
	}
	else
		strcpy(StateStr, "");
	
	pdlg->m_ListMain.SetItemText ( iDispRow, 7, StateStr ); 
}

void DrawMain_EstableTime(TRUNK_STRUCT *pOneTrunk)
{
	char StateStr[100];
	int  iDispRow;
	
	if (pOneTrunk == NULL || pdlg == NULL)
		return;
	
	iDispRow = CalDispRow(pOneTrunk->iSeqID);
	if (iDispRow < 0) 
		return;
	
	sprintf(StateStr, "%d", pOneTrunk->iEstablishTime);
	pdlg->m_ListMain.SetItemText(iDispRow, 9, StateStr);
}

void DrawMain_Reason ( TRUNK_STRUCT *pOneTrunk, DJ_S32 s32ErrCode, DJ_S32 s32ClearCause )
{
	char StateStr[256]={0};
	int	 iDispRow;
	
	if (pOneTrunk == NULL || pdlg == NULL)
		return;
	
	iDispRow = CalDispRow(pOneTrunk->iSeqID); 
	if ( iDispRow < 0 )	return;
	
	sprintf ( StateStr, "Err=%d,Cause=%d", s32ErrCode, s32ClearCause ); 		
	pdlg->m_ListMain.SetItemText ( iDispRow, 8, StateStr ); 

	sprintf(StateStr, "(%s, %d, %d ), Err=%d,Cause=%d", 
		GetString_DeviceMain(pOneTrunk->deviceID.m_s16DeviceMain),
		pOneTrunk->deviceID.m_s8ModuleID,
		pOneTrunk->deviceID.m_s16ChannelID,
		s32ErrCode, s32ClearCause);
	WriteLog(LEVEL_ERROR, StateStr);
	AddMsg(StateStr);
}



void DrawTrunk_DTMF( TRUNK_STRUCT *pOneTrunk )
{
	char StateStr[100];
	int	 iDispRow;
	
	if (pOneTrunk == NULL || pdlg == NULL)
		return;
	
	iDispRow = pOneTrunk->iSeqID;
	
	strcpy(StateStr,pOneTrunk->DtmfBuf);
	pdlg->m_ListMain.SetItemText ( iDispRow, 5, StateStr ); 
	
}
void DrawMain_CallInfo( TRUNK_STRUCT *pOneTrunk )
{
	char StateStr[100];
	int	 iDispRow;
	
	iDispRow = CalDispRow(pOneTrunk->iSeqID); 
	if ( iDispRow < 0 )	return;
	
	strcpy ( StateStr, pOneTrunk->CalleeCode );
	pdlg->m_ListMain.SetItemText ( iDispRow, 10, StateStr ); 
	
	strcpy ( StateStr, pOneTrunk->CallerCode );
	pdlg->m_ListMain.SetItemText ( iDispRow, 11, StateStr ); 
}
// -------------------------------------------------------------------------------------------------
void	InitListCount(void)
{
	LV_COLUMN	lvc;
	DWORD dwExStyle;
	
	if (NULL == pdlg)
		return;
	
	dwExStyle = LVS_EX_FULLROWSELECT | /*LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES |*/
		LVS_EX_HEADERDRAGDROP;// | LVS_EX_TRACKSELECT;
	pdlg->m_ListCount.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(dwExStyle));	
	
	lvc.mask =  LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	
	lvc.iSubItem = 0;
	lvc.pszText = "ID" ;
	lvc.cx = 40;
	pdlg->m_ListCount.InsertColumn ( 0, &lvc ); 
	
	lvc.iSubItem = 1;
	lvc.pszText = "ModID" ;
	lvc.cx = 45;
	pdlg->m_ListCount.InsertColumn ( 1, &lvc ); 
	
	lvc.iSubItem = 2;
	lvc.pszText = "Pcm/Open" ;
	lvc.cx = 70;
	pdlg->m_ListCount.InsertColumn ( 2, &lvc ); 
	
	lvc.iSubItem = 3;
	lvc.pszText = "Trk/Open" ;
	lvc.cx = 70;
	pdlg->m_ListCount.InsertColumn ( 3, &lvc ); 
	
	lvc.iSubItem = 4;
	lvc.pszText = "Voc/Open" ;
	lvc.cx = 70;
	pdlg->m_ListCount.InsertColumn ( 4, &lvc ); 
	
	lvc.iSubItem = 5;
	lvc.pszText = "VocFree" ;
	lvc.cx = 55;
	pdlg->m_ListCount.InsertColumn ( 5, &lvc ); 
}

void	DrawCount_Pcm ( DJ_S8	s8ModID )
{
	char	TmpStr[256];
	int		iDispRow;
	
	if (NULL == pdlg)
		return ;
	
	iDispRow = AllDeviceRes[s8ModID].iSeqID + 1;
	
	sprintf ( TmpStr, "%3d /%3d", AllDeviceRes[s8ModID].lPcmNum, AllDeviceRes[s8ModID].lPcmOpened );
	pdlg->m_ListCount.SetItemText ( iDispRow, 2, TmpStr );
	
	// total
	iDispRow = 0;
	sprintf ( TmpStr, "%3d /%3d", g_iTotalPcm, g_iTotalPcmOpened );
	pdlg->m_ListCount.SetItemText ( iDispRow, 2, TmpStr );
	
}

void	DrawCount_Trunk ( DJ_S8	s8ModID )
{
	char	TmpStr[256];
	int		iDispRow;
	
	if (pdlg == NULL)
		return;
	
	iDispRow = AllDeviceRes[s8ModID].iSeqID + 1;
	
	sprintf ( TmpStr, "%3d/%3d", AllDeviceRes[s8ModID].lTrunkNum, AllDeviceRes[s8ModID].lTrunkOpened );
	pdlg->m_ListCount.SetItemText ( iDispRow, 3, TmpStr );
	
	// total
	iDispRow = 0;
	sprintf ( TmpStr, "%3d/%3d", g_iTotalTrunk, g_iTotalTrunkOpened );
	pdlg->m_ListCount.SetItemText ( iDispRow, 3, TmpStr );
}

void	DrawCount_Voc ( DJ_S8	s8ModID )
{
	char	TmpStr[256];
	int		iDispRow;
	
	if (NULL == pdlg)
		return;
	
	iDispRow = AllDeviceRes[s8ModID].iSeqID + 1;
	
	sprintf ( TmpStr, "%3d/%3d", AllDeviceRes[s8ModID].lVocNum, AllDeviceRes[s8ModID].lVocOpened );
	pdlg->m_ListCount.SetItemText ( iDispRow, 4, TmpStr );
	
	sprintf ( TmpStr, "%3d", AllDeviceRes[s8ModID].lVocFreeNum );
	pdlg->m_ListCount.SetItemText ( iDispRow, 5, TmpStr );
	
	// total
	iDispRow = 0;
	sprintf ( TmpStr, "%3d/%3d", g_iTotalVoice, g_iTotalVoiceOpened );
	pdlg->m_ListCount.SetItemText ( iDispRow, 4, TmpStr );
	
	sprintf ( TmpStr, "%3d", g_iTotalVoiceFree );
	pdlg->m_ListCount.SetItemText ( iDispRow, 5, TmpStr );
}


// -------------------------------------------------------------------------------------------------
void	ReDrawAll (void)
{
	char	TmpStr[256];
	int		i, iDispRow;
	TRUNK_STRUCT	*pOneTrunk;
	
	if (pdlg == NULL)
		return;
	
	// m_ListMain
	pdlg->m_ListMain.DeleteAllItems ();
	for ( i = 0; i < g_iTotalTrunk; i ++ )
	{
		iDispRow = CalDispRow ( i ); 
		if ( iDispRow < 0 )	continue;
		
		// 固定不变的内容
		sprintf ( TmpStr, "%3d", iDispRow );
		pdlg->m_ListMain.InsertItem ( iDispRow, TmpStr );
		
		sprintf ( TmpStr, "%d,%d", MapTable_Trunk[i].m_s8ModuleID, MapTable_Trunk[i].m_s16ChannelID );
		pdlg->m_ListMain.SetItemText ( iDispRow, 1, TmpStr );
		
		pOneTrunk = &M_OneTrunk(MapTable_Trunk[i]);
		pdlg->m_ListMain.SetItemText ( iDispRow, 2, GetString_DeviceSub( pOneTrunk->deviceID.m_s16DeviceSub) );
		
		// 可能改变的内容
		DrawMain_LineState ( pOneTrunk );
		DrawMain_State ( pOneTrunk );	
	}
	
	
	// m_ListCount
	pdlg->m_ListCount.DeleteAllItems ();
	// 先添加行
	pdlg->m_ListCount.InsertItem ( 0, "Total" );
	for ( i = 0; i < g_iTotalModule; i ++ )
	{
		sprintf ( TmpStr, "%3d", i );
		pdlg->m_ListCount.InsertItem ( i+1, TmpStr );
		
		sprintf ( TmpStr, "%3d", MapTable_Module[i] );
		pdlg->m_ListCount.SetItemText ( i+1, 1, TmpStr );
	}
	
	// 再显示内容
	for ( i = 0; i < g_iTotalModule; i ++ )
	{
		DrawCount_Pcm ( MapTable_Module[i] );
		DrawCount_Trunk ( MapTable_Module[i] );
		DrawCount_Voc ( MapTable_Module[i] );
	}
}

// --------------------------------------------------------------------------------
bool	InitSystem()
{
	RetCode_t	r=0;
	char		MsgStr[256]={0};
	
	pdlg = (CXMS_TestDialDlg *)theApp.m_pMainWnd;
	
	// Read From "XMS_Dial.INI"
	ReadFromConfig();
	
	InitLogCfg();//根据配置文件信息初始化日志
	
	// Special code for CAS(SS1)
	// Read From "C:\\DJXMS\\Samples\\CAS_Common_Code\\SaXMS_CAS_Cfg.INI"
	if ( CAS_Common_Cfg_ReadCfg ( &g_Param_CAS ) != 0 )
	{
		sprintf ( MsgStr, "CAS_Common_Cfg_ReadCfg \"C:\\DJXMS\\Samples\\CAS_Common_Code\\XMS_CAS_Cfg.INI\" Error!" );
		MessageBox(NULL, MsgStr, "Init System", MB_OK ) ;
	}
	// end of code for CAS(SS1)
	
	//Unable the check result button
	pdlg->GetDlgItem(IDC_BUTTON_CHECK)->EnableWindow(FALSE);
	
	// Init m_ListMain
	InitListMain();
	
	InitListMsg();

	InitListCount();
	
	// Init Conifg's text box
	InitTextBox();
	
	// now open ACS Server
	/*调用acsOpenStream 建立与ACS服务器的连接*/
	r = XMS_acsOpenStream ( &g_acsHandle, 
		&cfg_ServerID,
		cfg_AppUnitID,		// App Unit ID 
		64,				// sendQSize, in K Bytes
		64,				// recvQSize, in K Bytes
		1,
		NULL);
	
	if ( r < 0 )
	{
		sprintf ( MsgStr, "XMS_acsOpenStream(IP Addr : %s, port : %d) FAIL. ret = %d", 
			cfg_ServerID.m_s8ServerIp, cfg_ServerID.m_u32ServerPort, r );		
		MessageBox(NULL, MsgStr, "Init System", MB_OK ) ;
		return false;
	}else
	{
		sprintf ( MsgStr, "XMS_acsOpenStream(IP Addr: %s, port: %d).", cfg_ServerID.m_s8ServerIp, cfg_ServerID.m_u32ServerPort);
		AddMsg(MsgStr);
		WriteLog(LEVEL_DEBUG, MsgStr);
	}
	
	r = XMS_acsSetESR ( g_acsHandle, (EsrFunc)EvtHandler, 0, 1 );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "XMS_acsSetESR() FAIL! ret = %d", r );
		WriteLog(LEVEL_ERROR,MsgStr);
		return false;
	}else
	{
		sprintf ( MsgStr, "XMS_acsSetESR() OK!" );
		AddMsg(MsgStr);
		WriteLog(LEVEL_DEBUG,MsgStr);
	}
	
	InitAllDeviceRes ();
	
	r = XMS_acsGetDeviceList ( g_acsHandle, NULL );
	
	return true;
}

/************************************************************************/
/* 退出程序                                                             */
/************************************************************************/
void	ExitSystem() 
{
	RetCode_t	r;
	int			i;
	DWORD		exitCode=0;
	g_threadRunning = FALSE;//改变变量，线程结束
	
	TerminateThread(&g_dwThreadID,exitCode);//退出线程

	//close all device
	for ( i = 0; i < g_iTotalModule; i ++ )
	{
		CloseAllDevice_Dsp ( MapTable_Module[i] );
	}
	
	r = XMS_acsCloseStream ( g_acsHandle, NULL );
	
	FreeAllDeviceRes ();
	
	// save to "XMS_Dial.INI"
	FetchFromText();
	WriteToConfig();
	
}

// --------------------------------------------------------------------------------
void	OpenTrunkDevice ( TRUNK_STRUCT *pOneTrunk )
{
	RetCode_t	r;
	char        TmpStr[500] = {0};
	
	if (pOneTrunk == NULL)
		return;
	
	pOneTrunk->State= TRK_WAITOPEN;
	
	r = XMS_ctsOpenDevice ( g_acsHandle, &pOneTrunk->deviceID, NULL );
	if ( r < 0 )
	{
		sprintf(TmpStr, "XMS_ctsOpenDevice Fail in OpenTrunkDevice()! ret=%d", r);
		WriteLog(LEVEL_ERROR,TmpStr);
	}
}

void	OpenVoiceDevice ( VOICE_STRUCT *pOneVoice )
{
	RetCode_t	r;
	char        TmpStr[256]={0};
	
	pOneVoice->State= VOC_WAITOPEN;
	
	r = XMS_ctsOpenDevice ( g_acsHandle, &pOneVoice->deviceID, NULL );
	if ( r < 0 )
	{
		sprintf(TmpStr, "XMS_ctsOpenDevice Fail in OpenVoiceDevice()! ret=%d", r);
		WriteLog(LEVEL_ERROR,TmpStr);
	}
}

void	OpenPcmDevice ( PCM_STRUCT *pOnePcm )
{
	RetCode_t	r;
	char        TmpStr[500]={0};
	
	r = XMS_ctsOpenDevice ( g_acsHandle, &pOnePcm->deviceID, NULL );
	if ( r < 0 )
	{
		sprintf(TmpStr, "XMS_ctsOpenDevice Fail in OpenPcmDevice()! ret=%d", r);
		WriteLog(LEVEL_ERROR,TmpStr);
	}
}

void	OpenBoardDevice ( DeviceID_t *pBoardDevID )
{
	RetCode_t	r;
	char        TmpStr[256]={0};
	
	r = XMS_ctsOpenDevice ( g_acsHandle, pBoardDevID, NULL );
	
	if ( r < 0 )
	{
		sprintf(TmpStr, "XMS_ctsOpenDevice Fail in OpenBoardDevice()! ret=%d", r);
		WriteLog(LEVEL_ERROR, TmpStr);
	}
}

void	OpenAllDevice_Dsp ( DJ_S8 s8DspModID )
{
	int			 i;
	
	// Open Board
	OpenBoardDevice ( &AllDeviceRes[s8DspModID].deviceID );
	
	// pVoice
	for ( i = 0; i < AllDeviceRes[s8DspModID].lVocNum; i++ )
	{
		OpenVoiceDevice ( &AllDeviceRes[s8DspModID].pVoice[i] );
	}
	
	// pPcm
	for ( i = 0; i < AllDeviceRes[s8DspModID].lPcmNum; i++ )
	{
		OpenPcmDevice ( &AllDeviceRes[s8DspModID].pPcm[i] );
	}
	
	// pTrunk
	for ( i = 0; i < AllDeviceRes[s8DspModID].lTrunkNum; i++ )
	{
		OpenTrunkDevice ( &AllDeviceRes[s8DspModID].pTrunk[i] );
	}
}
/************************************************************************/
/* 设置语音参数                                                         */
/************************************************************************/
void SetGTD_Voc(DeviceID_t* pDevId)
{
	//========Set GTG Begin========
	CmdParamData_Voice_t cmdVoc;
	memset(&cmdVoc,0,sizeof(cmdVoc));
//	cmdVoc.m_u8InputCtrlValid=1;
//	cmdVoc.m_VocInputControl.m_u8EcEnable = 1;         //使能回声抑制

	cmdVoc.m_u8GtdCtrlValid = 1 ;						//Enable GTD
	cmdVoc.m_VocGtdControl.m_u8ChannelEnable = 1;		//Enable Gtd channel
	cmdVoc.m_VocGtdControl.m_u8DTMFEnable = 1;			// Detect DTMF
	cmdVoc.m_VocGtdControl.m_u8MR2FEnable = 0;
	cmdVoc.m_VocGtdControl.m_u8MR2BEnable = 0;
	cmdVoc.m_VocGtdControl.m_u8GTDEnable = 0;			// Detect GTD 
	cmdVoc.m_VocGtdControl.m_u8FSKEnable = 0;			// Detect FSK for receive CallerID
	//cmdVoc.m_VocGtdControl.m_u8EXTEnable = 0x6;		// Enable PVD & MSK Detect
	
	strcpy((char*)&cmdVoc.m_VocGtdControl.m_u8GTDID[0],"GHIJK");	// Detect Busy Tone
	
	DJ_U16 u16ParamType = VOC_PARAM_UNIPARAM;
	DJ_U16 u16ParamSize = sizeof(cmdVoc);
	void* p = (void*) &cmdVoc;
	
	int r = XMS_ctsSetParam( g_acsHandle,pDevId,u16ParamType,u16ParamSize,(void*)p);
}
/************************************************************************/
/* 查找空闲语音设备                                                     */
/************************************************************************/
int	SearchOneFreeVoice (  TRUNK_STRUCT *pOneTrunk, DeviceID_t *pFreeVocDeviceID )
{
	DJ_S8			s8ModID;
	DJ_S16			s16ChID;
	int				i;
	static	int		iLoopStart = 0;
	VOICE_STRUCT	*pOneVoice;
	DJ_S8			s8SearchModID;
	long			lNowMostFreeNum;
	
	if (pOneTrunk == NULL)
		return -1;
	
	s8ModID = pOneTrunk->deviceID.m_s8ModuleID;
	
	if ( cfg_iVoiceRule == 0 )			// Fix relationship between Trunk & Voice
	{
		s16ChID = pOneTrunk->deviceID.m_s16ChannelID;
		
		i = pOneTrunk->iSeqID;
		
		if ( i < AllDeviceRes[s8ModID].lVocNum )
		{
			pOneVoice = &AllDeviceRes[s8ModID].pVoice[i];
			if ( pOneVoice->State != VOC_WAITOPEN )
			{
				*pFreeVocDeviceID = pOneVoice->deviceID;
				
				if(pOneVoice->State == VOC_FREE)
				{
					// 占用该voice device 
					pOneVoice->State = VOC_USED;
					AllDeviceRes[s8ModID].lVocFreeNum--;
					g_iTotalVoiceFree --;
					DrawCount_Voc ( s8ModID );
				}
				return i;
			}
		}
		
		return -1;
	}
	
	if ( cfg_iVoiceRule == 1 )			// Search in Same Module
	{
		s8SearchModID = s8ModID;
	}
	else		// Search in Most free resource module
	{
		s8SearchModID = -1;
		lNowMostFreeNum = -1;
		for ( i = 0; i < g_iTotalModule; i ++ )
		{
			if ( AllDeviceRes[MapTable_Module[i]].lVocFreeNum > lNowMostFreeNum )
			{
				s8SearchModID = MapTable_Module[i];
				lNowMostFreeNum = AllDeviceRes[MapTable_Module[i]].lVocFreeNum;
			}
		}
	}
	
	for ( i = 0; i < AllDeviceRes[s8SearchModID].lVocNum; i ++ )
	{
		pOneVoice = &AllDeviceRes[s8SearchModID].pVoice[i];
		if ( pOneVoice->State == VOC_FREE )
		{
			*pFreeVocDeviceID = pOneVoice->deviceID;
			
			// 占用该voice device 
			pOneVoice->State = VOC_USED;
			AllDeviceRes[s8SearchModID].lVocFreeNum--;
			g_iTotalVoiceFree --;
			DrawCount_Voc ( s8SearchModID );
			return i;
		}
	}
	
	return -1;
}

void	My_DualLink ( DeviceID_t *pDev1, DeviceID_t *pDev2 )
{
	char tmpStr[512]={0};
	XMS_ctsLinkDevice ( g_acsHandle, pDev1, pDev2, NULL ); 
	XMS_ctsLinkDevice ( g_acsHandle, pDev2, pDev1, NULL ); 
	sprintf ( tmpStr, "CMD: My_DualLink(), (%s, %s, %d, %d) (%s, %s, %d, %d)", 
		GetString_DeviceMain(pDev1->m_s16DeviceMain),
		GetString_DeviceSub(pDev1->m_s16DeviceSub),
		pDev1->m_s8ModuleID,pDev1->m_s16ChannelID,
		GetString_DeviceMain(pDev2->m_s16DeviceMain),
		GetString_DeviceSub(pDev2->m_s16DeviceSub),
		pDev2->m_s8ModuleID, pDev2->m_s16ChannelID);
	WriteLog(LEVEL_DEBUG, tmpStr);
}

void	My_DualUnlink ( DeviceID_t *pDev1, DeviceID_t *pDev2 )
{
	char tmpStr[512]={0};
	XMS_ctsUnlinkDevice ( g_acsHandle, pDev1, pDev2, NULL ); 
	XMS_ctsUnlinkDevice ( g_acsHandle, pDev2, pDev1, NULL ); 
	sprintf ( tmpStr, "CMD: My_DualUnlink(), (%s, %s, %d, %d) (%s, %s, %d, %d)", 
		GetString_DeviceMain(pDev1->m_s16DeviceMain),
		GetString_DeviceSub(pDev1->m_s16DeviceSub),
		pDev1->m_s8ModuleID,pDev1->m_s16ChannelID,
		GetString_DeviceMain(pDev2->m_s16DeviceMain),
		GetString_DeviceSub(pDev2->m_s16DeviceSub),
		pDev2->m_s8ModuleID, pDev2->m_s16ChannelID);
	WriteLog(LEVEL_DEBUG, tmpStr);
}

// Search Free Voice for get CallerID(FSK)
void	PrepareForCallerID ( TRUNK_STRUCT *pOneTrunk )
{
	DeviceID_t FreeVocDeviceID;
	
	if ( pOneTrunk->deviceID.m_s16DeviceSub != XMS_DEVSUB_ANALOG_TRUNK && pOneTrunk->deviceID.m_s16DeviceSub !=XMS_DEVSUB_ANALOG_USER )
	{
		return;
	}
	if ( SearchOneFreeVoice( pOneTrunk,  &FreeVocDeviceID ) >= 0 )
	{
		pOneTrunk->VocDevID = FreeVocDeviceID;
		M_OneVoice(FreeVocDeviceID).UsedDevID = pOneTrunk->deviceID; 
		My_DualLink ( &FreeVocDeviceID, &pOneTrunk->deviceID );
		DrawMain_VocInfo ( pOneTrunk );
	}
}

void	OpenDeviceOK ( DeviceID_t *pDevice )
{
	TRUNK_STRUCT	*pOneTrunk;
	VOICE_STRUCT	*pOneVoice;
	PCM_STRUCT		*pOnePcm;
	
	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_BOARD )
	{
		AllDeviceRes[pDevice->m_s8ModuleID].deviceID.m_CallID = pDevice->m_CallID;		// this line is very important, must before all operation
	}
	
	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
	{
		pOneTrunk = &M_OneTrunk(*pDevice);
		
		pOneTrunk->deviceID.m_CallID = pDevice->m_CallID;		// this line is very important, must before all operation
		
		InitTrunkChannel(pOneTrunk);
		InitTrunkcounter(pOneTrunk);
		
		XMS_ctsResetDevice ( g_acsHandle, pDevice, NULL );
		XMS_ctsGetDevState ( g_acsHandle, pDevice, NULL );
		
		// modify the count
		g_iTotalTrunkOpened ++;
		AllDeviceRes[pDevice->m_s8ModuleID].lTrunkOpened ++;
		DrawCount_Trunk ( pDevice->m_s8ModuleID  );

		if (pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK)
		{
			SetParam_AnalogTrunk(&pOneTrunk->deviceID);
		}
		//PrepareForCallerID(pOneTrunk);//绑定语音
	}
	
	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_VOICE )
	{
		pOneVoice = &M_OneVoice(*pDevice);
		
		pOneVoice->deviceID.m_CallID = pDevice->m_CallID;		// this is very important
		pOneVoice->State = VOC_FREE;
		
		g_iTotalVoiceOpened ++;
		g_iTotalVoiceFree ++;
		AllDeviceRes[pDevice->m_s8ModuleID].lVocOpened ++;
		AllDeviceRes[pDevice->m_s8ModuleID].lVocFreeNum ++;
		SetGTD_Voc(pDevice);	
		DrawCount_Voc ( pDevice->m_s8ModuleID );
	}
	
	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_DIGITAL_PORT )
	{
		pOnePcm = &M_OnePcm(*pDevice);
		
		pOnePcm->deviceID.m_CallID = pDevice->m_CallID;		// this is very important
		
		// init the Device: Pcm
		XMS_ctsResetDevice ( g_acsHandle, pDevice, NULL );
		XMS_ctsGetDevState ( g_acsHandle, pDevice, NULL );
		
		// moidfy the count
		g_iTotalPcmOpened ++;
		AllDeviceRes[pDevice->m_s8ModuleID].lPcmOpened ++;
		
		DrawCount_Pcm  ( pDevice->m_s8ModuleID );
	}
}

// --------------------------------------------------------------------------------
void	CloseTrunkDevice ( TRUNK_STRUCT *pOneTrunk )
{
	RetCode_t	r;
	char        TmpStr[500] = {0};
	
	if (pOneTrunk == NULL)
		return;
	
	pOneTrunk->State= TRK_WAITOPEN;
	
	r = XMS_ctsCloseDevice ( g_acsHandle, &pOneTrunk->deviceID, NULL );
	if ( r < 0 )
	{
		sprintf(TmpStr, "XMS_ctsCloseDevice Fail in CloseTrunkDevice()! ret=%d", r);
		WriteLog(LEVEL_ERROR, TmpStr);
	}
}

void	CloseVoiceDevice ( VOICE_STRUCT *pOneVoice )
{
	RetCode_t	r;
	char        TmpStr[256] = {0};
	
	r = XMS_ctsCloseDevice ( g_acsHandle, &pOneVoice->deviceID, NULL );
	if ( r < 0 )
	{
		sprintf(TmpStr, "XMS_ctsCloseDevice Fail in CloseVoiceDevice()! ret=%d", r);
		WriteLog(LEVEL_ERROR, TmpStr);
	}
}

void	ClosePcmDevice ( PCM_STRUCT *pOnePcm )
{
	RetCode_t	r;
	char        TmpStr[500] = {0};
	
	r = XMS_ctsCloseDevice ( g_acsHandle, &pOnePcm->deviceID, NULL );
	if ( r < 0 )
	{
		sprintf(TmpStr, "XMS_ctsCloseDevice Fail in ClosePcmDevice()! ret=%d", r);
		WriteLog(LEVEL_ERROR, TmpStr);
	}
}

void	CloseBoardDevice ( DeviceID_t *pBoardDevID )
{
	RetCode_t	r;
	char TmpStr[500] = "";
	
	r = XMS_ctsCloseDevice ( g_acsHandle, pBoardDevID, NULL );
	
	if ( r < 0 )
	{
		sprintf(TmpStr, "XMS_ctsCloseDevice Fail in CloseBoardDevice()! ret = %d", r);
		WriteLog(LEVEL_ERROR, TmpStr);
	}
}

void	CloseAllDevice_Dsp ( DJ_S8 s8DspModID )
{
	int			 i;
	
	// close Board
	CloseBoardDevice ( &AllDeviceRes[s8DspModID].deviceID );
	
	// pTrunk
	for ( i = 0; i < AllDeviceRes[s8DspModID].lTrunkNum; i++ )
	{
		CloseTrunkDevice ( &AllDeviceRes[s8DspModID].pTrunk[i] );
	}
	
	// pVoice
	for ( i = 0; i < AllDeviceRes[s8DspModID].lVocNum; i++ )
	{
		CloseVoiceDevice ( &AllDeviceRes[s8DspModID].pVoice[i] );
	}
	
	// pPcm
	for ( i = 0; i < AllDeviceRes[s8DspModID].lPcmNum; i++ )
	{
		ClosePcmDevice ( &AllDeviceRes[s8DspModID].pPcm[i] );
	}
}

// -------------------------------------------------------------------------------------------------
void	HandleDevState ( Acs_Evt_t *pAcsEvt )
{
	TRUNK_STRUCT	*pOneTrunk;
	PCM_STRUCT		*pOnePcm;
	Acs_GeneralProc_Data *pGeneralData = NULL;
	
	pGeneralData = (Acs_GeneralProc_Data *)FetchEventData(pAcsEvt);
	if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
	{
		pOneTrunk = &M_OneTrunk(pAcsEvt->m_DeviceID);
		
		pOneTrunk->iLineState = pGeneralData->m_s32DeviceState;
		DrawMain_LineState( pOneTrunk );		
	}
	
	if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_DIGITAL_PORT )
	{
		pOnePcm = &M_OnePcm(pAcsEvt->m_DeviceID);
		
		pOnePcm->u8E1Type = (pGeneralData->m_s32DeviceState >> 16) & 0xFF;
		pOnePcm->s32AlarmVal = (pGeneralData->m_s32DeviceState & 0xFFFF);
	}
	
}

void	Change_State ( TRUNK_STRUCT *pOneTrunk, TRUNK_STATE NewState )
{	
	if (pOneTrunk == NULL)
		return;
	
	pOneTrunk->State = NewState;
	
	DrawMain_State ( pOneTrunk );
}

void My_InitDtmfBuf ( TRUNK_STRUCT *pOneTrunk )
{
	if (pOneTrunk == NULL)
		return;
	
	pOneTrunk->DtmfCount = 0;
	pOneTrunk->DtmfBuf[0] = 0; 
}

void My_AddDtmfBuf ( TRUNK_STRUCT *pOneTrunk, char TmpDtmf )
{
	if (pOneTrunk == NULL)
		return;
	
	if ( pOneTrunk->DtmfCount < (32-1) )
	{
		pOneTrunk->DtmfBuf[pOneTrunk->DtmfCount] = TmpDtmf; 
		pOneTrunk->DtmfBuf[pOneTrunk->DtmfCount+1] = 0; 
		pOneTrunk->DtmfCount ++;
	}
}


int		FreeOneFreeVoice (  DeviceID_t *pFreeVocDeviceID )
{
	DJ_S8	s8ModID;
	
	s8ModID = pFreeVocDeviceID->m_s8ModuleID;
	if ( AllDeviceRes[s8ModID].lFlag == 1 )
	{
		M_OneVoice(*pFreeVocDeviceID).State = VOC_FREE;
		
		AllDeviceRes[s8ModID].lVocFreeNum++;
		g_iTotalVoiceFree ++;
		DrawCount_Voc ( s8ModID );
		return	0;		// OK
	}
	
	return -1;			// invalid VocDeviceID
}

void    InitTrunkcounter(TRUNK_STRUCT *pOneTrunk)
{
	if (pOneTrunk == NULL)
		return;
	
	pOneTrunk->iCallErrCnt = 0;
	pOneTrunk->iCheckErrCnt = 0;
	pOneTrunk->iSucCnt = 0;
	pOneTrunk->iTotalCnt = 0;
}

void	InitTrunkChannel ( TRUNK_STRUCT *pOneTrunk )
{
	if (pOneTrunk == NULL)
	{
		return;
	}
	
	Change_State ( pOneTrunk, TRK_FREE );
	
	memset ( &pOneTrunk->VocDevID, 0, sizeof(DeviceID_t) );		// 0表示还没有分配Voc Device
	DrawMain_VocInfo ( pOneTrunk );
	
	My_InitDtmfBuf ( pOneTrunk );
	DrawTrunk_DTMF(pOneTrunk);
	
	pOneTrunk->iEstablishTime = 0;
	pOneTrunk->iCallOutTime = 0;
	
}

/*停止放音*/
DJ_S32 StopPlayFile ( DeviceID_t	*pVocDevID )
{
	RetCode_t		r;
	ControlPlay_t	controlPlay;
	
	controlPlay.m_u16ControlType = XMS_STOP_PLAY;
	
	r = XMS_ctsControlPlay ( g_acsHandle, pVocDevID, &controlPlay, NULL );
	
	return r;
}

/*停止录音*/
DJ_S32 StopRecordFile ( DeviceID_t	*pVocDevID )
{
	RetCode_t		r;
	ControlRecord_t	controlRecord;
	
	controlRecord.m_u32ControlType = XMS_STOP_RECORD;
	
	r = XMS_ctsControlRecord ( g_acsHandle, pVocDevID, &controlRecord, NULL );
	
	return r;
}

/*录音处理*/
DJ_S32	 RecordFile ( DeviceID_t	*pVocDevID, DJ_S8 *s8FileName, DJ_U32 u32RecSize, bool bIsAppend )
{	
	DJ_U32				i = 0;	
	RetCode_t			r;
	char				tmpStr[512]={0};
	RecordProperty_t	recordProperty;	
	DeviceID_t	*pLinkTrkDevID;
	pLinkTrkDevID = &M_OneVoice(*pVocDevID).UsedDevID;

	memset(&recordProperty,0,sizeof(RecordProperty_t));		
	recordProperty.m_u32MaxSize = u32RecSize;

	if ( bIsAppend )
		recordProperty.m_s8IsAppend = 1;
	else
		recordProperty.m_s8IsAppend = 0;
	
	strcpy ( recordProperty.m_s8FileName, s8FileName );
	
	r = XMS_ctsRecord ( g_acsHandle, pVocDevID, &recordProperty, NULL );
	sprintf(tmpStr,"CMD: XMS_ctsRecord() %s, (%s, %s, %d, %d), LinkTrk(%s, %s, %d, %d)",
		s8FileName,
		GetString_DeviceMain(pVocDevID->m_s16DeviceMain),
		GetString_DeviceSub(pVocDevID->m_s16DeviceSub),
		pVocDevID->m_s8ModuleID, pVocDevID->m_s16ChannelID,
		GetString_DeviceMain(pLinkTrkDevID->m_s16DeviceMain),
		GetString_DeviceSub(pLinkTrkDevID->m_s16DeviceSub),
		pLinkTrkDevID->m_s8ModuleID,
		pLinkTrkDevID->m_s16ChannelID);
	WriteLog(LEVEL_DEBUG, tmpStr);
	return r;
}

bool CheckRecordEnd(TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt)
{
	char				tmpStr[512];
	memset(tmpStr,0,sizeof(tmpStr));

	if (pOneTrunk == NULL)
	{
		return false;
	}
	
	if (pAcsEvt->m_s32EventType == XMS_EVT_RECORD)
	{
		sprintf(tmpStr,"CheckRecordEnd(), Trk(%s, %s, %d, %d), DTMFCount:%d ",			
			GetString_DeviceMain(pOneTrunk->deviceID.m_s16DeviceMain),
			GetString_DeviceSub(pOneTrunk->deviceID.m_s16DeviceSub),
			pOneTrunk->deviceID.m_s8ModuleID,
			pOneTrunk->deviceID.m_s16ChannelID,
			pOneTrunk->DtmfCount);
		WriteLog(LEVEL_DEBUG, tmpStr);
		return true;
	}
	
	return false;
}

/*放音处理*/
DJ_S32	 PlayFile ( DeviceID_t	*pVocDevID, DJ_S8 *s8FileName, DJ_U8 u8PlayTag, bool bIsQueue )
{	
	char			tmpStr[1024]={0};
	DJ_U32           i = 0;	
	RetCode_t		 r;
	PlayProperty_t   playProperty;
	DeviceID_t			*pLinkTrkDevID;
	pLinkTrkDevID = &M_OneVoice(*pVocDevID).UsedDevID;

	memset(&playProperty,0,sizeof(playProperty));
	
	if ( bIsQueue )
	{
		playProperty.m_u16PlayType = XMS_PLAY_TYPE_FILE_QUEUE;	
	}else
	{
		playProperty.m_u16PlayType = XMS_PLAY_TYPE_FILE;	
	}
	
	playProperty.m_u8TaskID = u8PlayTag;
	playProperty.m_u32MaxTime = 0;
	
	strcpy ( playProperty.m_s8PlayContent, s8FileName );
	
	r = XMS_ctsPlay ( g_acsHandle, pVocDevID, &playProperty, NULL );
	sprintf(tmpStr,"CMD: XMS_ctsPlay() %s, (%s, %s, %d, %d), LinkTrk(%s, %s, %d, %d).",
		s8FileName,
		GetString_DeviceMain(pVocDevID->m_s16DeviceMain),
		GetString_DeviceSub(pVocDevID->m_s16DeviceSub),
		pVocDevID->m_s8ModuleID,
		pVocDevID->m_s16ChannelID,
		GetString_DeviceMain(pLinkTrkDevID->m_s16DeviceMain),
		GetString_DeviceSub(pLinkTrkDevID->m_s16DeviceSub),
		pLinkTrkDevID->m_s8ModuleID,
		pLinkTrkDevID->m_s16ChannelID);
	WriteLog(LEVEL_DEBUG, tmpStr);
	return r;
}

void ResetTrunk ( TRUNK_STRUCT *pOneTrunk )
{

	if (pOneTrunk == NULL)
		return;
	
	// free the used Voice Resource
	if ( pOneTrunk->VocDevID.m_s16DeviceMain != 0 )
	{
		StopRecordFile(&pOneTrunk->VocDevID);
		StopPlayFile(&pOneTrunk->VocDevID);
		
		My_DualUnlink ( &pOneTrunk->VocDevID, &pOneTrunk->deviceID );		
		FreeOneFreeVoice (  &pOneTrunk->VocDevID );		
		memset ( &M_OneVoice(pOneTrunk->VocDevID).UsedDevID,	0, sizeof(DeviceID_t) );		// 0表示还没有分配 Device 
		memset ( &pOneTrunk->VocDevID, 0, sizeof(DeviceID_t) );		// 0表示还没有分配 Device
		DrawMain_VocInfo ( pOneTrunk );
	}
	
	InitTrunkChannel ( pOneTrunk );
}

bool	CheckPlayEnd ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	Acs_MediaProc_Data	*pMediaData = NULL;
	char				tmpStr[512];
	memset(tmpStr,0,sizeof(tmpStr));

	if ( pAcsEvt->m_s32EventType == XMS_EVT_PLAY )	
	{
		sprintf(tmpStr,"CheckPlayEnd(), Trk(%s, %s, %d, %d)",			
			GetString_DeviceMain(pOneTrunk->deviceID.m_s16DeviceMain),
			GetString_DeviceSub(pOneTrunk->deviceID.m_s16DeviceSub),
			pOneTrunk->deviceID.m_s8ModuleID,
			pOneTrunk->deviceID.m_s16ChannelID);
		WriteLog(LEVEL_DEBUG, tmpStr);
		pMediaData = (Acs_MediaProc_Data *) FetchEventData(pAcsEvt);
		if ( pMediaData->m_u8TaskID == pOneTrunk->u8PlayTag )
		{
			return true;
		}
	}
	
	return false;
}

char   My_GetDtmfCode(TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt)
{
	Acs_IO_Data				*pIOData = NULL;
	char		*p;
	
	if ( pAcsEvt->m_s32EventType == XMS_EVT_RECVIODATA )	/*IO Data事件*/
	{
		pIOData = (Acs_IO_Data *)FetchEventData(pAcsEvt);
		
		if ( ( pIOData->m_u16IoType == XMS_IO_TYPE_DTMF ) 
			&& ( pIOData->m_u16IoDataLen > 0 ) )
		{
			p = (char *)FetchIOData(pAcsEvt);
			
			return *p;
		}
	}
	
	return -1;	// not a good DTMF
}
unsigned char *My_GetFskCode ( Acs_Evt_t *pAcsEvt )
{
	Acs_IO_Data				*pIOData = NULL;
	unsigned char		*p;
	
	if ( pAcsEvt->m_s32EventType == XMS_EVT_RECVIODATA )	/*IO Data Evemt*/
	{
		pIOData = (Acs_IO_Data *)FetchEventData(pAcsEvt);
		
		if ( ( pIOData->m_u16IoType== XMS_IO_TYPE_FSK ) 
			&& ( pIOData->m_u16IoDataLen > 0 ) )
		{
			p = (unsigned char *)FetchIOData(pAcsEvt);
			
			return p;
		}
	}
	
	return NULL;	// not a good FSK
}

/************************************************************************/
/* 设置模拟中继参数                                                     */
/************************************************************************/
void   SetParam_AnalogTrunk(DeviceID_t *Dev)
{
	   void* p = NULL;
	   CmdParamData_AnalogTrunk_t cmdAnalogTrunk;
	   DJ_U16 u16ParamType = ANALOGTRUNK_PARAM_UNIPARAM ;
	   DJ_U16 u16ParamSize = sizeof(CmdParamData_AnalogTrunk_t);
	   memset(&cmdAnalogTrunk,0,sizeof(cmdAnalogTrunk));
	   cmdAnalogTrunk.m_u16CallInRingCount = 2;			// 2 times RingBack, 振铃两次后产生XMS_EVT_CALLIN事件
	   // 单位ms，6秒，每次振铃时，定时器就会启动。超时后，设备状态变成DCS_Free。
	   // 此参数和实际振铃时间有关
	   cmdAnalogTrunk.m_u16CallInRingTimeOut = 8000;		
	   p = (void*)&cmdAnalogTrunk;
	   RetCode_t r ;
	   r = XMS_ctsSetParam( g_acsHandle, Dev, u16ParamType,u16ParamSize, (DJ_Void *)p );
}


// Special code for CAS(SS1)
void	SpecialForCas ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	char tmpStr[512]={0};
	char tmpRecFileName[100]={0};
	DeviceID_t				FreeVocDeviceID;
	
	if ( pOneTrunk->deviceID.m_s16DeviceSub != XMS_DEVSUB_E1_CAS )
		return;
	
	if ( pAcsEvt->m_s32EventType == XMS_EVT_CAS_MFC_START )	/* 开始接收MFC，需要一个Voc设备*/
	{
		if ( SearchOneFreeVoice ( pOneTrunk,  &FreeVocDeviceID ) >= 0 )
		{
			pOneTrunk->VocDevID = FreeVocDeviceID;
			M_OneVoice(FreeVocDeviceID).UsedDevID = pOneTrunk->deviceID; 
			DrawMain_VocInfo ( pOneTrunk );
			My_DualLink ( &FreeVocDeviceID, &pOneTrunk->deviceID );

			SetGTD_Voc(&FreeVocDeviceID);

			g_Param_CAS.m_VocDevID = FreeVocDeviceID;
			XMS_ctsSetParam ( g_acsHandle, &pOneTrunk->deviceID, 
				CAS_PARAM_UNIPARAM, sizeof(CmdParamData_CAS_t), (DJ_Void *)&g_Param_CAS );

			g_Param_CAS.m_u8CountryCode = CHINA;
			XMS_ctsSetParam ( g_acsHandle, &pOneTrunk->deviceID, 
				CAS_PARAM_MFCTYPE, sizeof(CmdParamData_CAS_t), (DJ_Void *)&g_Param_CAS );
			
		//	sprintf ( tmpRecFileName, "%sRec\\CASMFC_Rec_%03d.pcm", cfg_VocPath, pOneTrunk->iSeqID );
		//	RecordFile ( &pOneTrunk->VocDevID, tmpRecFileName, 8000L*10, false );

		}
	}
 //   if ( pAcsEvt->m_s32EventType == XMS_EVT_CALLOUT )		
	if ( pAcsEvt->m_s32EventType == XMS_EVT_CAS_MFC_END )	//接收MFC完毕，释放Voc设备
	{
// 		My_DualUnlink(&pOneTrunk->VocDevID,&pOneTrunk->deviceID );
// 		sprintf ( tmpStr, "My_DualUnlink(), (%s, %s, %d, %d) (%s, %s, %d, %d)", 
// 			GetString_DeviceMain(pOneTrunk->deviceID.m_s16DeviceMain),
// 			GetString_DeviceSub(pOneTrunk->deviceID.m_s16DeviceSub),
// 			pOneTrunk->deviceID.m_s8ModuleID,pOneTrunk->deviceID.m_s16ChannelID,
// 			GetString_DeviceMain(pOneTrunk->VocDevID.m_s16DeviceMain),
// 			GetString_DeviceSub(pOneTrunk->VocDevID.m_s16DeviceSub),
// 			pOneTrunk->VocDevID.m_s8ModuleID,pOneTrunk->VocDevID.m_s16ChannelID);
// 			WriteLog(LEVEL_DEBUG, tmpStr);
	//	StopRecordFile(&pOneTrunk->VocDevID);
// 		FreeOneFreeVoice (  &pOneTrunk->VocDevID );
// 		
// 		memset ( &M_OneVoice(pOneTrunk->VocDevID).UsedDevID,	0, sizeof(DeviceID_t) );		// 0表示还没有分配 Device 
// 		memset ( &pOneTrunk->VocDevID, 0, sizeof(DeviceID_t) );		// 0表示还没有分配 Device
// 		DrawMain_VocInfo ( pOneTrunk );
	}
}
// end of code for CAS(SS1)
void AddMsg ( char *str)
{
	char		TmpStr[256]={0};
	static	int		iTotal_ListMsg = 0;
	
	if(pdlg->m_ListMsg.GetCount() >= 1000)
	{
		pdlg->m_ListMsg.ResetContent();
		iTotal_ListMsg = 0;
	}else
	{
		iTotal_ListMsg = pdlg->m_ListMsg.GetCount();
	}
	
	sprintf( TmpStr, "%3d: ", iTotal_ListMsg+1 );
	strcat( TmpStr, str );
	pdlg->m_ListMsg.AddString ( TmpStr );
	
	iTotal_ListMsg++;	
	pdlg->m_ListMsg.SetCurSel (iTotal_ListMsg-1);
	
}
/************************************************************************/
/* User坐席侧事件处理函数                                               */
/************************************************************************/
void AnalogUserWork ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	int         r=0;
	int          iFreeVoc;
	DeviceID_t   FreeVocDeviceID;
	Acs_CallControl_Data *	pCallControl = NULL;
	char        FileName[256] = {0};
	char		TmpDtmf;
	char        TmpFileName[256] = "";
	char        chError[512] = "";
	char        DTMFBuff[12]="";
	char		tmpStr[256]={0};
	
	if (pOneTrunk == NULL)
	{
		return;
	}
	if ( pAcsEvt->m_s32EventType == XMS_EVT_DEV_TIMER )	/*定时器事件*/
	{
		Change_State(pOneTrunk, TRK_WAITHANGUP);
		XMS_ctsClearCall ( g_acsHandle, &pOneTrunk->deviceID, 0 ,NULL );
		sprintf(tmpStr,"(%s, %s, %d, %d), XMS_ctsClearCall(), 定时器超时,主动拆线",
			GetString_DeviceMain(pOneTrunk->deviceID.m_s16DeviceMain),
			GetString_DeviceSub(pOneTrunk->deviceID.m_s16DeviceSub),
			pOneTrunk->deviceID.m_s8ModuleID,
			pOneTrunk->deviceID.m_s16ChannelID);
		AddMsg(tmpStr);
		return;
	}
	
	if ( pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL )	/*拆线事件*/
	{
		XMS_ctsGetDevState( g_acsHandle, &pOneTrunk->deviceID, NULL);
		if (pOneTrunk->State == TRK_WAITHANGUP)
		{
			if (cfg_iVoiceCheck == 1)
			{
				GetFilePathName(pOneTrunk, TmpFileName);
				sprintf(FileName, "%sUser%03d_Rec.pcm", TmpFileName, pOneTrunk->iSeqID);
				
				if(!IsCorrectDtmf(pOneTrunk))//接收DTMF出错
				{
					sprintf (TmpFileName, "%sWrong_User%03d_Rec_%d.pcm", TmpFileName, pOneTrunk->iSeqID,pOneTrunk->iTotalCnt);
					rename ( FileName, TmpFileName );
					
					pOneTrunk->iCheckErrCnt++;
					g_iTotalCheckErr++;
					SYSTEMTIME	sysTime;			
					GetLocalTime( &sysTime );			//获得系统当前时间
					sprintf(chError, "(%s, %s, %d, %d), GetDtmf [%s] error, callTimes=%d, CheckErrCount=%d, %04d%02d%02d-%02d:%02d:%02d:%03d", 
						GetString_DeviceMain(pOneTrunk->deviceID.m_s16DeviceMain),
						GetString_DeviceSub(pOneTrunk->deviceID.m_s16DeviceSub),
						pOneTrunk->deviceID.m_s8ModuleID,
						pOneTrunk->deviceID.m_s16ChannelID,
						pOneTrunk->DtmfBuf, 
						pOneTrunk->iTotalCnt,
						pOneTrunk->iCheckErrCnt,
						sysTime.wYear, 
						sysTime.wMonth, 
						sysTime.wDay, 
						sysTime.wHour, 
						sysTime.wMinute, 
						sysTime.wSecond, 
						sysTime.wMilliseconds);
					WriteLog(LEVEL_ERROR, chError);	
					AddMsg(chError);
				}else
				{
					pOneTrunk->iSucCnt++;
					DeleteFile(FileName);//删除录音文件
				}							
			}					
		}
		XMS_ctsSetDevTimer ( g_acsHandle, &pOneTrunk->deviceID, 0 );//清除定时器		
		ResetTrunk ( pOneTrunk );	
		Change_State (pOneTrunk, TRK_FREE);
		DrawTrunk_DTMF(pOneTrunk);
		DrawMain_EstableTime(pOneTrunk);		
		
		return ; 
	}
	
	switch(pOneTrunk->State)
	{
	case TRK_FREE:
		break;
		
	case TRK_CALLOUT: // 带语音检测,User摘机
		if ( pAcsEvt->m_s32EventType == XMS_EVT_CALLOUT )	
		{
			XMS_ctsGetDevState( g_acsHandle, &pOneTrunk->deviceID, NULL);
			pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);
			
			if ( pCallControl->m_s32AcsEvtState == 1 )	// 呼出成功，对方摘机
			{
				pOneTrunk->iEstablishTime = GetTickCount() - pOneTrunk->iCallOutTime;
				g_iEstablishSum += pOneTrunk->iEstablishTime;
				DrawMain_EstableTime(pOneTrunk);
				g_iCallSuc++;
				
				iFreeVoc = SearchOneFreeVoice(pOneTrunk, &FreeVocDeviceID);
				if (iFreeVoc >= 0)
				{
					pOneTrunk->u8PlayTag = 0;					
					pOneTrunk->VocDevID = FreeVocDeviceID;
					M_OneVoice(FreeVocDeviceID).UsedDevID = pOneTrunk->deviceID; 
					My_DualLink(&pOneTrunk->deviceID, &FreeVocDeviceID);					
					DrawMain_VocInfo ( pOneTrunk );
					sprintf(FileName, "%ssig%d.pcm", cfg_VocPath, (pOneTrunk->iSeqID)%15);
					PlayFile (&pOneTrunk->VocDevID, FileName, pOneTrunk->u8PlayTag, false);
					Change_State (pOneTrunk, TRK_PLAY);
				}else
				{
					Change_State(pOneTrunk, TRK_WAITHANGUP);
					XMS_ctsClearCall ( g_acsHandle, &pOneTrunk->deviceID, 0, NULL );
					sprintf(tmpStr,"(%s, %s, %d, %d), XMS_ctsClearCall(), No Free Voice,主动拆线",
						GetString_DeviceMain(pOneTrunk->deviceID.m_s16DeviceMain),
						GetString_DeviceSub(pOneTrunk->deviceID.m_s16DeviceSub),
						pOneTrunk->deviceID.m_s8ModuleID,
						pOneTrunk->deviceID.m_s16ChannelID);
					AddMsg(tmpStr);
				}			
				XMS_ctsSetDevTimer ( g_acsHandle, &pOneTrunk->deviceID, g_iElapseTime );

			}else		// 呼出失败
			{
				DrawMain_Reason ( pOneTrunk, pCallControl->m_s32AcsEvtErrCode, pCallControl->m_s32CallClearCause );
				Change_State ( pOneTrunk, TRK_FREE );
				g_iTotalCallErr++;
				pOneTrunk->iCallErrCnt++;
			}	
		}	
		break;
		
	case TRK_PLAY:
        if ( CheckPlayEnd ( pOneTrunk, pAcsEvt))
		{
			pOneTrunk->u8PlayTag++;	
			GetFilePathName(pOneTrunk, TmpFileName);
			sprintf(FileName, "%sUser%03d_DSP%02d_Chn%03d_Rec.pcm", TmpFileName, pOneTrunk->iSeqID,pOneTrunk->deviceID.m_s8ModuleID,pOneTrunk->deviceID.m_s16ChannelID);		
			RecordFile ( &pOneTrunk->VocDevID, FileName, CALLER_RECORD_TIME, false);//analog user
			Change_State (pOneTrunk, TRK_RECORDFILE);
		}
		break;
		
	case TRK_RECORDFILE:
		TmpDtmf = My_GetDtmfCode ( pOneTrunk, pAcsEvt );
		if (TmpDtmf != -1)
		{
			My_AddDtmfBuf ( pOneTrunk, TmpDtmf );
			DrawTrunk_DTMF ( pOneTrunk );
		}
		if (CheckRecordEnd(pOneTrunk, pAcsEvt)) //analog user
		{
			Change_State (pOneTrunk, TRK_WAITHANGUP);
		}
		if (pOneTrunk->DtmfCount == 10)
		{
			StopRecordFile(&pOneTrunk->VocDevID);//坐席侧收到了10个DTMF，就停止录音了
		}
		break;
	}
}
/************************************************************************/
/* ISDN主叫端                                                           */
/************************************************************************/
void TrunkWork_ISDN_CALLER( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	int         r=0;
	DeviceID_t   FreeVocDeviceID;
	Acs_CallControl_Data *	pCallControl = NULL;
	char        FileName[256] = {0};
	char		TmpDtmf;
	char        TmpFileName[256] = "";
	char        chError[256] = "";
	char        DTMFBuff[12]="";
	char		tmpStr[256]={0};
	int			retCode = -1;
	if (pOneTrunk == NULL)
	{
		return;
	}
	if ( pAcsEvt->m_s32EventType == XMS_EVT_DEV_TIMER )	/*定时器事件*/
	{
		Change_State(pOneTrunk, TRK_WAITHANGUP);
		XMS_ctsClearCall ( g_acsHandle, &pOneTrunk->deviceID, 0 ,NULL );
		sprintf(tmpStr,"(%s, %s, %d, %d), XMS_ctsClearCall(), 定时器超时,主动拆线",
			GetString_DeviceMain(pOneTrunk->deviceID.m_s16DeviceMain),
			GetString_DeviceSub(pOneTrunk->deviceID.m_s16DeviceSub),
			pOneTrunk->deviceID.m_s8ModuleID,
			pOneTrunk->deviceID.m_s16ChannelID);
		WriteLog(LEVEL_ERROR, tmpStr);
		AddMsg(tmpStr);
		return;
	}
	
	if ( pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL )	/*拆线事件*/
	{
		XMS_ctsGetDevState( g_acsHandle, &pOneTrunk->deviceID, NULL);
		if (pOneTrunk->State == TRK_WAITHANGUP)
		{
			if (cfg_iVoiceCheck == 1)
			{
				GetFilePathName(pOneTrunk, TmpFileName);
				sprintf(FileName, "%sCaller%03d_DSP%02d_Chn%03d_Rec.pcm", TmpFileName, pOneTrunk->iSeqID,pOneTrunk->deviceID.m_s8ModuleID,pOneTrunk->deviceID.m_s16ChannelID);	

				if(!IsCorrectDtmf(pOneTrunk))//接收DTMF出错
				{
					sprintf (TmpFileName, "%sWrong_Caller%03d_%d.pcm", TmpFileName, pOneTrunk->iSeqID,pOneTrunk->iTotalCnt);
					rename ( FileName, TmpFileName );
					
					pOneTrunk->iCheckErrCnt++;
					g_iTotalCheckErr++;
					SYSTEMTIME	sysTime;			
					GetLocalTime( &sysTime );			//获得系统当前时间
					sprintf(chError, "(%s, %s, %d, %d), GetDtmf [%s] error, callTimes=%d, CheckErrCount=%d, %04d%02d%02d-%02d:%02d:%02d:%03d", 
						GetString_DeviceMain(pOneTrunk->deviceID.m_s16DeviceMain),
						GetString_DeviceSub(pOneTrunk->deviceID.m_s16DeviceSub),
						pOneTrunk->deviceID.m_s8ModuleID,
						pOneTrunk->deviceID.m_s16ChannelID,
						pOneTrunk->DtmfBuf, 
						pOneTrunk->iTotalCnt,
						pOneTrunk->iCheckErrCnt,
						sysTime.wYear, 
						sysTime.wMonth, 
						sysTime.wDay, 
						sysTime.wHour, 
						sysTime.wMinute, 
						sysTime.wSecond, 
						sysTime.wMilliseconds);
					WriteLog(LEVEL_ERROR, chError);	
					AddMsg(chError);			
				}else
				{
					pOneTrunk->iSucCnt++;
					DeleteFile(FileName);//删除录音文件
				}							
			}					
		}
		XMS_ctsSetDevTimer ( g_acsHandle, &pOneTrunk->deviceID, 0 );//清除定时器		
		ResetTrunk ( pOneTrunk );	
		Change_State (pOneTrunk, TRK_FREE);
		DrawTrunk_DTMF(pOneTrunk);
		DrawMain_EstableTime(pOneTrunk);		
		
		return ; 
	}
	
	switch(pOneTrunk->State)
	{
	case TRK_FREE:
		break;
		
	case TRK_CALLOUT: // 带语音检测,User摘机
		if ( pAcsEvt->m_s32EventType == XMS_EVT_CALLOUT )	
		{
			XMS_ctsGetDevState( g_acsHandle, &pOneTrunk->deviceID, NULL);
			pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);
			
			if ( pCallControl->m_s32AcsEvtState == 1 )	// 呼出成功，对方摘机
			{
				pOneTrunk->iEstablishTime = GetTickCount() - pOneTrunk->iCallOutTime;
				g_iEstablishSum += pOneTrunk->iEstablishTime;
				DrawMain_EstableTime(pOneTrunk);
				g_iCallSuc++;
				
				if ( pOneTrunk->VocDevID.m_s16DeviceMain == 0) 
				{
					retCode = SearchOneFreeVoice ( pOneTrunk,  &FreeVocDeviceID );
					if ( retCode >=0 )
					{
						pOneTrunk->u8PlayTag = 0;					
						pOneTrunk->VocDevID = FreeVocDeviceID;
						M_OneVoice(FreeVocDeviceID).UsedDevID = pOneTrunk->deviceID; 
						My_DualLink(&pOneTrunk->deviceID, &FreeVocDeviceID);					
						DrawMain_VocInfo ( pOneTrunk );
					}else
					{
						Change_State(pOneTrunk, TRK_WAITHANGUP);
						XMS_ctsClearCall ( g_acsHandle, &pOneTrunk->deviceID, 0, NULL );
						sprintf(tmpStr,"(%s, %s, %d, %d), XMS_ctsClearCall(), No Free Voice",
							GetString_DeviceMain(pOneTrunk->deviceID.m_s16DeviceMain),
							GetString_DeviceSub(pOneTrunk->deviceID.m_s16DeviceSub),
							pOneTrunk->deviceID.m_s8ModuleID,
							pOneTrunk->deviceID.m_s16ChannelID);
						WriteLog(LEVEL_ERROR,tmpStr);
						AddMsg(tmpStr);
					}	
				}					
				sprintf(FileName, "%ssig%d.pcm", cfg_VocPath, (pOneTrunk->iSeqID)%15);
				PlayFile (&pOneTrunk->VocDevID, FileName, pOneTrunk->u8PlayTag, false);
				
				
				Change_State (pOneTrunk, TRK_PLAY);
						
				XMS_ctsSetDevTimer ( g_acsHandle, &pOneTrunk->deviceID, g_iElapseTime );
				sprintf(tmpStr,"(%s, %s, %d, %d), XMS_ctsSetDevTimer(), g_iElapseTime:%d",
					GetString_DeviceMain(pOneTrunk->deviceID.m_s16DeviceMain),
					GetString_DeviceSub(pOneTrunk->deviceID.m_s16DeviceSub),
					pOneTrunk->deviceID.m_s8ModuleID,
					pOneTrunk->deviceID.m_s16ChannelID,
					g_iElapseTime);
				WriteLog(LEVEL_DEBUG,tmpStr);

			}else		// 呼出失败
			{
				DrawMain_Reason ( pOneTrunk, pCallControl->m_s32AcsEvtErrCode, pCallControl->m_s32CallClearCause );
				Change_State ( pOneTrunk, TRK_FREE );
				g_iTotalCallErr++;
				pOneTrunk->iCallErrCnt++;
			}	
		}	
		break;
		
	case TRK_PLAY:
        if ( CheckPlayEnd ( pOneTrunk, pAcsEvt))
		{
			pOneTrunk->u8PlayTag++;	
			GetFilePathName(pOneTrunk, TmpFileName);
			sprintf(FileName, "%sCaller%03d_DSP%02d_Chn%03d_Rec.pcm", TmpFileName, pOneTrunk->iSeqID,pOneTrunk->deviceID.m_s8ModuleID,pOneTrunk->deviceID.m_s16ChannelID);	
			RecordFile ( &pOneTrunk->VocDevID, FileName, CALLER_RECORD_TIME, false);//isdn caller

			Change_State (pOneTrunk, TRK_RECORDFILE);
		}
		break;
		
	case TRK_RECORDFILE:
		TmpDtmf = My_GetDtmfCode ( pOneTrunk, pAcsEvt );
		if (TmpDtmf != -1)
		{
			My_AddDtmfBuf ( pOneTrunk, TmpDtmf );
			DrawTrunk_DTMF ( pOneTrunk );
		}
		
		if (CheckRecordEnd(pOneTrunk, pAcsEvt))
		{
			Change_State (pOneTrunk, TRK_WAITHANGUP);//isdn caller
		}
		break;
	}
}

/************************************************************************/
/* CAS主叫端                                                           */
/************************************************************************/
void TrunkWork_CAS_CALLER( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	int         r=0;
	DeviceID_t   FreeVocDeviceID;
	Acs_CallControl_Data *	pCallControl = NULL;
	char        FileName[256] = {0};
	char		TmpDtmf;
	char        TmpFileName[256] = "";
	char        chError[256] = "";
	char        DTMFBuff[12]="";
	char		tmpStr[512]={0};
	int			retCode = -1;
	if (pOneTrunk == NULL)
	{
		return;
	}
	if ( pAcsEvt->m_s32EventType == XMS_EVT_DEV_TIMER )	/*定时器事件*/
	{
		Change_State(pOneTrunk, TRK_WAITHANGUP);
		XMS_ctsClearCall ( g_acsHandle, &pOneTrunk->deviceID, 0 ,NULL );
		sprintf(tmpStr,"(%s, %s, %d, %d), XMS_ctsClearCall(), 定时器超时,主动拆线",
			GetString_DeviceMain(pOneTrunk->deviceID.m_s16DeviceMain),
			GetString_DeviceSub(pOneTrunk->deviceID.m_s16DeviceSub),
			pOneTrunk->deviceID.m_s8ModuleID,
			pOneTrunk->deviceID.m_s16ChannelID);
		WriteLog(LEVEL_ERROR, tmpStr);
		AddMsg(tmpStr);
		return;
	}
	
	if ( pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL )	/*拆线事件*/
	{
		XMS_ctsGetDevState( g_acsHandle, &pOneTrunk->deviceID, NULL);
		if (pOneTrunk->State == TRK_WAITHANGUP)
		{
			if (cfg_iVoiceCheck == 1)
			{
				GetFilePathName(pOneTrunk, TmpFileName);
				sprintf(FileName, "%sCaller%03d_DSP%02d_Chn%03d_Rec.pcm", TmpFileName, pOneTrunk->iSeqID,pOneTrunk->deviceID.m_s8ModuleID,pOneTrunk->deviceID.m_s16ChannelID);	

				if(!IsCorrectDtmf(pOneTrunk))//接收DTMF出错
				{
					sprintf (TmpFileName, "%sWrong_Caller%03d_%d.pcm", TmpFileName, pOneTrunk->iSeqID,pOneTrunk->iTotalCnt);
					rename ( FileName, TmpFileName );
					
					pOneTrunk->iCheckErrCnt++;
					g_iTotalCheckErr++;
					SYSTEMTIME	sysTime;			
					GetLocalTime( &sysTime );			//获得系统当前时间
					sprintf(chError, "(%s, %s, %d, %d), GetDtmf [%s] error, callTimes=%d, CheckErrCount=%d, %04d%02d%02d-%02d:%02d:%02d:%03d", 
						GetString_DeviceMain(pOneTrunk->deviceID.m_s16DeviceMain),
						GetString_DeviceSub(pOneTrunk->deviceID.m_s16DeviceSub),
						pOneTrunk->deviceID.m_s8ModuleID,
						pOneTrunk->deviceID.m_s16ChannelID,
						pOneTrunk->DtmfBuf, 
						pOneTrunk->iTotalCnt,
						pOneTrunk->iCheckErrCnt,
						sysTime.wYear, 
						sysTime.wMonth, 
						sysTime.wDay, 
						sysTime.wHour, 
						sysTime.wMinute, 
						sysTime.wSecond, 
						sysTime.wMilliseconds);
					WriteLog(LEVEL_ERROR, chError);	
					AddMsg(chError);
				}else
				{
					pOneTrunk->iSucCnt++;
					DeleteFile(FileName);//删除录音文件
				}							
			}					
		}
		XMS_ctsSetDevTimer ( g_acsHandle, &pOneTrunk->deviceID, 0 );//清除定时器		
		ResetTrunk ( pOneTrunk );	
		Change_State (pOneTrunk, TRK_FREE);
		DrawTrunk_DTMF(pOneTrunk);
		DrawMain_EstableTime(pOneTrunk);		
		
		return ; 
	}
	
	switch(pOneTrunk->State)
	{
	case TRK_FREE:
		//Special code for CAS(SS1)
		SpecialForCas(pOneTrunk,pAcsEvt);
		break;
		
	case TRK_CALLOUT: // 带语音检测, CAS摘机
		// Special code for CAS(SS1)
		SpecialForCas(pOneTrunk,pAcsEvt);
		if ( pAcsEvt->m_s32EventType == XMS_EVT_CALLOUT )	
		{
			XMS_ctsGetDevState( g_acsHandle, &pOneTrunk->deviceID, NULL);
			pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);
			
			if ( pCallControl->m_s32AcsEvtState == 1 )	// 呼出成功，对方摘机
			{
				pOneTrunk->iEstablishTime = GetTickCount() - pOneTrunk->iCallOutTime;
				g_iEstablishSum += pOneTrunk->iEstablishTime;
				DrawMain_EstableTime(pOneTrunk);
				g_iCallSuc++;
				
				if ( pOneTrunk->VocDevID.m_s16DeviceMain == 0) 
				{
					retCode = SearchOneFreeVoice ( pOneTrunk,  &FreeVocDeviceID );
					if (retCode >=0)
					{			
						pOneTrunk->VocDevID = FreeVocDeviceID;			
						M_OneVoice(FreeVocDeviceID).UsedDevID = pOneTrunk->deviceID; 				
						My_DualLink ( &FreeVocDeviceID, &pOneTrunk->deviceID ); 
						DrawMain_VocInfo ( pOneTrunk );
					}else
					{
						Change_State(pOneTrunk, TRK_WAITHANGUP);
						XMS_ctsClearCall ( g_acsHandle, &pOneTrunk->deviceID, 0, NULL );
						sprintf(tmpStr,"Error, find voc fail!, CMD: XMS_ctsClearCall(), (%s, %s, %d, %d).",
							GetString_DeviceMain(pOneTrunk->deviceID.m_s16DeviceMain),
							GetString_DeviceSub(pOneTrunk->deviceID.m_s16DeviceSub),
							pOneTrunk->deviceID.m_s8ModuleID,
							pOneTrunk->deviceID.m_s16ChannelID);
						WriteLog(LEVEL_ERROR, tmpStr);
						AddMsg(tmpStr);
						return;
					}				
					
				}			pOneTrunk->u8PlayTag = 0;					
				//播放指定语音文件					
				sprintf(FileName, "%ssig%d.pcm", cfg_VocPath, (pOneTrunk->iSeqID)%15);
				PlayFile (&pOneTrunk->VocDevID, FileName, pOneTrunk->u8PlayTag, false);
				Change_State (pOneTrunk, TRK_PLAY);
				
				XMS_ctsSetDevTimer ( g_acsHandle, &pOneTrunk->deviceID, g_iElapseTime );
				
			}else		// 呼出失败
			{
				DrawMain_Reason ( pOneTrunk, pCallControl->m_s32AcsEvtErrCode, pCallControl->m_s32CallClearCause );
				Change_State ( pOneTrunk, TRK_FREE );
				g_iTotalCallErr++;
				pOneTrunk->iCallErrCnt++;
				GetFilePathName(pOneTrunk, TmpFileName);
				sprintf(FileName, "%sCASMFC_Rec_%03d.pcm", TmpFileName, pOneTrunk->iSeqID);
				
				sprintf (TmpFileName, "%sWrong_CASMFC_Rec_%03d.pcm", TmpFileName, pOneTrunk->iSeqID,pOneTrunk->iTotalCnt);
				rename ( FileName, TmpFileName );
					
				sprintf(tmpStr,"Error, XMS_EVT_CALLOUT fail! (%s, %s, %d, %d). m_s32AcsEvtErrCode:%d, m_s32CallClearCause%d, iTotalCnt:%d",
					GetString_DeviceMain(pOneTrunk->deviceID.m_s16DeviceMain),
					GetString_DeviceSub(pOneTrunk->deviceID.m_s16DeviceSub),
					pOneTrunk->deviceID.m_s8ModuleID,
					pOneTrunk->deviceID.m_s16ChannelID,
					pCallControl->m_s32AcsEvtErrCode, 
					pCallControl->m_s32CallClearCause,
					pOneTrunk->iTotalCnt);
				WriteLog(LEVEL_ERROR, tmpStr);
				AddMsg(tmpStr);
			}	
		}	
		break;
		
	case TRK_PLAY:
        if ( CheckPlayEnd ( pOneTrunk, pAcsEvt))
		{
			pOneTrunk->u8PlayTag++;	
			GetFilePathName(pOneTrunk, TmpFileName);
			sprintf(FileName, "%sCaller%03d_DSP%02d_Chn%03d_Rec.pcm", TmpFileName, pOneTrunk->iSeqID,pOneTrunk->deviceID.m_s8ModuleID,pOneTrunk->deviceID.m_s16ChannelID);	
			RecordFile ( &pOneTrunk->VocDevID, FileName, CALLER_RECORD_TIME, false);//isdn caller

			Change_State (pOneTrunk, TRK_RECORDFILE);
		}
		break;
		
	case TRK_RECORDFILE:
		TmpDtmf = My_GetDtmfCode ( pOneTrunk, pAcsEvt );
		if (TmpDtmf != -1)
		{
			My_AddDtmfBuf ( pOneTrunk, TmpDtmf );
			DrawTrunk_DTMF ( pOneTrunk );
		}
		
		if (CheckRecordEnd(pOneTrunk, pAcsEvt)) //cas caller
		{
			Change_State (pOneTrunk, TRK_WAITHANGUP);
		}
		break;
	}
}
/************************************************************************/
/* 模拟中继事件处理函数,模拟中继收到User的呼叫，应答，录音&收DTMF，		*/
/* 将录音回放，拆线		                                                */
/************************************************************************/
void AnalogTrunkWork ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	Acs_CallControl_Data *	pCallControl = NULL;
	Acs_AnalogInterface_Data *pInterfaceData=NULL;
	DeviceID_t				FreeVocDeviceID;
	char					FileName[256];
	char                    chTmpFilePath[256] = "";
    char                    tmpStr[256]={0};
	char				    TmpDtmf;
	
	if ( pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL )	/*拆线事件*/
	{
		XMS_ctsGetDevState( g_acsHandle, &pOneTrunk->deviceID, NULL);
		if (cfg_iVoiceCheck == 1)
		{
			GetFilePathName(pOneTrunk, chTmpFilePath);
			sprintf(FileName, "%sATrk%03d_Rec.pcm", chTmpFilePath, pOneTrunk->iSeqID);
			//删除录音文件			
			DeleteFile(FileName);
		}		
		ResetTrunk ( pOneTrunk);
		Change_State(pOneTrunk, TRK_FREE);
		
		return ; 
	}
	
	switch(pOneTrunk->State)
	{
	case TRK_FREE:
		// Special code for CAS(SS1)
//		SpecialForCas(pOneTrunk,pAcsEvt);
		if (pAcsEvt->m_s32EventType == XMS_EVT_ANALOG_INTERFACE)
		{
			pInterfaceData = (Acs_AnalogInterface_Data *)FetchEventData(pAcsEvt);
			if (pInterfaceData->m_u8AnalogInterfaceState == 0)
			{
				sprintf(tmpStr,"(%s, %s, %d, %d) Ring.", 
					GetString_DeviceMain(pOneTrunk->deviceID.m_s16DeviceMain),
					GetString_DeviceSub(pOneTrunk->deviceID.m_s16DeviceSub),
					pOneTrunk->deviceID.m_s8ModuleID,
					pOneTrunk->deviceID.m_s16ChannelID);
				//AddMsg(tmpStr);
				Change_State ( pOneTrunk, TRK_RING );
			}
		}
		break;
	case TRK_RING:
		if ( pAcsEvt->m_s32EventType == XMS_EVT_CALLIN)
		{
			pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);
			XMS_ctsAlertCall ( g_acsHandle, &pOneTrunk->deviceID, NULL );			
			XMS_ctsAnswerCallIn ( g_acsHandle, &pOneTrunk->deviceID, NULL );
			
			sprintf ( pOneTrunk->CalleeCode, pCallControl->m_s8CalledNum );
			sprintf ( pOneTrunk->CallerCode, pCallControl->m_s8CallingNum );
			DrawMain_CallInfo( pOneTrunk );
			
			Change_State ( pOneTrunk, TRK_WAIT_ANSWERCALL );
		}
		break;
		
	case TRK_WAIT_ANSWERCALL:
		// Special code for CAS(SS1)
//		SpecialForCas(pOneTrunk,pAcsEvt);
		
		if ( pAcsEvt->m_s32EventType == XMS_EVT_ANSWERCALL )	/*应答呼入完成事件*/
		{
			if ( SearchOneFreeVoice ( pOneTrunk,  &FreeVocDeviceID ) >= 0 )
			{
				pOneTrunk->u8PlayTag = 16;			
				pOneTrunk->VocDevID = FreeVocDeviceID;			
				M_OneVoice(FreeVocDeviceID).UsedDevID = pOneTrunk->deviceID; 				
				My_DualLink ( &FreeVocDeviceID, &pOneTrunk->deviceID ); 
				DrawMain_VocInfo ( pOneTrunk );
				//开始录音
				GetFilePathName(pOneTrunk, chTmpFilePath);
				sprintf(FileName, "%sAnaTrk%03d_DSP%02d_Chn%03d_Rec.pcm", chTmpFilePath, pOneTrunk->iSeqID,pOneTrunk->deviceID.m_s8ModuleID,pOneTrunk->deviceID.m_s16ChannelID);		
				RecordFile ( &pOneTrunk->VocDevID, FileName, CALLED_RECORD_TIME, false); //analog trunk
				Change_State ( pOneTrunk, TRK_RECORDFILE );
			}else
			{	
				Change_State(pOneTrunk, TRK_WAITHANGUP);
				XMS_ctsClearCall ( g_acsHandle, &pOneTrunk->deviceID, 0, NULL );
				sprintf(tmpStr,"(%s, %s, %d, %d), XMS_ctsClearCall(), No Free Voice,主动拆线",
					GetString_DeviceMain(pOneTrunk->deviceID.m_s16DeviceMain),
					GetString_DeviceSub(pOneTrunk->deviceID.m_s16DeviceSub),
					pOneTrunk->deviceID.m_s8ModuleID,
					pOneTrunk->deviceID.m_s16ChannelID);
				AddMsg(tmpStr);				
			}
		}
		break;
				
	case TRK_RECORDFILE:
		TmpDtmf = My_GetDtmfCode ( pOneTrunk, pAcsEvt );
		if (TmpDtmf != -1)
		{
			My_AddDtmfBuf ( pOneTrunk, TmpDtmf );
			DrawTrunk_DTMF ( pOneTrunk );
		}
		if ( CheckRecordEnd ( pOneTrunk, pAcsEvt) )	/*analog trunk, record完成事件*/
		{
			GetFilePathName(pOneTrunk, chTmpFilePath);
			sprintf(FileName, "%sAnaTrk%03d_DSP%02d_Chn%03d_Rec.pcm", chTmpFilePath, pOneTrunk->iSeqID,pOneTrunk->deviceID.m_s8ModuleID,pOneTrunk->deviceID.m_s16ChannelID);		
			pOneTrunk->u8PlayTag ++;
			PlayFile ( &pOneTrunk->VocDevID, FileName, pOneTrunk->u8PlayTag, false );
			Change_State (pOneTrunk, TRK_PLAYRECORD);
			My_InitDtmfBuf ( pOneTrunk );
			DrawTrunk_DTMF(pOneTrunk);			
		}	
		break;
		
	case TRK_PLAYRECORD:
		if ( CheckPlayEnd ( pOneTrunk, pAcsEvt) )	/*play完成事件*/
		{
			XMS_ctsClearCall ( g_acsHandle, &pOneTrunk->deviceID, 0, NULL );			// 主动挂机				
			Change_State ( pOneTrunk, TRK_WAITHANGUP );
			sprintf(tmpStr,"(%s, %s, %d, %d), XMS_ctsClearCall(), 放音完成, 中继侧, 主动拆线",
				GetString_DeviceMain(pOneTrunk->deviceID.m_s16DeviceMain),
				GetString_DeviceSub(pOneTrunk->deviceID.m_s16DeviceSub),
				pOneTrunk->deviceID.m_s8ModuleID,
				pOneTrunk->deviceID.m_s16ChannelID);
			WriteLog(LEVEL_DEBUG,tmpStr);
		}
		break;
	}
}
/************************************************************************/
/* ISDN 被叫端                                                          */
/************************************************************************/
void TrunkWork_ISDN_CALLED( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	Acs_CallControl_Data *	pCallControl = NULL;
	Acs_AnalogInterface_Data *pInterfaceData=NULL;
	DeviceID_t				FreeVocDeviceID;
	char					FileName[256];
	char                    chTmpFilePath[256] = "";
    char                    tmpStr[256]={0};
	char				    TmpDtmf;
	char					logBuf[512]={0};
	if ( pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL )	/*拆线事件*/
	{
		XMS_ctsGetDevState( g_acsHandle, &pOneTrunk->deviceID, NULL);
		if (cfg_iVoiceCheck == 1)
		{
			GetFilePathName(pOneTrunk, chTmpFilePath);
			sprintf(FileName, "%sCalled%03d_DSP%02d_Chn%03d_Rec.pcm", chTmpFilePath, pOneTrunk->iSeqID,pOneTrunk->deviceID.m_s8ModuleID,pOneTrunk->deviceID.m_s16ChannelID);		

			//删除录音文件			
			DeleteFile(FileName);
		}		
		ResetTrunk ( pOneTrunk);
		Change_State(pOneTrunk, TRK_FREE);
		
		return ; 
	}
	
	switch(pOneTrunk->State)
	{
	case TRK_FREE:
		// Special code for CAS(SS1)
// 		SpecialForCas(pOneTrunk,pAcsEvt);
// 		if (pAcsEvt->m_s32EventType == XMS_EVT_ANALOG_INTERFACE)
// 		{
// 			pInterfaceData = (Acs_AnalogInterface_Data *)FetchEventData(pAcsEvt);
// 			if (pInterfaceData->m_u8AnalogInterfaceState == 0)
// 			{
// 				sprintf(tmpStr,"(%s, %s, %d, %d) Ring.", 
// 					GetString_DeviceMain(pOneTrunk->deviceID.m_s16DeviceMain),
// 					GetString_DeviceSub(pOneTrunk->deviceID.m_s16DeviceSub),
// 					pOneTrunk->deviceID.m_s8ModuleID,
// 					pOneTrunk->deviceID.m_s16ChannelID);
// 				AddMsg(tmpStr);
// 				Change_State ( pOneTrunk, TRK_RING );
// 			}
// 		}
// 		break;
// 	case TRK_RING:
		if ( pAcsEvt->m_s32EventType == XMS_EVT_CALLIN)
		{
			pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);
			XMS_ctsAlertCall ( g_acsHandle, &pOneTrunk->deviceID, NULL );		
			
			XMS_ctsAnswerCallIn ( g_acsHandle, &pOneTrunk->deviceID, NULL );
			
			sprintf ( pOneTrunk->CalleeCode, pCallControl->m_s8CalledNum );
			sprintf ( pOneTrunk->CallerCode, pCallControl->m_s8CallingNum );
			DrawMain_CallInfo( pOneTrunk );
			
			Change_State ( pOneTrunk, TRK_WAIT_ANSWERCALL );
		}
		break;
		
	case TRK_WAIT_ANSWERCALL:
		// Special code for CAS(SS1)
		//SpecialForCas(pOneTrunk,pAcsEvt);
		
		if ( pAcsEvt->m_s32EventType == XMS_EVT_ANSWERCALL )	/*应答呼入完成事件*/
		{
			XMS_ctsGetDevState( g_acsHandle, &pOneTrunk->deviceID, NULL);
			pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);
			
			if ( SearchOneFreeVoice ( pOneTrunk,  &FreeVocDeviceID ) >= 0 )
			{
				pOneTrunk->u8PlayTag = 16;			
				pOneTrunk->VocDevID = FreeVocDeviceID;			
				M_OneVoice(FreeVocDeviceID).UsedDevID = pOneTrunk->deviceID; 				
				My_DualLink ( &FreeVocDeviceID, &pOneTrunk->deviceID ); 
				DrawMain_VocInfo ( pOneTrunk );
				//开始录音
				GetFilePathName(pOneTrunk, chTmpFilePath);
				sprintf(FileName, "%sCalled%03d_DSP%02d_Chn%03d_Rec.pcm", chTmpFilePath, pOneTrunk->iSeqID,pOneTrunk->deviceID.m_s8ModuleID,pOneTrunk->deviceID.m_s16ChannelID);		
				RecordFile ( &pOneTrunk->VocDevID, FileName, CALLED_RECORD_TIME, false);//isdn called
				Change_State ( pOneTrunk, TRK_RECORDFILE );
			}			
		}
		break;
				
	case TRK_RECORDFILE:
		TmpDtmf = My_GetDtmfCode ( pOneTrunk, pAcsEvt );
		if (TmpDtmf != -1)
		{
			My_AddDtmfBuf ( pOneTrunk, TmpDtmf );
			DrawTrunk_DTMF ( pOneTrunk );
		}
		
		if ( CheckRecordEnd ( pOneTrunk, pAcsEvt) )	/*record完成事件*/
		{
			GetFilePathName(pOneTrunk, chTmpFilePath);
			sprintf(FileName, "%sCalled%03d_DSP%02d_Chn%03d_Rec.pcm", chTmpFilePath, pOneTrunk->iSeqID,pOneTrunk->deviceID.m_s8ModuleID,pOneTrunk->deviceID.m_s16ChannelID);		
			pOneTrunk->u8PlayTag ++;
			PlayFile ( &pOneTrunk->VocDevID, FileName, pOneTrunk->u8PlayTag, false );
			Change_State (pOneTrunk, TRK_PLAYRECORD);
			My_InitDtmfBuf ( pOneTrunk );
			DrawTrunk_DTMF(pOneTrunk);			
		}	
		break;
		
	case TRK_PLAYRECORD:
		if ( CheckPlayEnd ( pOneTrunk, pAcsEvt) )	/*play完成事件*/
		{
			XMS_ctsClearCall ( g_acsHandle, &pOneTrunk->deviceID, 0, NULL );			// 主动挂机				
			Change_State ( pOneTrunk, TRK_WAITHANGUP );
		}
		break;
	}
}
/************************************************************************/
/* CAS 被叫端                                                          */
/************************************************************************/
void TrunkWork_CAS_CALLED( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	Acs_CallControl_Data *	pCallControl = NULL;
	Acs_AnalogInterface_Data *pInterfaceData=NULL;
	DeviceID_t				FreeVocDeviceID;
	char					FileName[256];
	char                    chTmpFilePath[256] = "";
    char                    tmpStr[256]={0};
	char				    TmpDtmf;
	int						retCode=-1;
	if ( pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL )	/*拆线事件*/
	{
		XMS_ctsGetDevState( g_acsHandle, &pOneTrunk->deviceID, NULL);
		if (cfg_iVoiceCheck == 1)
		{
			GetFilePathName(pOneTrunk, chTmpFilePath);
			sprintf(FileName, "%sCalled%03d_DSP%02d_Chn%03d_Rec.pcm", chTmpFilePath, pOneTrunk->iSeqID,pOneTrunk->deviceID.m_s8ModuleID,pOneTrunk->deviceID.m_s16ChannelID);		

			//删除录音文件			
			DeleteFile(FileName);
		}		
		ResetTrunk ( pOneTrunk);
		Change_State(pOneTrunk, TRK_FREE);
		
		return ; 
	}
	
	switch(pOneTrunk->State)
	{
	case TRK_FREE:
		// Special code for CAS(SS1)
 		SpecialForCas(pOneTrunk,pAcsEvt);
		if ( pAcsEvt->m_s32EventType == XMS_EVT_CALLIN)
		{
			pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);
			XMS_ctsAlertCall ( g_acsHandle, &pOneTrunk->deviceID, NULL );			
			XMS_ctsAnswerCallIn ( g_acsHandle, &pOneTrunk->deviceID, NULL );

			sprintf(tmpStr,"CMD: XMS_ctsAnswerCallIn(), (%s, %s, %d, %d).",
				GetString_DeviceMain(pOneTrunk->deviceID.m_s16DeviceMain),
				GetString_DeviceSub(pOneTrunk->deviceID.m_s16DeviceSub),
				pOneTrunk->deviceID.m_s8ModuleID,pOneTrunk->deviceID.m_s16ChannelID);
			WriteLog(LEVEL_DEBUG, tmpStr);

			sprintf ( pOneTrunk->CalleeCode, pCallControl->m_s8CalledNum );
			sprintf ( pOneTrunk->CallerCode, pCallControl->m_s8CallingNum );
			DrawMain_CallInfo( pOneTrunk );
			
			Change_State ( pOneTrunk, TRK_WAIT_ANSWERCALL );
		}
		break;
		
	case TRK_WAIT_ANSWERCALL:
		// Special code for CAS(SS1)
		//SpecialForCas(pOneTrunk,pAcsEvt);
		
		if ( pAcsEvt->m_s32EventType == XMS_EVT_ANSWERCALL )	/*应答呼入完成事件*/
		{
			XMS_ctsGetDevState( g_acsHandle, &pOneTrunk->deviceID, NULL);
			pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);
			
			if ( pOneTrunk->VocDevID.m_s16DeviceMain == 0) 
			{
				retCode = SearchOneFreeVoice ( pOneTrunk,  &FreeVocDeviceID );
				if (retCode >=0)
				{			
					pOneTrunk->VocDevID = FreeVocDeviceID;			
					M_OneVoice(FreeVocDeviceID).UsedDevID = pOneTrunk->deviceID; 				
					My_DualLink ( &FreeVocDeviceID, &pOneTrunk->deviceID ); 
					DrawMain_VocInfo ( pOneTrunk );
				}else
				{
					sprintf(tmpStr,"Error, search voc fail!, (%s, %s, %d, %d).",
						GetString_DeviceMain(pOneTrunk->deviceID.m_s16DeviceMain),
						GetString_DeviceSub(pOneTrunk->deviceID.m_s16DeviceSub),
						pOneTrunk->deviceID.m_s8ModuleID,pOneTrunk->deviceID.m_s16ChannelID);
					WriteLog(LEVEL_ERROR, tmpStr);
					AddMsg(tmpStr);
				}
			}
			
			pOneTrunk->u8PlayTag = 16;			
			//开始录音
			GetFilePathName(pOneTrunk, chTmpFilePath);
			sprintf(FileName, "%sATrk%03d_Rec.pcm", chTmpFilePath, pOneTrunk->iSeqID);		
			RecordFile ( &pOneTrunk->VocDevID, FileName, CALLED_RECORD_TIME, false);//cas called
			Change_State ( pOneTrunk, TRK_RECORDFILE );
			
		}
		break;
		
	case TRK_RECORDFILE:
		TmpDtmf = My_GetDtmfCode ( pOneTrunk, pAcsEvt );
		if (TmpDtmf != -1)
		{
			My_AddDtmfBuf ( pOneTrunk, TmpDtmf );
			DrawTrunk_DTMF ( pOneTrunk );
		}
		
		if ( CheckRecordEnd ( pOneTrunk, pAcsEvt) )	/*record完成事件*/
		{
			GetFilePathName(pOneTrunk, chTmpFilePath);
			sprintf(FileName, "%sCalled%03d_DSP%02d_Chn%03d_Rec.pcm", chTmpFilePath, pOneTrunk->iSeqID,pOneTrunk->deviceID.m_s8ModuleID,pOneTrunk->deviceID.m_s16ChannelID);		
			pOneTrunk->u8PlayTag ++;
			PlayFile ( &pOneTrunk->VocDevID, FileName, pOneTrunk->u8PlayTag, false );
			Change_State (pOneTrunk, TRK_PLAYRECORD);
			My_InitDtmfBuf ( pOneTrunk );
			DrawTrunk_DTMF(pOneTrunk);			
		}	
		break;
		
	case TRK_PLAYRECORD:
		if ( CheckPlayEnd ( pOneTrunk, pAcsEvt) )	/*play完成事件*/
		{
			XMS_ctsClearCall ( g_acsHandle, &pOneTrunk->deviceID, 0, NULL );			// 主动挂机	
			sprintf(tmpStr,"CMD: XMS_ctsClearCall(), (%s, %s, %d, %d).",
				GetString_DeviceMain(pOneTrunk->deviceID.m_s16DeviceMain),
				GetString_DeviceSub(pOneTrunk->deviceID.m_s16DeviceSub),
				pOneTrunk->deviceID.m_s8ModuleID,pOneTrunk->deviceID.m_s16ChannelID);
			WriteLog(LEVEL_DEBUG, tmpStr);
			Change_State ( pOneTrunk, TRK_WAITHANGUP );
		}
		break;
	}
}
/************************************************************************/
/* 录音模块处理函数                                                     */
/************************************************************************/
void AnalogRecWork ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	Acs_CallControl_Data *	pCallControl = NULL;
	Acs_AnalogInterface_Data *pInterfaceData=NULL;
	DeviceID_t				FreeVocDeviceID;
	char					FileName[256];
	char                    chTmpFilePath[256] = "";
    char                    tmpStr[256]={0};
	char				    TmpDtmf;
	
	if ( pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL )	/*拆线事件*/
	{
		XMS_ctsGetDevState( g_acsHandle, &pOneTrunk->deviceID, NULL);
		if (cfg_iVoiceCheck == 1)
		{
			GetFilePathName(pOneTrunk, chTmpFilePath);
			sprintf(FileName, "%sARec%03d_Rec.pcm", chTmpFilePath, pOneTrunk->iSeqID);
			//删除录音文件			
			DeleteFile(FileName);
		}		
		ResetTrunk ( pOneTrunk);
		Change_State(pOneTrunk, TRK_FREE);
		
		return ; 
	}
	
	switch(pOneTrunk->State)
	{
	case TRK_FREE:
// 		// Special code for CAS(SS1)
// 		SpecialForCas(pOneTrunk,pAcsEvt);
// 		if (pAcsEvt->m_s32EventType == XMS_EVT_ANALOG_INTERFACE)
// 		{
// 			pInterfaceData = (Acs_AnalogInterface_Data *)FetchEventData(pAcsEvt);
// 			if (pInterfaceData->m_u8AnalogInterfaceState == 0)
// 			{
// 				sprintf(tmpStr,"(%s, %s, %d, %d) Ring.", 
// 					GetString_DeviceMain(pOneTrunk->deviceID.m_s16DeviceMain),
// 					GetString_DeviceSub(pOneTrunk->deviceID.m_s16DeviceSub),
// 					pOneTrunk->deviceID.m_s8ModuleID,
// 					pOneTrunk->deviceID.m_s16ChannelID);
// 				AddMsg(tmpStr);
// 				Change_State ( pOneTrunk, TRK_RING );
// 			}
// 		}
// 		break;
// 	case TRK_RING:
		if ( pAcsEvt->m_s32EventType == XMS_EVT_CALLIN)
		{
			XMS_ctsGetDevState( g_acsHandle, &pOneTrunk->deviceID, NULL);
			pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);
			
			if ( SearchOneFreeVoice ( pOneTrunk,  &FreeVocDeviceID ) >= 0 )
			{
				pOneTrunk->u8PlayTag = 16;			
				pOneTrunk->VocDevID = FreeVocDeviceID;			
				M_OneVoice(FreeVocDeviceID).UsedDevID = pOneTrunk->deviceID; 				
				My_DualLink ( &FreeVocDeviceID, &pOneTrunk->deviceID ); 
				DrawMain_VocInfo ( pOneTrunk );
				//开始录音
				GetFilePathName(pOneTrunk, chTmpFilePath);
				sprintf(FileName, "%sARec%03d_Rec.pcm", chTmpFilePath, pOneTrunk->iSeqID);		
				RecordFile ( &pOneTrunk->VocDevID, FileName, NULL, false);//analog rec
				Change_State ( pOneTrunk, TRK_RECORDFILE );
			}			
		}
		break;
				
	case TRK_RECORDFILE:
		TmpDtmf = My_GetDtmfCode ( pOneTrunk, pAcsEvt );
		if (TmpDtmf != -1)
		{
			My_AddDtmfBuf ( pOneTrunk, TmpDtmf );
			DrawTrunk_DTMF ( pOneTrunk );
		}
		if ( pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL )
		{
			if ( strlen(pOneTrunk->DtmfBuf) !=20 )
			{
				pOneTrunk->iCheckErrCnt++;
				g_iTotalCheckErr++;
			}
			
			StopRecordFile ( &pOneTrunk->VocDevID );			
			ResetTrunk ( pOneTrunk );			
			Change_State ( pOneTrunk, TRK_FREE );
		}		
		break;
	}
		
}
bool	IsAnalogUser ( DJ_S16 s16DevSub )
{
	
	if  (s16DevSub == XMS_DEVSUB_ANALOG_USER)
		
		return true;
    else
		
		return false;
}
bool	IsAnalogTrunk ( DJ_S16 s16DevSub )
{
	
	if  (s16DevSub == XMS_DEVSUB_ANALOG_TRUNK)
		
		return true;
    else
		
		return false;
}
bool	IsDigitTrunk ( DJ_S16 s16DevSub )
{
	
	if ( (s16DevSub == XMS_DEVSUB_E1_PCM)
		|| (s16DevSub == XMS_DEVSUB_E1_CAS)
		|| (s16DevSub == XMS_DEVSUB_E1_DSS1)
		|| (s16DevSub == XMS_DEVSUB_E1_SS7_TUP)
		|| (s16DevSub == XMS_DEVSUB_E1_SS7_ISUP) 
		)
	{
		return true;
	}
	
	return false;
}
/************************************************************************/
/* 单通道的测试结果显示                                                 */
/************************************************************************/
void DrawMain_TrunkResult(TRUNK_STRUCT *pOneTrunk)
{
	char   TmpStr[500] = {0};
	
	if (pOneTrunk == NULL || pdlg == NULL )
	{
		return;	
	}
	if (pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER 
		|| (pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_E1_DSS1 && IsCallerE1(pOneTrunk->deviceID.m_s16ChannelID/32 +1))
		|| (pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_E1_SS7_TUP && IsCallerE1(pOneTrunk->deviceID.m_s16ChannelID/32 +1))
		|| (pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_E1_SS7_ISUP && IsCallerE1(pOneTrunk->deviceID.m_s16ChannelID/32 +1))
		|| (pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_E1_CAS && IsCallerE1(pOneTrunk->deviceID.m_s16ChannelID/32+1)))
	{
		sprintf(TmpStr, "%d/%d/%d", pOneTrunk->iTotalCnt, pOneTrunk->iCallErrCnt, pOneTrunk->iCheckErrCnt);
		pdlg->m_ListMain.SetItemText(pOneTrunk->iSeqID, 6, TmpStr);
	}

}

void DrawAllTrunkResult(void)
{
	char   MsgStr[100] = {0};
	static  unsigned long ulPostTimer = GetTickCount();
	unsigned long  ulCurTimer;
		
	ulCurTimer = GetTickCount();
	if (ulCurTimer - ulPostTimer >= 1000)
	{
		ulPostTimer = ulCurTimer;
		
		memset(MsgStr, 0, 100 );
		sprintf(MsgStr, "%d", g_iTotalCnt);
		pdlg->GetDlgItem(IDC_STATIC_Count)->SetWindowText(MsgStr);
		
		
		sprintf(MsgStr, "%ld", g_iTotalCallErr, g_iTotalCheckErr);
		pdlg->GetDlgItem(IDC_STATIC_Fail)->SetWindowText(MsgStr);
		
		sprintf(MsgStr, "%ld", g_iTotalCheckErr);
		pdlg->GetDlgItem(IDC_STATIC_DtmfErr)->SetWindowText(MsgStr);
		
		if (g_iTotalCnt != 0)
		{
			memset(MsgStr, 0, sizeof(MsgStr));
			sprintf(MsgStr, "%f%%/%f%%", ((float)(g_iTotalCallErr*100))/(float)(g_iTotalCnt),
				((float)(g_iTotalCheckErr*100))/(float)(g_iTotalCnt));
			pdlg->GetDlgItem(IDC_STATIC_SucRate)->SetWindowText(MsgStr);
		}
		
		if (g_iEstablishSum <= MAXLONGLONG && g_iCallSuc > 0)
		{
			sprintf(MsgStr, "%ld ms", g_iEstablishSum/g_iCallSuc);
			pdlg->GetDlgItem(IDC_STATIC_AvsSetUp)->SetWindowText(MsgStr);
			TRACE("Establise=%d, g_iCallSuc=%d\n", g_iEstablishSum, g_iCallSuc);
		}	
	}
}

void InitTrunkData(int  iTotalTrunk)
{
	int    i;	
	DJ_S8         s8ModID;
	DJ_S16        s16ChID;	
	TRUNK_STRUCT  *pOneTrunk = NULL;
	
	for (i=0; i<iTotalTrunk; i++)
	{
		s8ModID = MapTable_Trunk[i].m_s8ModuleID;
		s16ChID = MapTable_Trunk[i].m_s16ChannelID;	
		
		if (AllDeviceRes[s8ModID].pTrunk == NULL)	
			break;
		
		if((IsAnalogUser(AllDeviceRes[s8ModID].pTrunk[s16ChID].deviceID.m_s16DeviceSub) )\
			||(IsAnalogTrunk(AllDeviceRes[s8ModID].pTrunk[s16ChID].deviceID.m_s16DeviceSub) ))
		{
			pOneTrunk = &AllDeviceRes[s8ModID].pTrunk[s16ChID];
			
			//	if (pOneTrunk->iLineState != DCS_FREE)
			//		XMS_ctsClearCall ( g_acsHandle, &pOneTrunk->deviceID, 0 ,NULL );
			pOneTrunk->iCallOutTime = 0;
			pOneTrunk->iCallErrCnt = 0;
			pOneTrunk->iCheckErrCnt = 0;
			pOneTrunk->iEstablishTime = 0;
			pOneTrunk->iSucCnt = 0;
			pOneTrunk->iTotalCnt = 0;		
		}
		
	}	
	
	//	memset(pOneTrunk->CalleeCode, 0, sizeof(pOneTrunk->CalleeCode));
	//	memset(pOneTrunk->CallerCode, 0, sizeof(pOneTrunk->CallerCode));
}

bool  IsOverTimeTest(int iTotalTrunk)
{
	
	if (g_iTotalCnt > MAXLONGLONG 
		|| ((GetTickCount() - g_iTestStartTime) >= cfg_iTestTime*60*1000))
	{		
		return true;
	}
	
	return false;
}

/************************************************************************/
/* 初始化统计数据                                                       */
/************************************************************************/
void InitStatData()
{
	g_iTotalCnt = 0; 
	g_iTotalCheckErr = 0;
	g_iTotalCallErr = 0;
	g_iEstablishSum = 0;
}


void DrawTotalTestTime()
{
	char  TmpStr[100] = {0};
	LONGLONG lTotlalTime = 0;
	int     iH = 0;
	int     iM = 0;
	int     iS = 0;
	
	if (NULL == pdlg)
		return;
	
	lTotlalTime = (GetTickCount() - g_iTestStartTime)/1000;
	
	iH = (int)lTotlalTime/3600;
	iM = (int)(lTotlalTime - iH*3600)/60;
	iS = (int)(lTotlalTime - iH*3600 - iM * 60);
	sprintf(TmpStr, "%02d:%02d:%02d", iH, iM, iS);
	
	
	pdlg->GetDlgItem(IDC_STATIC_TestTimer)->SetWindowText(TmpStr);
}
int   GetPFBAerror(char *pPFBA)
{
	DJ_S8           s8ModID;
	DJ_S16          s16ChID;
	TRUNK_STRUCT	*pOneTrunk = NULL;
	int  Isite;
	
    for (int i=0; i<g_iTotalTrunk; i++)
	{
		s8ModID = MapTable_Trunk[i].m_s8ModuleID;
		s16ChID = MapTable_Trunk[i].m_s16ChannelID;	
		pOneTrunk = &AllDeviceRes[s8ModID].pTrunk[s16ChID];
		Isite=1+pOneTrunk->iSeqID/32;
		
		if(pOneTrunk->State == TRK_WAITOPEN&&
			pOneTrunk->deviceID.m_s16DeviceMain==XMS_DEVMAIN_INTERFACE_CH&&
			pOneTrunk->deviceID.m_s16DeviceSub!=XMS_DEVSUB_UNUSABLE)
		{
			if(0==Isite%2)
				sprintf(pPFBA,"MB%d",Isite/2+1);
			
			else
				sprintf(pPFBA,"MA%d",Isite/2+2);
			
			return 1;
		}
	}
	return 0;
	
}
DWORD WINAPI ThreadFunc(VOID) 
{ 
	int             i;
	RetCode_t       ret;
	DeviceID_t		FreeTrkDeviceID;
	TRUNK_STRUCT	*pOneTrunk = NULL;	
	char			MsgStr[256]={0};
	DJ_S8           s8ModID;
	DJ_S16          s16ChID;
	int             iTotalTrunk;
	int             iStartChn = 0, iEndChn = 0;
	
	if (pdlg == NULL)
		return 0;
	
	WriteLog(LEVEL_INFO, "Test Start");
	WriteLog(LEVEL_INFO, "------------------------------------------------------------------");
	InitStatData();
	InitTrunkData(g_iTotalTrunk);
	
	g_iTestStartTime = GetTickCount();
	
	
	while(g_threadRunning)
	{
		DrawTotalTestTime();
		DrawAllTrunkResult();

		iTotalTrunk = g_iTotalTrunk;				
		if (IsOverTimeTest(iTotalTrunk))
		{
			WriteTestResult();		
			pdlg->GetDlgItem(IDC_EDIT_Start)->EnableWindow(TRUE);
            pdlg->GetDlgItem(IDC_BUTTON_CHECK)->EnableWindow(TRUE);
			
			if(g_iTotalCheckErr!=0||g_iTotalCallErr!=0)
			{
				AfxMessageBox("Error!");
			}else
			{
				AfxMessageBox("OK");
			}
			return 0;
			
		}
		
		for (i=0; i<g_iTotalTrunk; i++)
		{
			s8ModID = MapTable_Trunk[i].m_s8ModuleID;
			s16ChID = MapTable_Trunk[i].m_s16ChannelID;	
			
			if (AllDeviceRes[s8ModID].pTrunk == NULL)
			{
				continue;
			}
			
			pOneTrunk = &AllDeviceRes[s8ModID].pTrunk[s16ChID];
			DrawMain_TrunkResult(pOneTrunk);	//坐席侧呼叫结果,每秒刷新一次
			
			if (pOneTrunk->iCheckErrCnt > 0)
			{
				continue;//如果出错则此通道不再进行呼叫				
			}

			if (   (pOneTrunk->iLineState == DCS_FREE && pOneTrunk->State == TRK_FREE
				&& pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER)
				|| (pOneTrunk->iLineState == DCS_FREE && pOneTrunk->State == TRK_FREE
				&&pOneTrunk->deviceID.m_s16DeviceSub==XMS_DEVSUB_E1_DSS1 && IsCallerE1(pOneTrunk->deviceID.m_s16ChannelID/32+1) )
				|| (pOneTrunk->iLineState == DCS_FREE && pOneTrunk->State == TRK_FREE
				&&pOneTrunk->deviceID.m_s16DeviceSub==XMS_DEVSUB_E1_SS7_TUP && IsCallerE1(pOneTrunk->deviceID.m_s16ChannelID/32+1) )
				|| (pOneTrunk->iLineState == DCS_FREE && pOneTrunk->State == TRK_FREE
				&&pOneTrunk->deviceID.m_s16DeviceSub==XMS_DEVSUB_E1_SS7_ISUP && IsCallerE1(pOneTrunk->deviceID.m_s16ChannelID/32+1) )
				|| ( pOneTrunk->iLineState == DCS_FREE && pOneTrunk->State == TRK_FREE
				&&pOneTrunk->deviceID.m_s16DeviceSub==XMS_DEVSUB_E1_CAS && IsCallerE1(pOneTrunk->deviceID.m_s16ChannelID/32+1) )
				)
			{		
				
				FreeTrkDeviceID = pOneTrunk->deviceID;
				Sleep(30);	//增加呼叫延时
				ret = XMS_ctsMakeCallOut ( g_acsHandle, &FreeTrkDeviceID, cfg_CallingNum, cfg_SimCalledNum, NULL );
				if ( ret > 0 )
				{											
					Change_State (pOneTrunk, TRK_CALLOUT );	
					pOneTrunk->iCallOutTime = GetTickCount();					
					pOneTrunk->iTotalCnt++;
					sprintf ( MsgStr, "CMD: [%d] XMS_ctsMakeCallOut()  OK, (%s, %s, %d, %d)", 
						pOneTrunk->iTotalCnt,
						GetString_DeviceMain(pOneTrunk->deviceID.m_s16DeviceMain),
						GetString_DeviceSub(pOneTrunk->deviceID.m_s16DeviceSub),
						pOneTrunk->deviceID.m_s8ModuleID,
						pOneTrunk->deviceID.m_s16ChannelID );
					WriteLog(LEVEL_DEBUG, MsgStr);
					//AddMsg(MsgStr);
					g_iTotalCnt++;

				}else
				{
					sprintf ( MsgStr, "(%s, %s, %d, %d), XMS_ctsMakeCallOut() FAIL! ret = %d", 
						GetString_DeviceMain(pOneTrunk->deviceID.m_s16DeviceMain),
						GetString_DeviceSub(pOneTrunk->deviceID.m_s16DeviceSub),
						pOneTrunk->deviceID.m_s8ModuleID,
						pOneTrunk->deviceID.m_s16ChannelID,
						ret );
					WriteLog(LEVEL_ERROR, MsgStr);
					AddMsg(MsgStr);
				}
			}
			
			
		}
		
		Sleep(1000);
	}
	
	return 0; 
}  

bool GetFilePathName(TRUNK_STRUCT *pOneTrunk, char *pchFilePath)
{
	if (pOneTrunk==NULL || pchFilePath == NULL)
	{
		return false;
	}
	
	if (pOneTrunk->deviceID.m_s8ModuleID > 0)
	{
		sprintf(pchFilePath, "%s", cfg_VocPath);		
	}
	
	return true;	
}

void	SimulateCallOut(void)
{
	DWORD g_dwThreadID = 0;
	HANDLE hThread;
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ThreadFunc, NULL, 0, &g_dwThreadID);  
	
	if (hThread != NULL)
	{		
		CloseHandle(hThread);
	}
}

void   TrunkWork ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	if(XMS_DEVSUB_ANALOG_USER == pOneTrunk->deviceID.m_s16DeviceSub)
	{
		AnalogUserWork ( pOneTrunk, pAcsEvt );
	}else if(XMS_DEVSUB_ANALOG_TRUNK == pOneTrunk->deviceID.m_s16DeviceSub)
	{
		AnalogTrunkWork ( pOneTrunk, pAcsEvt );
	}else if (XMS_DEVSUB_ANALOG_REC == pOneTrunk->deviceID.m_s16DeviceSub)
	{
		AnalogRecWork ( pOneTrunk, pAcsEvt );

	}else if (XMS_DEVSUB_E1_DSS1 == pOneTrunk->deviceID.m_s16DeviceSub 
		&& IsCallerE1(pOneTrunk->deviceID.m_s16ChannelID/32 + 1 ) )						//ISDN 主叫端
	{
		TrunkWork_ISDN_CALLER ( pOneTrunk, pAcsEvt );
	}else if (XMS_DEVSUB_E1_DSS1 == pOneTrunk->deviceID.m_s16DeviceSub 
		&& IsCalledE1(pOneTrunk->deviceID.m_s16ChannelID/32 + 1 ) )						//ISDN被叫端
	{
		TrunkWork_ISDN_CALLED ( pOneTrunk, pAcsEvt );
	}else if (XMS_DEVSUB_E1_SS7_TUP == pOneTrunk->deviceID.m_s16DeviceSub 
		&& IsCallerE1(pOneTrunk->deviceID.m_s16ChannelID/32 + 1 ) )						//TUP 主叫端
	{
		TrunkWork_ISDN_CALLER ( pOneTrunk, pAcsEvt );
	}else if (XMS_DEVSUB_E1_SS7_TUP == pOneTrunk->deviceID.m_s16DeviceSub 
		&& IsCalledE1(pOneTrunk->deviceID.m_s16ChannelID/32 + 1 ) )						//TUP 被叫端
	{
		TrunkWork_ISDN_CALLED ( pOneTrunk, pAcsEvt );
	}else if (XMS_DEVSUB_E1_SS7_ISUP == pOneTrunk->deviceID.m_s16DeviceSub 
		&& IsCallerE1(pOneTrunk->deviceID.m_s16ChannelID/32 + 1 ) )						//ISUP 主叫端
	{
		TrunkWork_ISDN_CALLER ( pOneTrunk, pAcsEvt );
	}else if (XMS_DEVSUB_E1_SS7_ISUP == pOneTrunk->deviceID.m_s16DeviceSub 
		&& IsCalledE1(pOneTrunk->deviceID.m_s16ChannelID/32 + 1 ) )						//ISUP 被叫端
	{
		TrunkWork_ISDN_CALLED ( pOneTrunk, pAcsEvt );
	}else if (XMS_DEVSUB_E1_CAS == pOneTrunk->deviceID.m_s16DeviceSub 
		&& IsCallerE1(pOneTrunk->deviceID.m_s16ChannelID/32 + 1 ) )						//CAS 主叫端
	{
		TrunkWork_CAS_CALLER ( pOneTrunk, pAcsEvt );

	}else if (XMS_DEVSUB_E1_CAS == pOneTrunk->deviceID.m_s16DeviceSub 
		&& IsCalledE1(pOneTrunk->deviceID.m_s16ChannelID/32 + 1 ) )						//CAS 被叫端
	{
		TrunkWork_CAS_CALLED ( pOneTrunk, pAcsEvt );

	}
}
