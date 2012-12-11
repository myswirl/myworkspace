/************************************************************
 * Copyright (C), 2009-2011, Donjin Tech. Co., Ltd.
 * FileName:		// ddb_main.h
 * Author:			// lmy
 * Date:			// 20100630
 * Description:		//
 * Version:			//
 * Function List:	//
 *     1. -------
 * History:			//
 *     <author>  <time>   <version >   <desc>
 *     David    96/10/12     1.0     build this moudle
 ***********************************************************/
#define DEVICE_NAME			( L"\\Device\\DJKBWDM" )
#define DEVICE_SYMLINKNAME	( L"\\DosDevices\\DJKBWDM" )

typedef struct _DEVICE_EXTENSION
{
	PDEVICE_OBJECT	fdo;
	PDEVICE_OBJECT	PhyDevice;
	PDEVICE_OBJECT	NextStackDevice;
	UNICODE_STRING	ustrDeviceName;     // 设备名
	UNICODE_STRING	ustrSymLinkName;    // 符号链接名
	PKINTERRUPT		InterruptObject;    // address of interrupt object
	PUCHAR			portbase;           // IO端口地址
	ULONG			nports;             // IO端口地址的数量
	PVOID			MemBar0;            // 内存基地址0
	ULONG			nMem0;              // 基地址BAR0占用字节数
	PVOID			MemBar1;            // 内存基地址1
	ULONG			nMem1;              // 基地址BAR1占用字节数
	PVOID			MemForImage;        // 连续物理地址用来dma读取图像
	BOOLEAN			mappedport;         // 如果为真需要做IO端口映射
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;

#define arraysize( p ) ( sizeof( p ) / sizeof( ( p )[0] ) )

NTSTATUS DjkbWdmAddDevice( IN PDRIVER_OBJECT DriverObject, IN PDEVICE_OBJECT PhysicalDeviceObject );


NTSTATUS DjkbWdmPnp( IN PDEVICE_OBJECT fdo, IN PIRP Irp );


NTSTATUS DjkbWdmDeviceControl( IN PDEVICE_OBJECT fdo, IN PIRP Irp );


NTSTATUS DjkbWdmDispatchRoutine( IN PDEVICE_OBJECT fdo, IN PIRP Irp );


NTSTATUS ReadWriteConfigSpace( IN PDEVICE_OBJECT DeviceObject, IN ULONG ReadOrWrite, IN PVOID Buffer, IN ULONG Offset, IN ULONG Length );


void DjkbWdmUnload( IN PDRIVER_OBJECT DriverObject );


void DjkbWdmDelay( long millsecond );


NTSTATUS DriverEntry( IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath );


BOOLEAN OnInterrupt( PKINTERRUPT InterruptObject, PDEVICE_EXTENSION pdx );


/************************************** The End Of File **************************************/

