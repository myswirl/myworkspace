/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// �ļ���
 * Author:			// ����
 * Date:			// ����
 * Description:		// ģ������
 * Version:			// �汾��Ϣ
 * Function List:	// ��Ҫ�������书��
 *     1. -------
 * History:			// ��ʷ�޸ļ�¼
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#ifndef __DDB_COMMON_H__
#define __DDB_COMMON_H__

#ifdef LINUX_SYSTEM
#include <linux/pci.h>
typedef unsigned char	UCHAR;
typedef UCHAR			*PUCHAR;
typedef void			*PVOID;
typedef unsigned short	USHORT;
typedef USHORT			*PUSHORT;
typedef unsigned long	ULONG;
typedef ULONG			*PULONG;
#define FALSE	0
#define TRUE	1
#define PDEBUG( x ) printk x
#else
#define PDEBUG( x ) KdPrint( x )
#endif

#include <wdm.h>
typedef unsigned char	BYTE;
typedef ULONG			DWORD;
typedef USHORT			WORD;
#define BOOL ULONG

//system define
#define MAX_CARD_NUMBER_ONE_PC	32
#define MAX_IOCTRL_BUF_SIZE		4096
#define MAX_PCI_CONFIG_SIZE		256
#define MAX_READ_WRITE_SIZE		256

//mem id
#define CSR_MEM_ID	0
#define HPI_MEM_ID	1
#define GPIO_MEM_ID 2

//data type id
#define INFO_BYTE	0
#define INFO_WORD	1
#define INFO_DWORD	2

//act type id
#define INFO_READ	0
#define INFO_WRITE	1

//io_ctrl id
#define FILE_DEVICE_DJ	0x00008000 + 0x333
#define DJ_IOCTL_INDEX	0x1000

#ifndef CTL_CODE
#define CTL_CODE( DeviceType, Function, Method, Access ) (     \
        ( ( DeviceType ) << 16 ) | ( ( Access ) << 14 ) | ( ( Function ) << 2 ) | ( Method ) \
        )

#define METHOD_BUFFERED 0
#define FILE_ANY_ACCESS 0
#endif

#define DJ_IOCTL( I ) CTL_CODE( FILE_DEVICE_DJ, DJ_IOCTL_INDEX + I, METHOD_BUFFERED, FILE_ANY_ACCESS )

#define IOCTRL_DJKB DJ_IOCTL( 100 )

//msg type
#define DJKB_MSG_ALL_CARD_INFO		0x01
#define DJKB_MSG_PCI_CONFIG_READ	0x02
#define DJKB_MSG_PCI_CONFIG_WRITE	0x03
#define DJKB_MSG_2040_READ			0x04
#define DJKB_MSG_2040_WRITE			0x05
#define DJKB_MSG_SEND_PACKET		0x06
#define DJKB_MSG_RECV_PACKET		0x07

//�忨��Ϣ
typedef struct {
	BYTE	cbIndex;        //slotNo
	BYTE	cbCardType;     //�忨����
	BYTE	cbSubType;      //�忨������
	WORD	wBusNum;        //���ߺ�
	WORD	wDeviceNum;     //�豸��
	WORD	wFunctionNum;   //���ܺ�
	ULONG	pBaseMem[3];    //�Ĵ�����ַ
	BYTE	*pMapMem[3];    //ӳ���ַ
	DWORD	dwMemSize[3];   //��ַ��С
}ONE_CARD_INFO, *P_ONE_CARD_INFO;

typedef struct
{
	WORD			wCardNum;
	ONE_CARD_INFO	cardInfo[MAX_CARD_NUMBER_ONE_PC];
}ALL_DEVICE_INFO, *P_ALL_DEVICE_INFO;

typedef struct {
	WORD	m_wAction;
	WORD	m_wCardID;
	BYTE	m_cbMemID;
	DWORD	m_dwOffset;
	BYTE	m_cbInfoType;
	DWORD	m_dwVal;
}READ_WRITE_STRUCT, *PREAD_WRITE_STRUCT;

typedef struct
{
	long	m_lMsgLen;  //��Ϣ����
	long	m_lMsgType; //��Ϣ����

	ULONG	m_hEvent;
	long	m_lRetVal;  //����ֵ

	long	m_lEvtDev;
	long	m_lEvtType;
	long	m_lDataLen;

	void * m_pSysData;

	long	m_lParam1;
	long	m_lParam2;
	long	m_lParam3;

	DWORD m_dwProcessID;

	long m_pMsgPtr;

	long	m_lrfu1;
	long	m_rfu[9];

	BYTE * m_pData;
}MSG_HEAD_STRUCT, *PMSG_HEAD_STRUCT;

extern ALL_DEVICE_INFO g_AllDevInfo;

#endif
/************************************** The End Of File **************************************/

