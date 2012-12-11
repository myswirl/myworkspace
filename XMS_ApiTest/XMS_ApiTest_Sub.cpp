/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// 文件名
 * Author:			// 作者
 * Date:			// 日期
 * Description:		// 模块描述
 * Version:			// 版本信息
 * History:			// 历史修改记录
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#include <stdio.h>
#include "XMS_ApiTest_Sub.h"
#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"
#include "DJAcsDevState.h"
#include "LogFile.h"
#include "tools.h"
#include "CAS_Common_Cfg.h"
#include "Analog_Common_Code.h"

TYPE_ANALOG_GTD_PARAM	g_Param_Analog;
CmdParamData_CAS_t		g_Param_CAS;
ACSHandle_t				g_acsHandle = -1;
int		TotalDevRes = 0;
TYPE_MY_DEVICE_STRUCT	*pDevRes = NULL;

TIni					g_cfg;
ServerID_t				cfg_ServerID;
char					cfg_IniName[] = "./XMS_ApiTest.ini";
int						cfg_s32DebugOn;
int						cfg_s32AppUnitID;


/***********************************************************
* Function:       // ReadFromConfig
* Description:    // 读取ini文件信息
***********************************************************/
void ReadFromConfig( void )
{
	g_cfg.OpenIniFile( cfg_IniName );

	read_string( g_cfg, "ConfigInfo", "IpAddr", cfg_ServerID.m_s8ServerIp, sizeof( cfg_ServerID.m_s8ServerIp ) );
	read_int( g_cfg, "ConfigInfo", "Port", ( int & )cfg_ServerID.m_u32ServerPort );
	read_string( g_cfg, "ConfigInfo", "UserName", cfg_ServerID.m_s8UserName, sizeof( cfg_ServerID.m_s8UserName ) );
	read_string( g_cfg, "ConfigInfo", "PassWord", cfg_ServerID.m_s8UserPwd, sizeof( cfg_ServerID.m_s8UserPwd ) );
	read_int( g_cfg, "ConfigInfo", "DebugOn", ( int & )cfg_s32DebugOn );
	read_int( g_cfg, "ConfigInfo", "AppUnitID", ( int & )cfg_s32AppUnitID );
}

