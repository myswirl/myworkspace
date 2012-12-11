// NT77.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "NT77Api.h"

int SystemAuthorizeCheck( )
{

	long nRet = 0;
	char hwID[64] ={0};	
	char tmpStr[64]={0};
	unsigned char lifeSpan[2] ={0};			//ʹ�����ޣ���������1��2���ֽڵ����ݣ���λСʱ������ʱд�벻�ܸ��ģ�
	unsigned char sysRunTime[2] ={0};		//����ʱ�䣬��������3��4���ֽڵ����ݣ���λСʱ������Ϊ0������һ��Сʱ�ۼƼ�1
	
	unsigned short tmpLifeSpan;
	unsigned short tmpSysRunTime;
	
	nRet = NTFindFirst("1234567890ABCDEF");			//���ҵ�һ�μ�������������ȷ�ļ�����ʶ����
	if(0 != nRet)
	{
		sprintf(tmpStr,"NT77 is not found! return %d\n ", nRet);
		return -1;
	}

	nRet = NTLogin("5d77a67b65d59b5ceec9a7e3cc068e6c");		//��¼������
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

	if(tmpLifeSpan == 0xFFFF)	//������Ȩ
	{
		NTLogout();
		return 1;
	}

	if( tmpSysRunTime>=tmpLifeSpan )	//��ʱ��Ȩ��ʱ
	{
		NTLogout();
		return -6;
	}
	
	tmpSysRunTime++;									//��ʱͳ�Ƽ�1
	sysRunTime[0] = (BYTE)((tmpSysRunTime&0xFF00)>>8);	//ȡ���ֽ�
	sysRunTime[1] = (BYTE)(tmpSysRunTime&0x00FF);		//ȡ���ֽ�
	nRet = NTWrite(2, 4, sysRunTime);		
	if( 0 != nRet)
	{
		sprintf(tmpStr,"Failed to read lifespan ! return %d\n ", nRet);
		return -4;
	}
	nRet = NTLogout();									//�˳�������
	if( 0 != nRet)
	{
		printf("Failed to log out ! return %d\n ", nRet);
		return -5;
	}

	return (tmpLifeSpan-tmpSysRunTime);	//����ʣ��Сʱ��
}

