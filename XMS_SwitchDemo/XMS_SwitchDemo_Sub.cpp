/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// �ļ���
 * Author:			// ����
 * Date:			// ����
 * Description:		// ģ������
 * Version:			// �汾��Ϣ
 * History:			// ��ʷ�޸ļ�¼
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"
#include "DJAcsDevState.h"

#include "XMS_SwitchDemo_Sub.h"
#include "XMS_SwitchDemo_String.h"
#include "XMS_SwitchDemo_Event.h"

#include "tools.h"
#include "myncurses.h"
#include "MyLog.h"
#include <unistd.h>

// Special code for CAS(SS1)
#ifdef WIN32
#include "..\CAS_Common_Code\CAS_Common_Cfg.h"
#include "..\Analog_Common_Code\Analog_Common_Code.h"
#else
#include "../CAS_Common_Code/CAS_Common_Cfg.h"
#include "../Analog_Common_Code/Analog_Common_Code.h"
#endif

CmdParamData_CAS_t		g_Param_CAS;
TYPE_ANALOG_GTD_PARAM	g_Param_Analog;

void SetGTD_ToneParam( DeviceID_t *pDevice );

// variable declaration
CMyLog	gLog;
TIni	g_cfg;

// var about work
ACSHandle_t g_acsHandle	   = -1;
DJ_U8		g_u8UnitID	   = 2;

// var about XMS_SwitchDemo.INI
char		cfg_IniName[] = "./XMS_SwitchDemo.ini";
int			cfg_iCalledLen				   = 4;             //���к��볤��
extern int 		TRK_NUM_PERPAGE;
char		cfg_CallingNum[32]			   = "888666";    //���к���
int			cfg_iCallOutRule;                               //���й���,2��ʾ��ϯ�����,3��ʾ��ϯ������
ServerID_t	cfg_ServerID;
char		cfg_VocPath[128];
int			cfg_iDispChnl;
int			cfg_iVoiceRule;
int			cfg_iPartWork;
int			cfg_iPartWorkModuleID;

int			cfg_s32DebugOn;

// var in XMS_SwitchDemo_Event.CPP
extern TYPE_XMS_DSP_DEVICE_RES_DEMO AllDeviceRes[MAX_DSP_MODULE_NUMBER_OF_XMS];

extern int							g_iTotalTrunk;
extern int							g_iTotalTrunkOpened;
extern TYPE_CHANNEL_MAP_TABLE		MapTable_Trunk[MAX_TRUNK_NUM_IN_THIS_DEMO];

extern int							g_iTotalVoice;
extern int							g_iTotalVoiceOpened;
extern int							g_iTotalVoiceFree;
extern TYPE_CHANNEL_MAP_TABLE		MapTable_Voice[MAX_TRUNK_NUM_IN_THIS_DEMO];
extern int							g_iTotalUser;
extern int							g_iTotalUserOpened;
extern TYPE_CHANNEL_MAP_TABLE		MapTable_User[MAX_TRUNK_NUM_IN_THIS_DEMO];
extern int							g_iTotalModule;
extern DJ_S8						MapTable_Module[MAX_DSP_MODULE_NUMBER_OF_XMS];

//����E1�˿ڷ����ͨ�����Ĳ�ͬ,���㵱ǰiSeqID���ڵ���-----------------------------
int CalDispRow( int iSeqID )
{
	int iRet;
	//cfg_iDispChnlÿ��E1�˿ڷ����ͨ��
	if( cfg_iDispChnl == 30 )
	{
		if( ( iSeqID % 16 ) == 0 )
		{
			return -1;
		}
		iRet = ( iSeqID / 32 ) * 30 + ( iSeqID % 32 );
		iRet--;
		if( ( iSeqID % 32 ) > 16 )
		{
			iRet--;
		}
		return iRet;
	}
	if( cfg_iDispChnl == 31 )
	{
		if( ( iSeqID % 32 ) == 0 )
		{
			return -1;
		}
		iRet = ( iSeqID / 32 ) * 31 + ( iSeqID % 32 );
		iRet--;
		return iRet;
	}
	return iSeqID;
}

//���� �ӿ�ͨ���б��� State ��־λ
void DrawMain_State( TRUNK_STRUCT *pOneTrunk )
{
	draw_main( pOneTrunk );
}

//���� �ӿ�ͨ���б�� called calling ��־λ---------------------------------------
void DrawMain_CallInfo( TRUNK_STRUCT *pOneTrunk )
{
	draw_main( pOneTrunk );
}