/***********************************************************
* Function:       // 函数名称
* Description:    // 函数功能、性能等的描述
* Input:          // 1.输入参数1，说明，包括每个参数的作用、取值说明及参数间关系
* Input:          // 2.输入参数2，说明，包括每个参数的作用、取值说明及参数间关系
* Output:         // 1.输出参数1，说明
* Return:         // 函数返回值的说明
* Others:         // 其它说明
***********************************************************/
char * GetString_EventType( EventType_t EvtType )
{
	static char TmpStr[40];

	switch( EvtType )
	{
		case XMS_EVT_OPEN_STREAM:  return "OPEN_STREAM";
		case XMS_EVT_QUERY_DEVICE:  return "QUERY_DEVICE";
		case XMS_EVT_QUERY_DEVICE_END: return "QUERY_DEVICE_END";
		case XMS_EVT_OPEN_DEVICE:  return "OPEN_DEVICE";
		case XMS_EVT_CLOSE_DEVICE:  return "CLOSE_DEVICE";
		case XMS_EVT_RESET_DEVICE:  return "RESET_DEVICE";
		case XMS_EVT_DEVICESTATE:  return "DEVICESTATE";
		case XMS_EVT_E1STATE:           return "E1STATE";
		case XMS_EVT_CTBUS_CLK_REF_SRC_STATE:  return "E1_CLK_STATE";
		case XMS_EVT_SETDEV_GROUP:  return "SETDEV_GROUP";
		case XMS_EVT_SETPARAM:   return "SETPARAM";
		case XMS_EVT_GETPARAM:   return "GETPARAM";
		case XMS_EVT_QUERY_ONE_DSP_START: return "QUERY_ONE_DSP_START";
		case XMS_EVT_QUERY_ONE_DSP_END:  return "QUERY_ONE_DSP_END";
		case XMS_EVT_QUERY_REMOVE_ONE_DSP_START: return "QUERY_REMOVE_ONE_DSP_START";
		case XMS_EVT_QUERY_REMOVE_ONE_DSP_END:  return "QUERY_REMOVE_ONE_DSP_END";
		case XMS_EVT_OPEN_STREAM_EXT: return "OPEN_STREAM_EXT";

		case XMS_EVT_CALLOUT:   return "CALLOUT";
		case XMS_EVT_CALLIN:   return "CALLIN";
		case XMS_EVT_ALERTCALL:   return "ALERTCALL";
		case XMS_EVT_ANSWERCALL:  return "ANSWERCALL";
		case XMS_EVT_LINKDEVICE:  return "LINKDEVICE";
		case XMS_EVT_UNLINKDEVICE:  return "UNLINKDEVICE";
		case XMS_EVT_CLEARCALL:   return "CLEARCALL";
		case XMS_EVT_ANALOG_INTERFACE: return "ANALOG_INTERFACE";
		case XMS_EVT_CAS_MFC_START:  return "XMS_EVT_CAS_MFC_START";
		case XMS_EVT_CAS_MFC_END:  return "XMS_EVT_CAS_MFC_END";

		case XMS_EVT_JOINTOCONF:  return "JOINTOCONF";
		case XMS_EVT_LEAVEFROMCONF:  return "LEAVEFROMCONF";
		case XMS_EVT_CLEARCONF:   return "CLEARCONF";

		case XMS_EVT_PLAY:    return "PLAY";
		case XMS_EVT_INITINDEX:   return "INITINDEX";
		case XMS_EVT_BUILDINDEX:  return "BUILDINDEX";
		case XMS_EVT_CONTROLPLAY:  return "CONTROLPLAY";
		case XMS_EVT_RECORD:   return "RECORD";
		case XMS_EVT_CONTROLRECORD:  return "CONTROLRECORD";
		case XMS_EVT_RECORDCSP:   return "RECORDCSP";
		case XMS_EVT_CONTROLRECORDCSP:  return "CONTROLRECORDCSP";
		case XMS_EVT_3GPP_PLAY:   return "3GPPPlay";
		case XMS_EVT_3GPP_CONTROLPLAY: return "3GPPControlPlay";
		case XMS_EVT_3GPP_RECORD:  return "3GPPRecord";
		case XMS_EVT_3GPP_CONTROLRECORD: return "3GPPControlRecord";
		case XMS_EVT_PLAYCSPREQ:  return "CSPPlayReq";

		case XMS_EVT_SENDFAX:   return "XMS_EVT_SENDFAX";
		case XMS_EVT_RECVFAX:   return "XMS_EVT_RECVFAX";

		case XMS_EVT_SENDIODATA:  return "SENDIODATA";
		case XMS_EVT_RECVIODATA:  return "RECVIODATA";

		case XMS_EVT_MODMONITOR:     return "MODMONITOR";
		case XMS_EVT_DEV_TIMER:   return "DEV_TIMER";
		case XMS_EVT_3GPP_INIT_INDEX: return "3GPPInitIndex";
		case XMS_EVT_3GPP_BUILD_INDEX: return "3GPPBuildIndex";
		case XMS_EVT_ACS_TIMER:         return "SYS_TIMER";

		case XMS_EVT_CTX_REG:           return "CTX_REG";
		case XMS_EVT_CTX_LINK:          return "CTX_LINK";
		case XMS_EVT_CTX_APPDATA:       return "CTX_APPDATA";

		case XMS_EVT_SET_MEDIAINFO:  return "MEDIAINFO";
		case XMS_EVT_MASTER_STATE:      return "MASTER STATE";
		case XMS_EVT_SLAVE_WORK_STATE:  return "SLAVE WORK STATE";
		case XMS_EVT_UNIFAILURE:  return "UNIFAILURE";
		case XMS_EVT_MODRELATE:         return "MODRELATE";
		case XMS_EVT_CLOSE_STREAM_EXT:  return "Close Stream EXT";
	}

	sprintf( TmpStr, "UNKNOWN(0x%X)", EvtType );
	return TmpStr;
}

