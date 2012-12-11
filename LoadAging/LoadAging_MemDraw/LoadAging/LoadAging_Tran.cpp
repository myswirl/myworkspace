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

char c='b';//����ͷ�������

SLinkList<char> sLinkList1(c);			//���մ�����������
SLinkList<char> sLinkList2(c);			//���մ�����������
SLinkList<char> sLinkList3(c);			//���մ�����������
SLinkList<char> sLinkList4(c);			//���մ�����������
SLinkList<char> sLinkList5(c);			//���մ�����������
SLinkList<char> sLinkList6(c);			//���մ�����������
SLinkList<char> sLinkList7(c);			//���մ�����������
SLinkList<char> sLinkList8(c);			//���մ�����������
SLinkList<char> sLinkList9(c);			//���մ�����������
SLinkList<char> sLinkList10(c);			//���մ�����������
SLinkList<char> sLinkList11(c);			//���մ�����������
SLinkList<char> sLinkList12(c);			//���մ�����������
SLinkList<char> sLinkList13(c);			//���մ�����������
SLinkList<char> sLinkList14(c);			//���մ�����������
SLinkList<char> sLinkList15(c);			//���մ�����������
SLinkList<char> sLinkList16(c);			//���մ�����������

// var about LoadAging.ini
char			cfg_IniName[256] = "";
char			cfg_IniShortName[] = "\\LoadAging.ini";
char			cfg_NormalPassword[128]={0};				//������Ա����
char			cfg_SuperPassword[128]={0};					//����ʦ����
char			cfg_DataRecordPath[256]="";					//���ݱ���·��
char			cfg_Title[256]="���Ӹ����ϻ�ϵͳ";			//����
int				cfg_EnableBeep;								//ʹ�ܷ�����
int				cfg_EnableLight;							//ʹ��ָʾ��
//ϵͳȫ�ֱ���--------------------------------------------
int				g_LoginCheckOK=-1;							//��¼У���Ƿ�ͨ��,-1:У��ʧ�ܣ�0:����ʦ��¼�ɹ���1:����Ա��¼�ɹ�
long			g_SystemRunTimeCounter=0;					//ϵͳ����ʱ�������
CAR_STRUCT		g_AllCar[MAX_CAR_COUNT];					//����Ҫ��ȫ�ֽṹ��
int				g_ComList[MAX_CAR_COUNT];					//���ô����б�
int				g_AvailableComCounter=0;					//���ô�����Ŀ
int				g_curSelTestCar=0;							//��ǰѡ�в��Գ�
CMyLog			g_data[MAX_CAR_COUNT];						//��¼����������־��
CSerialPort		g_Ports[MAX_CAR_COUNT];						//�����࣬����ÿ�����Գ���������
float			g_SetCurrentError=0.2;						//���õ���ֵƫ�Χ
float			g_SetVoltageError=1.0;						//���õ�ѹֵƫ�Χ

