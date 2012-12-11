/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// XMS_SwitchDemo_String.h
 * Author:			// lmy
 * Date:			// 2010.01.12
 * Description:		// 在程序界面显示内容方面的字符处理
 * Version:			// 1.0
 * History:			// 
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#ifndef _XMS_SWITCHDEMO_STRING_H_
#define _XMS_SWITCHDEMO_STRING_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//函数声明如下
char * GetString_EventType( EventType_t EvtType );


char * GetString_DeviceMain( DJ_S32 s32DeviceMain );


char * GetString_ErrorCode( DJ_S32 s32ErrorCode );


char * GetString_DeviceSub( DJ_S32 s32DeviceSub );
char* GetString_TrunkState(int State);

void GetString_LineState( char *StateStr, int iLineState );


#endif
/************************************** The End Of File **************************************/
