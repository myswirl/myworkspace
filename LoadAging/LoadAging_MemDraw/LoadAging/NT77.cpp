// NT77.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "NT77Api.h"

int SystemAuthorizeCheck( )
{

	long nRet = 0;
	char hwID[64] ={0};	
	char tmpStr[64]={0};
	unsigned char lifeSpan[2] ={0};			//使用期限，加密锁第1、2个字节的数据，单位小时，出厂时写入不能更改，
	unsigned char sysRunTime[2] ={0};		//运行时间，加密锁第3、4个字节的数据，单位小时，出厂为0，运行一个小时累计加1
	
	unsigned short tmpLifeSpan;
	unsigned short tmpSysRunTime;
	
	nRet = NTFindFirst("1234567890ABCDEF");			//查找第一次加密锁，输入正确的加密锁识别码
	if(0 != nRet)
	{
		sprintf(tmpStr,"NT77 is not found! return %d\n ", nRet);
		return -1;
	}

	nRet = NTLogin("5d77a67b65d59b5ceec9a7e3cc068e6c");		//登录加密锁
	if( 0 != nRet)
	{
		sprintf(tmpStr,"Failed to login! return %d\n ", nRet);
		return -2;
	}

	nRet = NTRead(0, 2, lifeSpan);
	if( 0 != nRet)
	{
		sprintf(tmpStr,"Failed to read lifespan ! return %d\n ", nRet);
		return -4;
	}

	nRet = NTRead(2, 4, sysRunTime);
	if( 0 != nRet)
	{
		sprintf(tmpStr,"Failed to read lifespan ! return %d\n ", nRet);
		return -4;
	}

	tmpLifeSpan = (unsigned short)(lifeSpan[0]<<8 | lifeSpan[1]);
	tmpSysRunTime = (unsigned short)(sysRunTime[0]<<8 | sysRunTime[1]);

	if(tmpLifeSpan == 0xFFFF)	//永久授权
	{
		NTLogout();
		return 1;
	}

	if( tmpSysRunTime>=tmpLifeSpan )	//临时授权超时
	{
		NTLogout();
		return -6;
	}
	
	tmpSysRunTime++;									//计时统计加1
	sysRunTime[0] = (BYTE)((tmpSysRunTime&0xFF00)>>8);	//取高字节
	sysRunTime[1] = (BYTE)(tmpSysRunTime&0x00FF);		//取低字节
	nRet = NTWrite(2, 4, sysRunTime);		
	if( 0 != nRet)
	{
		sprintf(tmpStr,"Failed to read lifespan ! return %d\n ", nRet);
		return -4;
	}
	nRet = NTLogout();									//退出加密锁
	if( 0 != nRet)
	{
		printf("Failed to log out ! return %d\n ", nRet);
		return -5;
	}

	return (tmpLifeSpan-tmpSysRunTime);	//返回剩余小时数
}

