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
	unsigned char lifeSpan[2] ={0};			//ʹ�����ޣ���������1��2���ֽڵ����ݣ���λСʱ������ʱд�벻�ܸ��ģ�
	unsigned char sysRunTime[2] ={0};		//����ʱ�䣬��������3��4���ֽڵ����ݣ���λСʱ������Ϊ0������10�����ۼƼ�1
	
	unsigned short tmpLifeSpan;
	unsigned short tmpSysRunTime;
	
	nRet = NTFindFirst("rishenyuan13662260755");			//���ҵ�һ�μ�������������ȷ�ļ�����ʶ����
	if(0 != nRet)
	{
		sprintf(tmpStr,"Dog is not found! return %d\n ", nRet);
		WriteLog(LEVEL_ERROR,tmpStr);
		return -1;
	}

	nRet = NTLogin("fa0bc02c8a4b483109bdc6412adb2efe");		//��¼������
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

	if(tmpLifeSpan == 0xFFFF)	//������Ȩ
	{
		NTLogout();
		return 0xFFFD;
	}

	if( tmpSysRunTime>=tmpLifeSpan )	//��ʱ��Ȩ��ʱ
	{
		sprintf(tmpStr,"Failed, the softeware is timeout !");
		WriteLog(LEVEL_ERROR,tmpStr);
		NTLogout();
		return -6;
	}
	sprintf(tmpStr,"Check OK, temporary lic, LifeSpan:%d, runTime:%d",tmpLifeSpan, tmpSysRunTime);
	WriteLog(LEVEL_DEBUG,tmpStr);

	tmpSysRunTime++;									//��ʱͳ�Ƽ�1
	sysRunTime[0] = (BYTE)((tmpSysRunTime&0xFF00)>>8);	//ȡ���ֽ�
	sysRunTime[1] = (BYTE)(tmpSysRunTime&0x00FF);		//ȡ���ֽ�
	nRet = NTWrite(2, 4, sysRunTime);		
	if( 0 != nRet)
	{
		sprintf(tmpStr,"Failed to write sysRunTime ! return %d\n ", nRet);
		WriteLog(LEVEL_ERROR,tmpStr);
		NTLogout();				
		return -4;
	}
	nRet = NTLogout();									//�˳�������
	if( 0 != nRet)
	{
		printf("Failed to log out ! return %d\n ", nRet);
		WriteLog(LEVEL_ERROR,tmpStr);
		return -5;
	}

	return (tmpLifeSpan-tmpSysRunTime);	//����ʣ����ٸ�10����
}

int FirstSystemAuthorizeCheck( )
{
	
	long nRet = 0;
	char hwID[64] ={0};	
	char tmpStr[64]={0};
	unsigned char lifeSpan[2] ={0};			//ʹ�����ޣ���������1��2���ֽڵ����ݣ���λСʱ������ʱд�벻�ܸ��ģ�
	unsigned char sysRunTime[2] ={0};		//����ʱ�䣬��������3��4���ֽڵ����ݣ���λСʱ������Ϊ0������10�����ۼƼ�1
	
	unsigned short tmpLifeSpan;
	unsigned short tmpSysRunTime;
	
	nRet = NTFindFirst("rishenyuan13662260755");			//���ҵ�һ�μ�������������ȷ�ļ�����ʶ����
	if(0 != nRet)
	{
		sprintf(tmpStr,"Dog is not found! return %d\n ", nRet);
		WriteLog(LEVEL_ERROR,tmpStr);
		return -1;
	}
	
	nRet = NTLogin("fa0bc02c8a4b483109bdc6412adb2efe");		//��¼������
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
	
	if(tmpLifeSpan == 0xFFFF)	//������Ȩ
	{
		NTLogout();
		return 0xFFFD;
	}
	
	if( tmpSysRunTime>=tmpLifeSpan )	//��ʱ��Ȩ��ʱ
	{
		sprintf(tmpStr,"Failed, the softeware is timeout !");
		WriteLog(LEVEL_ERROR,tmpStr);
		NTLogout();
		return -6;
	}
	sprintf(tmpStr,"Check OK, temporary lic, LifeSpan:%d, runTime:%d",tmpLifeSpan, tmpSysRunTime);
	WriteLog(LEVEL_DEBUG,tmpStr);

	nRet = NTLogout();									//�˳�������
	if( 0 != nRet)
	{
		printf("Failed to log out ! return %d\n ", nRet);
		WriteLog(LEVEL_ERROR,tmpStr);
		return -5;
	}
	
	return (tmpLifeSpan-tmpSysRunTime);	//����ʣ����ٸ�10����
}