//���� �ӿ�ͨ���б�� �� DTMF ��־λ
void DrawMain_DTMF( TRUNK_STRUCT *pOneTrunk )
{
	draw_main( pOneTrunk );
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
void DrawMain_VocInfo( TRUNK_STRUCT *pOneTrunk )
{
	draw_main( pOneTrunk );
}

//���Ľӿ�ͨ���б��� LineState ��־λ------------------------------------------------
void DrawMain_LineState( TRUNK_STRUCT *pOneTrunk )
{
	draw_main( pOneTrunk );
}

//���� ��ϯ��Դ�б�� �� FailReason----------------------------------------------
void DrawMain_FailReason( TRUNK_STRUCT *pOneUser, char * failReasonStr )
{
	int iDispRow;
	iDispRow = pOneUser->iUserSeqID;
	draw_main( pOneUser );
}

// �ػ� DSP�б�� CHN�б��
void ReDrawAll( void )
{
	char			TmpStr[256];
	int				i, iDispRow;
	TRUNK_STRUCT	*pOneTrunk;

	//�ػ�CHN�б��
//	pdlg->m_List_ChnRes.DeleteAllItems( );
	for( i = 0; i < g_iTotalTrunk; i++ )
	{
		iDispRow = CalDispRow( i );
		if( iDispRow < 0 )
		{
			continue;
		}

		// Fix content
		sprintf( TmpStr, "%3d", iDispRow );
//		pdlg->m_List_ChnRes.InsertItem( iDispRow, TmpStr );

		sprintf( TmpStr, "%d,%d", MapTable_Trunk[i].m_s8ModuleID, MapTable_Trunk[i].m_s16ChannelID );
//		pdlg->m_List_ChnRes.SetItemText( iDispRow, 1, TmpStr );

		pOneTrunk = &M_OneTrunk( MapTable_Trunk[i] ); //�õ���ǰͨ���� �ӿ�ͨ���ṹ��ָ��
//		pdlg->m_List_ChnRes.SetItemText( iDispRow, 2, GetString_DeviceSub( pOneTrunk->deviceID.m_s16DeviceSub ) );

		//Changed content
		DrawMain_LineState( pOneTrunk );
		DrawMain_State( pOneTrunk );
		DrawMain_CallInfo( pOneTrunk );
		DrawMain_DTMF( pOneTrunk );
	}

	//�ػ�DSP�б��
//	pdlg->m_List_DSPRes.DeleteAllItems( );
//	pdlg->m_List_DSPRes.InsertItem( 0, "Total" ); //����ͳ����
	for( i = 0; i < g_iTotalModule; i++ )
	{
		sprintf( TmpStr, "%3d", i );
//		pdlg->m_List_DSPRes.InsertItem( i + 1, TmpStr );

		sprintf( TmpStr, "%3d", MapTable_Module[i] );
//		pdlg->m_List_DSPRes.SetItemText( i + 1, 1, TmpStr );
	}

	//�ػ���ϯ��Դ�б��
//	pdlg->m_List_AnalogUser.DeleteAllItems( );
	for( i = 0; i < g_iTotalUser; i++ )
	{
		iDispRow = i;

		sprintf( TmpStr, "%3d", i );
//		pdlg->m_List_AnalogUser.InsertItem( iDispRow, TmpStr );

		sprintf( TmpStr, "%d,%d", MapTable_User[i].m_s8ModuleID, MapTable_User[i].m_s16ChannelID );
//		pdlg->m_List_AnalogUser.SetItemText( iDispRow, 1, TmpStr );

		pOneTrunk = &M_OneTrunk( MapTable_User[i] );
//		pdlg->m_List_AnalogUser.SetItemText( iDispRow, 2, GetString_DeviceSub( pOneTrunk->deviceID.m_s16DeviceSub ) );

	}
}

/***********************************************************
* Function:       // ��ʼ��ϵͳ
* Description:    //
* Input:          //
* Input:          //
* Output:         //
* Return:         //
* Others:         //
***********************************************************/
bool InitSystem( void )
{
	RetCode_t	r;
	char		MsgStr[160];
	char		logPath[256];
#ifdef WIN32
	memcpy( logPath, "D:\\mingyangworkspace\\MyLog", sizeof( "D:\\mingyangworkspace\\MyLog" ) );
#else
	memcpy( logPath, "./", sizeof( "./" ) );
#endif
	if( gLog.InitLogCfg( LEVEL_DEBUG, logPath, 3 * 1024 * 1024 ) < 0 )  //��ʼ����־����,��־����:WARN,��־���:3M
	{
		printf( "InitLogCfg() Fail!\n" );
	}
	//��ȡ�����ļ�XMS_SwitchDemo.ini
	ReadFromConfig( );                                                  //Read From "XMS_SwitchDemo.INI"

	// Special code for CAS(SS1)
	// Read From "C:\\DJKeygoe\\Samples\\CAS_Common_Code\\XMS_CAS_Cfg.INI"
	if( CAS_Common_Cfg_ReadCfg( &g_Param_CAS ) != 0 )
	{
		sprintf( MsgStr, "CAS_Common_Cfg_ReadCfg \"C:\\DJKeygoe\\Samples\\CAS_Common_Code\\XMS_CAS_Cfg.INI\" Error!" );
	}
	// end of code for CAS(SS1)

	// Special code for Analog
	// Read From "C:\\DJKeygoe\\Samples\\Analog_Common_Code\\XMS_Analog_Cfg.INI"
	if( Analog_Common_Cfg_ReadCfg( &g_Param_Analog ) != 0 )
	{
		sprintf( MsgStr, "Analog_Common_Cfg_ReadCfg \"C:\\DJKeygoe\\Samples\\Analog_Common_Code\\XMS_Analog_Cfg.INI\" Error!" );
	}
	// end of code for CAS(SS1)
	r = XMS_acsDllInit( );
	if( r < 0 )
	{
		sprintf( MsgStr, "XMS_acsDllInit failed! ret = %d\n", r );
		AddMsg( MsgStr );
	}else
	{
		sprintf( MsgStr, "XMS_acsDllInit OK!\n" );
		AddMsg( MsgStr );
	}
	// now open ACS Server
	/* Call acsOpenStream() to connect with ACS Server */
	r = XMS_acsOpenStream( &g_acsHandle,
	                       &cfg_ServerID,
	                       g_u8UnitID,      // App Unit ID
	                       32,              // sendQSize, in K Bytes
	                       32,              // recvQSize, in K Bytes
	                       cfg_s32DebugOn,  // Debug On/Off
	                       NULL );
	if( r < 0 )
	{
		sprintf( MsgStr, "XMS_acsOpenStream(IP Addr : %s, port : %d) FAIL. ret = %d",
		         cfg_ServerID.m_s8ServerIp, cfg_ServerID.m_u32ServerPort, r );
		AddMsg( MsgStr );
		return false;
	}else
	{
		sprintf( MsgStr, "XMS_acsOpenStream(%s,%d) OK!", cfg_ServerID.m_s8ServerIp, cfg_ServerID.m_u32ServerPort );
		AddMsg( MsgStr );
	}
	//���ûص�����
	r = XMS_acsSetESR( g_acsHandle, (EsrFunc)EvtHandler, 0, 1 );
	if( r < 0 )
	{
		sprintf( MsgStr, "XMS_acsSetESR() FAIL! ret = %d", r );
		AddMsg( MsgStr );
		return false;
	}else
	{
		sprintf( MsgStr, "XMS_acsSetESR() OK!" );
		AddMsg( MsgStr );
	}

	InitAllDeviceRes( );

	r = XMS_acsGetDeviceList( g_acsHandle, NULL );
	if ( r < 0 )
	{
		sprintf ( MsgStr, "XMS_acsGetDeviceList() FAIL! ret = %d", r );
		AddMsg ( MsgStr );
		return false;
	}
	else
	{
		sprintf ( MsgStr, "XMS_acsGetDeviceList() OK!" );
		AddMsg ( MsgStr );
	}

	return true;
}

//��ȡ��ʼ���ļ�XMS_SwitchDemo.INI-----------------------------------------------------
void ReadFromConfig( void )
{
	g_cfg.OpenIniFile( cfg_IniName );

	read_string( g_cfg, "ConfigInfo", "IpAddr", cfg_ServerID.m_s8ServerIp, sizeof( cfg_ServerID.m_s8ServerIp ) );
	read_int( g_cfg, "ConfigInfo", "Port", ( int & )cfg_ServerID.m_u32ServerPort );

	read_string( g_cfg, "ConfigInfo", "UserName", cfg_ServerID.m_s8UserName, sizeof( cfg_ServerID.m_s8UserName ) );
	read_string( g_cfg, "ConfigInfo", "PassWord", cfg_ServerID.m_s8UserPwd, sizeof( cfg_ServerID.m_s8UserPwd ) );
	read_string( g_cfg, "ConfigInfo", "VocPath", cfg_VocPath, sizeof( cfg_VocPath ) );
	read_string( g_cfg, "ConfigInfo", "CallingNum", cfg_CallingNum, sizeof( cfg_CallingNum ) );

	read_int( g_cfg, "ConfigInfo", "DispChnl", ( int & )cfg_iDispChnl );
	read_int( g_cfg, "ConfigInfo", "VoiceRule", ( int & )cfg_iVoiceRule );
	read_int( g_cfg, "ConfigInfo", "PartWork", ( int & )cfg_iPartWork );
	read_int( g_cfg, "ConfigInfo", "PartWorkModuleID", ( int & )cfg_iPartWorkModuleID );
	read_int( g_cfg, "ConfigInfo", "DebugOn", ( int & )cfg_s32DebugOn );
	read_int( g_cfg, "ConfigInfo", "CallOutRule", ( int & )cfg_iCallOutRule );
	
	read_int( g_cfg, "ConfigInfo", "TRK_NUM_PERPAGE", ( int & )TRK_NUM_PERPAGE );
	if(TRK_NUM_PERPAGE > MAX_TRK_NUM_PERPAGE)
	{
		TRK_NUM_PERPAGE = MAX_TRK_NUM_PERPAGE;
	}
	read_int( g_cfg, "ConfigInfo", "CalledLen", ( int & )cfg_iCalledLen );
	if( cfg_iCalledLen > 20 )
	{
		cfg_iCalledLen = 16;
	}


}



//д��־��Ϣ
void AddMsg( char *str )
{
	gLog.WriteLog(LEVEL_DEBUG, "%s",str);
}


//�򿪽ӿ�ͨ���豸----------------------------------------------------------------
void OpenTrunkDevice( TRUNK_STRUCT *pOneTrunk )
{
	RetCode_t r;
	if( pOneTrunk->State == TRK_WAITOPEN ) // not Open yet
	{
		r = XMS_ctsOpenDevice( g_acsHandle, &pOneTrunk->deviceID, NULL );
		if( r < 0 )
		{
			AddMsg( "XMS_ctsOpenDevice Fail in OpenTrunkDevice()!" );
		}
	}
}

//�������豸------------------------------------------------------------------
void OpenVoiceDevice( VOICE_STRUCT *pOneVoice )
{
	RetCode_t r;
	if( pOneVoice->State == VOC_WAITOPEN ) // not Open yet
	{
		r = XMS_ctsOpenDevice( g_acsHandle, &pOneVoice->deviceID, NULL );
		if( r < 0 )
		{
			AddMsg( "OpenVoiceDevice() Fail!" );
		}
	}
}

//�򿪰忨�豸------------------------------------------------------------
void OpenBoardDevice( DJ_S8 s8DspModID )
{
	RetCode_t r;
	if( AllDeviceRes[s8DspModID].bOpenFlag == false ) // not Open yet
	{
		r = XMS_ctsOpenDevice( g_acsHandle, &AllDeviceRes[s8DspModID].deviceID, NULL );

		if( r < 0 )
		{
			AddMsg( "XMS_ctsOpenDevice Fail in OpenBoardDevice()!" );
		}
	}
}

//��ģ���Ϊs8DspModID��DSP�忨�豸�ϵ������豸---------------------------------------------------
void OpenAllDevice_Dsp( DJ_S8 s8DspModID )
{
	int i;

	AllDeviceRes[s8DspModID].bErrFlag	   = false;
	AllDeviceRes[s8DspModID].RemoveState   = DSP_REMOVE_STATE_NONE; //Do not remove the DSP hardware

	//�򿪰忨�豸(DSPģ��)
	OpenBoardDevice( s8DspModID );

	//������ͨ���豸
	for( i = 0; i < AllDeviceRes[s8DspModID].lVocNum; i++ )
	{
		OpenVoiceDevice( &AllDeviceRes[s8DspModID].pVoice[i] );
	}

	//�򿪽ӿ�ͨ���豸
	for( i = 0; i < AllDeviceRes[s8DspModID].lTrunkNum; i++ )
	{
		OpenTrunkDevice( &AllDeviceRes[s8DspModID].pTrunk[i] );
	}
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
void OpenDeviceOK( DeviceID_t *pDevice )
{
	TRUNK_STRUCT *pOneTrunk;

	if( pDevice->m_s16DeviceMain == XMS_DEVMAIN_BOARD )                             //�忨�豸
	{
		AllDeviceRes[pDevice->m_s8ModuleID].deviceID.m_CallID  = pDevice->m_CallID; // this line is very important, must before all operation
		AllDeviceRes[pDevice->m_s8ModuleID].bOpenFlag		   = true;
		SetGTD_ToneParam( pDevice );
	}
	if( pDevice->m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )                      //ͨ���ӿ��豸
	{
		pOneTrunk					   = &M_OneTrunk( *pDevice );
		pOneTrunk->deviceID.m_CallID   = pDevice->m_CallID;                         // this line is very important, must before all operation

		if( pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER )          //���豸����Ϊ��ϯ�豸
		{
			InitUserChannel( pOneTrunk );
			XMS_ctsResetDevice( g_acsHandle, pDevice, NULL );
			XMS_ctsGetDevState( g_acsHandle, pDevice, NULL );
			g_iTotalUserOpened++;                                                   //��������ϯ������
		}else
		{
			InitTrunkChannel( pOneTrunk );
			XMS_ctsResetDevice( g_acsHandle, pDevice, NULL );
			XMS_ctsGetDevState( g_acsHandle, pDevice, NULL );
		}
		// modify the count
		g_iTotalTrunkOpened++;
		AllDeviceRes[pDevice->m_s8ModuleID].lTrunkOpened++;

		if( pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK )
		{
			// Set AnalogTrunk
			void						* p = NULL;
			CmdParamData_AnalogTrunk_t	cmdAnalogTrunk;
			DJ_U16						u16ParamType   = ANALOGTRUNK_PARAM_UNIPARAM;
			DJ_U16						u16ParamSize   = sizeof( CmdParamData_AnalogTrunk_t );

			memset( &cmdAnalogTrunk, 0, sizeof( cmdAnalogTrunk ) );
			cmdAnalogTrunk.m_u16CallInRingCount	   = 3;     // 3 times RingBack
			cmdAnalogTrunk.m_u16CallInRingTimeOut  = 6000;  // 60 seconds

			p = (void*)&cmdAnalogTrunk;

			RetCode_t	r;
			r = XMS_ctsSetParam( g_acsHandle, &pOneTrunk->deviceID, u16ParamType, u16ParamSize, (DJ_Void *)p );
			char		szbuffer[1024];
			memset( szbuffer, 0, sizeof( szbuffer ) );
			sprintf( szbuffer, "Set AnalogTrunk  ret = %d\n", r );
			AddMsg( szbuffer );

			// Search Free Voice for get CallerID(FSK)
			// if you needn't CallerID, ignore next line
			PrepareForCallerID( pOneTrunk );
		}else if( pOneTrunk->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER )
		{
			// Set AnalogUser
			void						* p = NULL;

			CmdParamData_AnalogUser_t	g_AnalogUser;
			DJ_U16						u16ParamType   = ANALOGUSER_PARAM_UNIPARAM;
			DJ_U16						u16ParamSize   = sizeof( CmdParamData_AnalogUser_t );

			memset( &g_AnalogUser, 0, sizeof( g_AnalogUser ) );

			g_AnalogUser.m_u8CallerIdOption		   = 1;
			g_AnalogUser.m_u8RingCadenceType	   = 0;
			g_AnalogUser.m_u8CallOutRingTimeOut	   = 30;

			p = (void*)&g_AnalogUser;

			RetCode_t r;

			r = XMS_ctsSetParam( g_acsHandle, &pOneTrunk->deviceID, u16ParamType, u16ParamSize, (DJ_Void *)p );
			char szbuffer[1024];
			memset( szbuffer, 0, sizeof( szbuffer ) );
			sprintf( szbuffer, "Set AnalogTrunk  ret = %d\n", r );
			AddMsg( szbuffer );
		}
	}

	if( pDevice->m_s16DeviceMain == XMS_DEVMAIN_VOICE )
	{
		M_OneVoice( *pDevice ).deviceID.m_CallID = pDevice->m_CallID; // this is very important

		// init this Device: Voice
		Change_Voc_State( &M_OneVoice( *pDevice ), VOC_FREE );

		XMS_ctsResetDevice( g_acsHandle, pDevice, NULL );
		XMS_ctsGetDevState( g_acsHandle, pDevice, NULL );

		// Build Index
		if( g_iTotalVoiceOpened == 0 )
		{
			My_BuildIndex( pDevice );
		}

		// modify the count
		g_iTotalVoiceOpened++;
		g_iTotalVoiceFree++;
		AllDeviceRes[pDevice->m_s8ModuleID].lVocOpened++;
		AllDeviceRes[pDevice->m_s8ModuleID].lVocFreeNum++;

	}
	draw_count();
}

// Special code for Analog
void SetGTD_ToneParam( DeviceID_t *pDevice )
{
	RetCode_t					r;
	DJ_U16						u16ParamType, u16ParamSize;
	CmdParamData_GtdFreq_t		TmpGtdFreq;
	CmdParamData_GtdProtoType_t TmpGtdProto;

	// ---------- set Freq ----------
	u16ParamType   = BOARD_PARAM_SETGTDFREQ;
	u16ParamSize   = sizeof( CmdParamData_GtdFreq_t );

	// freq0
	TmpGtdFreq.m_u16Freq_Index = 0;
	TmpGtdFreq.m_u16Freq_Coef  = g_Param_Analog.m_u16Freq0;
	r						   = XMS_ctsSetParam( g_acsHandle, pDevice, u16ParamType, u16ParamSize, (DJ_Void *)&TmpGtdFreq );

	// freq1
	TmpGtdFreq.m_u16Freq_Index = 1;
	TmpGtdFreq.m_u16Freq_Coef  = g_Param_Analog.m_u16Freq1;
	r						   = XMS_ctsSetParam( g_acsHandle, pDevice, u16ParamType, u16ParamSize, (DJ_Void *)&TmpGtdFreq );

	// ---------- set DialTone ----------
	u16ParamType   = BOARD_PARAM_SETGTDTONE;
	u16ParamSize   = sizeof( CmdParamData_GtdProtoType_t );

	memset( &TmpGtdProto, 0, sizeof( CmdParamData_GtdProtoType_t ) );
	TmpGtdProto.m_u16GtdID		   = 48;                                        // DialTone, we use GTD's ID 48
	TmpGtdProto.m_u16Freq_Mask	   = g_Param_Analog.m_u16DialTone_FreqIndexMask;
	TmpGtdProto.m_u16Envelope_Mode = 0;
	TmpGtdProto.m_u16Repeat_Count  = 1;
	TmpGtdProto.m_u16Min_On_Time1  = g_Param_Analog.m_u16DialTone_On_Time / 15; // the unit is 15 ms

	r = XMS_ctsSetParam( g_acsHandle, pDevice, u16ParamType, u16ParamSize, (DJ_Void *)&TmpGtdProto );

	// ---------- set RingBack Tone ----------
	u16ParamType   = BOARD_PARAM_SETGTDTONE;
	u16ParamSize   = sizeof( CmdParamData_GtdProtoType_t );

	memset( &TmpGtdProto, 0, sizeof( CmdParamData_GtdProtoType_t ) );
	TmpGtdProto.m_u16GtdID		   = 49;                                                                        // DialTone, we use GTD's ID 49
	TmpGtdProto.m_u16Freq_Mask	   = g_Param_Analog.m_u16RingBackTone_FreqIndexMask;
	TmpGtdProto.m_u16Envelope_Mode = 1;
	TmpGtdProto.m_u16Repeat_Count  = 1;
	TmpGtdProto.m_u16Min_On_Time1  = ( g_Param_Analog.m_u16RingBackTone_On_Time *
	                                   ( 100 - g_Param_Analog.m_u16RingBackTone_TimeDeviation ) / 100 ) / 15;   // the unit is 15 ms
	TmpGtdProto.m_u16Max_On_Time1 = ( g_Param_Analog.m_u16RingBackTone_On_Time *
	                                  ( 100 + g_Param_Analog.m_u16RingBackTone_TimeDeviation ) ) / 100 / 15;    // the unit is 15 ms
	TmpGtdProto.m_u16Min_Off_Time1 = ( g_Param_Analog.m_u16RingBackTone_Off_Time *
	                                   ( 100 - g_Param_Analog.m_u16RingBackTone_TimeDeviation ) ) / 100 / 15;   // the unit is 15 ms
	TmpGtdProto.m_u16Max_Off_Time1 = ( g_Param_Analog.m_u16RingBackTone_Off_Time *
	                                   ( 100 + g_Param_Analog.m_u16RingBackTone_TimeDeviation ) ) / 100 / 15;   // the unit is 15 ms

	r = XMS_ctsSetParam( g_acsHandle, pDevice, u16ParamType, u16ParamSize, (DJ_Void *)&TmpGtdProto );

	// ---------- set Busy Tone0 ----------
	u16ParamType   = BOARD_PARAM_SETGTDTONE;
	u16ParamSize   = sizeof( CmdParamData_GtdProtoType_t );

	memset( &TmpGtdProto, 0, sizeof( CmdParamData_GtdProtoType_t ) );
	TmpGtdProto.m_u16GtdID		   = 50;                                                                    // BusyTone0, we use GTD's ID 50
	TmpGtdProto.m_u16Freq_Mask	   = g_Param_Analog.m_u16BusyTone0_FreqIndexMask;
	TmpGtdProto.m_u16Envelope_Mode = 1;
	TmpGtdProto.m_u16Repeat_Count  = 1;
	TmpGtdProto.m_u16Min_On_Time1  = ( g_Param_Analog.m_u16BusyTone0_On_Time *
	                                   ( 100 - g_Param_Analog.m_u16BusyTone0_TimeDeviation ) / 100 ) / 15;  // the unit is 15 ms
	TmpGtdProto.m_u16Max_On_Time1 = ( g_Param_Analog.m_u16BusyTone0_On_Time *
	                                  ( 100 + g_Param_Analog.m_u16BusyTone0_TimeDeviation ) ) / 100 / 15;   // the unit is 15 ms
	TmpGtdProto.m_u16Min_Off_Time1 = ( g_Param_Analog.m_u16BusyTone0_Off_Time *
	                                   ( 100 - g_Param_Analog.m_u16BusyTone0_TimeDeviation ) ) / 100 / 15;  // the unit is 15 ms
	TmpGtdProto.m_u16Max_Off_Time1 = ( g_Param_Analog.m_u16BusyTone0_Off_Time *
	                                   ( 100 + g_Param_Analog.m_u16BusyTone0_TimeDeviation ) ) / 100 / 15;  // the unit is 15 ms

	r = XMS_ctsSetParam( g_acsHandle, pDevice, u16ParamType, u16ParamSize, (DJ_Void *)&TmpGtdProto );

	// ---------- set Busy Tone1 ----------
	u16ParamType   = BOARD_PARAM_SETGTDTONE;
	u16ParamSize   = sizeof( CmdParamData_GtdProtoType_t );

	memset( &TmpGtdProto, 0, sizeof( CmdParamData_GtdProtoType_t ) );
	TmpGtdProto.m_u16GtdID		   = 51;                                                                    // BusyTone1, we use GTD's ID 51
	TmpGtdProto.m_u16Freq_Mask	   = g_Param_Analog.m_u16BusyTone1_FreqIndexMask;
	TmpGtdProto.m_u16Envelope_Mode = 1;
	TmpGtdProto.m_u16Repeat_Count  = 1;
	TmpGtdProto.m_u16Min_On_Time1  = ( g_Param_Analog.m_u16BusyTone1_On_Time *
	                                   ( 100 - g_Param_Analog.m_u16BusyTone1_TimeDeviation ) / 100 ) / 15;  // the unit is 15 ms
	TmpGtdProto.m_u16Max_On_Time1 = ( g_Param_Analog.m_u16BusyTone1_On_Time *
	                                  ( 100 + g_Param_Analog.m_u16BusyTone1_TimeDeviation ) ) / 100 / 15;   // the unit is 15 ms
	TmpGtdProto.m_u16Min_Off_Time1 = ( g_Param_Analog.m_u16BusyTone1_Off_Time *
	                                   ( 100 - g_Param_Analog.m_u16BusyTone1_TimeDeviation ) ) / 100 / 15;  // the unit is 15 ms
	TmpGtdProto.m_u16Max_Off_Time1 = ( g_Param_Analog.m_u16BusyTone1_Off_Time *
	                                   ( 100 + g_Param_Analog.m_u16BusyTone1_TimeDeviation ) ) / 100 / 15;  // the unit is 15 ms

	r = XMS_ctsSetParam( g_acsHandle, pDevice, u16ParamType, u16ParamSize, (DJ_Void *)&TmpGtdProto );

	// ---------- set Busy Tone2 ----------
	u16ParamType   = BOARD_PARAM_SETGTDTONE;
	u16ParamSize   = sizeof( CmdParamData_GtdProtoType_t );

	memset( &TmpGtdProto, 0, sizeof( CmdParamData_GtdProtoType_t ) );
	TmpGtdProto.m_u16GtdID		   = 52;                                                                    // BusyTone2, we use GTD's ID 52
	TmpGtdProto.m_u16Freq_Mask	   = g_Param_Analog.m_u16BusyTone2_FreqIndexMask;
	TmpGtdProto.m_u16Envelope_Mode = 1;
	TmpGtdProto.m_u16Repeat_Count  = 1;
	TmpGtdProto.m_u16Min_On_Time1  = ( g_Param_Analog.m_u16BusyTone2_On_Time *
	                                   ( 100 - g_Param_Analog.m_u16BusyTone2_TimeDeviation ) / 100 ) / 15;  // the unit is 15 ms
	TmpGtdProto.m_u16Max_On_Time1 = ( g_Param_Analog.m_u16BusyTone2_On_Time *
	                                  ( 100 + g_Param_Analog.m_u16BusyTone2_TimeDeviation ) ) / 100 / 15;   // the unit is 15 ms
	TmpGtdProto.m_u16Min_Off_Time1 = ( g_Param_Analog.m_u16BusyTone2_Off_Time *
	                                   ( 100 - g_Param_Analog.m_u16BusyTone2_TimeDeviation ) ) / 100 / 15;  // the unit is 15 ms
	TmpGtdProto.m_u16Max_Off_Time1 = ( g_Param_Analog.m_u16BusyTone2_Off_Time *
	                                   ( 100 + g_Param_Analog.m_u16BusyTone2_TimeDeviation ) ) / 100 / 15;  // the unit is 15 ms

	r = XMS_ctsSetParam( g_acsHandle, pDevice, u16ParamType, u16ParamSize, (DJ_Void *)&TmpGtdProto );
}

// end of code for Analog

void Change_State( TRUNK_STRUCT *pOneTrunk, TRUNK_STATE NewState )
{
	pOneTrunk->State = NewState;
	DrawMain_State( pOneTrunk );

	// Check if ready to remove DSP hardware
	if( ( AllDeviceRes[pOneTrunk->deviceID.m_s8ModuleID].RemoveState == DSP_REMOVE_STATE_START )
	    && ( NewState == TRK_FREE ) )
	{
		pOneTrunk->State = TRK_WAIT_REMOVE;
		DrawMain_State( pOneTrunk );
		CheckRemoveReady( pOneTrunk->deviceID.m_s8ModuleID );
	}
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
void CheckRemoveReady( DJ_S8 s8DspModID )
{
	int i;

	// check device : INTERFACE_CH
	for( i = 0; i < AllDeviceRes[s8DspModID].lTrunkNum; i++ )
	{
		if( ( AllDeviceRes[s8DspModID].pTrunk[i].State != TRK_WAITOPEN )
		    && ( AllDeviceRes[s8DspModID].pTrunk[i].State != TRK_WAIT_REMOVE ) )
		{
			return;
		}
	}

	// check device : VOICE
	for( i = 0; i < AllDeviceRes[s8DspModID].lVocNum; i++ )
	{
		if( ( AllDeviceRes[s8DspModID].pVoice[i].State != VOC_WAITOPEN )
		    && ( AllDeviceRes[s8DspModID].pVoice[i].State != VOC_WAIT_REMOVE ) )
		{
			return;
		}
	}

	// all device in this DSP is ready for remove
	AllDeviceRes[s8DspModID].RemoveState = DSP_REMOVE_STATE_READY;
}

// Search Free Voice for get CallerID(FSK)
void PrepareForCallerID( TRUNK_STRUCT *pOneTrunk )
{
	DeviceID_t FreeVocDeviceID;

	if( pOneTrunk->deviceID.m_s16DeviceSub != XMS_DEVSUB_ANALOG_TRUNK )
	{
		return;
	}

	if( SearchOneFreeVoice( pOneTrunk, &FreeVocDeviceID ) >= 0 )
	{
		pOneTrunk->VocDevID						   = FreeVocDeviceID;
		M_OneVoice( FreeVocDeviceID ).UsedDevID	   = pOneTrunk->deviceID;
		DrawMain_VocInfo( pOneTrunk );
		My_DualLink( &FreeVocDeviceID, &pOneTrunk->deviceID );
		SetGtD_AnalogTrunk( &FreeVocDeviceID );
	}
}

//�ı������豸״̬-----------------------------------------------------------
void Change_Voc_State( VOICE_STRUCT *pOneVoice, VOICE_STATE NewState )
{
	pOneVoice->State = NewState;
}

// --------------------------------------------------------------------------------
void My_BuildIndex( DeviceID_t *pVocDevID )
{
	RetCode_t		r;
	char			FileName[256], MsgStr[256];
	int				i;
	PlayProperty_t	playProperty;

	r = XMS_ctsInitPlayIndex( g_acsHandle, pVocDevID, NULL );
	if( r < 0 )
	{
		sprintf( MsgStr, "XMS_ctsInitPlayIndex() FAIL. ret = r", r );
		AddMsg( MsgStr );
		return;
	}

	memset( &playProperty, 0, sizeof( playProperty ) );
	for( i = 0; i < 17; i++ )
	{
		if( i == 16 )
		{
			playProperty.m_u16PlayType = XMS_BUILD_INDEX_FILE;
			sprintf( FileName, "%s\\bank.008", cfg_VocPath );
		}else
		{
			playProperty.m_u16PlayType = XMS_BUILD_INDEX_RAM;
			sprintf( FileName, "%s\\D%d", cfg_VocPath, i );
		}

		strcpy( playProperty.m_s8PlayContent, FileName );

		r = XMS_ctsBuildPlayIndex( g_acsHandle, pVocDevID, &playProperty, NULL );
		if( r < 0 )
		{
			sprintf( MsgStr, "XMS_ctsBuildPlayIndex(%s) FAIL. ret = r", FileName, r );
			AddMsg( MsgStr );
		}
	}
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
void My_InitDtmfBuf( TRUNK_STRUCT *pOneTrunk )
{
	pOneTrunk->DtmfCount   = 0;
	pOneTrunk->DtmfBuf[0]  = 0;
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
int  SearchOneFreeVoice( TRUNK_STRUCT *pOneTrunk, DeviceID_t *pFreeVocDeviceID )
{
	DJ_S8			s8ModID;
	DJ_S16			s16ChID;
	int				i;
	static int		iLoopStart = 0;
	VOICE_STRUCT	*pOneVoice;
	DJ_S8			s8SearchModID;
	long			lNowMostFreeNum;

	s8ModID = pOneTrunk->deviceID.m_s8ModuleID;

	if( cfg_iVoiceRule == 0 ) // Fix relationship between Trunk & Voice
	{
		s16ChID = pOneTrunk->deviceID.m_s16ChannelID;

		i = pOneTrunk->iModSeqID;

		if( i < AllDeviceRes[s8ModID].lVocNum )
		{
			pOneVoice = &AllDeviceRes[s8ModID].pVoice[i];
			if( pOneVoice->State != VOC_WAITOPEN )
			{
				*pFreeVocDeviceID = pOneVoice->deviceID;

				// use this voice device
				Change_Voc_State( pOneVoice, VOC_USED );
				AllDeviceRes[s8ModID].lVocFreeNum--;
				g_iTotalVoiceFree--;
				draw_count();
				return i;
			}
		}

		return -1;
	}

	if( cfg_iVoiceRule == 1 ) // Search in Same Module
	{
		s8SearchModID = s8ModID;
	}else // Search in Most free resource module
	{
		s8SearchModID	   = -1;
		lNowMostFreeNum	   = -1;
		for( i = 0; i < g_iTotalModule; i++ )
		{
			if( AllDeviceRes[MapTable_Module[i]].lVocFreeNum > lNowMostFreeNum )
			{
				s8SearchModID	   = MapTable_Module[i];
				lNowMostFreeNum	   = AllDeviceRes[MapTable_Module[i]].lVocFreeNum;
			}
		}
	}

	for( i = 0; i < AllDeviceRes[s8SearchModID].lVocNum; i++ )
	{
		pOneVoice = &AllDeviceRes[s8SearchModID].pVoice[i];
		if( pOneVoice->State == VOC_FREE )
		{
			*pFreeVocDeviceID = pOneVoice->deviceID;

			// user this voice device
			Change_Voc_State( pOneVoice, VOC_USED );
			AllDeviceRes[s8SearchModID].lVocFreeNum--;
			g_iTotalVoiceFree--;
			draw_count();
			return i;
		}
	}

	return -1;
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
void My_DualLink( DeviceID_t *pDev1, DeviceID_t *pDev2 )
{
	int		retCode = XMS_ctsLinkDevice( g_acsHandle, pDev1, pDev2, NULL );
	char	tempStr[10];
	sprintf( tempStr, "retCode: %d", retCode );
	AddMsg( tempStr );
	XMS_ctsLinkDevice( g_acsHandle, pDev2, pDev1, NULL );
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
void SetGtD_AnalogTrunk( DeviceID_t* pDevId )
{
	//========Set GTG Begin========
	CmdParamData_Voice_t cmdVoc;
	memset( &cmdVoc, 0, sizeof( cmdVoc ) );
	cmdVoc.m_u8GtdCtrlValid					   = 1;                 //Enable GTD
	cmdVoc.m_VocGtdControl.m_u8ChannelEnable   = 1;                 //Enable Gtd channel
	cmdVoc.m_VocGtdControl.m_u8DTMFEnable	   = 1;                 // Detect DTMF
	cmdVoc.m_VocGtdControl.m_u8GTDEnable	   = 1;                 // Detect GTD
	cmdVoc.m_VocGtdControl.m_u8FSKEnable	   = 1;                 // Detect FSK for receive CallerID
	cmdVoc.m_VocGtdControl.m_u8EXTEnable	   = 0x2;               // Enable PVD Detect

	strcpy( (char*)&cmdVoc.m_VocGtdControl.m_u8GTDID[0], "GHIJK" ); // Detect Busy Tone

	DJ_U16	u16ParamType   = VOC_PARAM_UNIPARAM;
	DJ_U16	u16ParamSize   = sizeof( cmdVoc );
	void	* p			   = (void*)&cmdVoc;

	int		r = XMS_ctsSetParam( g_acsHandle, pDevId, u16ParamType, u16ParamSize, (void*)p );
	char	szbuffer[1024];
	memset( szbuffer, 0, sizeof( szbuffer ) );
	sprintf( szbuffer, "Set GTD ret = %d\n", r );
	AddMsg( szbuffer );

	//========Set GTG End  ========
}

// --------------------------------------------------------------------------------
void CloseDeviceOK( DeviceID_t *pDevice )
{
	TRUNK_STRUCT *pOneTrunk;

	AllDeviceRes[pDevice->m_s8ModuleID].bErrFlag = true;

	if( pDevice->m_s16DeviceMain == XMS_DEVMAIN_BOARD )
	{
		AllDeviceRes[pDevice->m_s8ModuleID].bOpenFlag = false;
	}

	if( pDevice->m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
	{
		pOneTrunk = &M_OneTrunk( *pDevice );
		Change_State( pOneTrunk, TRK_WAITOPEN );

		// modify the count
		g_iTotalTrunkOpened--;
		AllDeviceRes[pDevice->m_s8ModuleID].lTrunkOpened--;

	}

	if( pDevice->m_s16DeviceMain == XMS_DEVMAIN_VOICE )
	{
		// init this Device: Voice
		Change_Voc_State( &M_OneVoice( *pDevice ), VOC_WAITOPEN );

		// modify the count
		g_iTotalVoiceOpened--;
		g_iTotalVoiceFree--;
		AllDeviceRes[pDevice->m_s8ModuleID].lVocOpened--;
		AllDeviceRes[pDevice->m_s8ModuleID].lVocFreeNum--;

	}
	draw_count();
}

// -------------------------------------------------------------------------------------------------
void HandleDevState( Acs_Evt_t *pAcsEvt )
{
	TRUNK_STRUCT			*pOneTrunk;
	Acs_GeneralProc_Data	*pGeneralData = NULL;

	pGeneralData = (Acs_GeneralProc_Data *)FetchEventData( pAcsEvt );
	if( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
	{
		pOneTrunk = &M_OneTrunk( pAcsEvt->m_DeviceID );

		pOneTrunk->iLineState = pGeneralData->m_s32DeviceState;
		DrawMain_LineState( pOneTrunk );
		
	}
}

//��ʼ���м�ͨ���豸--------------------------------------------------------
void InitTrunkChannel( TRUNK_STRUCT *pOneTrunk )
{
	memset( &pOneTrunk->VocDevID, 0, sizeof( DeviceID_t ) ); // 0: didn't alloc Voc Device
	pOneTrunk->CallerCode[0]   = 0;
	pOneTrunk->CalledCode[0]   = 0;

	Change_State( pOneTrunk, TRK_FREE );
	My_InitDtmfBuf( pOneTrunk );
	DrawMain_DTMF( pOneTrunk );
	DrawMain_VocInfo( pOneTrunk );
	DrawMain_CallInfo( pOneTrunk );
}

//---------------------------
void ResetTrunk( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	if( pOneTrunk->VocDevID.m_s16DeviceMain != 0 )                                          //�������豸����м�����
	{
		My_DualUnlink( &pOneTrunk->VocDevID, &pOneTrunk->deviceID );
		FreeOneFreeVoice( &pOneTrunk->VocDevID );
		memset( &M_OneVoice( pOneTrunk->VocDevID ).UsedDevID, 0, sizeof( DeviceID_t ) );    // 0: didn't alloc Device
	}
	if( pOneTrunk->LinkDevID.m_s16DeviceMain != 0 )                                         //��ǰ��ϯ�������豸��������
	{
		My_DualUnlink( &pOneTrunk->LinkDevID, &pOneTrunk->deviceID );
	}
	InitTrunkChannel( pOneTrunk );
	// Search Free Voice for get CallerID(FSK)
	// if you needn't CallerID, ignore next line
	PrepareForCallerID( pOneTrunk );
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
void ResetUser( TRUNK_STRUCT *pOneUser, Acs_Evt_t *pAcsEvt )
{
	TRUNK_STRUCT	*pLinkDev  = NULL;
	TRUNK_STRUCT	*pLinkUser = NULL;

	if( pOneUser->VocDevID.m_s16DeviceMain != 0 )                   //��ǰ��ϯ��������Դ��������
	{
		My_DualUnlink( &pOneUser->VocDevID, &pOneUser->deviceID );
		FreeOneFreeVoice( &pOneUser->VocDevID );
	}
	if( pOneUser->LinkDevID.m_s16DeviceMain != 0 )                  //��ǰ��ϯ�������豸��������
	{
		My_DualUnlink( &pOneUser->deviceID, &pOneUser->LinkDevID );
		pLinkDev = &M_OneTrunk( pOneUser->LinkDevID );
		memset( &pLinkDev->LinkDevID, 0, sizeof( DeviceID_t ) );    // 0: Didn't alloc Link Device

		XMS_ctsClearCall( g_acsHandle, &pOneUser->LinkDevID, 0, NULL );
	}

	if( pOneUser->LinkDevID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER )
	{
		if( pLinkDev->State == USER_RING )
		{
			InitUserChannel( pLinkDev );
		}
	}else if( pOneUser->LinkDevID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK )
	{
		AddMsg( "ResetUser() & �Է����豸���� XMS_DEVSUB_ANALOG_TRUNK" );
		if( pLinkDev->State == TRK_CALLOUT )
		{
			InitTrunkChannel( pLinkDev );
		}
	}

	InitUserChannel( pOneUser );
}

//ҵ������--------------------------------------------------------------------
void TrunkWork( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	Acs_CallControl_Data	* pCallControl = NULL;
	DeviceID_t				FreeVocDeviceID;
	DeviceID_t				OutUserDeviceID;
	char					FileName[256];
	char					tmpStr[256];
	char					TmpDtmf, TmpGtd;
	RetCode_t				r;
	TRUNK_STRUCT			*pLinkUser;
	TRUNK_STRUCT			*pLinkDev;


	/*XMS_EVT_CLEARCALL��������: ����XMS_ctsClearCall()���ɹ����͹һ��źź󷵻�
	   �����һ���ɺ󷵻ص��¼�;����Analog Trunk�ڵ���API����XMS_ctsClearCall()�ɹ��󷵻�;
	   ����Analog User�ڵ���API����XMS_ctsClearCall()�ɹ��󣬻��ڼ�⵽�Է��һ�ʱ�򷵻ء�*/
	if( pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL )
	{
		if( ( pOneTrunk->State != TRK_FREE ) && ( pOneTrunk->State != TRK_WAIT_REMOVE ) )
		{
			ResetTrunk( pOneTrunk, pAcsEvt );
			return;
		}
	}
	TmpGtd = My_GetGtdCode( pAcsEvt );

	if( pOneTrunk->State != TRK_CONNECT )
	{
		if( TmpGtd == 'I' || TmpGtd == 'J' || TmpGtd == 'K' )               // Busy Tone
		{
			XMS_ctsResetDevice( g_acsHandle, &pOneTrunk->deviceID, NULL );  //��λ�豸
			XMS_ctsClearCall( g_acsHandle, &pOneTrunk->deviceID, 0, NULL ); //�������
			ResetTrunk( pOneTrunk, pAcsEvt );
		}
	}

	switch( pOneTrunk->State )
	{
		case TRK_FREE:
			SpecialForCas( pOneTrunk, pAcsEvt );    //Special code for CAS(SS1)    XMS_DEVSUB_E1_CAS:E1һ�����·�豸
			                                        //XMS_EVT_CALLIN ��������: ����Analog Trunk�豸��⵽�����¼��󷵻�
			if( pAcsEvt->m_s32EventType == XMS_EVT_CALLIN )
			{
				AddMsg( "TRK_FREE & XMS_EVT_CALLIN..............................." );
				pCallControl = (Acs_CallControl_Data *)FetchEventData( pAcsEvt );

				unsigned char *p;
				p = My_GetFskCode( pAcsEvt );
				if( p != NULL )
				{
					ConvertRawFskToCallerID( p, pOneTrunk->CallerCode, 20 );
				}
				if( pOneTrunk->VocDevID.m_s16DeviceMain != 0 )                                          //������м̹���������Դ
				{
					My_DualUnlink( &pOneTrunk->VocDevID, &pOneTrunk->deviceID );
					FreeOneFreeVoice( &pOneTrunk->VocDevID );
					memset( &M_OneVoice( pOneTrunk->VocDevID ).UsedDevID, 0, sizeof( DeviceID_t ) );    // 0: didn't alloc Device
					memset( &pOneTrunk->VocDevID, 0, sizeof( DeviceID_t ) );                            // 0: didn't alloc Device
					DrawMain_VocInfo( pOneTrunk );
				}

				XMS_ctsAlertCall( g_acsHandle, &pOneTrunk->deviceID, NULL );                            //����
				XMS_ctsAnswerCallIn( g_acsHandle, &pOneTrunk->deviceID, NULL );                         //����Ӧ��

				DrawMain_CallInfo( pOneTrunk );
				Change_State( pOneTrunk, TRK_WAIT_ANSWERCALL );
			}
			break;
		case TRK_WAIT_ANSWERCALL:                                                                       //����Ự�ɹ�������
			SpecialForCas( pOneTrunk, pAcsEvt );                                                        // Special code for CAS(SS1)
			                                                                                            //XMS_EVT_ANSWERCALL ����XMS_ctsAnswerCallIn()���ɹ�ִ�к󷵻ء�
			                                                                                            //����Analog Trunk�����û�����API����XMS_cstAnswerCall()�ɹ���ʱ�򷵻�;����Analog User���ڼ�⵽�Է�ժ��ʱ���ء�
			if( pAcsEvt->m_s32EventType == XMS_EVT_ANSWERCALL )                                         /*Answer Call In End Event*/
			{
				AddMsg( "TRK_WAIT_ANSWERCALL & XMS_EVT_ANSWERCALL �Ự�����ɹ�" );
				pCallControl = (Acs_CallControl_Data *)FetchEventData( pAcsEvt );

				if( SearchOneFreeVoice( pOneTrunk, &FreeVocDeviceID ) >= 0 )
				{
					pOneTrunk->u8PlayTag					   = 16;
					pOneTrunk->VocDevID						   = FreeVocDeviceID;
					M_OneVoice( FreeVocDeviceID ).UsedDevID	   = pOneTrunk->deviceID;
					DrawMain_VocInfo( pOneTrunk );
					My_DualLink( &FreeVocDeviceID, &pOneTrunk->deviceID );
					SetGtD_AnalogTrunk( &FreeVocDeviceID ); // prepare for get Busy Tone
					Change_State( pOneTrunk, TRK_WAIT_LINKOK );
				}
			}
			break;

		case TRK_WAIT_LINKOK:                               //�豸��ʱ϶���ӽ����ɹ�: Analog Trunk, Analog User�����û�����API����XMS_ctsLinkDevice()�ɹ������û�����
			if( pAcsEvt->m_s32EventType == XMS_EVT_LINKDEVICE )
			{
				AddMsg( "TRK_WAIT_LINKOK & XMS_EVT_LINKDEVICE �м�ͨ���豸������ͨ���豸 ʱ϶���ӽ����ɹ�" );
				strcpy( FileName, cfg_VocPath );
				strcat( FileName, "Bank.001" );
				pOneTrunk->u8PlayTag++;
				PlayFile( &pOneTrunk->VocDevID, FileName, pOneTrunk->u8PlayTag, false );
				Change_State( pOneTrunk, TRK_WELCOME );
			}
			break;

		case TRK_WELCOME:
			if( CheckPlayEnd( pOneTrunk, pAcsEvt ) ) /*play end Event*/
			{
				strcpy( FileName, cfg_VocPath );
				strcat( FileName, "Bank.002" );
				pOneTrunk->u8PlayTag++;
				PlayFile( &pOneTrunk->VocDevID, FileName, pOneTrunk->u8PlayTag, false );

				Change_State( pOneTrunk, TRK_EXTNUM );
			}

			TmpDtmf = My_GetDtmfCode( pAcsEvt );
			if( TmpDtmf != -1 ) /*DTMF key*/
			{
				My_AddDtmfBuf( pOneTrunk, TmpDtmf );
				DrawMain_DTMF( pOneTrunk );
				StopPlayFile( &pOneTrunk->VocDevID );
				Change_State( pOneTrunk, TRK_EXTNUM );
			}
			break;

		case TRK_EXTNUM:
			TmpDtmf = My_GetDtmfCode( pAcsEvt );
			if( TmpDtmf != -1 )                                                     /*DTMF Key*/
			{
				My_AddDtmfBuf( pOneTrunk, TmpDtmf );
				DrawMain_DTMF( pOneTrunk );
				StopPlayFile( &pOneTrunk->VocDevID );
				if( pOneTrunk->DtmfCount >= 4 )                                     //������λ�ֻ���
				{
					int i = GetOutUserID( pOneTrunk->DtmfBuf, &OutUserDeviceID );   //�������һ��������ϯ,����ͨ��
					if( i > -1 )
					{
						r = XMS_ctsMakeCallOut( g_acsHandle, &OutUserDeviceID, pOneTrunk->DtmfBuf, "1001", NULL );
						if( r > 0 )
						{
							                                                        //�����߲��ű�������
							strcpy( FileName, cfg_VocPath );
							strcat( FileName, "ConfMusi.Pcm" );
							pOneTrunk->u8PlayTag++;
							PlayFile( &pOneTrunk->VocDevID, FileName, pOneTrunk->u8PlayTag, false );

							                                                        // call out OK
							pLinkUser			   = &M_OneTrunk( OutUserDeviceID );
							pOneTrunk->LinkDevID   = OutUserDeviceID;
							pLinkUser->LinkDevID   = pOneTrunk->deviceID;

							Change_State( pOneTrunk, TRK_WAITCONNECT );
							Change_State( pLinkUser, USER_RING );
							My_InitDtmfBuf( pOneTrunk );
						}else
						{
							AddMsg( "TRK_EXTNUM & XMS_ctsMakeCallOut() fail..." );
							Change_State( pOneTrunk, TRK_CONNECT_FAILURE );
						}
					}else
					{
						AddMsg( "TRK_EXTNUM & δ�ҵ����е���ϯ��Դ" );
						Change_State( pOneTrunk, TRK_CONNECT_FAILURE );
					}
				}
			}
			break;
		case TRK_WAITCONNECT:
			break;
		case TRK_CONNECT_FAILURE:
			break;
		case TRK_CALLOUT:

			if( TmpGtd == 'h' || TmpGtd == 'P' ) //�յ� ����������h�����߱�����P
			{
				pLinkDev = &M_OneTrunk( pOneTrunk->LinkDevID );
				Change_State( pOneTrunk, TRK_CONNECT );
				if( pLinkDev->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER )
				{
					Change_State( pLinkDev, USER_CONNECT );
				}
			}
			if( pAcsEvt->m_s32EventType == XMS_EVT_ANALOG_INTERFACE )
			{
				sprintf( tmpStr, "A_TRK(%d,%d) û�н���!", pAcsEvt->m_DeviceID.m_s8ModuleID, pAcsEvt->m_DeviceID.m_s16ChannelID );
				AddMsg( tmpStr );
			}
			break;
		case TRK_CONNECT:
			if( TmpGtd == 'I' || TmpGtd == 'J' || TmpGtd == 'K' ) // Busy Tone
			{
//              XMS_ctsResetDevice(g_acsHandle,&pOneTrunk->deviceID,NULL);//��λ�豸
//              XMS_ctsClearCall(g_acsHandle,&pOneTrunk->deviceID,0,NULL);//�������
//              ResetTrunk(pOneTrunk,pAcsEvt);
				pLinkDev = &M_OneTrunk( pOneTrunk->LinkDevID );
				if( pLinkDev->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER )
				{
					if( pLinkDev->VocDevID.m_s16DeviceMain != 0 ) //��ϯ�Ƿ�������Voc
					{
						AddMsg( "USER_CONNECT & XMS_EVT_CLEARCALL Event  & ��������Դ����" );
					}else
					{
						if( SearchOneFreeVoice( pLinkDev, &FreeVocDeviceID ) >= 0 )
						{
							pLinkDev->VocDevID						   = FreeVocDeviceID;
							M_OneVoice( FreeVocDeviceID ).UsedDevID	   = pLinkDev->deviceID;
							My_DualLink( &pLinkDev->VocDevID, &pLinkDev->deviceID );
						}
					}
					PlayTone( &pLinkDev->VocDevID, 2 ); // busy tone
					Change_State( pLinkDev, USER_CONNECT_FAILURE );
					//DrawUser_FailReason( pLinkDev, "PEER ONHOOK" );
				}
			}
			break;
		default:
			break;
	}
}

/* Stop Play */
DJ_S32 StopPlayFile( DeviceID_t *pVocDevID )
{
	RetCode_t		r;
	ControlPlay_t	controlPlay;
	controlPlay.m_u16ControlType   = XMS_STOP_PLAY;
	r							   = XMS_ctsControlPlay( g_acsHandle, pVocDevID, &controlPlay, NULL );
	return r;
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
void My_DualUnlink( DeviceID_t *pDev1, DeviceID_t *pDev2 )
{
	XMS_ctsUnlinkDevice( g_acsHandle, pDev1, pDev2, NULL );
	XMS_ctsUnlinkDevice( g_acsHandle, pDev2, pDev1, NULL );
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
int FreeOneFreeVoice( DeviceID_t *pFreeVocDeviceID )
{
	DJ_S8 s8ModID;

	s8ModID = pFreeVocDeviceID->m_s8ModuleID;
	if( AllDeviceRes[s8ModID].lFlag == 1 )
	{
		Change_Voc_State( &M_OneVoice( *pFreeVocDeviceID ), VOC_FREE );

		AllDeviceRes[s8ModID].lVocFreeNum++;
		g_iTotalVoiceFree++;
		draw_count();
		return 0;   // OK
	}
	return -1;      // invalid VocDeviceID
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
char My_GetGtdCode( Acs_Evt_t *pAcsEvt )
{
	Acs_IO_Data *pIOData = NULL;
	char		sbuffer[200];
	char		*p;

	memset( sbuffer, 0, sizeof( sbuffer ) );
	if( pAcsEvt->m_s32EventType == XMS_EVT_RECVIODATA ) /*IO Data Event*/
	{
		pIOData = (Acs_IO_Data *)FetchEventData( pAcsEvt );
		if( ( pIOData->m_u16IoType == XMS_IO_TYPE_GTG ) && ( pIOData->m_u16IoDataLen > 0 ) )
		{
			p = (char *)FetchIOData( pAcsEvt );
			sprintf( sbuffer, "My_GetGtdCode: [%s] ", p );
			AddMsg( sbuffer );
			return *p;
		}
	}
	return -1; // not a good GTD
}

// Special code for CAS(SS1)   XMS_DEVSUB_E1_CAS:E1һ�����·�豸
void SpecialForCas( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	DeviceID_t FreeVocDeviceID;
	if( pOneTrunk->deviceID.m_s16DeviceSub != XMS_DEVSUB_E1_CAS )
	{
		return;
	}
	if( pAcsEvt->m_s32EventType == XMS_EVT_CAS_MFC_START ) /* start receive MFC, need a Voice Device */
	{
		if( SearchOneFreeVoice( pOneTrunk, &FreeVocDeviceID ) >= 0 )
		{
			// add next line from V2.2.0, link by Application Program
			My_DualLink( &FreeVocDeviceID, &pOneTrunk->deviceID );

			pOneTrunk->VocDevID						   = FreeVocDeviceID;
			M_OneVoice( FreeVocDeviceID ).UsedDevID	   = pOneTrunk->deviceID;
			DrawMain_VocInfo( pOneTrunk );

			g_Param_CAS.m_VocDevID = FreeVocDeviceID;
			XMS_ctsSetParam( g_acsHandle, &pOneTrunk->deviceID,
			                 CAS_PARAM_UNIPARAM, sizeof( CmdParamData_CAS_t ), (DJ_Void *)&g_Param_CAS );
		}
	}

	if( pAcsEvt->m_s32EventType == XMS_EVT_CAS_MFC_END )                                    /* receive MFC end, release Voice Device */
	{
		// add next line from V2.2.0, unlink by Application Program
		My_DualUnlink( &pOneTrunk->VocDevID, &pOneTrunk->deviceID );

		FreeOneFreeVoice( &pOneTrunk->VocDevID );

		memset( &M_OneVoice( pOneTrunk->VocDevID ).UsedDevID, 0, sizeof( DeviceID_t ) );    // 0: didn't alloc Voc Device
		memset( &pOneTrunk->VocDevID, 0, sizeof( DeviceID_t ) );                            // 0: didn't alloc Voc Device
		DrawMain_VocInfo( pOneTrunk );
	}
}

// end of code for CAS(SS1)
unsigned char *My_GetFskCode( Acs_Evt_t *pAcsEvt )
{
	Acs_IO_Data		*pIOData = NULL;
	unsigned char	*p;

	if( pAcsEvt->m_s32EventType == XMS_EVT_RECVIODATA ) /*IO Data Evemt*/
	{
		pIOData = (Acs_IO_Data *)FetchEventData( pAcsEvt );

		if( ( pIOData->m_u16IoType == XMS_IO_TYPE_FSK ) && ( pIOData->m_u16IoDataLen > 0 ) )
		{
			p = (unsigned char *)FetchIOData( pAcsEvt );
			return p;
		}
	}

	return NULL; // not a good FSK
}

/* Handle Play */
DJ_S32  PlayFile( DeviceID_t *pVocDevID, DJ_S8 *s8FileName, DJ_U8 u8PlayTag, bool bIsQueue = false )
{
	DJ_U32			i = 0;
	PlayProperty_t	playProperty;
	RetCode_t		r;

	memset( &playProperty, 0, sizeof( playProperty ) );

	if( bIsQueue )
	{
		playProperty.m_u16PlayType = XMS_PLAY_TYPE_FILE_QUEUE;
	} else
	{
		playProperty.m_u16PlayType = XMS_PLAY_TYPE_FILE;
	}

	playProperty.m_u8TaskID = u8PlayTag;
	//playProperty.m_u32StopMode =
	strcpy( playProperty.m_s8PlayContent, s8FileName );

	r = XMS_ctsPlay( g_acsHandle, pVocDevID, &playProperty, NULL );

	return r;
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
DJ_S32 PlayTone( DeviceID_t *pVocDevID, int iPlayType )
{
	DJ_U32		i		   = 0;
	DJ_U16		u16IoLen   = 0;
	DJ_U16		u16IoType  = 0;
	RetCode_t	r;
	char		IoDataBuf[MAX_SEND_IODATA_DTMF_LEN] = { 0 };

	u16IoType  = XMS_IO_TYPE_GTG;
	u16IoLen   = 1;

	switch( iPlayType )
	{
		case -1:    // Stop Play Tone
			u16IoLen = 0;
			break;
		case 0:     // Dial Tone
			IoDataBuf[0] = 'G';
			break;
		case 1:     // Ring Back Tone
			IoDataBuf[0] = 'H';
			break;
		case 2:     // Busy Tone
			IoDataBuf[0] = 'I';
			break;
		case 3:     // Busy Tone
			IoDataBuf[0] = 'J';
			break;
		case 4:     // Busy Tone
			IoDataBuf[0] = 'K';
			break;
	}

	//����DTMF/TONE��/FSK/����
	r = XMS_ctsSendIOData( g_acsHandle, pVocDevID, u16IoType, u16IoLen, IoDataBuf );
	return r;
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
void My_AddDtmfBuf( TRUNK_STRUCT *pOneTrunk, char TmpDtmf )
{
	if( pOneTrunk->DtmfCount < ( 32 - 1 ) )
	{
		pOneTrunk->DtmfBuf[pOneTrunk->DtmfCount]	   = TmpDtmf;
		pOneTrunk->DtmfBuf[pOneTrunk->DtmfCount + 1]   = 0;
		pOneTrunk->DtmfCount++;
	}
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
char My_GetDtmfCode( Acs_Evt_t *pAcsEvt )
{
	Acs_IO_Data *pIOData = NULL;
	char		sbuffer[200];
	char		*p;

	memset( sbuffer, 0, sizeof( sbuffer ) );
	if( pAcsEvt->m_s32EventType == XMS_EVT_RECVIODATA ) /*IO Data Event*/
	{
		pIOData = (Acs_IO_Data *)FetchEventData( pAcsEvt );

		if( ( pIOData->m_u16IoType == XMS_IO_TYPE_DTMF )
		    && ( pIOData->m_u16IoDataLen > 0 ) )
		{
			p = (char *)FetchIOData( pAcsEvt );
			sprintf( sbuffer, "[%s]", p );
			AddMsg( sbuffer );
			sprintf( sbuffer, "Recevice DTMF" );
			AddMsg( sbuffer );

			return *p;
		}
	}

	return -1; // not a good DTMF
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
bool CheckPlayEnd( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt )
{
	Acs_MediaProc_Data *pMediaData = NULL;

	if( pAcsEvt->m_s32EventType == XMS_EVT_PLAY )
	{
		pMediaData = (Acs_MediaProc_Data *)FetchEventData( pAcsEvt );
		if( pMediaData->m_u8TaskID == pOneTrunk->u8PlayTag )
		{
			return true;
		}
	}

	return false;
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
void InitUserChannel( TRUNK_STRUCT *pOneUser )
{
	Change_State( pOneUser, USER_FREE );
	My_InitDtmfBuf( pOneUser );
	memset( &pOneUser->VocDevID, 0, sizeof( DeviceID_t ) );     // 0: didn't alloc Voc Device
	memset( &pOneUser->LinkDevID, 0, sizeof( DeviceID_t ) );    // 0: didn't alloc Link Device
//	DrawUser_FailReason( pOneUser, " " );
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
void UserWork( TRUNK_STRUCT *pOneUser, Acs_Evt_t *pAcsEvt )
{
	char					TmpDtmf;
	char					tmpStr[100];
	DeviceID_t				FreeVocDeviceID, FreeTrkDeviceID, OutUserDeviceID;
	TRUNK_STRUCT			*pLinkTrunk	   = NULL;
	TRUNK_STRUCT			*pLinkUser	   = NULL;
	TRUNK_STRUCT			*pLinkDev	   = NULL;
	Acs_GeneralProc_Data	*pGeneral	   = NULL;
	Acs_CallControl_Data	*pCallControl  = NULL;

	if( pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL ) /*��ǰ״̬USER_RING,��ϯ�һ��¼�User Offhook Event*/
	{
		if( pOneUser->State != USER_FREE && pOneUser->State != USER_CONNECT )
		{
			ResetUser( pOneUser, pAcsEvt );
			return;
		}
	}
	switch( pOneUser->State )
	{
		case USER_FREE:
			if( pAcsEvt->m_s32EventType == XMS_EVT_CALLIN ) /*��ǰ״̬USER_FREE,��ϯժ���¼�User Offhook Event*/
			{
				sprintf(tmpStr,"User(%d,%d) ��ǰ״̬USER_FREE,��ϯժ���¼�User Offhook Event",
					pOneUser->deviceID.m_s8ModuleID, pOneUser->deviceID.m_s16ChannelID);
				AddMsg( tmpStr );
				if( SearchOneFreeVoice( pOneUser, &FreeVocDeviceID ) >= 0 )
				{
					pOneUser->VocDevID						   = FreeVocDeviceID;
					M_OneVoice( FreeVocDeviceID ).UsedDevID	   = pOneUser->deviceID;
					DrawMain_VocInfo( pOneUser );
					My_DualLink( &FreeVocDeviceID, &pOneUser->deviceID );
					if( cfg_iCallOutRule == 2 )                                                                                     // ��ϯ����ϯ
					{
						PlayTone( &pOneUser->VocDevID, 0 );                                                                         // dial tone
						Change_State( pOneUser, USER_OFFHOOK );
					}else if( cfg_iCallOutRule == 3 )                                                                               //��ϯ������
					{
						if( SearchOneFreeTrunk( &FreeTrkDeviceID ) > 0 )                                                            //�ҵ�����ģ���м�ͨ��
						{
							if( pOneUser->VocDevID.m_s16DeviceMain != 0 )
							{
								My_DualUnlink( &pOneUser->deviceID, &pOneUser->VocDevID );                                          //�Ͽ���ϯ�������豸����
								FreeOneFreeVoice( &pOneUser->VocDevID );                                                            //�ͷ�������Դ
							}

							My_DualLink( &FreeTrkDeviceID, &pOneUser->deviceID );                                                   //��ϯ���ҵ��Ŀ����м̽�������,SendIODataͨ����ϯ�绰����
							if( XMS_ctsMakeCallOut( g_acsHandle, &FreeTrkDeviceID, cfg_CallingNum, pOneUser->DtmfBuf, NULL ) > 0 )  //call out OK
							{
								pLinkTrunk			   = &M_OneTrunk( FreeTrkDeviceID );
								pOneUser->LinkDevID	   = FreeTrkDeviceID;
								pLinkTrunk->LinkDevID  = pOneUser->deviceID;
//								DrawUser_LinkDev( pOneUser );

								strcpy( pLinkTrunk->CallerCode, cfg_CallingNum );
								DrawMain_CallInfo( pLinkTrunk );

								Change_State( pOneUser, USER_CALLOUT );
								Change_State( pLinkTrunk, TRK_CALLOUT );
							}else //call out fail
							{
								AddMsg( "��ϯ��������ʧ�� XMS_ctsMakeCallOut() FAIL!" );
								Change_State( pOneUser, USER_CONNECT_FAILURE );
							}
						}else
						{ //�޷��ҵ������м�
							AddMsg( "USER_OFFHOOK & SearchOneFreeTrunk() FAIL! �޷��ҵ������м�" );
							PlayTone( &pOneUser->VocDevID, 2 ); // busy tone
							Change_State( pOneUser, USER_CONNECT_FAILURE );
						} // end of For Link User & Trunk
					}
				}else
				{
					AddMsg( "USER_FREE & SearchOneFreeVoice() Fail..." );
				}
			}else
			{
				AddMsg( "USER_FREE & not XMS_EVT_CALLIN Event" );
			}
			break;

		case USER_OFFHOOK:
			TmpDtmf = My_GetDtmfCode( pAcsEvt );
			AddMsg( "USER_OFFHOOK....................." );
			if( TmpDtmf != -1 )                                                                                                     /*DTMF Event*/
			{
				AddMsg( "USER_OFFHOOK DTMF Event............................." );
				PlayTone( &pOneUser->VocDevID, -1 );                                                                                //ֹͣ����
				My_AddDtmfBuf( pOneUser, TmpDtmf );
				DrawMain_DTMF( pOneUser );
				if( pOneUser->DtmfCount >= cfg_iCalledLen )                                                                         //������벻�ܶ�����λ
				{
					if( cfg_iCallOutRule == 2 )                                                                                     // ��ϯ����ϯ
					{
						AddMsg( "��ϯ����ϯ........." );
						if( GetOutUserID( pOneUser->DtmfBuf, &OutUserDeviceID ) > -1 )                                              //�������һ��������ϯ,����ͨ��
						{
							if( XMS_ctsMakeCallOut( g_acsHandle, &OutUserDeviceID, cfg_CallingNum, pOneUser->DtmfBuf, NULL ) > 0 )  // call out OK
							{
								PlayTone( &pOneUser->VocDevID, 1 );                                                                 // RingBack tone
								pLinkUser			   = &M_OneTrunk( OutUserDeviceID );
								pOneUser->LinkDevID	   = OutUserDeviceID;
								pLinkUser->LinkDevID   = pOneUser->deviceID;
//								DrawUser_LinkDev( pOneUser );
	//							DrawUser_LinkDev( pLinkUser );
								Change_State( pOneUser, USER_CALLOUT );
								Change_State( pLinkUser, USER_RING );
								break;
							}else   //��ϯ����ϯ����ʧ��
							{
								AddMsg( "��ϯ����ϯʧ��  XMS_ctsMakeCallOut() FAIL!" );
								PlayTone( &pOneUser->VocDevID, 2 ); // busy tone
								Change_State( pOneUser, USER_CONNECT_FAILURE );
							}
						}else       //�޷��ҵ�������ϯ
						{
							AddMsg( "USER_OFFHOOK & GetOutUserID() FAIL �޷��ҵ�������ϯ" );
							PlayTone( &pOneUser->VocDevID, 2 ); // busy tone
							Change_State( pOneUser, USER_CONNECT_FAILURE );
						}           // end of For Link User & User
					}else
					{
						AddMsg( "�����ļ���û�ж���� ���й���" );
					}
				}
			}
			break;
		case USER_CALLOUT:
			AddMsg( "USER_CALLOUT & XMS_EVT_CLEARCALL Event............................." );
			break;
		case USER_CONNECT:                                                      //�����һ��¼�����ϯ���������ӵ���ϯ������æ��
			if( pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL )                  /*��ǰ״̬USER_RING,��ϯ�һ��¼�User Offhook Event*/
			{
				AddMsg( "USER_CONNECT & XMS_EVT_CLEARCALL Event............................." );
				pLinkDev = &M_OneTrunk( pOneUser->LinkDevID );
				if( pLinkDev->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER )
				{
					My_DualUnlink( &pOneUser->deviceID, &pLinkDev->deviceID );  //�Ͽ���ϯ��˫������
					if( pLinkDev->VocDevID.m_s16DeviceMain != 0 )               //��ϯ�Ƿ�������Voc
					{
						AddMsg( "USER_CONNECT & XMS_EVT_CLEARCALL Event  & ��������Դ����" );
					}else
					{
						if( SearchOneFreeVoice( pLinkDev, &FreeVocDeviceID ) >= 0 )
						{
							pLinkDev->VocDevID						   = FreeVocDeviceID;
							M_OneVoice( FreeVocDeviceID ).UsedDevID	   = pLinkDev->deviceID;
							My_DualLink( &pLinkDev->VocDevID, &pLinkDev->deviceID );
						}
					}
					PlayTone( &pLinkDev->VocDevID, 2 );             // busy tone
					Change_State( pOneUser, USER_CONNECT_FAILURE );
					Change_State( pLinkDev, USER_CONNECT_FAILURE );
					//DrawUser_FailReason( pLinkDev, "PEER ONHOOK" );

					if( pOneUser->VocDevID.m_s16DeviceMain != 0 )   //��ǰ��ϯ��������Դ��������
					{
						My_DualUnlink( &pOneUser->VocDevID, &pOneUser->deviceID );
						FreeOneFreeVoice( &pOneUser->VocDevID );
						DrawMain_VocInfo( pOneUser );
					}
					InitUserChannel( pOneUser );
				}else if( pLinkDev->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK )
				{
					My_DualUnlink( &pOneUser->deviceID, &pLinkDev->deviceID );  //�Ͽ���ϯ���м̼�˫������
					if( pLinkDev->VocDevID.m_s16DeviceMain != 0 )               //�м��Ƿ�����Voc
					{
						AddMsg( "USER_CONNECT & XMS_EVT_CLEARCALL Event  & ��������Դ����" );
					}else
					{
						if( SearchOneFreeVoice( pLinkDev, &FreeVocDeviceID ) >= 0 )
						{
							pLinkDev->VocDevID						   = FreeVocDeviceID;
							M_OneVoice( FreeVocDeviceID ).UsedDevID	   = pLinkDev->deviceID;
							My_DualLink( &pLinkDev->VocDevID, &pLinkDev->deviceID );
						}
					}
					ResetUser( pOneUser, pAcsEvt );
					XMS_ctsClearCall( g_acsHandle, &pLinkDev->deviceID, 0, NULL );

					Change_State( pLinkDev, TRK_CONNECT_FAILURE );
				}
			}
			break;

		// User to User's call
		case USER_RING:
			if( pAcsEvt->m_s32EventType == XMS_EVT_CALLOUT )
			{
				pCallControl   = (Acs_CallControl_Data *)FetchEventData( pAcsEvt );     //�������ݻ�ȡ
				pLinkDev	   = &M_OneTrunk( pOneUser->LinkDevID );
				if( pLinkDev->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER )       //��ϯ���д���.....................
				{
					if( pCallControl->m_s32AcsEvtState == 1 )                           //���гɹ�����ϯժ��
					{
						AddMsg( "USER_RING & XMS_EVT_CALLOUT Event............................." );
						Change_State( pOneUser, USER_OFFHOOK );
						pLinkUser = &M_OneTrunk( pOneUser->LinkDevID );
						if( pLinkUser->VocDevID.m_s16DeviceMain != 0 )                  //������ϯ�����豸����
						{
							PlayTone( &pLinkUser->VocDevID, -1 );                       //stop playtone wait tone
							My_DualUnlink( &pLinkUser->VocDevID, &pLinkUser->deviceID );
							FreeOneFreeVoice( &pLinkUser->VocDevID );
							memset( &pLinkUser->VocDevID, 0, sizeof( DeviceID_t ) );    // 0: Didn't alloc Voc Device
							DrawMain_VocInfo( pLinkUser );                              //���ӵ��豸����ϯ���ı�VocInfo
						}
						My_DualLink( &pOneUser->deviceID, &pLinkUser->deviceID );       //����ʱ϶����
						Change_State( pOneUser, USER_CONNECT );
						Change_State( pLinkUser, USER_CONNECT );                    //��������豸Ϊ��ϯ���ı�״̬
					}else //����ʧ��

					{
						AddMsg( "USER_RING & ����ʧ��ԭ��" );
						char errMsg[100];
						sprintf( errMsg, "Err=%d,Cause=%d", pCallControl->m_s32AcsEvtErrCode, pCallControl->m_s32CallClearCause );
						AddMsg( errMsg );
						pLinkUser = &M_OneTrunk( pOneUser->LinkDevID );
						Change_State( pOneUser, USER_CONNECT_FAILURE );
						Change_State( pLinkUser, USER_CONNECT_FAILURE );
						//DrawUser_FailReason( pLinkUser, "Peer Nobody" );
						My_DualUnlink( &pOneUser->deviceID, &pLinkUser->deviceID ); //�Ͽ���ϯ��˫������

						                                                            //???????????????????????????? �Ѿ���������Դ��������ϯ�����˰�
						if( pLinkUser->VocDevID.m_s16DeviceMain != 0 )
						{
							AddMsg( "����ϯ�Ѿ��������豸����������������" );
							PlayTone( &pLinkUser->VocDevID, 2 );
						}else if( SearchOneFreeVoice( pLinkUser, &FreeVocDeviceID ) >= 0 )
						{
							pLinkUser->VocDevID						   = FreeVocDeviceID;
							M_OneVoice( FreeVocDeviceID ).UsedDevID	   = pLinkUser->deviceID;
							My_DualLink( &pLinkUser->VocDevID, &pLinkUser->deviceID );
							PlayTone( &pLinkUser->VocDevID, 2 );
						}else
						{
							AddMsg( "�޿���������Դ" );
						}
					} // ��ϯ���д������...........................
				}else if( pLinkDev->deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK ) //�м̺��д���ʼ......................
				{
					if( pCallControl->m_s32AcsEvtState == 1 ) //���гɹ�����ϯժ��
					{
						AddMsg( "USER_RING & XMS_EVT_CALLOUT Event............................." );
						Change_State( pOneUser, USER_OFFHOOK );
						pLinkTrunk = &M_OneTrunk( pOneUser->LinkDevID );

						PlayTone( &pLinkTrunk->VocDevID, -1 );                          //stop playtone wait tone
						if( pLinkTrunk->VocDevID.m_s16DeviceMain != 0 )
						{
							My_DualUnlink( &pLinkTrunk->VocDevID, &pLinkTrunk->deviceID );
							FreeOneFreeVoice( &pLinkTrunk->VocDevID );
							memset( &pLinkTrunk->VocDevID, 0, sizeof( DeviceID_t ) );   // 0: Didn't alloc Voc Device
							DrawMain_VocInfo( pLinkTrunk );                             //���ӵ��豸���м̣��ı�VocInfo
						}
						My_DualLink( &pOneUser->deviceID, &pLinkTrunk->deviceID );      //����ʱ϶����
						Change_State( pOneUser, USER_CONNECT );
						Change_State( pLinkTrunk, TRK_CONNECT );                        //��������豸Ϊ��ϯ���ı�״̬
					}else //����ʧ��
					{
						AddMsg( "USER_RING & XMS_EVT_CALLOUT Event connect fail............................." );
						pLinkTrunk = &M_OneTrunk( pOneUser->LinkDevID );
						Change_State( pOneUser, USER_CONNECT_FAILURE );
						Change_State( pLinkTrunk, USER_CONNECT_FAILURE );
						My_DualUnlink( &pLinkTrunk->deviceID, &pLinkUser->deviceID );   //�Ͽ���ϯ��˫������

						                                                                //???????????????????????????? �Ѿ���������Դ��������ϯ�����˰�
						if( SearchOneFreeVoice( pLinkTrunk, &FreeVocDeviceID ) >= 0 )
						{
							pLinkTrunk->VocDevID					   = FreeVocDeviceID;
							M_OneVoice( FreeVocDeviceID ).UsedDevID	   = pLinkTrunk->deviceID;
							My_DualLink( &pLinkTrunk->VocDevID, &pLinkTrunk->deviceID );
							PlayTone( &pLinkTrunk->VocDevID, 2 );
						}else
						{
							AddMsg( "�޿���������Դ" );
						}
					} //�м̺��д������
				}else
				{
					AddMsg( "���豸��������" );
				}
			}else
			{
				AddMsg( "USER_RING & not XMS_EVT_CALLOUT event" );
			}
			break;
		default:
			break;
	}
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
int  GetOutUserID( char *pDtmf, DeviceID_t *pUserDeviceID )
{
	int			iTmp;
	DJ_S8		s8ModID;
	DJ_S16		s16ChID;
	static int	iLoopStart = 0;

	if( cfg_iCallOutRule == 2 || cfg_iCallOutRule == 3 ) // Select a Fix Channel by DTMF's tail 3
	{
		iTmp = 0;
		sscanf( pDtmf, "%d", &iTmp );
		iTmp %= 1000;

		s8ModID	   = MapTable_Trunk[iTmp].m_s8ModuleID;
		s16ChID	   = MapTable_Trunk[iTmp].m_s16ChannelID;

		if( iTmp >= g_iTotalTrunk )
		{
			return -1;
		}

		if( ( AllDeviceRes[s8ModID].pTrunk[s16ChID].iLineState == DCS_FREE )
		    && ( AllDeviceRes[s8ModID].pTrunk[s16ChID].deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER ) )
		{
			*pUserDeviceID = AllDeviceRes[s8ModID].pTrunk[s16ChID].deviceID;
			return iTmp;
		}
		return -1;
	}

	return -1;
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
int  SearchOneFreeTrunk( DeviceID_t *pFreeTrkDeviceID )
{
	DJ_S8	s8ModID;
	DJ_S16	s16ChID;
	for( int i = 0; i < g_iTotalTrunk; i++ )
	{
		s8ModID	   = MapTable_Trunk[i].m_s8ModuleID;
		s16ChID	   = MapTable_Trunk[i].m_s16ChannelID;
		if( ( AllDeviceRes[s8ModID].pTrunk[s16ChID].iLineState == DCS_FREE )
		    && ( AllDeviceRes[s8ModID].pTrunk[s16ChID].deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK ) )
		{
			*pFreeTrkDeviceID = AllDeviceRes[s8ModID].pTrunk[s16ChID].deviceID;
			return 2;
		}
	}
	return -1;
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
bool IsTrunk( DJ_S16 s16DevSub )
{
	if( ( s16DevSub == XMS_DEVSUB_E1_PCM )
	    || ( s16DevSub == XMS_DEVSUB_E1_CAS )
	    || ( s16DevSub == XMS_DEVSUB_E1_DSS1 )
	    || ( s16DevSub == XMS_DEVSUB_E1_SS7_TUP )
	    || ( s16DevSub == XMS_DEVSUB_E1_SS7_ISUP )
	    || ( s16DevSub == XMS_DEVSUB_ANALOG_TRUNK )
	    )
	{
		return true;
	}
	return false;
}


/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
void WriteToConfig( void )
{
	/*char TmpStr[256];

	WritePrivateProfileString( "ConfigInfo", "IpAddr", cfg_ServerID.m_s8ServerIp, cfg_IniName );

	sprintf( TmpStr, "%d", cfg_ServerID.m_u32ServerPort );
	WritePrivateProfileString( "ConfigInfo", "Port", TmpStr, cfg_IniName );

	sprintf( TmpStr, "%d", cfg_iCalledLen );
	WritePrivateProfileString( "ConfigInfo", "CalledLen", TmpStr, cfg_IniName );

	WritePrivateProfileString( "ConfigInfo", "CallingNum", cfg_CallingNum, cfg_IniName );

	sprintf( TmpStr, "%d", cfg_iCallOutRule );
	WritePrivateProfileString( "ConfigInfo", "CallOutRule", TmpStr, cfg_IniName );

	WritePrivateProfileString( "ConfigInfo", "SimCalledRNum", cfg_CallingNum, cfg_IniName );

	sprintf( TmpStr, "%d", cfg_iPartWork );
	WritePrivateProfileString( "ConfigInfo", "PartWork", TmpStr, cfg_IniName );

	sprintf( TmpStr, "%s", cfg_VocPath );
	WritePrivateProfileString( "ConfigInfo", "VocPath", TmpStr, cfg_IniName );

	sprintf( TmpStr, "%d", cfg_iPartWorkModuleID );
	WritePrivateProfileString( "ConfigInfo", "PartWorkModuleID", TmpStr, cfg_IniName );

	sprintf( TmpStr, "%d", cfg_s32DebugOn );
	WritePrivateProfileString( "ConfigInfo", "DebugOn", TmpStr, cfg_IniName );
*/}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
void ExitSystem( void )
{
	RetCode_t r;


	/*
	   // close all device
	   for ( i = 0; i < g_iTotalModule; i ++ )
	   {
	   CloseAllDevice_Dsp ( MapTable_Module[i] );
	   }
	 */

	r = XMS_acsCloseStream( g_acsHandle, NULL );

//	FreeAllDeviceRes ();

	// save to "XMS_Dial.INI"
	WriteToConfig( );
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
int main( int argc, char** argv )
{
	printf( "XMS_Switch_Demo Program.\n" );

	InitSystem( );

	mainWork( );

	return 0;
}

/************************************** The End Of File **************************************/

