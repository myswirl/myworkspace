// NT77.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "NT77Api.h"
#include "LogFile.h"
int SystemAuthorizeCheck( )
{

	long nRet = 0;
	char hwID[64] ={0};	
	char tmpStr[64]={0};
	unsigned char lifeSpan[2] ={0};			//使用期限，加密锁第1、2个字节的数据，单位小时，出厂时写入不能更改，
	unsigned char sysRunTime[2] ={0};		//运行时间，加密锁第3、4个字节的数据，单位小时，出厂为0，运行10分钟累计加1
	
	unsigned short tmpLifeSpan;
	unsigned short tmpSysRunTime;
	
	nRet = NTFindFirst("rishenyuan13662260755");			//查找第一次加密锁，输入正确的加密锁识别码
	if(0 != nRet)
	{
		sprintf(tmpStr,"Dog is not found! return %d\n ", nRet);
		WriteLog(LEVEL_ERROR,tmpStr);
		return -1;
	}

	nRet = NTLogin("fa0bc02c8a4b483109bdc6412adb2efe");		//登录加密锁
	if( 0 != nRet)
	{
		sprintf(tmpStr,"Failed to login! return %d\n ", nRet);
		WriteLog(LEVEL_ERROR,tmpStr);
		return -2;
	}
	
	nRet = NTRead(0, 2, lifeSpan);
	if( 0 != nRet)
	{
		sprintf(tmpStr,"Failed to read lifespan ! return %d\n ", nRet);
		WriteLog(LEVEL_ERROR,tmpStr);
		NTLogout();		
		return -4;
	}

	nRet = NTRead(2, 4, sysRunTime);
	if( 0 != nRet)
	{
		sprintf(tmpStr,"Failed to read sysRunTime ! return %d\n ", nRet);
		WriteLog(LEVEL_ERROR,tmpStr);
		NTLogout();		
		return -4;
	}

	tmpLifeSpan = (unsigned short)(lifeSpan[0]<<8 | lifeSpan[1]);
	tmpSysRunTime = (unsigned short)(sysRunTime[0]<<8 | sysRunTime[1]);

	if(tmpLifeSpan == 0xFFFF)	//永久授权
	{
		NTLogout();
		return 0xFFFD;
	}

	if( tmpSysRunTime>=tmpLifeSpan )	//临时授权超时
	{
		sprintf(tmpStr,"Failed, the softeware is timeout !");
		WriteLog(LEVEL_ERROR,tmpStr);
		NTLogout();
		return -6;
	}
	sprintf(tmpStr,"Check OK, temporary lic, LifeSpan:%d, runTime:%d",tmpLifeSpan, tmpSysRunTime);
	WriteLog(LEVEL_DEBUG,tmpStr);

	tmpSysRunTime++;									//计时统计加1
	sysRunTime[0] = (BYTE)((tmpSysRunTime&0xFF00)>>8);	//取高字节
	sysRunTime[1] = (BYTE)(tmpSysRunTime&0x00FF);		//取低字节
	nRet = NTWrite(2, 4, sysRunTime);		
	if( 0 != nRet)
	{
		sprintf(tmpStr,"Failed to write sysRunTime ! return %d\n ", nRet);
		WriteLog(LEVEL_ERROR,tmpStr);
		NTLogout();				
		return -4;
	}
	nRet = NTLogout();									//退出加密锁
	if( 0 != nRet)
	{
		printf("Failed to log out ! return %d\n ", nRet);
		WriteLog(LEVEL_ERROR,tmpStr);
		return -5;
	}

	return (tmpLifeSpan-tmpSysRunTime);	//返回剩余多少个10分钟
}

int FirstSystemAuthorizeCheck( )
{
	
	long nRet = 0;
	char hwID[64] ={0};	
	char tmpStr[64]={0};
	unsigned char lifeSpan[2] ={0};			//使用期限，加密锁第1、2个字节的数据，单位小时，出厂时写入不能更改，
	unsigned char sysRunTime[2] ={0};		//运行时间，加密锁第3、4个字节的数据，单位小时，出厂为0，运行10分钟累计加1
	
	unsigned short tmpLifeSpan;
	unsigned short tmpSysRunTime;
	
	nRet = NTFindFirst("rishenyuan13662260755");			//查找第一次加密锁，输入正确的加密锁识别码
	if(0 != nRet)
	{
		sprintf(tmpStr,"Dog is not found! return %d\n ", nRet);
		WriteLog(LEVEL_ERROR,tmpStr);
		return -1;
	}
	
	nRet = NTLogin("fa0bc02c8a4b483109bdc6412adb2efe");		//登录加密锁
	if( 0 != nRet)
	{
		sprintf(tmpStr,"Failed to login! return %d\n ", nRet);
		WriteLog(LEVEL_ERROR,tmpStr);
		return -2;
	}
	
	nRet = NTRead(0, 2, lifeSpan);
	if( 0 != nRet)
	{
		sprintf(tmpStr,"Failed to read lifespan ! return %d\n ", nRet);
		WriteLog(LEVEL_ERROR,tmpStr);
		NTLogout();		
		return -4;
	}
	
	nRet = NTRead(2, 4, sysRunTime);
	if( 0 != nRet)
	{
		sprintf(tmpStr,"Failed to read sysRunTime ! return %d\n ", nRet);
		WriteLog(LEVEL_ERROR,tmpStr);
		NTLogout();		
		return -4;
	}
	
	tmpLifeSpan = (unsigned short)(lifeSpan[0]<<8 | lifeSpan[1]);
	tmpSysRunTime = (unsigned short)(sysRunTime[0]<<8 | sysRunTime[1]);
	
	if(tmpLifeSpan == 0xFFFF)	//永久授权
	{
		NTLogout();
		return 0xFFFD;
	}
	
	if( tmpSysRunTime>=tmpLifeSpan )	//临时授权超时
	{
		sprintf(tmpStr,"Failed, the softeware is timeout !");
		WriteLog(LEVEL_ERROR,tmpStr);
		NTLogout();
		return -6;
	}
	sprintf(tmpStr,"Check OK, temporary lic, LifeSpan:%d, runTime:%d",tmpLifeSpan, tmpSysRunTime);
	WriteLog(LEVEL_DEBUG,tmpStr);

	nRet = NTLogout();									//退出加密锁
	if( 0 != nRet)
	{
		printf("Failed to log out ! return %d\n ", nRet);
		WriteLog(LEVEL_ERROR,tmpStr);
		return -5;
	}
	
	return (tmpLifeSpan-tmpSysRunTime);	//返回剩余多少个10分钟
}

