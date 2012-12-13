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
char			cfg_SoftwareVersion[32]="V1.10";					//����汾��ʶ
char			cfg_IniName[256] = "";
char			cfg_IniShortName[] = "\\LoadAging.ini";
char			cfg_NormalPassword[128]={0};				//������Ա����
char			cfg_SuperPassword[128]={0};					//����ʦ����
char			cfg_DataRecordPath[256]="";					//���ݱ���·��
char			cfg_Title[256]="���Ӹ����ϻ�ϵͳ";			//����
int				cfg_EnableBeep;								//ʹ�ܷ�����
int				cfg_EnableLight;							//ʹ��ָʾ��
int				cfg_DisplayTestInfo;						//�Ƿ�ͳ�Ʋ�����Ϣ
int				cfg_PowerMax;								//��������ֵ
int				cfg_CurrentErrorPercent=10;					//����ƫ��ٷֱ� ���趨ֵ�Ļ����������ٷ�֮ʮ
int				cfg_VoltageErrorPercent=10;					//��ѹƫ��ٷֱ� ���趨ֵ�Ļ����������ٷ�֮ʮ
int				cfg_IsDealwithRecvData=1;					//�Ƿ�ʹ�����ݴ���
int				cfg_FixDataPercent=3;						//��ֵ��ƫ�Χǧ��֮1---3
int				cfg_NoFixDataPercent=3;						//�Ƕ�ֵ��ƫ�Χǧ��֮1---3

//ϵͳȫ�ֱ���--------------------------------------------
int				g_LoginCheckOK=-1;							//��¼У���Ƿ�ͨ��,-1:У��ʧ�ܣ�0:����ʦ��¼�ɹ���1:����Ա��¼�ɹ�
long			g_SystemRunTimeCounter=0;					//ϵͳ����ʱ�������
CAR_STRUCT		g_AllCar[MAX_CAR_COUNT];					//����Ҫ��ȫ�ֽṹ��
int				g_ComList[MAX_CAR_COUNT];					//���ô����б�
int				g_AvailableComCounter=0;					//���ô�����Ŀ
int				g_curSelTestCar=0;							//��ǰѡ�в��Գ�
int				g_curImportCar=0;							//��ǰѡ�е������ݵĲ��Գ�
CMyLog			g_data[MAX_CAR_COUNT];						//��¼����������־��
CSerialPort		g_Ports[MAX_CAR_COUNT];						//�����࣬����ÿ�����Գ���������
float			g_SetCurrentError=0.2;						//���õ���ֵƫ�Χ
float			g_SetVoltageError=2.0;						//���õ�ѹֵƫ�Χ
int				g_LastTimeSeriesCMD=LOAD_COMMAND_220V_ON;		//��һ��ʱ�����110V����220V���������忪�ص���������			
int				g_ValidHour=0;								//���ܹ�
const float EPSINON	= 0.000000	;	

/************************************************************************/
/* ��ȡ�����ļ�ini                                                      */
/************************************************************************/
void	ReadFromConfig(void)
{
	GetCurrentDirectory ( 256-32, cfg_IniName );
	strcat ( cfg_IniName, cfg_IniShortName );
	
	cfg_LogOn = GetPrivateProfileInt ( "ConfigInfo", "LogOn", 1, cfg_IniName);					//�Ƿ�����־
	cfg_LogLevel = GetPrivateProfileInt ( "ConfigInfo", "LogLevel", 0, cfg_IniName);			//��־����
	cfg_DisplayTestInfo = GetPrivateProfileInt ( "ConfigInfo", "DisplayTestInfo", 0, cfg_IniName);		//�Ƿ���ʾ����ͳ����Ϣ
	cfg_EnableBeep = GetPrivateProfileInt ( "ConfigInfo", "EnableBeep", 1, cfg_IniName);		//�Ƿ�ʹ�ܷ�����
	cfg_EnableLight = GetPrivateProfileInt ( "ConfigInfo", "EnableLight", 1, cfg_IniName);		//�Ƿ�ʹ��ָʾ��
	
	cfg_PowerMax = GetPrivateProfileInt ( "ConfigInfo", "PowerMax", 75, cfg_IniName);		//��������ֵ��add by lmy 20120613

	cfg_CurrentErrorPercent = GetPrivateProfileInt ( "ConfigInfo", "CurrentErrorPercent", 10, cfg_IniName);	
	cfg_VoltageErrorPercent = GetPrivateProfileInt ( "ConfigInfo", "VoltageErrorPercent", 10, cfg_IniName);	
	cfg_IsDealwithRecvData= GetPrivateProfileInt ( "ConfigInfo", "IsDealwithRecvData", 1, cfg_IniName);					//�Ƿ�ʹ�����ݴ���
	cfg_FixDataPercent = GetPrivateProfileInt ( "ConfigInfo", "FixDataPercent", 3, cfg_IniName);						//��ֵ��ƫ�Χǧ��֮1---3
	cfg_NoFixDataPercent= GetPrivateProfileInt ( "ConfigInfo", "NoFixDataPercent", 3, cfg_IniName);						//�Ƕ�ֵ��ƫ�Χǧ��֮1---3

	GetPrivateProfileString ( "ConfigInfo", "NormalPassword", "000000", cfg_NormalPassword, sizeof(cfg_NormalPassword), cfg_IniName);
	GetPrivateProfileString ( "ConfigInfo", "SuperPassword", "000000", cfg_SuperPassword, sizeof(cfg_SuperPassword), cfg_IniName);
	GetPrivateProfileString ( "ConfigInfo", "DataRecordPath", "..\\DataRecord\\", cfg_DataRecordPath, sizeof(cfg_DataRecordPath), cfg_IniName);
	GetPrivateProfileString ( "ConfigInfo", "Title", "", cfg_Title, sizeof(cfg_Title), cfg_IniName);
	
}