/***********************************************************
* Function:       // AddDeviceRes
* Description:    // 
***********************************************************/
void AddDeviceRes( Acs_Dev_List_Head_t *pAcsDevList )
{
	void		*p;
	int			i;
	char		TmpStr[256];
	DeviceID_t	*pDev;

	if( pDevRes == NULL )
	{
		pDevRes = (TYPE_MY_DEVICE_STRUCT *)malloc( pAcsDevList->m_s32DeviceNum * sizeof( TYPE_MY_DEVICE_STRUCT ) );
		if( pDevRes == NULL )
		{
			WriteLog( LEVEL_ERROR, "malloc() Fail in My Function AddDeviceRes()!" );
			return;
		}
	}else
	{
		p = realloc( pDevRes, ( TotalDevRes + pAcsDevList->m_s32DeviceNum ) * sizeof( TYPE_MY_DEVICE_STRUCT ) );
		if( p == NULL )
		{
			sprintf( TmpStr, "realloc() from %d to %d Fail in My Function AddDeviceRes()!",
			         TotalDevRes, TotalDevRes + pAcsDevList->m_s32DeviceNum );
			WriteLog( LEVEL_DEBUG, TmpStr );
			return;
		}

		pDevRes = (TYPE_MY_DEVICE_STRUCT *)p;
	}

	pDev = (DeviceID_t *)( (DJ_S8 *)pAcsDevList + sizeof( Acs_Dev_List_Head_t ) );
	for( i = 0; i < pAcsDevList->m_s32DeviceNum; i++ )
	{
		pDevRes[TotalDevRes + i].DevID	   = pDev[i];
		pDevRes[TotalDevRes + i].pDevData  = NULL;
	}

	TotalDevRes += pAcsDevList->m_s32DeviceNum;
}

/***********************************************************
* Function:       // 函数名称
* Description:    // 函数功能、性能等的描述
* Input:          // 1.输入参数1，说明，包括每个参数的作用、取值说明及参数间关系
* Input:          // 2.输入参数2，说明，包括每个参数的作用、取值说明及参数间关系
* Output:         // 1.输出参数1，说明
* Return:         // 函数返回值的说明
* Others:         // 其它说明
***********************************************************/
void DispEventInfo( Acs_Evt_t *pAcsEvt )
{
	char tmpStr[512];
	memset( tmpStr, 0, sizeof( tmpStr ) );

	sprintf( tmpStr, "%s", GetString_EventType( pAcsEvt->m_s32EventType ) );
	printf( tmpStr );
	WriteLog( LEVEL_DEBUG, tmpStr );
}

//回调函数-----------------------------------------------------------------------
DJ_Void EvtHandler( DJ_U32 esrParam )
{
	Acs_Evt_t			* pAcsEvt = NULL;

	Acs_Dev_List_Head_t * pAcsDevList  = NULL;
	Acs_GeneralProc_Data* pGeneral	   = NULL;

	char				TmpStr[256];
	char				* pStr = NULL;

	pAcsEvt = (Acs_Evt_t *)esrParam;

	DispEventInfo( pAcsEvt );
	switch( pAcsEvt->m_s32EventType )
	{
		case XMS_EVT_QUERY_DEVICE:
		{
			pAcsDevList = ( Acs_Dev_List_Head_t *)( (DJ_S8 *)pAcsEvt + sizeof( Acs_Evt_t ) );
			AddDeviceRes( pAcsDevList );
		}
		break;

		case XMS_EVT_QUERY_DEVICE_END: //get device list completely
		{
			pGeneral = (Acs_GeneralProc_Data*)FetchEventData(pAcsEvt);
			sprintf ( TmpStr, "Start work. DeviceNum = %d\n", TotalDevRes );
			printf(TmpStr);
			WriteLog(LEVEL_DEBUG, TmpStr );

		}
		break;
	}
}

