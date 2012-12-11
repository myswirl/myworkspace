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
#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"
#include "DJAcsDevState.h"

#include "XMS_SwitchDemo_Sub.h"
#include "XMS_SwitchDemo_String.h"
#include "XMS_SwitchDemo_Event.h"


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
	switch( EvtType )
	{
		case XMS_EVT_OPEN_STREAM:  return "OPEN_STREAM";
		case XMS_EVT_QUERY_DEVICE:  return "QUERY_DEVICE";
		case XMS_EVT_QUERY_DEVICE_END: return "QUERY_DEVICE_END";
		case XMS_EVT_OPEN_DEVICE:  return "OPEN_DEVICE";
		case XMS_EVT_CLOSE_DEVICE:  return "CLOSE_DEVICE";
		case XMS_EVT_RESET_DEVICE:  return "RESET_DEVICE";
		case XMS_EVT_DEVICESTATE:  return "DEVICESTATE";
		case XMS_EVT_SETDEV_GROUP:  return "SETDEV_GROUP";
		case XMS_EVT_SETPARAM:   return "SETPARAM";
		case XMS_EVT_GETPARAM:   return "GETPARAM";
		case XMS_EVT_QUERY_ONE_DSP_START:    return "QUERY_ONE_DSP_START";
		case XMS_EVT_QUERY_ONE_DSP_END:     return "QUERY_ONE_DSP_END";
		case XMS_EVT_QUERY_REMOVE_ONE_DSP_START:  return "QUERY_REMOVE_ONE_DSP_START";
		case XMS_EVT_QUERY_REMOVE_ONE_DSP_END:   return "QUERY_REMOVE_ONE_DSP_END";

		case XMS_EVT_CALLOUT:   return "CALLOUT";
		case XMS_EVT_CALLIN:   return "CALLIN";
		case XMS_EVT_ALERTCALL:   return "ALERTCALL";
		case XMS_EVT_ANSWERCALL:  return "ANSWERCALL";
		case XMS_EVT_LINKDEVICE:  return "LINKDEVICE";
		case XMS_EVT_UNLINKDEVICE:  return "UNLINKDEVICE";
		case XMS_EVT_CLEARCALL:   return "CLEARCALL";
		case XMS_EVT_ANALOG_INTERFACE: return "ANALOG_INTERFACE";
		case XMS_EVT_CAS_MFC_START:  return "CAS_MFC_START";
		case XMS_EVT_CAS_MFC_END:  return "CAS_MFC_END";

		case XMS_EVT_JOINTOCONF:  return "JOINTOCONF";
		case XMS_EVT_LEAVEFROMCONF:  return "LEAVEFROMCONF";
		case XMS_EVT_CLEARCONF:   return "CLEARCONF";

		case XMS_EVT_PLAY:    return "PLAY";
		case XMS_EVT_INITINDEX:   return "INITINDEX";
		case XMS_EVT_BUILDINDEX:  return "BUILDINDEX";
		case XMS_EVT_CONTROLPLAY:  return "CONTROLPLAY";
		case XMS_EVT_RECORD:   return "RECORD";
		case XMS_EVT_CONTROLRECORD:  return "CONTROLRECORD";

		case XMS_EVT_SENDFAX:   return "SENDFAX";
		case XMS_EVT_RECVFAX:   return "RECVFAX";

		case XMS_EVT_SENDIODATA:  return "SENDIODATA";
		case XMS_EVT_RECVIODATA:  return "RECVIODATA";
		case XMS_EVT_UNIFAILURE:  return "UNIFAILURE";
	}

	return "UNKNOWN";
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
char *GetString_DeviceMain( DJ_S32 s32DeviceMain )
{
	switch( s32DeviceMain )
	{
		case XMS_DEVMAIN_VOICE:    return "VOC  ";
		case XMS_DEVMAIN_FAX:    return "FAX  ";
		case XMS_DEVMAIN_DIGITAL_PORT:  return "DIGIT";
		case XMS_DEVMAIN_INTERFACE_CH:  return "INTCH";
		case XMS_DEVMAIN_DSS1_LINK:   return "DSS1 ";
		case XMS_DEVMAIN_SS7_LINK:   return "SS7  ";
		case XMS_DEVMAIN_BOARD:    return "BRD  ";
		case XMS_DEVMAIN_CTBUS_TS:   return "CTBUS";
		case XMS_DEVMAIN_VOIP:    return "VOIP ";
		case XMS_DEVMAIN_CONFERENCE:  return "CONF ";
		case XMS_DEVMAIN_VIDEO:    return "VIDEO";
	}

	return "UNKNOWN";
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
char *GetString_ErrorCode( DJ_S32 s32ErrorCode )
{
	switch( s32ErrorCode )
	{
		case ACSERR_LOADLIBERR:     return "LOADLIBERR";
		case ACSERR_BADPARAMETER:    return "BADPARAMETER";
		case ACSERR_NOSERVER:     return "NOSERVER";
		case ACSERR_MAXCONN_EXCEED:    return "MAXCONN_EXCEED";
		case ACSERR_BADHDL:      return "ABADHDL";

		case ACSERR_FAILSEND:     return "FAILSEND";
		case ACSERR_LINKBROKEN:     return "LINKBROKEN";
		case ACSERR_NOMESSAGE:     return "NOMESSAGE";
		case ACSERR_GETRESFAIL:     return "GETRESFAIL";
		case ACSERR_NOSUPPORTCMD:    return "NOSUPPORTCMD";
	}

	return "UNKNOWN";
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
void GetString_LineState( char *StateStr, int iLineState )
{
	switch( iLineState )
	{
		case DCS_FREE:       strcpy( StateStr, "S_FREE" );   break;
		case DCS_LOCAL_BLOCK:     strcpy( StateStr, "S_LOCAL_BLOCK" ); break;
		case DCS_REMOTE_BLOCK:     strcpy( StateStr, "S_REMOTE_BLOCK" ); break;
		case DCS_BLOCK:       strcpy( StateStr, "S_BLOCK" );   break;
		case DCS_UNAVAILABLE:     strcpy( StateStr, "S_UNAVAILABLE" ); break;

		case DCS_CALLOUT:      strcpy( StateStr, "S_CALLOUT" );  break;
		case DCS_CALLIN:      strcpy( StateStr, "S_CALLIN" );   break;
		case DCS_ALERTING:      strcpy( StateStr, "S_ALERTING" );  break;

		case DCS_CONNECT:      strcpy( StateStr, "S_CONNECT" );  break;
		case DCS_DISCONNECT:     strcpy( StateStr, "S_DISCONNECT" );  break;
		case DCS_WAIT_FREE:      strcpy( StateStr, "S_WAIT_FREE" );  break;
		default:
			sprintf( StateStr, "0x%X", iLineState );
			break;
	}
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
char *GetString_DeviceSub( DJ_S32 s32DeviceSub )
{
	switch( s32DeviceSub )
	{
		case XMS_DEVSUB_BASE:      return "BASE   ";
		case XMS_DEVSUB_ANALOG_TRUNK:    return "A_TRUNK";
		case XMS_DEVSUB_ANALOG_USER:    return "A_USER ";
		case XMS_DEVSUB_ANALOG_HIZ:     return "A_HIZ  ";
		case XMS_DEVSUB_ANALOG_EMPTY:    return "A_EMPTY";
		case XMS_DEVSUB_E1_PCM:      return "E1_PCM ";
		case XMS_DEVSUB_E1_CAS:      return "E1_CAS ";
		case XMS_DEVSUB_E1_DSS1:     return "E1_DSS1";
		case XMS_DEVSUB_E1_SS7_TUP:     return "E1_TUP ";
		case XMS_DEVSUB_E1_SS7_ISUP:    return "E1_ISUP";
		case XMS_DEVSUB_ANALOG_VOC2W:    return "A_VOC2W";
		case XMS_DEVSUB_ANALOG_VOC4W:    return "A_VOC4W";
		case XMS_DEVSUB_ANALOG_EM:     return "A_EM   ";
		case XMS_DEVSUB_E1_DCH:      return "E1_DCH ";
		case XMS_DEVSUB_E1_BCH:      return "E1_BCH ";
		case XMS_DEVSUB_UNUSABLE:     return "UNUSE  ";

		// new add next types from V2.2.0
		case XMS_DEVSUB_HIZ_CAS:     return "HIZ_CAS";
		case XMS_DEVSUB_HIZ_PRI:     return "HIZ_PRI";
		case XMS_DEVSUB_HIZ_SS7:     return "HIZ_SS7";
		case XMS_DEVSUB_HIZ_PRI_LINK:    return "HIZ_PRI_LINK";
		case XMS_DEVSUB_HIZ_SS7_64K_LINK:   return "HIZ_SS7_64K_LINK";
		case XMS_DEVSUB_HIZ_SS7_2M_LINK:   return "HIZ_SS7_2M_LINK";
		case XMS_DEVSUB_SS7_LINK:     return "SS7_LINK";
		case XMS_DEVSUB_LINESIDE:     return "LINESIDE";
		case XMS_DEVSUB_ANALOG_REC:     return "A_REC";
		case XMS_DEVSUB_DIGITAL_REC:    return "D_REC";
		case XMS_DEVSUB_HIZ_HDLC_N64K_LINK:   return "HDLC_N64_LINK";
//	case XMS_DEVSUB_3G_324:						return "3G_324";
	}

	return "UNKNOWN";
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
char* GetString_TrunkState( int State )
{
	switch( State )
	{
		case TRK_WAITOPEN:   		return "Wait Open";
		case TRK_FREE:   			return "Free";
		case TRK_WELCOME:   		return "Welcome";
		case TRK_EXTNUM:   			return "Ext Num";
		case TRK_CONNECT_FAILURE:   return "Conn Fail";
		case TRK_WAITCONNECT:   	return "Wait Con";
		case TRK_CONNECT:   		return "Connect";
		case TRK_CALLOUT:   		return "Callout";
		case TRK_NOTHANDLE:   		return "NotHandle";
		case TRK_WAIT_ANSWERCALL:   return "Wait Ans";
		case TRK_WAIT_LINKOK:   	return "Wait Link";
		case TRK_WAIT_REMOVE:   	return "DSP Remov";
		case USER_WAITOPEN:   		return "Wait Open";
		case USER_FREE:   			return "Free";
		case USER_OFFHOOK:   		return "OffHook";
		case USER_CALLOUT:   		return "CallOut";
		case USER_CONNECT:   		return "Connect";
		case USER_RING:   			return "Ring";
		case USER_CONNECT_FAILURE:	return "Con Fail";
		default:   					return "Other";
	}
}

/************************************** The End Of File **************************************/