/************************************************************************/
/* д�����ļ�ini                                                        */
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
/* ϵͳ��ʼ������                                                       */
/************************************************************************/
bool InitSystem(void) 
{
	pdlg = (CLoadAgingDlg	*)theApp.m_pMainWnd;
	
	InitLogCfg();			//��ʼ��ϵͳ��־
	GetAvailableComList();	//��ʼ��ϵͳ��ǰ���ô����б�
	
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
/* ��ȡ����ʱ���ַ���                                                   */
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
/*��ȡ���Գ�״̬                                                        */
/************************************************************************/
char*	GetTestCarStateName(int carID)									
{
	if (carID<0||carID>MAX_CAR_COUNT)
	{
		return "ERRORID";
	}
	//���Գ���ǰ״̬���Լ�����ʱ�䡢���԰�ť��ʾ
	switch(g_AllCar[carID].m_CarState)
	{
	case CAR_STATE_NOIMPORT:
		return "δ����";
	case CAR_STATE_IMPORTED:
		return "�ѵ���";
	case CAR_STATE_TESTING:
		if(g_AllCar[carID].m_CarTestingState == CAR_STATE_TESTING_READ)
		{
			return "������";
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
			return "������";
		}
	case CAR_STATE_TESTFINISH:
		return "������";
	}
	return "UNKNOWN";
}
/************************************************************************/
/* ˢ�µ�ǰ���Գ�״̬��Ϣ                                               */
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
	//������Ϣ
	if(g_AllCar[g_curSelTestCar].m_ComID == 0)
	{
		sprintf(comStr,"��");
	}else
	{
		sprintf(comStr,"COM%d",g_AllCar[g_curSelTestCar].m_ComID);		
	}
	//��������ʱ��
	if(	CAR_STATE_TESTING == g_AllCar[g_curSelTestCar].m_CarState ||
		cfg_DisplayTestInfo==1)
	{
		DrawTotalTestTime(testTimeStr,g_curSelTestCar);	
	}else
	{
		strcpy(testTimeStr,"00:00:00");
	}
	
	//����������Ϣ
	if(	CAR_STATE_NOIMPORT == g_AllCar[g_curSelTestCar].m_CarState )
	{
		strcpy(testResultStr, "����:0,�ϸ�:0,����:0,������:0.00\%");
		
	}else
	{
		int	errorLoadCounter=0;	 //��������
		int	totalLoadCounter=g_AllCar[g_curSelTestCar].testParam.endLoadNum - g_AllCar[g_curSelTestCar].testParam.startLoadNum + 1;  //�ܹ������Ը�����Ŀ
		float errorPercent; 
		for (int loadIndex=g_AllCar[g_curSelTestCar].testParam.startLoadNum; loadIndex<=g_AllCar[g_curSelTestCar].testParam.endLoadNum; loadIndex++)
		{
			if(g_AllCar[g_curSelTestCar].m_Load[loadIndex-1].m_LoadErrorCounter != 0 )
			{
				errorLoadCounter++;
			}
		}
		errorPercent = (float)errorLoadCounter/(float)totalLoadCounter*100;//������
		sprintf(testResultStr, "����:%d,�ϸ�:%d,����:%d,������:%2.2f%%",
			totalLoadCounter,totalLoadCounter-errorLoadCounter,errorLoadCounter,errorPercent);
	}		
	
	sprintf(tmpStr,"��ǰ��:%s, ����:%s, ״̬:%s, ����:%d*%d, ɨ���ַ:[%d,%d], �������:%d, %s, ��ʱ��:%d����, ����ʱ��:%s",
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
/* ��ȡϵͳ�����豸�б�                                                 */
/************************************************************************/
void GetAvailableComList(void)
{
	int nCom;
	CString strCom;
	HANDLE hCom;
	
	//��ʼ��COM�б�,����16������,�򿪴��ڷ�Χ[1,99]------------------
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
/*��ȡ���Գ�����                                                        */
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
/*��ȡ����ģʽ����                                                      */
/************************************************************************/
char*	GetLoadModeName(int modeID)									
{
	switch(modeID)
	{
	case LOAD_MODE_CC:		return "CCģʽ";
	case LOAD_MODE_CV:		return "CVģʽ";
	}
	return "UNKNOWN";
}
/************************************************************************/
/*��ȡ����ģʽ����                                                      */
/************************************************************************/
char*	GetParaModeName(int modeID)									
{
	if (modeID == 0)
	{
		return "�޲���";
	}else if (modeID == 1)
	{
		return "��·����";
	}else if (modeID == 2)
	{
		return "��·����";
	}
	return "UNKNOWN";
}
char* GetParamSetMode(int paramSetModID)
{
	switch(paramSetModID)
	{
	case 0:		return "��������";
	case 1:		return "�ֲ�����";
	}
	return "UNKNOWN";
}
/************************************************************************/
/*��ȡ����ģʽ����                                                      */
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
/*��ȡͨ��״̬�����ַ���                                                */
/************************************************************************/
char*   GetChnStateName(int chnMode)
{
	switch (chnMode)
	{
	case	CHN_STATE_NOLINK:	return	"δ����";
	case	CHN_STATE_OK:		return	"�ϸ�";
	case	CHN_STATE_LOWVOL:	return	"Ƿѹ";
	case	CHN_STATE_OVERVOL:	return	"��ѹ";
	case	CHN_STATE_LOWCUR:	return	"Ƿ��";
	case	CHN_STATE_OVERCUR:	return	"����";
	}
	return "UNKNOWN";
}
/************************************************************************/
/*��ȡtitle                                                             */
/************************************************************************/
char*	GetChnTitleName(int modeID)									
{
	switch(modeID)
	{
	case LOAD_MODE_CC:		return "������ֵ,��ѹ����,��ѹ����,��Ʒ�ͺ�";
	case LOAD_MODE_CV:		return "����ѹֵ,��������,��������,��Ʒ�ͺ�";
	}
	return "UNKNOWN";
}
/************************************************************************/
/* ���ݴ��ںŲ��Ҳ��Գ�ID                                               */
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

	//���ݴ��ںŶ�λ���Գ�
	carID = GetCarIDByPortNo(portNo);	
	if ( carID < 0 )
	{
		sprintf(tmpStr,"Error, carID:%d is wrong!", carID);
		WriteLog(LEVEL_ERROR, tmpStr);
		return;
	}
	
	//����ȡ��������copy��recvbuffer������
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
		g_AllCar[carID].m_ReadEventCounter++;//ÿ̨���յ����¼�ͳ��
		if(cfg_DisplayTestInfo == 1)//����ʵʱ��ʾ��������ͳ����Ϣ	
		{	
			sprintf(tmpStr,"[%d/%d],%d",g_AllCar[g_curSelTestCar].m_ReadCommandCounter,g_AllCar[g_curSelTestCar].m_ReadEventCounter,
				g_AllCar[g_curSelTestCar].m_ReadCommandCounter-g_AllCar[g_curSelTestCar].m_ReadEventCounter);
			pdlg->SetDlgItemText(IDC_STATIC_SERPORT_TEST,tmpStr);
		}

	}

}

/************************************************************************/
/* ˢ�µ���LED��, curCarID:[0,15], loadIndex:[0,59], chnIndex:[0,3]     */
/************************************************************************/
void RefreshOneLED( int curCarID, int loadIndex, int chnIndex)
{
	CWnd*   pW;
	char	tmpStr[256];
	int		iPerLayer=0;//��ǰ��ĵڼ�������
	
	int iLedIndex=loadIndex*4+chnIndex;	//����Ƶ�ȫ��������,��Χ[0,239]
	if (iLedIndex<0 || iLedIndex>239)
	{
		return;
	}
	if(g_curSelTestCar != curCarID)
	{
		return;//����˶�ʱ�����ǵ�ǰѡ����������Ҫˢ��LED
	}
	//ˢ��LED��ɫ��������ɫ
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
		case LOAD_STATE_SELECT:		//�Ѿ�ѡ�У���ɫ
			pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorSelect);
			sprintf(tmpStr,"λ��:��%d���%d��, ����:%d, ͨ��:%d \n״̬:��ѡ��\n�趨:%s,%0.3f%s, %0.3f%s~%0.3f%s\nʵʱ:����= \n����= , ��ѹ=",
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
		case LOAD_STATE_NOLINK:		//δ���ӣ�Ҳ����������ص��ĸ�ͨ����û�нӵ�Դ����ɫ
			pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorSelect);
			sprintf(tmpStr,"λ��:��%d���%d��, ����:%d, ͨ��:%d \n״̬:δ����\n�趨:%s,%0.3f%s, %0.3f%s~%0.3f%s\nʵʱ:����= \n����= , ��ѹ=",
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
		case LOAD_STATE_NOSETPARAM:	//δ���ò�������Ҫ�������ò�������ɫ
			pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorSelect);
			sprintf(tmpStr,"λ��:��%d���%d��, ����:%d, ͨ��:%d \n״̬:δ����\n�趨:%s,%0.3f%s, %0.3f%s~%0.3f%s\nʵʱ:����= \n����= , ��ѹ=",
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
		case LOAD_STATE_WORKING:	//�����У�����״̬��ʵʱ��ȡ����
			{
				char testResult[5]={0};
				switch (g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_ChnState)
				{
				case CHN_STATE_NOLINK:	//δ���ӣ���ɫ
					pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorSelect);//��ɫ
					strcpy(testResult,"δ����");
					break;
				case CHN_STATE_OK:		//�ϸ���ɫ
					pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorGreen);//��ɫ		
					strcpy(testResult,"�ϸ�");
					break;
				case CHN_STATE_LOWVOL:	//Ƿѹ����ɫ
					pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorRed);//��ɫ						
					strcpy(testResult,"Ƿѹ");
					break;
				case CHN_STATE_OVERVOL:	//��ѹ����ɫ
					pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorRed);//��ɫ						
					strcpy(testResult,"��ѹ");
					break;
				case CHN_STATE_LOWCUR:	//Ƿ������ɫ
					pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorRed);//��ɫ						
					strcpy(testResult,"Ƿ��");
					break;
				case CHN_STATE_OVERCUR:	//��������ɫ
					pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorRed);//��ɫ
					strcpy(testResult,"����");
					break;
				}
				
				sprintf(tmpStr,"λ��:��%d���%d��, ����:%d, ͨ��:%d \n״̬:%s\n�趨:%s,%0.3f%s, %0.3f%s~%0.3f%s\nʵʱ:����=%0.2f \n����=%0.3f , ��ѹ=%0.3f",
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
		case LOAD_STATE_PROTECT:	//�¶ȹ��ߣ�ϵͳ���ڱ���״̬����ʱ����ֹͣ
			pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorBlack);
			sprintf(tmpStr,"λ��:��%d���%d��, ����:%d, ͨ��:%d \n״̬:����\n�趨:%s,%0.3f%s, %0.3f%s~%0.3f%s\nʵʱ:����= \n����= , ��ѹ=",
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
			pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorNotSelect);//δѡ��
			sprintf(tmpStr,"λ��:%d,  ״̬:δѡ��\n�趨\nʵʱ:����= \n����= , ��ѹ=",iLedIndex+1);
			
		}
		
		//ˢ����ʾ��
		pW=pdlg->GetDlgItem(1400+1+iLedIndex);
		pdlg->m_tooltip.AddTool(pW,tmpStr); 	
		
}
/************************************************************************/
/* ˢ�µ�ǰ���Գ���LED�ƣ�curCarID��Χ[0,15]                            */
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
	 
	//ѭ������LED��
	for(int iLedIndex=0; iLedIndex<MAX_LED_COUNT; iLedIndex++)
	{
		int loadIndex = iLedIndex/4;	//��LED�����ڵĸ��غ�, [0,49]
		int chnIndex = iLedIndex%4;		//��LED�����ڵ�ͨ����, [0,3]
		int	iPerLayer=0;
		
		//ˢ��LED��ɫ��������ɫ
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
			case LOAD_STATE_SELECT:		//�Ѿ�ѡ�У���ɫ
				pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorSelect);
				sprintf(tmpStr,"λ��:��%d���%d��, ����:%d, ͨ��:%d \n״̬:��ѡ��\n�趨:%s,%0.3f%s, %0.3f%s~%0.3f%s\nʵʱ:����= \n����= , ��ѹ=",
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
// 			case LOAD_STATE_NOLINK:		//δ���ӣ�Ҳ����������ص��ĸ�ͨ����û�нӵ�Դ����ɫ
// 				pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorSelect);
// 				sprintf(tmpStr,"λ��:��%d���%d��, ����:%d, ͨ��:%d \n״̬:δ����\n�趨:%s,%0.3f%s, %0.3f%s~%0.3f%s\nʵʱ:����= \n����= , ��ѹ=",
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
// 			case LOAD_STATE_NOSETPARAM:	//δ���ò�������Ҫ�������ò���
// 				pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorSelect);
// 				sprintf(tmpStr,"λ��:��%d���%d��, ����:%d, ͨ��:%d \n״̬:δ����\n�趨:%s,%0.3f%s, %0.3f%s~%0.3f%s\nʵʱ:����= \n����= , ��ѹ=",
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
			case LOAD_STATE_WORKING:	//�����У�����״̬��ʵʱ��ȡ����
				{
					char testResult[5]={0};
					switch (g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_ChnState)
					{
					case CHN_STATE_NOLINK:	//δ���ӣ���ɫ
						if (g_AllCar[curCarID].m_Load[loadIndex].m_Channel[chnIndex].m_ChnHaveBeenRight == 0)
						{
							pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorSelect);//��ɫ
							strcpy(testResult,"δ����");
						}else
						{
							pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorRed);//
							strcpy(testResult,"�����");
						}
						break;
					case CHN_STATE_OK:		//�ϸ���ɫ
						pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorGreen);		
						strcpy(testResult,"�ϸ�");
						break;
					case CHN_STATE_LOWVOL:	//Ƿѹ����ɫ
						pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorRed);//��ɫ						
						strcpy(testResult,"Ƿѹ");
						break;
					case CHN_STATE_OVERVOL:	//��ѹ����ɫ
						pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorRed);//��ɫ						
						strcpy(testResult,"��ѹ");
						break;
					case CHN_STATE_LOWCUR:	//Ƿ������ɫ
						pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorRed);//��ɫ						
						strcpy(testResult,"Ƿ��");
						break;
					case CHN_STATE_OVERCUR:	//��������ɫ
						pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorRed);//��ɫ
						strcpy(testResult,"����");
						break;
					case CHN_STATE_2_PARALELL:
						pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorNotSelect);
						strcpy(testResult,"��·����");
						break;
					case CHN_STATE_4_PARALELL:
						pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorNotSelect);
						strcpy(testResult,"��·����");
						break;						
					}
					
					sprintf(tmpStr,"λ��:��%d���%d��, ����:%d, ͨ��:%d \n״̬:%s\n�趨:%s,%0.3f%s, %0.3f%s~%0.3f%s\nʵʱ:����=%0.2f \n����=%0.3f , ��ѹ=%0.3f",
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
			case LOAD_STATE_PROTECT:	//�¶ȹ��ߣ�ϵͳ���ڱ���״̬����ʱ����ֹͣ
				pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorBlack);
				sprintf(tmpStr,"λ��:��%d���%d��, ����:%d, ͨ��:%d \n״̬:����\n�趨:%s,%0.3f%s, %0.3f%s~%0.3f%s\nʵʱ:����= \n����= , ��ѹ=",
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
			//ˢ����ʾ��
			pW=pdlg->GetDlgItem(1400+1+iLedIndex);
			pdlg->m_tooltip.AddTool(pW,tmpStr); 		
		}else
		{
			pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorNotSelect);
			sprintf(tmpStr,"λ��:%d,  ״̬:δѡ��\n�趨\nʵʱ:����= \n����= , ��ѹ=",iLedIndex+1);
			//ˢ����ʾ��
			pW=pdlg->GetDlgItem(1400+1+iLedIndex);
			pdlg->m_tooltip.AddTool(pW,tmpStr); 

		}
	}
}
/************************************************************************/
/*���ݲ���ʱ���ڲ���ʱ���е�λ�ã��жϵ�ǰ�����������ͣ�loadNum��Χ[1,63]   */
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
				for( i=0;i<4;i++)//�˸��ص��ĸ�ͨ����һ�����ִ������·��澯
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
			g_AllCar[carID].m_CMDAlreadyBeepOn=1;//�ط�����7��,����Ϊ����������
			g_AllCar[carID].m_CMDBeepOnCounter=0;
		}
	}
	if( cfg_EnableLight==1 && g_AllCar[carID].m_CMDAlreadyLightOn==0 )
	{
		if(g_AllCar[carID].m_CMDLightOnCounter<7)
		{	
			for(iLoad=g_AllCar[g_curSelTestCar].testParam.startLoadNum-1; iLoad<g_AllCar[g_curSelTestCar].testParam.endLoadNum; iLoad++)
			{
				for( i=0;i<4;i++)//�˸��ص��ĸ�ͨ����һ�����ִ������·��澯
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
			g_AllCar[carID].m_CMDAlreadyLightOn=1;//�ط�����7������Ϊ�澯������
			g_AllCar[carID].m_CMDLightOnCounter=0;
		}
	}


	//�жϵ�ǰ����ʱ���Ƿ��˲���ʱ����
	for(int testItemIndex=0;testItemIndex< g_AllCar[carID].testItemNum; testItemIndex++)
	{
		int testItem=g_AllCar[carID].pTimeSeires[testItemIndex].testItem;
		long startTime=g_AllCar[carID].pTimeSeires[testItemIndex].startTime;
		long testTime=g_AllCar[carID].pTimeSeires[testItemIndex].testTime;
//		TRACE("carID:%d, timerCounter:%d, testItemIndex:%d, startTime:%d, testTime:%d, testItem:%d\n", 
//			carID,g_AllCar[carID].m_TimeCounter,testItemIndex, startTime,testTime,testItem);

// 		//����ʱ��Ŀ�ʼ��
// 		if (g_AllCar[carID].m_TimeCounter*COMMAND_INTERVAL == startTime*1000) 
// 		{
// 			return testItem;			
// 		}
		
		//����ʱ��Ĺ�����
		if (g_AllCar[carID].m_TimeCounter*COMMAND_INTERVAL >= startTime*1000 &&
			g_AllCar[carID].m_TimeCounter*COMMAND_INTERVAL < startTime*1000+testTime*1000 ) 
		{
			if(g_AllCar[carID].pTimeSeires[testItemIndex].recvEvt == 0)//���δ�����ذ�����
			{
				return testItem;//ʱ������У��ж��Ƿ�Ƭ�����¼���У����ƣ���ʽ�������ݶ�ʧ
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
	//��ǰ����״̬Ϊδ���ò�������Ҫ�������ò���
	if (g_AllCar[carID].m_Load[loadNum-1].m_LoadState == LOAD_STATE_NOSETPARAM )
	{
		g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_SELECT;
		return LOAD_COMMAND_SET;
	}
	//�Ը���ʵʱ��ȡ	
	return LOAD_COMMAND_READ;//���·�����
	
}
/************************************************************************/
/*��ǰ���Գ�����ǰ���Ը��غ�[1,63]						                */
/************************************************************************/
int	GetCurLoadNum(int carID)
{
	
	if ( carID<0 || carID>MAX_CAR_COUNT )
	{
		TRACE("WARNING: GetCurLoadNum() error.. carID: %d\n", carID);
		return -1;
	}
	//�������Է�Χ,С�ڿ�ʼֵ
	if (g_AllCar[carID].m_curLoadNum < g_AllCar[carID].testParam.startLoadNum ||
		g_AllCar[carID].m_curLoadNum > g_AllCar[carID].testParam.endLoadNum)
	{
		g_AllCar[carID].m_curLoadNum = g_AllCar[carID].testParam.startLoadNum;	//��ǰ���Ը��غ�
	}
	
	return g_AllCar[carID].m_curLoadNum;
	
}
/************************************************************************/
/* �·����ڲ������carID��Χ[0,15]��curLoadNum��Χ[1,64], 64�㲥����  */
/************************************************************************/
void WriteSerialPortCommand(int carID, int serialPortCommandType, int curLoadNum)
{
	
	char wBuffer[39];
	char logBuf[512];
	int	 setValue;
	char	mainCtrlBoardCommad=0x3f;//���ذ岦�뿪�غ�
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
	
	
	wBuffer[0] = '\x6B';				//��ʼ���
	wBuffer[1] = curLoadNum;			//��ǰ���غ�
	wBuffer[38] = '\x6E';				//�������
		
	switch (serialPortCommandType)
	{
	case LOAD_COMMAND_SET:
		sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_SET     ,", carID, curLoadNum);
		wBuffer[2] = '\x0E';			//������
		wBuffer[3] = g_AllCar[carID].m_Load[curLoadNum-1].m_LoadMode;//����ģʽ
		//��·����
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
		//��·����
		if (2 == g_AllCar[carID].m_Load[curLoadNum-1].m_ParaMode && 0 == g_AllCar[carID].m_Load[curLoadNum-1].m_LoadMode)
		{
			setValue = (int)(g_AllCar[carID].m_Load[curLoadNum-1].m_Channel[0].m_SetValue*25.5/4);
			wBuffer[5] = (char)setValue;
			wBuffer[13] = (char)setValue;
			wBuffer[21] = (char)setValue;
			wBuffer[29] = (char)setValue;
			break;
		}
		//�޲���
		if (0 == g_AllCar[carID].m_Load[curLoadNum-1].m_ParaMode)
		{
			if (0 == g_AllCar[carID].m_Load[curLoadNum-1].m_LoadMode)//����
			{
				setValue = (int)(g_AllCar[carID].m_Load[curLoadNum-1].m_Channel[0].m_SetValue*25.5);
			}else if (1 == g_AllCar[carID].m_Load[curLoadNum-1].m_LoadMode)//��ѹ
			{
				setValue = (int)(g_AllCar[carID].m_Load[curLoadNum-1].m_Channel[0].m_SetValue*2.55);
			}		
			wBuffer[5] = (char)setValue;
			
			
			if (0 == g_AllCar[carID].m_Load[curLoadNum-1].m_LoadMode)//����
			{
				setValue = (int)(g_AllCar[carID].m_Load[curLoadNum-1].m_Channel[1].m_SetValue*25.5);
			}else if (1 == g_AllCar[carID].m_Load[curLoadNum-1].m_LoadMode)//��ѹ
			{
				setValue = (int)(g_AllCar[carID].m_Load[curLoadNum-1].m_Channel[1].m_SetValue*2.55);
			}		
			wBuffer[13] = (char)setValue;
			
			if (0 == g_AllCar[carID].m_Load[curLoadNum-1].m_LoadMode)//����
			{
				setValue = (int)(g_AllCar[carID].m_Load[curLoadNum-1].m_Channel[2].m_SetValue*25.5);	
			}else if (1 == g_AllCar[carID].m_Load[curLoadNum-1].m_LoadMode)//��ѹ
			{
				setValue = (int)(g_AllCar[carID].m_Load[curLoadNum-1].m_Channel[2].m_SetValue*2.55);
			}		
			wBuffer[21] = (char)setValue;
			
			if (0 == g_AllCar[carID].m_Load[curLoadNum-1].m_LoadMode)//����
			{
				setValue = (int)(g_AllCar[carID].m_Load[curLoadNum-1].m_Channel[3].m_SetValue*25.5);
			}else if (1 == g_AllCar[carID].m_Load[curLoadNum-1].m_LoadMode)//��ѹ
			{
				setValue = (int)(g_AllCar[carID].m_Load[curLoadNum-1].m_Channel[3].m_SetValue*2.55);	
			}		
			wBuffer[29] = (char)setValue;
		}
		
		break;
		case LOAD_COMMAND_READ:
			g_AllCar[carID].m_CarTestingState = CAR_STATE_TESTING_READ;//������״̬��Read
			wBuffer[2] = '\x0D';			//������
			sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_READ[%d,%d,%d]    ,",
				carID, curLoadNum,g_AllCar[carID].m_ReadCommandCounter,g_AllCar[carID].m_ReadEventCounter,g_AllCar[carID].m_LostReadEventCounter);
			break;
		case LOAD_COMMAND_STOP:
			sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_STOP    ,", carID, curLoadNum);
			wBuffer[2] = '\x0C';			//������
			break;
		case LOAD_COMMAND_110V_OFF:			//
			g_AllCar[carID].m_CarTestingState = CAR_STATE_TESTING_110VOFF;//������״̬
			sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_110V_OFF    ,", carID,mainCtrlBoardCommad);
			wBuffer[1] = mainCtrlBoardCommad;
			wBuffer[2] = '\x09';			//������
			break;
		case LOAD_COMMAND_110V_ON:
			g_AllCar[carID].m_CarTestingState = CAR_STATE_TESTING_110VON;//������״̬
			sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_110V_ON    ,", carID,mainCtrlBoardCommad);
			wBuffer[1] = mainCtrlBoardCommad;
			wBuffer[2] = '\x08';			//������
			break;
		case LOAD_COMMAND_220V_ON:
			g_AllCar[carID].m_CarTestingState = CAR_STATE_TESTING_220VON;//������״̬
			sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_220V_ON ,", carID,mainCtrlBoardCommad);
			wBuffer[1] = mainCtrlBoardCommad;
			wBuffer[2] = '\x06';			//������
			break;
		case LOAD_COMMAND_220V_OFF:
			g_AllCar[carID].m_CarTestingState = CAR_STATE_TESTING_220VOFF;//������״̬
			sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_220V_OFF,", carID,mainCtrlBoardCommad);
			wBuffer[1] = mainCtrlBoardCommad;
			wBuffer[2] = '\x07';			//������
			break;
		case LOAD_COMMAND_BEEPON:
			sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_BEEPON  ,", carID,mainCtrlBoardCommad);
			wBuffer[1] = mainCtrlBoardCommad;
			wBuffer[2] = '\x10';			//������
			break;
		case LOAD_COMMAND_BEEPOFF:
			sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_BEEPOFF ,", carID,mainCtrlBoardCommad);
			wBuffer[1] = mainCtrlBoardCommad;
			wBuffer[2] = LOAD_COMMAND_BEEPOFF;			//������
			break;
		case LOAD_COMMAND_LIGHTON:
			sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_LIGHTON ,", carID,mainCtrlBoardCommad);
			wBuffer[1] = mainCtrlBoardCommad;
			wBuffer[2] = '\x12';			//������
			break;
		case LOAD_COMMAND_LIGHTOFF:
			sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_LIGHTOFF,", carID,mainCtrlBoardCommad);
			wBuffer[1] = mainCtrlBoardCommad;
			wBuffer[2] = LOAD_COMMAND_LIGHTOFF;			//������
			break;
		default:
			return;
			
	}
	//���뿪��0��1��2��10��13��14ͬʱ����ʱ�������ղ��������
	if (serialPortCommandType ==LOAD_COMMAND_SET || 
		serialPortCommandType == LOAD_COMMAND_READ ||
		serialPortCommandType == LOAD_COMMAND_STOP)
	{
		if (curLoadNum==0)
		{
			curLoadNum=63;
			wBuffer[1] = curLoadNum;			//���帺�غ�
		}else if (curLoadNum==1)
		{
			curLoadNum=61;
			wBuffer[1] = curLoadNum;			//���غ�1
		}else if (curLoadNum==2)
		{
			curLoadNum=62;
			wBuffer[1] = curLoadNum;			//���غ�2
		}else if (curLoadNum==3)
		{
				curLoadNum=65;
				wBuffer[1] = curLoadNum;			//���غ�3, add by lmy 20120613
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
	//�������ݷ��س�ʱ����
	if(serialPortCommandType==LOAD_COMMAND_READ && 
		g_AllCar[carID].m_ReadCommandCounter > g_AllCar[carID].m_ReadEventCounter+g_AllCar[carID].m_LostReadEventCounter)
	{
		g_AllCar[carID].m_LostEventTimer++;
		if(g_AllCar[carID].m_LostEventTimer<7)//5sû�����ݷ������·���һ������
		{
			g_AllCar[carID].m_curLoadNum--;//����ͣ���ڵ�ǰ���أ�
			return;
		}
		g_AllCar[carID].m_LostReadEventCounter++;//ȷ�����ݶ�ʧ�ˣ��·���һ������

	}
	g_AllCar[carID].m_LostEventTimer=0;//��ʱδ�������ݼ�����
	g_Ports[carID].WriteToPort(wBuffer);

	if(serialPortCommandType==LOAD_COMMAND_READ)
	{
		g_AllCar[carID].m_ReadCommandCounter++;//ÿ̨���������ͳ��
	}
	for (int i=0;i<39;i++)
	{
		sprintf(logBuf,"%s %02X",logBuf,wBuffer[i]);
	}
	WriteLog(LEVEL_DEBUG,logBuf);
	
}
/************************************************************************/
/* ָ��ͨ�����Խ���Ƚ�                                                 */
/************************************************************************/
bool ChnTestDataCompare(int carID, int loadNum, int iChnIndex)
{
	bool	existErrorData = FALSE;							//�Ƿ���ڳ�������
	char	tmpStr[100]={0};
	if(g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError==1) //���ֹ������򲻽��м���
	{
		return existErrorData;
	}

	//ͨ��״̬, ����ͳ��			
	if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//����
	{	
		if( g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage == 0 && g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent== 0)
		{			
			if (g_AllCar[carID].m_TimeCounter>200)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����				
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
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//Ƿѹ
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
				g_AllCar[carID].m_ErrorCounter++;
				g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
				existErrorData = TRUE;							//�Ƿ���ڳ�������
				sprintf(tmpStr,"Error, CHN_STATE_LOWVOL, carID:%d, loadNum:%d, iChnIndex:%d", carID,loadNum, iChnIndex);
				WriteLog(LEVEL_DEBUG, tmpStr);
			}
		}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >	g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
		{
			if (g_AllCar[carID].m_TimeCounter>200)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
				g_AllCar[carID].m_ErrorCounter++;
				g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
				existErrorData = TRUE;							//�Ƿ���ڳ�������
				sprintf(tmpStr,"Error, CHN_STATE_OVEROWVOL, carID:%d, loadNum:%d, iChnIndex:%d", carID,loadNum, iChnIndex);
				WriteLog(LEVEL_DEBUG, tmpStr);
			}
		}else if ((g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent - g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue * (( 100.0 +cfg_CurrentErrorPercent )/100.0) ) > EPSINON)
		{
			if (g_AllCar[carID].m_TimeCounter>200)
			{				
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;	//����
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
				g_AllCar[carID].m_ErrorCounter++;	
				g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
				existErrorData = TRUE;							//�Ƿ���ڳ�������
				sprintf(tmpStr,"Error, CHN_STATE_OVERCUR, carID:%d, loadNum:%d, iChnIndex:%d", carID,loadNum, iChnIndex);
				WriteLog(LEVEL_DEBUG, tmpStr);
			}
			
		}else if ((g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent -g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue * (( 100.0 - cfg_CurrentErrorPercent )/100.0) ) < EPSINON)
		{
			if (g_AllCar[carID].m_TimeCounter>200)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
				g_AllCar[carID].m_ErrorCounter++;	
				g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
				existErrorData = TRUE;							//�Ƿ���ڳ�������
				sprintf(tmpStr,"Error, CHN_STATE_LOWCUR, carID:%d, loadNum:%d, iChnIndex:%d", carID,loadNum, iChnIndex);
				WriteLog(LEVEL_DEBUG, tmpStr);	
			}
		}else
		{
			g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
			g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenRight =1;
		}
	}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//��ѹ
	{
		if( g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage == 0 && g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent== 0)
		{
			g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
			if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenRight == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
				existErrorData = TRUE;	
			}
		}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <	g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
		{
			if (g_AllCar[carID].m_TimeCounter>200)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
				g_AllCar[carID].m_ErrorCounter++;
				g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
				existErrorData = TRUE;							//�Ƿ���ڳ�������
				sprintf(tmpStr,"Error, CHN_STATE_LOWCUR, carID:%d, loadNum:%d, iChnIndex:%d", carID,loadNum, iChnIndex);
				WriteLog(LEVEL_DEBUG, tmpStr);	
			}
			
		}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >	g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
		{
			if (g_AllCar[carID].m_TimeCounter>200)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
				g_AllCar[carID].m_ErrorCounter++;	
				g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
				existErrorData = TRUE;							//�Ƿ���ڳ�������
				sprintf(tmpStr,"Error, CHN_STATE_OVERCUR, carID:%d, loadNum:%d, iChnIndex:%d", carID,loadNum, iChnIndex);
				WriteLog(LEVEL_DEBUG, tmpStr);
			}
			
		}else if ((g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage - g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue*((100.0+cfg_VoltageErrorPercent)/100.0) ) > EPSINON)
		{
			if (g_AllCar[carID].m_TimeCounter>200)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
				g_AllCar[carID].m_ErrorCounter++;	
				g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
				existErrorData = TRUE;							//�Ƿ���ڳ�������
				sprintf(tmpStr,"Error, CHN_STATE_OVERVOL, carID:%d, loadNum:%d, iChnIndex:%d", carID,loadNum, iChnIndex);
				WriteLog(LEVEL_DEBUG, tmpStr);	
			}		
			
		}else if ((g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage - g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue*((100-cfg_VoltageErrorPercent)/100.0) ) < EPSINON)
		{
			if (g_AllCar[carID].m_TimeCounter>200)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//Ƿѹ
				g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
				g_AllCar[carID].m_ErrorCounter++;	
				g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
				existErrorData = TRUE;							//�Ƿ���ڳ�������
				sprintf(tmpStr,"Error, CHN_STATE_OVERVOL, carID:%d, loadNum:%d, iChnIndex:%d", carID,loadNum, iChnIndex);
				WriteLog(LEVEL_DEBUG, tmpStr);	
			}		
			
		}else
		{
			g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
			g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenRight =1;

		}
	}
	return existErrorData;
}