/***********************************************************
* Function:       // InitSystem
* Description:    // 系统初始化函数
***********************************************************/
int InitSystem( void )
{
	int		retCode = -1;
	char	tmpStr[512];
	memset( tmpStr, 0, sizeof( tmpStr ) );

	//初始化日志
	retCode = InitLogCfg( );
	if( retCode < 0 )
	{
		printf( "Error, InitLogCfg() fail!\n" );
		return -1;
	}

	//读取配置文件XMS_ApiTest.ini
	ReadFromConfig( );

	if( CAS_Common_Cfg_ReadCfg( &g_Param_CAS ) != 0 )
	{
		sprintf( tmpStr, "Error, CAS_Common_Cfg_ReadCfg() read XMS_CAS_Cfg.INI fail!" );
		WriteLog( LEVEL_ERROR, tmpStr );
	}
	if( Analog_Common_Cfg_ReadCfg( &g_Param_Analog ) != 0 )
	{
		sprintf( tmpStr, "Error, Analog_Common_Cfg_ReadCfg() read XMS_Analog_Cfg.INI fail!" );
		WriteLog( LEVEL_ERROR, tmpStr );
	}

	// end of code for CAS(SS1)
	retCode = XMS_acsDllInit( );
	if( retCode < 0 )
	{
		sprintf( tmpStr, "XMS_acsDllInit failed! ret = %d\n", retCode );
		WriteLog( LEVEL_ERROR, tmpStr );
		return -1;
	}else
	{
		sprintf( tmpStr, "\nXMS_acsDllInit OK!" );
		WriteLog( LEVEL_DEBUG, tmpStr );
	}
	//打开与Keygoe的连接
	retCode = XMS_acsOpenStream( &g_acsHandle,
	                             &cfg_ServerID,
	                             cfg_s32AppUnitID,  // App Unit ID
	                             32,                // sendQSize, in K Bytes
	                             32,                // recvQSize, in K Bytes
	                             cfg_s32DebugOn,    // Debug On/Off
	                             NULL );
	if( retCode < 0 )
	{
		sprintf( tmpStr, "XMS_acsOpenStream(IP Addr : %s, port : %d) FAIL. ret = %d",
		         cfg_ServerID.m_s8ServerIp, cfg_ServerID.m_u32ServerPort, retCode );
		WriteLog( LEVEL_ERROR, tmpStr );
		return -1;
	}else
	{
		sprintf( tmpStr, "XMS_acsOpenStream(%s,%d) OK!", cfg_ServerID.m_s8ServerIp, cfg_ServerID.m_u32ServerPort );
		WriteLog( LEVEL_DEBUG, tmpStr );
	}

	//设置回调函数
	retCode = XMS_acsSetESR( g_acsHandle, (EsrFunc)EvtHandler, 0, 1 );
	if( retCode < 0 )
	{
		sprintf( tmpStr, "XMS_acsSetESR() FAIL! ret = %d", retCode );
		WriteLog( LEVEL_ERROR, tmpStr );
		return -1;
	}else
	{
		sprintf( tmpStr, "XMS_acsSetESR() OK!" );
		WriteLog( LEVEL_DEBUG, tmpStr );
	}
	//获取设备列表
	retCode = XMS_acsGetDeviceList( g_acsHandle, NULL );
	if( retCode < 0 )
	{
		sprintf( tmpStr, "XMS_acsGetDeviceList() FAIL! ret = %d", retCode );
		WriteLog( LEVEL_ERROR, tmpStr );
		return -1;
	}else
	{
		sprintf( tmpStr, "XMS_acsGetDeviceList() OK!" );
		WriteLog( LEVEL_DEBUG, tmpStr );
	}
	return 1;
}

/************************************** The End Of File **************************************/