/************************************************************************/
/* ��ȡ�����ļ�ini                                                      */
/************************************************************************/
void	ReadFromConfig(void)
{
	GetCurrentDirectory ( 256-32, cfg_IniName );
	strcat ( cfg_IniName, cfg_IniShortName );
	
	cfg_LogOn = GetPrivateProfileInt ( "ConfigInfo", "LogOn", 1, cfg_IniName);					//�Ƿ�����־
	cfg_LogLevel = GetPrivateProfileInt ( "ConfigInfo", "LogLevel", 0, cfg_IniName);			//��־����
	
	cfg_EnableBeep = GetPrivateProfileInt ( "ConfigInfo", "EnableBeep", 1, cfg_IniName);		//�Ƿ�ʹ�ܷ�����
	cfg_EnableLight = GetPrivateProfileInt ( "ConfigInfo", "EnableLight", 1, cfg_IniName);		//�Ƿ�ʹ��ָʾ��
	
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
		return "������";
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
	if(	CAR_STATE_TESTING == g_AllCar[g_curSelTestCar].m_CarState )
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
/************************************************************************/
/* �����ڽ�������                                                     */
/************************************************************************/
void	ProcessReceiveData( char recvByte, int portNo)
{
	int carID;
	char cStartFlag = (char)0x6B;						//���ʼ���
	char cEndFlag	= (char)0x6E;						//����������
	
	char logBuf[512];
	memset(logBuf,0,sizeof(logBuf));
	
	if (portNo<=0 || portNo>=MAX_PORT_NO)
	{
		sprintf(logBuf,"Error, portNo:%d is wrong!", portNo);
		WriteLog(LEVEL_ERROR, logBuf);
		return;
	}
	
	//���ݴ��ںŶ�λ���Գ�
	carID = GetCarIDByPortNo(portNo);	
	if ( carID < 0 )
	{
		sprintf(logBuf,"Error, carID:%d is wrong!", carID);
		WriteLog(LEVEL_ERROR, logBuf);
		return;
	}
				
	//�����ֽڲ��뵽������ÿ�ζ�ȡһ���ֽ�
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
	
	//�����־
	WriteLog(LEVEL_DEBUG, logBuf);
	
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
			pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorRed);
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
		
	//ѭ������LED��
	for(int iLedIndex=0; iLedIndex<MAX_LED_COUNT; iLedIndex++)
	{
		int loadIndex = iLedIndex/4;	//��LED�����ڵĸ��غ�, [0,49]
		int chnIndex = iLedIndex%4;		//��LED�����ڵ�ͨ����, [0,3]
		int	iPerLayer=0;
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
				pdlg->m_LED[iLedIndex].SetOnFgColor(g_ColorRed);
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
}
/************************************************************************/
/*���ݲ���ʱ���ڲ���ʱ���е�λ�ã��жϵ�ǰ�����������ͣ�loadNum��Χ[1,63]   */
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
	//�жϵ�ǰ����ʱ���Ƿ��˲���ʱ����
	for(int testItemIndex=0;testItemIndex< g_AllCar[carID].testItemNum; testItemIndex++)
	{
		int testItem=g_AllCar[carID].pTimeSeires[testItemIndex].testItem;
		long startTime=g_AllCar[carID].pTimeSeires[testItemIndex].startTime;
		long testTime=g_AllCar[carID].pTimeSeires[testItemIndex].testTime;
		
		//����ʱ��Ŀ�ʼ��
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
		/*/����ʱ��Ľ�����
		if (g_AllCar[carID].m_TimeCounter*COMMAND_INTERVAL == startTime*1000+testTime*1000) 
		{
			if (testItem == TEST_FLOW_PULSEOFF)//�����ز���ʱ��������·�ON
			{
				return LOAD_COMMAND_PULSEON;				
			}
		}*/
		
		//����ʱ��Ĺ�����
		if (g_AllCar[carID].m_TimeCounter*COMMAND_INTERVAL > startTime*1000 &&
			g_AllCar[carID].m_TimeCounter*COMMAND_INTERVAL < startTime*1000+testTime*1000 ) 
		{
			return 0;	//ʱ������У����·�Read����
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
		break;
	case LOAD_COMMAND_READ:
		wBuffer[2] = '\x0D';			//������
		sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_READ    ,", carID, curLoadNum);
		break;
	case LOAD_COMMAND_STOP:
		sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_STOP    ,", carID, curLoadNum);
		wBuffer[2] = '\x0C';			//������
		break;
	case LOAD_COMMAND_220V:				//220V
		TRACE("WriteSerialPortCommand(), LOAD_COMMAND_220V\n");
		sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_200V    ,", carID,mainCtrlBoardCommad);
		wBuffer[1] = mainCtrlBoardCommad;
		wBuffer[2] = '\x09';			//������
		break;
	case LOAD_COMMAND_110V:
		TRACE("WriteSerialPortCommand(), LOAD_COMMAND_110V\n");
		sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_110V    ,", carID,mainCtrlBoardCommad);
		wBuffer[1] = mainCtrlBoardCommad;
		wBuffer[2] = '\x08';			//������
		break;
	case LOAD_COMMAND_PULSEON:
		TRACE("WriteSerialPortCommand(), LOAD_COMMAND_PULSEON\n");
		sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_PULSEON ,", carID,mainCtrlBoardCommad);
		wBuffer[1] = mainCtrlBoardCommad;
		wBuffer[2] = '\x06';			//������
		break;
	case LOAD_COMMAND_PULSEOFF:
		TRACE("WriteSerialPortCommand(), LOAD_COMMAND_PULSEOFF\n");
		sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_PULSEOFF,", carID,mainCtrlBoardCommad);
		wBuffer[1] = mainCtrlBoardCommad;
		wBuffer[2] = '\x07';			//������
		break;
	case LOAD_COMMAND_BEEPON:
		TRACE("WriteSerialPortCommand(), LOAD_COMMAND_BEEPON\n");
		sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_BEEPON  ,", carID,mainCtrlBoardCommad);
		wBuffer[1] = mainCtrlBoardCommad;
		wBuffer[2] = '\x10';			//������
		break;
	case LOAD_COMMAND_BEEPOFF:
		TRACE("WriteSerialPortCommand(), LOAD_COMMAND_BEEPOFF\n");
		sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_BEEPOFF ,", carID,mainCtrlBoardCommad);
		wBuffer[1] = mainCtrlBoardCommad;
		wBuffer[2] = '\x11';			//������
		break;
	case LOAD_COMMAND_LIGHTON:
		TRACE("WriteSerialPortCommand(), LOAD_COMMAND_LIGHTON\n");
		sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_LIGHTON ,", carID,mainCtrlBoardCommad);
		wBuffer[1] = mainCtrlBoardCommad;
		wBuffer[2] = '\x12';			//������
		break;
	case LOAD_COMMAND_LIGHTOFF:
		TRACE("WriteSerialPortCommand(), LOAD_COMMAND_LIGHTOFF\n");
		sprintf(logBuf,"carID:%02d, loadID:%02d, LOAD_COMMAND_LIGHTOFF,", carID,mainCtrlBoardCommad);
		wBuffer[1] = mainCtrlBoardCommad;
		wBuffer[2] = '\x13';			//������
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
/* �����ڽ��յ�������, carID��Χ[0,15]                                */
/************************************************************************/
void	DealWithSerialPortData(int carID)				
{
	char logBuf[512];
	char cStartFlag = (char)0x6B;				//���ʼ���
	char cEndFlag	= (char)0x6E;				//����������
	
	memset(logBuf, 0, sizeof(logBuf));
	
	if ( carID<0 || carID>MAX_CAR_COUNT )
	{
		return;	
	}
	
	
	//ֻ����A1�����������------------------------------------------------------------
	switch (carID)
	{
	case 0:
		{
			int	existErrorData = FALSE;							//�Ƿ���ڳ�������
			int startPos = sLinkList1.GetPos(cStartFlag);		//��ʼλ��
			int endPos = sLinkList1.GetPos(cEndFlag);			//����λ��
			
			if (sLinkList1.length < startPos+38)				//�������ȣ������¼�����39���ֽ�
			{
				return;
			}
			int loadNum = sLinkList1.GetElem(startPos+1);		//���غ�
			int funNum = sLinkList1.GetElem(startPos+2);		//������
			char endElem = sLinkList1.GetElem(startPos+38);		//����Ԫ��
			if (loadNum<1 || loadNum>=MAX_LOAD_PERCAR || funNum!=0x0D || endElem!= cEndFlag)
			{
				sprintf(logBuf,"Error, Serial Data is wrong! carID:%d, loadNum:%d, funNum:%X, endElem:%X", carID, loadNum, funNum, endElem);
				WriteLog(LEVEL_ERROR,logBuf);
				sLinkList1.DelElemEx(startPos+38);				//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
			}
			
			//�ײ㷵�صģ�����״̬
			int loadState = (int)sLinkList1.GetElem(startPos+37);
			if (loadState == 3)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOLINK;
				sLinkList1.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
				
			}else if (loadState == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_WORKING;
				
				
			}else if (loadState == 2)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_PROTECT;
				sLinkList1.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
				
			}else if (loadState == 0)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOSETPARAM;
				sLinkList1.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
			}
			
			
			//��ʼ����������---------------------------------------------------------
			BYTE A1,A2,B1,B2,D1,D2,E1,E2;
			int	 A,B,D,E;
			float C,F; 
			int iChnIndex;//ͨ��ID,��Χ[0,3]
			
			iChnIndex = 0;
			if(g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError==0)
			{
				//ͨ��1,��ѹ����
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
				
				
				
				//ͨ��1,��������
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
				
				
				//ͨ��1, ״̬, ����ͳ��			
				if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//����
				{	
					if( C == 0 && F== 0)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
						
					}else if ( g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage < g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin )
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;	//Ƿѹ
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else if ( g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage > g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax )
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;	//��ѹ
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else if ( g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError  )
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;	//����
						g_AllCar[carID].m_ErrorCounter++;	
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else if ( g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError  )
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;	//Ƿ��
						g_AllCar[carID].m_ErrorCounter++;	
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;		//�ϸ�
					}
				}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//��ѹ
				{
					if( C == 0 && F== 0)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
						
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;	
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;	
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//Ƿѹ
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;	
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
					}
				}
			}
			iChnIndex=1;
			if(g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError==0)
			{
				
				//ͨ��2,��ѹ����
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
				
				//ͨ��2,��������
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
				//ͨ��2, ״̬, ����ͳ��			
				if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//����
				{	
					if( C == 0 && F== 0)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
						
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//Ƿѹ
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;	
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
					}
				}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//��ѹ
				{
					if( C == 0 && F== 0)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
						
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;	
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//Ƿѹ
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;	
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
					}
				}
			}
			iChnIndex=2;
			if(g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError==0)
			{
				
				//ͨ��3,��ѹ����
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
				
				//ͨ��3,��������
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
				//ͨ��3, ״̬, ����ͳ��			
				if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//����
				{	
					if( C == 0 && F== 0)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
						
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//Ƿѹ
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;	
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;	
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;	
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
					}
				}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//��ѹ
				{
					if( C == 0 && F== 0)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
						
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//Ƿѹ
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
					}
				}
			}
			iChnIndex=3;
			if(g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError==0)
			{
				
				//ͨ��4,��ѹ����
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
				
				//ͨ��4,��������
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
				//ͨ��4, ״̬, ����ͳ��			
				if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//����
				{	
					if( C == 0 && F== 0)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
						
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//Ƿѹ
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;	
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
					}
				}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//��ѹ
				{
					if( C == 0 && F== 0)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
						
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;	
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;	
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else if (
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
					{
						
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//Ƿѹ
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;
						g_AllCar[carID].m_ErrorCounter++;	
						g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
						existErrorData = TRUE;							//�Ƿ���ڳ�������
					}else
					{
						g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
					}
				}
			}
			
			sLinkList1.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
			//�������ݣ�д����������ļ�
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
			int	existErrorData = FALSE;							//�Ƿ���ڳ�������
			int startPos = sLinkList2.GetPos(cStartFlag);		//��ʼλ��
			int endPos = sLinkList2.GetPos(cEndFlag);			//����λ��
			
			if (sLinkList2.length < startPos+38)				//�������ȣ������¼�����39���ֽ�
			{
				return;
			}
			int loadNum = sLinkList2.GetElem(startPos+1);		//���غ�
			int funNum = sLinkList2.GetElem(startPos+2);		//������
			char endElem = sLinkList2.GetElem(startPos+38);		//����Ԫ��
			if (loadNum<1 || loadNum>=MAX_LOAD_PERCAR || funNum!=0x0D || endElem!= cEndFlag)
			{
				sprintf(logBuf,"Error, Serial Data is wrong! carID:%d, loadNum:%d, funNum:%X, endElem:%X", carID, loadNum, funNum, endElem);
				WriteLog(LEVEL_ERROR,logBuf);
				sLinkList2.DelElemEx(startPos+38);				//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
			}
			
			//�ײ㷵�صģ�����״̬
			int loadState = (int)sLinkList2.GetElem(startPos+37);
			if (loadState == 3)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOLINK;
				sLinkList2.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
				
			}else if (loadState == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_WORKING;
				
				
			}else if (loadState == 2)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_PROTECT;
				sLinkList2.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
				
			}else if (loadState == 0)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOSETPARAM;
				sLinkList2.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
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
					
					//ͨ��,��������
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
					
					//ͨ��״̬, ����ͳ��			
					if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//����
					{	
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//Ƿѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
						}
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//��ѹ
					{
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//Ƿѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
						}
					}
				}
			}			
			sLinkList2.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
			//�������ݣ�д����������ļ�
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
			int	existErrorData = FALSE;							//�Ƿ���ڳ�������
			int startPos = sLinkList3.GetPos(cStartFlag);		//��ʼλ��
			int endPos = sLinkList3.GetPos(cEndFlag);			//����λ��
			
			if (sLinkList3.length < startPos+38)				//�������ȣ������¼�����39���ֽ�
			{
				return;
			}
			int loadNum = sLinkList3.GetElem(startPos+1);		//���غ�
			int funNum = sLinkList3.GetElem(startPos+2);		//������
			char endElem = sLinkList3.GetElem(startPos+38);		//����Ԫ��
			if (loadNum<1 || loadNum>=MAX_LOAD_PERCAR || funNum!=0x0D || endElem!= cEndFlag)
			{
				sprintf(logBuf,"Error, Serial Data is wrong! carID:%d, loadNum:%d, funNum:%X, endElem:%X", carID, loadNum, funNum, endElem);
				WriteLog(LEVEL_ERROR,logBuf);
				sLinkList3.DelElemEx(startPos+38);				//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
			}
			
			//�ײ㷵�صģ�����״̬
			int loadState = (int)sLinkList3.GetElem(startPos+37);
			if (loadState == 3)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOLINK;
				sLinkList3.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
				
			}else if (loadState == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_WORKING;
				
				
			}else if (loadState == 2)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_PROTECT;
				sLinkList3.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
				
			}else if (loadState == 0)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOSETPARAM;
				sLinkList3.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
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
					
					//ͨ��,��������
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
					
					//ͨ��״̬, ����ͳ��			
					if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//����
					{	
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//Ƿѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
						}
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//��ѹ
					{
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//Ƿѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
						}
					}
				}
			}			
			sLinkList3.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
			//�������ݣ�д����������ļ�
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
			int	existErrorData = FALSE;							//�Ƿ���ڳ�������
			int startPos = sLinkList4.GetPos(cStartFlag);		//��ʼλ��
			int endPos = sLinkList4.GetPos(cEndFlag);			//����λ��
			
			if (sLinkList4.length < startPos+38)				//�������ȣ������¼�����39���ֽ�
			{
				return;
			}
			int loadNum = sLinkList4.GetElem(startPos+1);		//���غ�
			int funNum = sLinkList4.GetElem(startPos+2);		//������
			char endElem = sLinkList4.GetElem(startPos+38);		//����Ԫ��
			if (loadNum<1 || loadNum>=MAX_LOAD_PERCAR || funNum!=0x0D || endElem!= cEndFlag)
			{
				sprintf(logBuf,"Error, Serial Data is wrong! carID:%d, loadNum:%d, funNum:%X, endElem:%X", carID, loadNum, funNum, endElem);
				WriteLog(LEVEL_ERROR,logBuf);
				sLinkList4.DelElemEx(startPos+38);				//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
			}
			
			//�ײ㷵�صģ�����״̬
			int loadState = (int)sLinkList4.GetElem(startPos+37);
			if (loadState == 3)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOLINK;
				sLinkList4.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
				
			}else if (loadState == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_WORKING;
				
				
			}else if (loadState == 2)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_PROTECT;
				sLinkList4.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
				
			}else if (loadState == 0)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOSETPARAM;
				sLinkList4.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
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
					
					//ͨ��,��������
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
					
					//ͨ��״̬, ����ͳ��			
					if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//����
					{	
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//Ƿѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
						}
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//��ѹ
					{
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//Ƿѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
						}
					}
					
				}
			}			
			sLinkList4.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
			//�������ݣ�д����������ļ�
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
			int	existErrorData = FALSE;							//�Ƿ���ڳ�������
			int startPos = sLinkList5.GetPos(cStartFlag);		//��ʼλ��
			int endPos = sLinkList5.GetPos(cEndFlag);			//����λ��
			
			if (sLinkList5.length < startPos+38)				//�������ȣ������¼�����39���ֽ�
			{
				return;
			}
			int loadNum = sLinkList5.GetElem(startPos+1);		//���غ�
			int funNum = sLinkList5.GetElem(startPos+2);		//������
			char endElem = sLinkList5.GetElem(startPos+38);		//����Ԫ��
			if (loadNum<1 || loadNum>=MAX_LOAD_PERCAR || funNum!=0x0D || endElem!= cEndFlag)
			{
				sprintf(logBuf,"Error, Serial Data is wrong! carID:%d, loadNum:%d, funNum:%X, endElem:%X", carID, loadNum, funNum, endElem);
				WriteLog(LEVEL_ERROR,logBuf);
				sLinkList5.DelElemEx(startPos+38);				//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
			}
			
			//�ײ㷵�صģ�����״̬
			int loadState = (int)sLinkList5.GetElem(startPos+37);
			if (loadState == 3)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOLINK;
				sLinkList5.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
				
			}else if (loadState == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_WORKING;
				
				
			}else if (loadState == 2)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_PROTECT;
				sLinkList5.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
				
			}else if (loadState == 0)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOSETPARAM;
				sLinkList5.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
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
					
					//ͨ��,��������
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
					
					//ͨ��״̬, ����ͳ��			
					if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//����
					{	
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//Ƿѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
						}
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//��ѹ
					{
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//Ƿѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
						}
					}
					
				}
			}			
			sLinkList5.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
			//�������ݣ�д����������ļ�
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
			int	existErrorData = FALSE;							//�Ƿ���ڳ�������
			int startPos = sLinkList6.GetPos(cStartFlag);		//��ʼλ��
			int endPos = sLinkList6.GetPos(cEndFlag);			//����λ��
			
			if (sLinkList6.length < startPos+38)				//�������ȣ������¼�����39���ֽ�
			{
				return;
			}
			int loadNum = sLinkList6.GetElem(startPos+1);		//���غ�
			int funNum = sLinkList6.GetElem(startPos+2);		//������
			char endElem = sLinkList6.GetElem(startPos+38);		//����Ԫ��
			if (loadNum<1 || loadNum>=MAX_LOAD_PERCAR || funNum!=0x0D || endElem!= cEndFlag)
			{
				sprintf(logBuf,"Error, Serial Data is wrong! carID:%d, loadNum:%d, funNum:%X, endElem:%X", carID, loadNum, funNum, endElem);
				WriteLog(LEVEL_ERROR,logBuf);
				sLinkList6.DelElemEx(startPos+38);				//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
			}
			
			//�ײ㷵�صģ�����״̬
			int loadState = (int)sLinkList6.GetElem(startPos+37);
			if (loadState == 3)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOLINK;
				sLinkList6.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
				
			}else if (loadState == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_WORKING;
				
				
			}else if (loadState == 2)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_PROTECT;
				sLinkList6.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
				
			}else if (loadState == 0)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOSETPARAM;
				sLinkList6.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
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
					
					//ͨ��,��������
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
					
					//ͨ��״̬, ����ͳ��			
					if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//����
					{	
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//Ƿѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
						}
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//��ѹ
					{
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//Ƿѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
						}
					}
					
				}
			}			
			sLinkList6.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
			//�������ݣ�д����������ļ�
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
			int	existErrorData = FALSE;							//�Ƿ���ڳ�������
			int startPos = sLinkList7.GetPos(cStartFlag);		//��ʼλ��
			int endPos = sLinkList7.GetPos(cEndFlag);			//����λ��
			
			if (sLinkList7.length < startPos+38)				//�������ȣ������¼�����39���ֽ�
			{
				return;
			}
			int loadNum = sLinkList7.GetElem(startPos+1);		//���غ�
			int funNum = sLinkList7.GetElem(startPos+2);		//������
			char endElem = sLinkList7.GetElem(startPos+38);		//����Ԫ��
			if (loadNum<1 || loadNum>=MAX_LOAD_PERCAR || funNum!=0x0D || endElem!= cEndFlag)
			{
				sprintf(logBuf,"Error, Serial Data is wrong! carID:%d, loadNum:%d, funNum:%X, endElem:%X", carID, loadNum, funNum, endElem);
				WriteLog(LEVEL_ERROR,logBuf);
				sLinkList7.DelElemEx(startPos+38);				//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
			}
			
			//�ײ㷵�صģ�����״̬
			int loadState = (int)sLinkList7.GetElem(startPos+37);
			if (loadState == 3)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOLINK;
				sLinkList7.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
				
			}else if (loadState == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_WORKING;
				
				
			}else if (loadState == 2)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_PROTECT;
				sLinkList7.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
				
			}else if (loadState == 0)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOSETPARAM;
				sLinkList7.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
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
					
					//ͨ��,��������
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
					
					//ͨ��״̬, ����ͳ��			
					if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//����
					{	
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//Ƿѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
						}
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//��ѹ
					{
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//Ƿѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
						}
					}
					
				}
			}			
			sLinkList7.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
			//�������ݣ�д����������ļ�
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
			int	existErrorData = FALSE;							//�Ƿ���ڳ�������
			int startPos = sLinkList8.GetPos(cStartFlag);		//��ʼλ��
			int endPos = sLinkList8.GetPos(cEndFlag);			//����λ��
			
			if (sLinkList8.length < startPos+38)				//�������ȣ������¼�����39���ֽ�
			{
				return;
			}
			int loadNum = sLinkList8.GetElem(startPos+1);		//���غ�
			int funNum = sLinkList8.GetElem(startPos+2);		//������
			char endElem = sLinkList8.GetElem(startPos+38);		//����Ԫ��
			if (loadNum<1 || loadNum>=MAX_LOAD_PERCAR || funNum!=0x0D || endElem!= cEndFlag)
			{
				sprintf(logBuf,"Error, Serial Data is wrong! carID:%d, loadNum:%d, funNum:%X, endElem:%X", carID, loadNum, funNum, endElem);
				WriteLog(LEVEL_ERROR,logBuf);
				sLinkList8.DelElemEx(startPos+38);				//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
			}
			
			//�ײ㷵�صģ�����״̬
			int loadState = (int)sLinkList8.GetElem(startPos+37);
			if (loadState == 3)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOLINK;
				sLinkList8.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
				
			}else if (loadState == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_WORKING;
				
				
			}else if (loadState == 2)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_PROTECT;
				sLinkList8.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
				
			}else if (loadState == 0)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOSETPARAM;
				sLinkList8.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
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
					
					//ͨ��,��������
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
					
					//ͨ��״̬, ����ͳ��			
					if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//����
					{	
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//Ƿѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
						}
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//��ѹ
					{
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//Ƿѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
						}
					}
					
				}
			}			
			sLinkList8.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
			//�������ݣ�д����������ļ�
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
			int	existErrorData = FALSE;							//�Ƿ���ڳ�������
			int startPos = sLinkList9.GetPos(cStartFlag);		//��ʼλ��
			int endPos = sLinkList9.GetPos(cEndFlag);			//����λ��
			
			if (sLinkList9.length < startPos+38)				//�������ȣ������¼�����39���ֽ�
			{
				return;
			}
			int loadNum = sLinkList9.GetElem(startPos+1);		//���غ�
			int funNum = sLinkList9.GetElem(startPos+2);		//������
			char endElem = sLinkList9.GetElem(startPos+38);		//����Ԫ��
			if (loadNum<1 || loadNum>=MAX_LOAD_PERCAR || funNum!=0x0D || endElem!= cEndFlag)
			{
				sprintf(logBuf,"Error, Serial Data is wrong! carID:%d, loadNum:%d, funNum:%X, endElem:%X", carID, loadNum, funNum, endElem);
				WriteLog(LEVEL_ERROR,logBuf);
				sLinkList9.DelElemEx(startPos+38);				//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
			}
			
			//�ײ㷵�صģ�����״̬
			int loadState = (int)sLinkList9.GetElem(startPos+37);
			if (loadState == 3)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOLINK;
				sLinkList9.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
				
			}else if (loadState == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_WORKING;
				
				
			}else if (loadState == 2)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_PROTECT;
				sLinkList9.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
				
			}else if (loadState == 0)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOSETPARAM;
				sLinkList9.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
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
					
					//ͨ��,��������
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
					
					//ͨ��״̬, ����ͳ��			
					if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//����
					{	
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//Ƿѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
						}
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//��ѹ
					{
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//Ƿѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
						}
					}
					
				}
			}			
			sLinkList9.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
			//�������ݣ�д����������ļ�
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
			int	existErrorData = FALSE;							//�Ƿ���ڳ�������
			int startPos = sLinkList10.GetPos(cStartFlag);		//��ʼλ��
			int endPos = sLinkList10.GetPos(cEndFlag);			//����λ��
			
			if (sLinkList10.length < startPos+38)				//�������ȣ������¼�����39���ֽ�
			{
				return;
			}
			int loadNum = sLinkList10.GetElem(startPos+1);		//���غ�
			int funNum = sLinkList10.GetElem(startPos+2);		//������
			char endElem = sLinkList10.GetElem(startPos+38);		//����Ԫ��
			if (loadNum<1 || loadNum>=MAX_LOAD_PERCAR || funNum!=0x0D || endElem!= cEndFlag)
			{
				sprintf(logBuf,"Error, Serial Data is wrong! carID:%d, loadNum:%d, funNum:%X, endElem:%X", carID, loadNum, funNum, endElem);
				WriteLog(LEVEL_ERROR,logBuf);
				sLinkList10.DelElemEx(startPos+38);				//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
			}
			
			//�ײ㷵�صģ�����״̬
			int loadState = (int)sLinkList10.GetElem(startPos+37);
			if (loadState == 3)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOLINK;
				sLinkList10.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
				
			}else if (loadState == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_WORKING;
				
				
			}else if (loadState == 2)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_PROTECT;
				sLinkList10.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
				
			}else if (loadState == 0)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOSETPARAM;
				sLinkList10.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
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
					
					//ͨ��,��������
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
					
					//ͨ��״̬, ����ͳ��			
					if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//����
					{	
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//Ƿѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
						}
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//��ѹ
					{
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//Ƿѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
						}
					}
					
				}
			}			
			sLinkList10.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
			//�������ݣ�д����������ļ�
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
			int	existErrorData = FALSE;							//�Ƿ���ڳ�������
			int startPos = sLinkList11.GetPos(cStartFlag);		//��ʼλ��
			int endPos = sLinkList11.GetPos(cEndFlag);			//����λ��
			
			if (sLinkList11.length < startPos+38)				//�������ȣ������¼�����39���ֽ�
			{
				return;
			}
			int loadNum = sLinkList11.GetElem(startPos+1);		//���غ�
			int funNum = sLinkList11.GetElem(startPos+2);		//������
			char endElem = sLinkList11.GetElem(startPos+38);		//����Ԫ��
			if (loadNum<1 || loadNum>=MAX_LOAD_PERCAR || funNum!=0x0D || endElem!= cEndFlag)
			{
				sprintf(logBuf,"Error, Serial Data is wrong! carID:%d, loadNum:%d, funNum:%X, endElem:%X", carID, loadNum, funNum, endElem);
				WriteLog(LEVEL_ERROR,logBuf);
				sLinkList11.DelElemEx(startPos+38);				//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
			}
			
			//�ײ㷵�صģ�����״̬
			int loadState = (int)sLinkList11.GetElem(startPos+37);
			if (loadState == 3)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOLINK;
				sLinkList11.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
				
			}else if (loadState == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_WORKING;
				
				
			}else if (loadState == 2)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_PROTECT;
				sLinkList11.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
				
			}else if (loadState == 0)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOSETPARAM;
				sLinkList11.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
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
					
					//ͨ��,��������
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
					
					//ͨ��״̬, ����ͳ��			
					if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//����
					{	
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//Ƿѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
						}
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//��ѹ
					{
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//Ƿѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
						}
					}
					
				}
			}			
			sLinkList11.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
			//�������ݣ�д����������ļ�
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
			int	existErrorData = FALSE;							//�Ƿ���ڳ�������
			int startPos = sLinkList13.GetPos(cStartFlag);		//��ʼλ��
			int endPos = sLinkList13.GetPos(cEndFlag);			//����λ��
			
			if (sLinkList13.length < startPos+38)				//�������ȣ������¼�����39���ֽ�
			{
				return;
			}
			int loadNum = sLinkList13.GetElem(startPos+1);		//���غ�
			int funNum = sLinkList13.GetElem(startPos+2);		//������
			char endElem = sLinkList13.GetElem(startPos+38);		//����Ԫ��
			if (loadNum<1 || loadNum>=MAX_LOAD_PERCAR || funNum!=0x0D || endElem!= cEndFlag)
			{
				sprintf(logBuf,"Error, Serial Data is wrong! carID:%d, loadNum:%d, funNum:%X, endElem:%X", carID, loadNum, funNum, endElem);
				WriteLog(LEVEL_ERROR,logBuf);
				sLinkList13.DelElemEx(startPos+38);				//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
			}
			
			//�ײ㷵�صģ�����״̬
			int loadState = (int)sLinkList13.GetElem(startPos+37);
			if (loadState == 3)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOLINK;
				sLinkList13.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
				
			}else if (loadState == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_WORKING;
				
				
			}else if (loadState == 2)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_PROTECT;
				sLinkList13.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
				
			}else if (loadState == 0)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOSETPARAM;
				sLinkList13.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
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
					
					//ͨ��,��������
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
					
					//ͨ��״̬, ����ͳ��			
					if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//����
					{	
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//Ƿѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
						}
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//��ѹ
					{
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//Ƿѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
						}
					}
					
				}
			}			
			sLinkList13.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
			//�������ݣ�д����������ļ�
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
			int	existErrorData = FALSE;							//�Ƿ���ڳ�������
			int startPos = sLinkList14.GetPos(cStartFlag);		//��ʼλ��
			int endPos = sLinkList14.GetPos(cEndFlag);			//����λ��
			
			if (sLinkList14.length < startPos+38)				//�������ȣ������¼�����39���ֽ�
			{
				return;
			}
			int loadNum = sLinkList14.GetElem(startPos+1);		//���غ�
			int funNum = sLinkList14.GetElem(startPos+2);		//������
			char endElem = sLinkList14.GetElem(startPos+38);		//����Ԫ��
			if (loadNum<1 || loadNum>=MAX_LOAD_PERCAR || funNum!=0x0D || endElem!= cEndFlag)
			{
				sprintf(logBuf,"Error, Serial Data is wrong! carID:%d, loadNum:%d, funNum:%X, endElem:%X", carID, loadNum, funNum, endElem);
				WriteLog(LEVEL_ERROR,logBuf);
				sLinkList14.DelElemEx(startPos+38);				//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
			}
			
			//�ײ㷵�صģ�����״̬
			int loadState = (int)sLinkList14.GetElem(startPos+37);
			if (loadState == 3)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOLINK;
				sLinkList14.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
				
			}else if (loadState == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_WORKING;
				
				
			}else if (loadState == 2)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_PROTECT;
				sLinkList14.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
				
			}else if (loadState == 0)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOSETPARAM;
				sLinkList14.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
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
					
					//ͨ��,��������
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
					
					//ͨ��״̬, ����ͳ��			
					if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//����
					{	
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//Ƿѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
						}
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//��ѹ
					{
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//Ƿѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
						}
					}
					
				}
			}			
			sLinkList14.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
			//�������ݣ�д����������ļ�
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
			int	existErrorData = FALSE;							//�Ƿ���ڳ�������
			int startPos = sLinkList15.GetPos(cStartFlag);		//��ʼλ��
			int endPos = sLinkList15.GetPos(cEndFlag);			//����λ��
			
			if (sLinkList15.length < startPos+38)				//�������ȣ������¼�����39���ֽ�
			{
				return;
			}
			int loadNum = sLinkList15.GetElem(startPos+1);		//���غ�
			int funNum = sLinkList15.GetElem(startPos+2);		//������
			char endElem = sLinkList15.GetElem(startPos+38);		//����Ԫ��
			if (loadNum<1 || loadNum>=MAX_LOAD_PERCAR || funNum!=0x0D || endElem!= cEndFlag)
			{
				sprintf(logBuf,"Error, Serial Data is wrong! carID:%d, loadNum:%d, funNum:%X, endElem:%X", carID, loadNum, funNum, endElem);
				WriteLog(LEVEL_ERROR,logBuf);
				sLinkList15.DelElemEx(startPos+38);				//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
			}
			
			//�ײ㷵�صģ�����״̬
			int loadState = (int)sLinkList15.GetElem(startPos+37);
			if (loadState == 3)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOLINK;
				sLinkList15.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
				
			}else if (loadState == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_WORKING;
				
				
			}else if (loadState == 2)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_PROTECT;
				sLinkList15.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
				
			}else if (loadState == 0)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOSETPARAM;
				sLinkList15.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
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
					
					//ͨ��,��������
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
					
					//ͨ��״̬, ����ͳ��			
					if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//����
					{	
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//Ƿѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
						}
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//��ѹ
					{
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//Ƿѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
						}
					}
					
				}
			}			
			sLinkList15.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
			//�������ݣ�д����������ļ�
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
			int	existErrorData = FALSE;							//�Ƿ���ڳ�������
			int startPos = sLinkList16.GetPos(cStartFlag);		//��ʼλ��
			int endPos = sLinkList16.GetPos(cEndFlag);			//����λ��
			
			if (sLinkList16.length < startPos+38)				//�������ȣ������¼�����39���ֽ�
			{
				return;
			}
			int loadNum = sLinkList16.GetElem(startPos+1);		//���غ�
			int funNum = sLinkList16.GetElem(startPos+2);		//������
			char endElem = sLinkList16.GetElem(startPos+38);		//����Ԫ��
			if (loadNum<1 || loadNum>=MAX_LOAD_PERCAR || funNum!=0x0D || endElem!= cEndFlag)
			{
				sprintf(logBuf,"Error, Serial Data is wrong! carID:%d, loadNum:%d, funNum:%X, endElem:%X", carID, loadNum, funNum, endElem);
				WriteLog(LEVEL_ERROR,logBuf);
				sLinkList16.DelElemEx(startPos+38);				//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
			}
			
			//�ײ㷵�صģ�����״̬
			int loadState = (int)sLinkList16.GetElem(startPos+37);
			if (loadState == 3)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOLINK;
				sLinkList16.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
				
			}else if (loadState == 1)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_WORKING;
				
				
			}else if (loadState == 2)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_PROTECT;
				sLinkList16.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
				return;
				
			}else if (loadState == 0)
			{
				g_AllCar[carID].m_Load[loadNum-1].m_LoadState = LOAD_STATE_NOSETPARAM;
				sLinkList16.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
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
					
					//ͨ��,��������
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
					
					//ͨ��״̬, ����ͳ��			
					if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CC )//����
					{	
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWVOL;//Ƿѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent > 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent < 
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetCurrentError )
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
						}
					}else if (g_AllCar[carID].m_Load[loadNum-1].m_LoadMode == LOAD_MODE_CV )//��ѹ
					{
						if( C == 0 && F== 0)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_NOLINK;//δ����
							
						}else if (g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMin)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_LOWCUR;//Ƿ��
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowCurrent >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetMax)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERCUR;//����
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage >
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue + g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//��ѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else if (
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_NowVoltage <
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_SetValue - g_SetVoltageError)
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OVERVOL;//Ƿѹ
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnHaveBeenError=1;			//���ֹ�����
							g_AllCar[carID].m_ErrorCounter++;	
							g_AllCar[carID].m_Load[loadNum-1].m_LoadErrorCounter++;
							existErrorData = TRUE;							//�Ƿ���ڳ�������
						}else
						{
							g_AllCar[carID].m_Load[loadNum-1].m_Channel[iChnIndex].m_ChnState = CHN_STATE_OK;//�ϸ�
						}
					}
					
				}
			}			
			sLinkList16.DelElemEx(startPos + 38 );//�ӵ�������endPosλ��֮ǰ��Ԫ��
			//�������ݣ�д����������ļ�
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
/* �������ݲ����߳�,������                                            */
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
/* �������ݲ����߳�,��������                                            */
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