/************************************************************************/
/* �Ƿ����ذ�����                                                       */
/************************************************************************/
bool IsMainBoardCommand(int funcNum)
{
	switch (funcNum)
	{
	case LOAD_COMMAND_220V_ON://		= 0x09,	//���ذ����220V�ϻ���������
	case LOAD_COMMAND_110V_ON://		= 0x08,	//���ذ����110V�ϻ���������
	case LOAD_COMMAND_220V_OFF://	= 0x06,	//���ذ�������忪
	case LOAD_COMMAND_110V_OFF://	= 0x07,	//���ذ���������
	case LOAD_COMMAND_BEEPON://		= 0x10,	//���ذ������������
	case LOAD_COMMAND_BEEPOFF://	= 0x11,	//���ذ������������
	case LOAD_COMMAND_LIGHTON://	= 0x12,	//���ذ����LED��
	case LOAD_COMMAND_LIGHTOFF://	= 0x13,	//���ذ����LED��
		return true;
	}
	return false;
}

//�Ե�ǰ��ȡ���ĵ�ѹ���ݽ��д�������ֵ-1��ʾ���������ض��پ���ʾ����
float DealWithSerialPortData_NowVoltage(int carID, int loadNum, int iChnIndex, float nowVoltage)
{
	//�� ��ѹ���ԣ� ��ѹ��ֵ��10%������������������15%��������ʱ��ʾ��ֵ����Ϊ ��ѹֵ������ǧ��֮1-ǧ��֮3����������ֵ������1%-3%������
	//�� �������ԣ� ������ֵ��10%��������ѹ��������15%��������ʱ��ʾ��ֵ����Ϊ ����ֵ������ǧ��֮1-ǧ��֮3��������ѹֵ������1%-3%������

    int randomNum_percent_begin = 1;
	int randomNum_percent=1;//���������ƫ��ٷֱȣ�1-3%
	
	int randomNum_oddeven_begin = 1;//���������odd even��1 or 2
	int randomNum_oddeven_end = 2;//���������odd even��1 or 2
	int randomNum_oddeven = 1;//���������odd even��1 or 2

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
	
	
	if (g_AllCar[carID].m_Load[loadNum-1].m_ParaMode == 0 && g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//��ǰ�����ѹֵ����������,��·����
	{	
		if ((nowVoltage - g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin*0.85 ) < EPSINON)
		{
			return -1;
		}
		if ((nowVoltage - g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax*1.15 ) > EPSINON)
		{
			return -1;
		}
		//�����ǰ��ѹֵ�ڵ�ǰ�����޵� 15%�����
		//δ�����趨ֵ������ �趨ֵ�� ����ǧ��֮1-10 ����
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

	}else if (g_AllCar[carID].m_Load[loadNum-1].m_ParaMode == 0 && g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//��ǰ�����ѹֵ����ѹ����·����
	{
		if ( (nowVoltage - g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue*1.1) > EPSINON )
		{
			return -1; //�����趨ֵ�İٷ�֮ʮ
		}
		if ( (nowVoltage - g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue*0.9) < EPSINON )
		{
			return -1;//�����趨ֵ�İٷ�֮ʮ
		}
		//δ�����趨ֵ������ �趨ֵ�� ����ǧ��֮1-3 ����
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
//�Ե�ǰ��ȡ���ĵ������ݽ��д�������ֵ-1��ʾ�쳣��
float DealWithSerialPortData_NowCurrent(int carID, int loadNum, int iChnIndex, float nowCurrent)
{
	//����1��3֮��������
    int randomNum_percent_begin = 1;
	int randomNum_percent=1;//���������ƫ��ٷֱȣ�1-3%
	int randomNum_oddeven_begin = 1;//���������odd even��1 or 2
	int randomNum_oddeven_end = 2;//���������odd even��1 or 2
	int randomNum_oddeven = 1;//���������odd even��1 or 2
	
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
	if (g_AllCar[carID].m_Load[loadNum-1].m_ParaMode == 0 && g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//��ǰ�������ֵ����ѹ�� ��·����
	{	
		if ((nowCurrent - g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin*0.85 ) < EPSINON)
		{
			return -1;
		}
		if ((nowCurrent - g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax*1.15 ) > EPSINON)
		{
			return -1;
		}
		//�����ǰ��ѹֵ�ڵ�ǰ�����޵� 15%�����
		//δ�����趨ֵ������ �趨ֵ�� ����ǧ��֮1-10 ����
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
		
	}else if (g_AllCar[carID].m_Load[loadNum-1].m_ParaMode == 0 && g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//��ǰ�������ֵ������,��·����
	{
		if ( (nowCurrent - g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue*1.1) > EPSINON )
		{
			return -1; //�����趨ֵ�İٷ�֮ʮ
		}
		if ( (nowCurrent - g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue*0.9) < EPSINON )
		{
			return -1;//�����趨ֵ�İٷ�֮ʮ
		}
		//δ�����趨ֵ������ �趨ֵ�� ����ǧ��֮1-3 ����
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
/* �����ڽ��յ�������, carID��Χ[0,15]                                */
/************************************************************************/
void	DealWithSerialPortData(int carID)				
{
	char logBuf[512];
	char cStartFlag = (char)0x6B;				//���ʼ���
	char cEndFlag	= (char)0x6E;				//����������
	float ret = -1;
	memset(logBuf, 0, sizeof(logBuf));
	
	if ( carID<0 || carID>MAX_CAR_COUNT )
	{
		return;	
	}
	int	existErrorData = FALSE;							//�Ƿ���ڳ�������
	int startElem = g_AllCar[carID].recvData[0];			//��ʼλ��	
	int loadNum = g_AllCar[carID].recvData[1];			//���غ�
	int funNum = g_AllCar[carID].recvData[2];			//������
	char endElem = g_AllCar[carID].recvData[38];		//����Ԫ��
	if (loadNum==0 && loadNum==0 && funNum==0 && startElem==0 && endElem ==0)
	{
		return;//��ʱδ�յ���������
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
		g_AllCar[carID].m_CMDAlreadyBeepOn=1;//��ʱ�ײ����������, modify by lmy 20120415
		g_AllCar[carID].m_CMDBeepOnCounter=0;
		EnterCriticalSection(&g_AllCar[g_curSelTestCar].CriticalSectionRecvBuffer);
		memset(g_AllCar[carID].recvData,0,sizeof(g_AllCar[carID].recvData));
		LeaveCriticalSection(&g_AllCar[g_curSelTestCar].CriticalSectionRecvBuffer);
		return;
	}
	if(LOAD_COMMAND_LIGHTON == funNum )
	{
		g_AllCar[carID].m_CMDAlreadyLightOn=1;//��ʱ�ײ�澯������, modify by lmy 20120415
		g_AllCar[carID].m_CMDLightOnCounter=0;
		EnterCriticalSection(&g_AllCar[g_curSelTestCar].CriticalSectionRecvBuffer);
		memset(g_AllCar[carID].recvData,0,sizeof(g_AllCar[carID].recvData));
		LeaveCriticalSection(&g_AllCar[g_curSelTestCar].CriticalSectionRecvBuffer);
		return;
	}

	//�ж��Ƿ���������
	if(IsMainBoardCommand(funNum))
	{		
		//��ǰ����ʱ��λ�ڲ���ʱ���λ��,�жϵ�ǰ����ʱ���Ƿ��˲���ʱ����
		for(int testItemIndex=0;testItemIndex< g_AllCar[carID].testItemNum; testItemIndex++)
		{
			int testItem=g_AllCar[carID].pTimeSeires[testItemIndex].testItem;
			long startTime=g_AllCar[carID].pTimeSeires[testItemIndex].startTime;
			long testTime=g_AllCar[carID].pTimeSeires[testItemIndex].testTime;				
			
			//����ʱ��Ĺ�����
			if (g_AllCar[carID].m_TimeCounter*COMMAND_INTERVAL >= startTime*1000 &&
				g_AllCar[carID].m_TimeCounter*COMMAND_INTERVAL < startTime*1000+testTime*1000 ) 
			{
				//�ڲ���ʱ��ṹ���б�ʶ�ѻ�����
				g_AllCar[carID].pTimeSeires[testItemIndex].recvEvt=1;
				EnterCriticalSection(&g_AllCar[g_curSelTestCar].CriticalSectionRecvBuffer);
				memset(g_AllCar[carID].recvData,0,sizeof(g_AllCar[carID].recvData));
				LeaveCriticalSection(&g_AllCar[g_curSelTestCar].CriticalSectionRecvBuffer);
				return;
			}						
		}	
	}
	//�ײ㷵�صģ�����״̬
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
	//��ʼ����������---------------------------------------------------------
	BYTE A1,A2,B1,B2,D1,D2,E1,E2;
	int	 A,B,D,E;
	float C,F; 
	int iChnIndex;//ͨ��ID,��Χ[0,3]
	//ͨ��[0,3]�ļ���
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
			
			//ͨ��,��������
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
			
			//�Ե�ǰ��ȡ���ĵ�ѹ���ݽ��д���
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
			//�Ե�ǰ��ȡ���ĵ������ݽ��д���
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
	//ͨ��[0,3]�Ĵ���״̬ͳ��
	if (g_AllCar[carID].m_Load[loadNum-1].m_ParaMode==1 && g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//���� 2·���� 
	{
		//ͨ��0
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
		//ͨ��1
		g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_ChnState = CHN_STATE_2_PARALELL;
		//ͨ��2
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
		//ͨ��3
		g_AllCar[carID].m_Load[loadNum-1].m_Channel[3].m_ChnState = CHN_STATE_2_PARALELL;
		
	}else if(g_AllCar[carID].m_Load[loadNum-1].m_ParaMode==2 && g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )////���� 4·���� 
	{
		//ͨ��0
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
		//ͨ��1
		g_AllCar[carID].m_Load[loadNum-1].m_Channel[1].m_ChnState = CHN_STATE_4_PARALELL;
		//ͨ��2
		g_AllCar[carID].m_Load[loadNum-1].m_Channel[2].m_ChnState = CHN_STATE_4_PARALELL;
		//ͨ��3
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
	
	//�ӵ�������endPosλ��֮ǰ��Ԫ��
	//�������ݣ�д����������ļ�
	if ( g_AllCar[carID].testParam.isSaveData == 1 && TRUE == existErrorData )
	{
		SaveCurrentLoadTestData(carID,loadNum);					
	}	
	
	EnterCriticalSection(&g_AllCar[g_curSelTestCar].CriticalSectionRecvBuffer);
	memset(g_AllCar[carID].recvData,0,sizeof(g_AllCar[carID].recvData));
	LeaveCriticalSection(&g_AllCar[g_curSelTestCar].CriticalSectionRecvBuffer);

}

/************************************************************************/
/* ����ָ������ͨ������                                                 */
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
/* �����������, [0,15]                                                 */
/************************************************************************/
void	SaveTestData(int carID)				
{
	if ( carID<0 || carID>MAX_CAR_COUNT )
	{
		return;
	}
	
	if (g_AllCar[carID].testParam.isSaveData == 0)//���������ѡ���˱����ļ�
	{
		return;
	}
	
	if(g_AllCar[carID].m_TimeCounter < 600 ) // 1����֮��ʼ��¼����
	{
		return;
	}

	if (g_AllCar[carID].m_CarState != CAR_STATE_TESTING)
	{
		return;
	}
	
	long testTime = g_AllCar[carID].m_TimeCounter*COMMAND_INTERVAL;				//���������˶೤ʱ�䣬��λms
	long saveDataInterval = g_AllCar[carID].testParam.saveDataInterval*1000;	//��������ʱ��������λms
	if (testTime%saveDataInterval == 0)
	{
		
		//���浱ǰ����
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
/* ��ý�嶨ʱ����������ͨ��      ,100ms                               */
/************************************************************************/
void	Proc_MutilMediaTimer(int carID,DWORD dwUser)	
{
	int		serialPortCommandType;	//������������
	int     curLoadNum;				//��ǰ���Ը��غ�, [1,63]
	char	testTimeStr[16]={0};
	char	tmpStr[64];
	char	logBuf[512];
	
	CLoadAgingDlg* pDlg = (CLoadAgingDlg*)dwUser;
	
	memset(tmpStr,0,sizeof(tmpStr));
	memset(logBuf,0,sizeof(logBuf));


	
	
	//������������
	DealWithSerialPortData(carID);
	
	//�����������
	SaveTestData(carID);
	
		//���CLEAR��,�·�BEEPOFF
	if(g_AllCar[carID].m_CMDBeepClear==1)
	{
		g_AllCar[g_curSelTestCar].m_CMDBeepOffCounter++;
		if(g_AllCar[g_curSelTestCar].m_CMDBeepOffCounter<2)
		{
			return;
		}
		EnterCriticalSection(&g_AllCar[carID].CriticalSectionClear);
		g_AllCar[carID].m_CMDBeepClear=0;//����Ҫ����
		g_AllCar[g_curSelTestCar].m_CMDBeepOffCounter=0;
		LeaveCriticalSection(&g_AllCar[carID].CriticalSectionClear);
		WriteSerialPortCommand(carID, LOAD_COMMAND_BEEPOFF, 0);			
		//WriteSerialPortCommand(carID, LOAD_COMMAND_BEEPOFF, 0);			
		return;	
		
	}
	//���CLEAR��,�·�LIGHTOFF
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
		g_AllCar[carID].m_CMDLightClear=0;//����Ҫ����
		g_AllCar[g_curSelTestCar].m_CMDLightOffCounter=0;
		LeaveCriticalSection(&g_AllCar[carID].CriticalSectionClear);
		WriteSerialPortCommand(carID, LOAD_COMMAND_LIGHTOFF, 0);
		//WriteSerialPortCommand(carID, LOAD_COMMAND_LIGHTOFF, 0);
		return;
	}
	//��ǰ���Գ�״̬��ֹͣ����
	if(	g_AllCar[carID].m_CarState == CAR_STATE_TESTFINISH || g_AllCar[carID].m_CarState == CAR_STATE_IMPORTED )
	{	
		//�رն�ʱ��
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
	
		WriteSerialPortCommand(carID, LOAD_COMMAND_STOP, 64);	//�·��������ֹͣ����
		WriteSerialPortCommand(carID, LOAD_COMMAND_STOP, 64);	//�·��������ֹͣ����
		Sleep(10);//�������Ҫ����
		WriteSerialPortCommand(carID, LOAD_COMMAND_STOP, 64);	//�·��������ֹͣ����
		
		pDlg->GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(TRUE);		
		if(g_curSelTestCar == carID)		//����������ǵ�ǰ���Գ�
		{
			sprintf(tmpStr,"%s��ʼ����",GetTestCarName(carID));
			pDlg->SetDlgItemText(IDC_BUTTON_TEST, tmpStr);
			pDlg->m_BtnTest.SetIcon(IDI_ICON_STARTTEST);
			if(	g_AllCar[carID].m_CarState == CAR_STATE_TESTFINISH )
			{
				sprintf(tmpStr,"%s�������",GetTestCarName(carID));
				AfxMessageBox(tmpStr);
			}
		}
		sprintf(logBuf,"carID:%d, ֹͣ����.",carID);
		WriteLog(LEVEL_DEBUG,logBuf);
		
		return;//ֹͣ����
		
	}else if ( g_AllCar[carID].m_CarState == CAR_STATE_TESTING )
	{
		//�������̣��·�����
		curLoadNum = GetCurLoadNum(carID);										//��ȡ��ǰ�����Ը��غ�
		serialPortCommandType = GetSerialPortCommandType(carID,curLoadNum);		//��ǰ���Գ�����ǰ���Ը��أ���ȡӦ�·���������
		WriteSerialPortCommand(carID, serialPortCommandType, curLoadNum);		//�·���������
	}
	
	g_AllCar[carID].m_TimeCounter++;							//����ʱ��ͳ��
	g_AllCar[carID].m_curLoadNum++;							//��ǰ���Ը��غţ�ѭ�����Ը���
	
	
	//�ܵĲ���ʱ�䵽��,��λms
	LONGLONG lTotlalTime = g_AllCar[carID].m_TimeCounter*COMMAND_INTERVAL;//�����˶���ms
	if ( lTotlalTime>= g_AllCar[carID].testParam.totalTestTime*60*1000 )
	{	
		sprintf(tmpStr,"%s:ֹͣ����, ʱ�䵽��.",GetTestCarName(carID));
		WriteLog(LEVEL_DEBUG,tmpStr);	
		g_AllCar[carID].m_CarState = CAR_STATE_TESTFINISH;		//����״̬��Ϊֹͣ
		
	}
}			
