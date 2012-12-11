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
#include "djkb_common.h"
#include "djkb_main.h"

ALL_DEVICE_INFO g_AllDevInfo;

/***********************************************************
* Function:       // DriverEntry
* Description:    // ��ʼ���������򣬶�λ������Ӳ����Դ�������ں˶���
* Input:          // 1.pDriverObject:��I/O�������д���������������
*			      // 2.pRegistryPath:����������ע�����е�·��
* Return:         // ���س�ʼ������״̬
* Others:         //
***********************************************************/
NTSTATUS DriverEntry( IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING pRegistryPath )
{
	PDEBUG(( "[djkb_main.c]:Enter DriverEntry()\n" ));

	pDriverObject->DriverExtension->AddDevice			   = DjkbWdmAddDevice;
	pDriverObject->MajorFunction[IRP_MJ_PNP]			   = DjkbWdmPnp;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL]	   = DjkbWdmDeviceControl;
	pDriverObject->MajorFunction[IRP_MJ_CREATE]			   = DjkbWdmDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE]			   = DjkbWdmDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_READ]			   = DjkbWdmDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_WRITE]			   = DjkbWdmDispatchRoutine;
	pDriverObject->DriverUnload							   = DjkbWdmUnload;

	KdPrint( ( "[djk_main.c]:Leave DriverEntry()\n" ) );
	return STATUS_SUCCESS;
}

/***********************************************************
* Function:       // DjkbWdmAddDevice
* Description:    // ������豸
* Input:          //
*       DriverObject:��I/O�������д���������������
*       PhysicalDeviceObject:��I/O�������д������������豸����
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
NTSTATUS DjkbWdmAddDevice( IN PDRIVER_OBJECT DriverObject, IN PDEVICE_OBJECT PhysicalDeviceObject )
{
	NTSTATUS			status;
	PDEVICE_OBJECT		fdo;
	PDEVICE_EXTENSION	pdx;
	UNICODE_STRING		devName;
	UNICODE_STRING		symLinkName;
	RtlInitUnicodeString( &devName, DEVICE_NAME );

	KdPrint( ( "[ddb_main.c]:Enter DDBWDMAddDevice()\n" ) );
	status = IoCreateDevice( DriverObject, sizeof( DEVICE_EXTENSION ), &devName, FILE_DEVICE_UNKNOWN, 0, FALSE, &fdo );
	if( !NT_SUCCESS( status ) )
	{
		return status;
	}
	pdx					   = (PDEVICE_EXTENSION)fdo->DeviceExtension;
	pdx->fdo			   = fdo;
	pdx->PhyDevice		   = PhysicalDeviceObject;
	pdx->NextStackDevice   = IoAttachDeviceToDeviceStack( fdo, PhysicalDeviceObject );
	RtlInitUnicodeString( &symLinkName, DEVICE_SYMLINKNAME );

	pdx->ustrDeviceName	   = devName;
	pdx->ustrSymLinkName   = symLinkName;
	status				   = IoCreateSymbolicLink( &symLinkName, &devName ); //��������

	if( !NT_SUCCESS( status ) )
	{
		IoDeleteSymbolicLink( &pdx->ustrSymLinkName );
		status = IoCreateSymbolicLink( &symLinkName, &devName );
		if( !NT_SUCCESS( status ) )
		{
			return status;
		}
	}

	fdo->Flags	  |= DO_BUFFERED_IO | DO_POWER_PAGABLE;
	fdo->Flags	  &= ~DO_DEVICE_INITIALIZING;

	KdPrint( ( "[ddb_main.c]:Leave DDBWDMAddDevice()\n" ) );
	return STATUS_SUCCESS;
}

/***********************************************************
* Function:       // DefaultPnpHandler
* Description:    // ��PNP IRP����ȱʡ����
* Input:          // 1.pdx:�豸�������չ
*                         // 2.Irp:��IO�����
* Return:         //
* Others:         //
***********************************************************/
NTSTATUS DefaultPnpHandler( PDEVICE_EXTENSION pdx, PIRP Irp )
{
	KdPrint( ( "[ddb_main.c]:Enter DefaultPnpHandler()\n" ) );
	IoSkipCurrentIrpStackLocation( Irp );
	KdPrint( ( "[ddb_main.c]:Leave DefaultPnpHandler()\n" ) );
	return IoCallDriver( pdx->NextStackDevice, Irp );
}

/***********************************************************
* Function:       // OnRequestComplete
* Description:    // �������ܡ����ܵȵ�����
* Input1:         // 1.�������1,˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input2:         // 2.�������2,˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.���������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
NTSTATUS OnRequestComplete( PDEVICE_OBJECT junk, PIRP Irp, PKEVENT pev )
{
	//��������������õȴ��¼�
	KeSetEvent( pev, 0, FALSE );
	//��־��IRP����Ҫ�ٴα����
	return STATUS_MORE_PROCESSING_REQUIRED;
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input1:         // 1.�������1,˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input2:         // 2.�������2,˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.���������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
NTSTATUS ForwardAndWait( PDEVICE_EXTENSION pdx, PIRP Irp )
{       // ForwardAndWait
	KEVENT event;
	//��ʼ���¼�
	KeInitializeEvent( &event, NotificationEvent, FALSE );

	//�������ջ��������һ���ջ
	IoCopyCurrentIrpStackLocationToNext( Irp );
	//�����������
	IoSetCompletionRoutine( Irp, (PIO_COMPLETION_ROUTINE)OnRequestComplete, (PVOID)&event, TRUE, TRUE, TRUE );

	//���õײ���������PDO
	IoCallDriver( pdx->NextStackDevice, Irp );
	//�ȴ�PDO���
	KeWaitForSingleObject( &event, Executive, KernelMode, FALSE, NULL );
	return Irp->IoStatus.Status;
}                                                       // ForwardAndWait

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input1:         // 1.�������1,˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input2:         // 2.�������2,˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.���������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
VOID ShowResources( IN PCM_PARTIAL_RESOURCE_LIST list )
{                                                       //ö����Դ
	ULONG							i;
	ULONG							nres = list->Count; //Descriptors,��������Ŀ

	PCM_PARTIAL_RESOURCE_DESCRIPTOR resource = list->PartialDescriptors;

	for( i = 0; i < nres; ++i, ++resource )
	{                                                   // for each resource
		ULONG		type = resource->Type;

		static char * name[] = {
			"CmResourceTypeNull",
			"CmResourceTypePort",
			"CmResourceTypeInterrupt",
			"CmResourceTypeMemory",
			"CmResourceTypeDma",
			"CmResourceTypeDeviceSpecific",
			"CmResourceTypeBusNumber",
			"CmResourceTypeDevicePrivate",
			"CmResourceTypeAssignedResource",
			"CmResourceTypeSubAllocateFrom",
		};

		KdPrint( ( "type=%d, typeName=%s \n", type, type < arraysize( name ) ? name[type] : "unknown" ) );

		switch( type )
		{   // select on resource type
			case CmResourceTypePort:
			case CmResourceTypeMemory:
				KdPrint( ( "CmResourceTypeMemory ===> start 0x%lX 0x%lX length:%d\n",
				           resource->u.Port.Start.HighPart,
				           resource->u.Port.Start.LowPart,
				           resource->u.Port.Length ) );
				break;
			case CmResourceTypeBusNumber:
				KdPrint( ( "CmResourceTypeBusNumber:::" ) );
				break;
			case CmResourceTypeInterrupt:
				KdPrint( ( "CmResourceTypeInterrupt ===> level:%X, vector:%X, affinity:%X\n",
				           resource->u.Interrupt.Level,
				           resource->u.Interrupt.Vector,
				           resource->u.Interrupt.Affinity ) );
				break;

			case CmResourceTypeDma:
				KdPrint( ( "CmResourceTypeDma ===> channel %d, port %X\n", resource->u.Dma.Channel, resource->u.Dma.Port ) );
		} // select on resource type
	}    // for each resource
}       // ShowResources

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // 1.�������1��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input:          // 2.�������2��˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.�������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
BOOLEAN OnInterrupt( PKINTERRUPT InterruptObject, PDEVICE_EXTENSION pdx )
{   // OnInterrupt
	//���ж�
	//UCHAR HSR = READ_PORT_UCHAR( pdx->portbase );
	//HSR = HSR | 0x4;
	//WRITE_PORT_UCHAR( pdx->portbase, HSR );

	KdPrint( ( "==============interrupt!!!\n" ) );

	//�ָ��ж��źŵ�ƽ
	//WRITE_REGISTER_UCHAR( (PUCHAR)pdx->MemBar1 + 0x400000, 0x10 );

	//IoRequestDpc( pdx->fdo, NULL, pdx );

	return TRUE;
}

/***********************************************************
* Function:       // InitMyPCI
* Description:    // �������ܡ����ܵȵ�����
* Input1:         // 1.�������1,˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input2:         // 2.�������2,˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.���������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
NTSTATUS InitMyPCI( IN PDEVICE_EXTENSION pdx, IN PCM_PARTIAL_RESOURCE_LIST list )
{
	BYTE							PciData[2];
	BYTE							hightbyte;
	ULONG							i;
	ULONG							nres;
	ULONG							nRegAddr;
	ULONG							vector;
	ULONG							BusNumber		   = 0;
	ULONG							length			   = 0;
	ULONG							propertyAddress	   = 0;
	USHORT							FunctionNumber	   = 0;
	USHORT							DeviceNumber	   = 0;
	BOOLEAN							irqshare;
	BOOLEAN							gotport		   = FALSE;
	BOOLEAN							gotinterrupt   = FALSE;
	BOOLEAN							IsMem0		   = TRUE;
	ULONG							typeMemory	   = 0;
	ULONG							wCardID		   = 0;
	NTSTATUS						status;
	KIRQL							irql;
	KAFFINITY						affinity;
	KINTERRUPT_MODE					mode;
	PHYSICAL_ADDRESS				portbase;
	PCI_COMMON_CONFIG				pci_config;

	PDEVICE_OBJECT					fdo		   = pdx->fdo;
	PCM_PARTIAL_RESOURCE_DESCRIPTOR resource   = list->PartialDescriptors;
	KdPrint( ( "---------------- Init the PCI card begin!!! ---------------------------------------------\n" ) );

	nres	   = list->Count;
	wCardID	   = g_AllDevInfo.wCardNum;
	//��ȡPCI���ÿռ�
	status = ReadWriteConfigSpace( fdo, 0, &pci_config, 0, sizeof( PCI_COMMON_CONFIG ) );

	if( NT_SUCCESS( status ) )
	{
		KdPrint( ( "======================PCI_COMMON_CONFIG Begin=====================\n" ) );
		KdPrint( ( "==VendorID:%x\n", pci_config.VendorID ) );
		KdPrint( ( "==DeviceID:%x\n", pci_config.DeviceID ) );
		KdPrint( ( "==Command:%x\n", pci_config.Command ) );
		KdPrint( ( "==Status:%x\n", pci_config.Status ) );
		KdPrint( ( "==RevisionID:%x\n", pci_config.RevisionID ) );
		KdPrint( ( "==ProgIf:%x\n", pci_config.ProgIf ) );
		KdPrint( ( "==SubClass:%x\n", pci_config.SubClass ) );
		KdPrint( ( "==BaseClass:%x\n", pci_config.BaseClass ) );
		KdPrint( ( "==CacheLineSize:%x\n", pci_config.CacheLineSize ) );
		KdPrint( ( "==LatencyTimer:%x\n", pci_config.LatencyTimer ) );
		KdPrint( ( "==HeaderType:%x\n", pci_config.HeaderType ) );
		KdPrint( ( "==BIST:%x\n", pci_config.BIST ) );
		for( i = 0; i < 6; i++ )
		{
			KdPrint( ( "==BaseAddresses[%d]:0x%08X\n", i, pci_config.u.type0.BaseAddresses[i] ) );
		}
		KdPrint( ( "==InterruptLine:%d\n", pci_config.u.type0.InterruptLine ) );
		KdPrint( ( "==InterruptPin:%d\n", pci_config.u.type0.InterruptPin ) );
		KdPrint( ( "======================PCI_COMMON_CONFIG End=====================\n" ) );

		if( ( pci_config.VendorID == 0X104C ) && ( pci_config.DeviceID == 0XAC60 ) )    //is C2040
		{
			hightbyte  = ( pci_config.u.type0.SubVendorID >> 8 );
			PciData[0] = hightbyte & 0xFF;
			PciData[1] = pci_config.u.type0.SubVendorID & 0xFF;

			if( PciData[1] == 'D' && PciData[0] == 'J' )                                //is DJ Card
			{
				status = IoGetDeviceProperty( pdx->PhyDevice, DevicePropertyBusNumber, sizeof( ULONG ), (PVOID)&BusNumber, &length );
				if( !NT_SUCCESS( status ) )
				{
					KdPrint( ( "IoGetDeviceProperty DeviceObject status = %x\n", status ) );
					STATUS_BUFFER_TOO_SMALL;
				}

				status = IoGetDeviceProperty( pdx->PhyDevice, DevicePropertyAddress, sizeof( ULONG ), (PVOID)&propertyAddress, &length );
				if( !NT_SUCCESS( status ) )
				{
					KdPrint( ( "IoGetDeviceProperty DevicePropertyAddress status = %x\n", status ) );
				}

				FunctionNumber = (USHORT)( ( propertyAddress ) & 0x0000FFFF );
				DeviceNumber   = (USHORT)( ( ( propertyAddress ) >> 16 ) & 0x0000FFFF );

				g_AllDevInfo.cardInfo[wCardID].wBusNum		   = (WORD)BusNumber;
				g_AllDevInfo.cardInfo[wCardID].wDeviceNum	   = (WORD)DeviceNumber;
				g_AllDevInfo.cardInfo[wCardID].wFunctionNum	   = (WORD)FunctionNumber;

				KdPrint( ( "CARD INFO ===> cardid=%d, bus=%d, DeviceNum=%d, FuncNum=%d \n", wCardID, BusNumber, DeviceNumber, FunctionNumber ) );
				//�ڴ�ӳ�䣬��PCI���ÿռ��ж�ȡ�Ĵ�����ַ
				nRegAddr											   = pci_config.u.type0.BaseAddresses[CSR_MEM_ID];
				g_AllDevInfo.cardInfo[wCardID].pBaseMem[CSR_MEM_ID]	   = nRegAddr;
				g_AllDevInfo.cardInfo[wCardID].dwMemSize[CSR_MEM_ID]   = 4 * 1024;
				//g_AllDevInfo.cardInfo[wCardID].pMapMem[CSR_MEM_ID]	   = C2040_MapMem( wCardID, nRegAddr, g_AllDevInfo.cardInfo[wCardID].dwMemSize[CSR_MEM_ID] );

				nRegAddr											   = pci_config.u.type0.BaseAddresses[HPI_MEM_ID];
				g_AllDevInfo.cardInfo[wCardID].pBaseMem[HPI_MEM_ID]	   = nRegAddr;
				g_AllDevInfo.cardInfo[wCardID].dwMemSize[HPI_MEM_ID]   = 32 * 1024;
				//g_AllDevInfo.cardInfo[wCardID].pMapMem[HPI_MEM_ID]	   = C2040_MapMem( wCardID, nRegAddr, g_AllDevInfo.cardInfo[wCardID].dwMemSize[HPI_MEM_ID] );

				nRegAddr											   = pci_config.u.type0.BaseAddresses[GPIO_MEM_ID];
				g_AllDevInfo.cardInfo[wCardID].pBaseMem[GPIO_MEM_ID]   = nRegAddr;
				g_AllDevInfo.cardInfo[wCardID].dwMemSize[GPIO_MEM_ID]  = 256;
				//g_AllDevInfo.cardInfo[wCardID].pMapMem[GPIO_MEM_ID]	   = C2040_MapMem( wCardID, nRegAddr, g_AllDevInfo.cardInfo[wCardID].dwMemSize[GPIO_MEM_ID] );

				g_AllDevInfo.cardInfo[wCardID].cbCardType = (BYTE)pci_config.u.type0.SubVendorID; //�豸����

				++g_AllDevInfo.wCardNum;
			}
		}
	}
	//������Դ
	for( i = 0; i < nres; ++i, ++resource )
	{       // for each resource
		ULONG type = resource->Type;

		switch( type )
		{   // select on resource type
			case CmResourceTypePort:
				portbase		   = resource->u.Port.Start;
				pdx->nports		   = resource->u.Port.Length;
				pdx->mappedport	   = ( resource->Flags & CM_RESOURCE_PORT_IO ) == 0;
				gotport			   = TRUE;
				break;

			case CmResourceTypeMemory:
				KdPrint( ( "CmResourceTypeMemory ===> HighPart:%lX LowPart:%lX len:%d\n", resource->u.Port.Start.HighPart, resource->u.Port.Start.LowPart, resource->u.Port.Length ) );
				break;
			case CmResourceTypeBusNumber:
				KdPrint( ( "CmResourceTypeBusNumber " ) );
				break;
			case CmResourceTypeInterrupt:
				irql		   = (KIRQL)resource->u.Interrupt.Level;
				vector		   = resource->u.Interrupt.Vector;
				affinity	   = resource->u.Interrupt.Affinity;
				mode		   = ( resource->Flags == CM_RESOURCE_INTERRUPT_LATCHED ) ? Latched : LevelSensitive;
				irqshare	   = resource->ShareDisposition == CmResourceShareShared;
				gotinterrupt   = TRUE;
				KdPrint( ( "CmResourceTypeInterrupt ===> irqlevel:%X, vector:%X, affinity:%X, mode:%X, irqshare:%X \n",
				           irql, vector, affinity, mode, irqshare ) );

				break;
			case CmResourceTypeDma:
				KdPrint( ( "CmResourceTypeDma ===> channel %d, port %X\n", resource->u.Dma.Channel, resource->u.Dma.Port ) );
				break;
			default:
				KdPrint( ( "Unexpected I/O resource type %d\n", resource->Type ) );
				break;
		}
	}
	//�����ж�
	status = IoConnectInterrupt( &pdx->InterruptObject, (PKSERVICE_ROUTINE)OnInterrupt,
	                             (PVOID)pdx, NULL, vector, irql, irql, LevelSensitive, TRUE, affinity, FALSE );
	if( !NT_SUCCESS( status ) )
	{
		KdPrint( ( "IoConnectInterrupt failed! - %X\n", status ) );
		return status;
	}else
	{
		KdPrint( ( "IoConnectInterrupt() Success\n" ) );
	}

	KdPrint( ( "---------------- Init the PCI card end!!!   -------------------------------------------------\n" ) );

	return STATUS_SUCCESS;
}

/***********************************************************
* Function:       // HandleStartDevice
* Description:    // �������ܡ����ܵȵ�����
* Input-1:        // 1.pdx
* Input-2:        // 2.Irp
* Output:         // 1.
* Return:         //
* Others:         //
***********************************************************/
NTSTATUS HandleStartDevice( PDEVICE_EXTENSION pdx, PIRP Irp )
{
	NTSTATUS						status;
	PIO_STACK_LOCATION				stack;
	PCM_PARTIAL_RESOURCE_LIST		raw;
	PCM_PARTIAL_RESOURCE_LIST		translated;
	PCM_FULL_RESOURCE_DESCRIPTOR	pfrd;
	status = ForwardAndWait( pdx, Irp );        //ת��IRP���ȴ�����
	KdPrint( ( "[ddb_main.c]:Enter HandleStartDevice()\n" ) );
	if( !NT_SUCCESS( status ) )                 //�ж�IRP�Ƿ��ڵײ������гɹ�����
	{
		Irp->IoStatus.Status = status;
		IoCompleteRequest( Irp, IO_NO_INCREMENT );
		return status;
	}

	//�õ���ǰ��ջ
	stack = IoGetCurrentIrpStackLocation( Irp );


	/*�ӵ�ǰ��ջ�õ�Դ��Ϣ
	   if( stack->Parameters.StartDevice.AllocatedResources )
	   {
	   raw = &stack->Parameters.StartDevice.AllocatedResources->List[0].PartialResourceList;
	   }else
	   {
	   raw = NULL;
	   }
	   KdPrint( ( "Show raw resources-----------------------\n" ) );
	   ShowResources( raw );*/
	//�ӵ�ǰ��ջ�õ�������Ϣ
	if( stack->Parameters.StartDevice.AllocatedResourcesTranslated )
	{
		translated = &stack->Parameters.StartDevice.AllocatedResourcesTranslated->List[0].PartialResourceList;
		pfrd	   = &stack->Parameters.StartDevice.AllocatedResourcesTranslated->List[0];
	} else
	{
		translated = NULL;
	}
	//Show translated resources
	//ShowResources( translated );

	InitMyPCI( pdx, translated );

	//���IRP
	Irp->IoStatus.Status = STATUS_SUCCESS;
	IoCompleteRequest( Irp, IO_NO_INCREMENT );

	KdPrint( ( "[ddb_main.c]:Leave HandleStartDevice()\n" ) );
	return status;
}

/***********************************************************
* Function:       // HandleRemoveDevice
* Description:    // ��IRP_MN_REMOVE_DEVICE IRP���д���
* Input:          // 1.fdo:�����豸����
*			      // 2.Irp:��IO�����
* Return:         // ����״̬
* Others:         //
***********************************************************/
NTSTATUS HandleRemoveDevice( PDEVICE_EXTENSION pdx, PIRP Irp )
{
	NTSTATUS status;
	Irp->IoStatus.Status   = STATUS_SUCCESS;
	status				   = DefaultPnpHandler( pdx, Irp );
	KdPrint( ( "[ddb_main.c]:Enter HandleRemoveDevice()\n" ) );

	IoDeleteSymbolicLink( &pdx->ustrSymLinkName );

	//����IoDetachDevice()��fdo���豸ջ���ѿ���
	if( pdx->NextStackDevice )
	{
		IoDetachDevice( pdx->NextStackDevice );
	}

	//ɾ��fdo��
	IoDeleteDevice( pdx->fdo );
	KdPrint( ( "[ddb_main.c]:Leave HandleRemoveDevice()\n" ) );
	return status;
}

/***********************************************************
* Function:       // DDBWDMPnp
* Description:    // �Լ��弴��IRP���д���
* Input:          // 1.fdo:�����豸����
*			      // 2.Irp:��IO�����
* Return:         // ����״̬
* Others:         //
***********************************************************/
NTSTATUS DjkbWdmPnp( IN PDEVICE_OBJECT fdo, IN PIRP Irp )
{
	NTSTATUS			status = STATUS_SUCCESS;
	PDEVICE_EXTENSION	pdx	   = (PDEVICE_EXTENSION)fdo->DeviceExtension;
	PIO_STACK_LOCATION	stack  = IoGetCurrentIrpStackLocation( Irp );
	static NTSTATUS		( *fcntab[] )( PDEVICE_EXTENSION pdx, PIRP Irp ) =
	{
		HandleStartDevice,  // IRP_MN_START_DEVICE
		DefaultPnpHandler,  // IRP_MN_QUERY_REMOVE_DEVICE
		HandleRemoveDevice, // IRP_MN_REMOVE_DEVICE
		DefaultPnpHandler,  // IRP_MN_CANCEL_REMOVE_DEVICE
		DefaultPnpHandler,  // IRP_MN_STOP_DEVICE
		DefaultPnpHandler,  // IRP_MN_QUERY_STOP_DEVICE
		DefaultPnpHandler,  // IRP_MN_CANCEL_STOP_DEVICE
		DefaultPnpHandler,  // IRP_MN_QUERY_DEVICE_RELATIONS
		DefaultPnpHandler,  // IRP_MN_QUERY_INTERFACE
		DefaultPnpHandler,  // IRP_MN_QUERY_CAPABILITIES
		DefaultPnpHandler,  // IRP_MN_QUERY_RESOURCES
		DefaultPnpHandler,  // IRP_MN_QUERY_RESOURCE_REQUIREMENTS
		DefaultPnpHandler,  // IRP_MN_QUERY_DEVICE_TEXT
		DefaultPnpHandler,  // IRP_MN_FILTER_RESOURCE_REQUIREMENTS
		DefaultPnpHandler,  //
		DefaultPnpHandler,  // IRP_MN_READ_CONFIG
		DefaultPnpHandler,  // IRP_MN_WRITE_CONFIG
		DefaultPnpHandler,  // IRP_MN_EJECT
		DefaultPnpHandler,  // IRP_MN_SET_LOCK
		DefaultPnpHandler,  // IRP_MN_QUERY_ID
		DefaultPnpHandler,  // IRP_MN_QUERY_PNP_DEVICE_STATE
		DefaultPnpHandler,  // IRP_MN_QUERY_BUS_INFORMATION
		DefaultPnpHandler,  // IRP_MN_DEVICE_USAGE_NOTIFICATION
		DefaultPnpHandler,  // IRP_MN_SURPRISE_REMOVAL
	};
	static char			* fcnname[] =
	{
		"IRP_MN_START_DEVICE",
		"IRP_MN_QUERY_REMOVE_DEVICE",
		"IRP_MN_REMOVE_DEVICE",
		"IRP_MN_CANCEL_REMOVE_DEVICE",
		"IRP_MN_STOP_DEVICE",
		"IRP_MN_QUERY_STOP_DEVICE",
		"IRP_MN_CANCEL_STOP_DEVICE",
		"IRP_MN_QUERY_DEVICE_RELATIONS",
		"IRP_MN_QUERY_INTERFACE",
		"IRP_MN_QUERY_CAPABILITIES",
		"IRP_MN_QUERY_RESOURCES",
		"IRP_MN_QUERY_RESOURCE_REQUIREMENTS",
		"IRP_MN_QUERY_DEVICE_TEXT",
		"IRP_MN_FILTER_RESOURCE_REQUIREMENTS",
		"",
		"IRP_MN_READ_CONFIG",
		"IRP_MN_WRITE_CONFIG",
		"IRP_MN_EJECT",
		"IRP_MN_SET_LOCK",
		"IRP_MN_QUERY_ID",
		"IRP_MN_QUERY_PNP_DEVICE_STATE",
		"IRP_MN_QUERY_BUS_INFORMATION",
		"IRP_MN_DEVICE_USAGE_NOTIFICATION",
		"IRP_MN_SURPRISE_REMOVAL",
	};
	ULONG				fcn = stack->MinorFunction;
	KdPrint( ( "[ddb_main.c]:Enter DDBWDMPnp()\n" ) );

	if( fcn >= arraysize( fcntab ) )
	{   // δ֪���ӹ��ܴ���
		status = DefaultPnpHandler( pdx, Irp );
		return status;
	}

	KdPrint( ( "PNP Request (%s)\n", fcnname[fcn] ) );

	status = ( *fcntab[fcn] )( pdx, Irp );
	KdPrint( ( "[ddb_main.c]:Leave DDBWDMPnp()\n" ) );
	return status;
}

/***********************************************************
* Function:       // DDBWDMDispatchRoutine
* Description:    // ��ȱʡIRP���д���
* Input:          // 1.fdo:�����豸����
*			      // 2.Irp:��IO�����
* Return:         // ����״̬
* Others:         //
***********************************************************/
NTSTATUS DjkbWdmDispatchRoutine( IN PDEVICE_OBJECT fdo, IN PIRP Irp )
{
	KdPrint( ( "[ddb_main.c]:Enter DDBWDMDispatchRoutine()\n" ) );
	Irp->IoStatus.Status	   = STATUS_SUCCESS;
	Irp->IoStatus.Information  = 0; // no bytes xfered
	IoCompleteRequest( Irp, IO_NO_INCREMENT );
	KdPrint( ( "[ddb_main.c]:Leave DDBWDMDispatchRoutine()\n" ) );
	return STATUS_SUCCESS;
}

/***********************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // �������˵��������ÿ�����������á�ȡֵ˵�����������ϵ
*       1.----
* Output:         // �����������˵��
*       1.----
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
NTSTATUS ReadWriteConfigSpace(
    IN PDEVICE_OBJECT DeviceObject,
    IN ULONG ReadOrWrite, // 0 for read 1 for write
    IN PVOID Buffer,
    IN ULONG Offset,
    IN ULONG Length
    )
{
	KEVENT				event;
	NTSTATUS			status;
	PIRP				irp;
	IO_STATUS_BLOCK		ioStatusBlock;
	PIO_STACK_LOCATION	irpStack;
	PDEVICE_OBJECT		targetObject;

	PAGED_CODE( );

	KeInitializeEvent( &event, NotificationEvent, FALSE );

	targetObject = IoGetAttachedDeviceReference( DeviceObject );

	irp = IoBuildSynchronousFsdRequest( IRP_MJ_PNP, targetObject, NULL, 0, NULL, &event, &ioStatusBlock );

	if( irp == NULL )
	{
		status = STATUS_INSUFFICIENT_RESOURCES;
		goto End;
	}

	irpStack = IoGetNextIrpStackLocation( irp );

	if( ReadOrWrite == 0 )
	{
		irpStack->MinorFunction = IRP_MN_READ_CONFIG;
	}else
	{
		irpStack->MinorFunction = IRP_MN_WRITE_CONFIG;
	}

	irpStack->Parameters.ReadWriteConfig.WhichSpace	   = PCI_WHICHSPACE_CONFIG;
	irpStack->Parameters.ReadWriteConfig.Buffer		   = Buffer;
	irpStack->Parameters.ReadWriteConfig.Offset		   = Offset;
	irpStack->Parameters.ReadWriteConfig.Length		   = Length;

	//
	// Initialize the status to error in case the bus driver does not
	// set it correctly.
	//

	irp->IoStatus.Status = STATUS_NOT_SUPPORTED;

	status = IoCallDriver( targetObject, irp );

	if( status == STATUS_PENDING )
	{
		KeWaitForSingleObject( &event, Executive, KernelMode, FALSE, NULL );
		status = ioStatusBlock.Status;
	}

End:
	// Done with reference
	ObDereferenceObject( targetObject );

	return status;
}

/***********************************************************
* Function:       // DDBWDMDeviceControl
* Description:    // �������ܡ����ܵȵ�����
* Input1:         // 1.�������1,˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Input2:         // 2.�������2,˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* Output:         // 1.���������1��˵��
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
***********************************************************/
NTSTATUS DjkbWdmDeviceControl( IN PDEVICE_OBJECT fdo, IN PIRP Irp )
{
	ULONG				cbin;
	ULONG				cbout;
	ULONG				code;
	ULONG				info = 0;
	PVOID				ioBuffer;
	NTSTATUS			status = STATUS_SUCCESS;
	PMSG_HEAD_STRUCT	pMsg;
	PDEVICE_EXTENSION	pdx;
	PIO_STACK_LOCATION	stack;

	KdPrint( ( "[ddb_main.c]:Enter DDBWDMDeviceControl()\n" ) );

	pdx		   = (PDEVICE_EXTENSION)fdo->DeviceExtension;
	stack	   = IoGetCurrentIrpStackLocation( Irp );
	ioBuffer   = Irp->AssociatedIrp.SystemBuffer;
	pMsg	   = ( PMSG_HEAD_STRUCT )ioBuffer;                      //��Ϣ�ṹ��
	cbin	   = stack->Parameters.DeviceIoControl.InputBufferLength;
	cbout	   = stack->Parameters.DeviceIoControl.OutputBufferLength;
	code	   = stack->Parameters.DeviceIoControl.IoControlCode;   //�õ�IOCTL��

	if( stack->MajorFunction != IRP_MJ_DEVICE_CONTROL )             //IRP������
	{
		return status;
	}
	//DdbDispatchCMD( code, pMsg );                                           //����ַ���ȥ

	if( code != IOCTRL_DJKB || pMsg == NULL )
	{
		return status;
	}

	KdPrint( ( "============= code=%d, m_lMsgType=%d ===================\n", code, pMsg->m_lMsgType ) );

	switch( pMsg->m_lMsgType )
	{
		case DJKB_MSG_ALL_CARD_INFO:
		{
			if( pMsg->m_pData == NULL )
			{
				pMsg->m_lRetVal = 0;
				return status;
			}
			memcpy( pMsg->m_pData, &g_AllDevInfo, sizeof( ALL_DEVICE_INFO ) );
			pMsg->m_lRetVal = 1;
			KdPrint( ( "[ddb_main.c]: DdbGetAllCardInfo ok.\n" ) );
		}
		break;
		case DJKB_MSG_PCI_CONFIG_READ:
		{
			int					i = 0;
			PCI_COMMON_CONFIG	pci_config;
			if( pMsg->m_pData == NULL || pMsg->m_lParam1 < 0 || pMsg->m_lParam1 >= g_AllDevInfo.wCardNum )
			{
				pMsg->m_lRetVal = 0;
				return status;
			}
			status = ReadWriteConfigSpace( fdo, 0, &pci_config, 0, sizeof( PCI_COMMON_CONFIG ) ); //�ڶ������� 0:read 1:write

			if( NT_SUCCESS( status ) )
			{
				KdPrint( ( "======================PCI_COMMON_CONFIG Begin=====================\n" ) );
				KdPrint( ( "==VendorID:%x\n", pci_config.VendorID ) );
				KdPrint( ( "==DeviceID:%x\n", pci_config.DeviceID ) );
				KdPrint( ( "==Command:%x\n", pci_config.Command ) );
				KdPrint( ( "==Status:%x\n", pci_config.Status ) );
				KdPrint( ( "==RevisionID:%x\n", pci_config.RevisionID ) );
				KdPrint( ( "==ProgIf:%x\n", pci_config.ProgIf ) );
				KdPrint( ( "==SubClass:%x\n", pci_config.SubClass ) );
				KdPrint( ( "==BaseClass:%x\n", pci_config.BaseClass ) );
				KdPrint( ( "==CacheLineSize:%x\n", pci_config.CacheLineSize ) );
				KdPrint( ( "==LatencyTimer:%x\n", pci_config.LatencyTimer ) );
				KdPrint( ( "==HeaderType:%x\n", pci_config.HeaderType ) );
				KdPrint( ( "==BIST:%x\n", pci_config.BIST ) );
				for( i = 0; i < 6; i++ )
				{
					KdPrint( ( "==BaseAddresses[%d]:0X%08X\n", i, pci_config.u.type0.BaseAddresses[i] ) );
				}
				KdPrint( ( "==InterruptLine:%d\n", pci_config.u.type0.InterruptLine ) );
				KdPrint( ( "==InterruptPin:%d\n", pci_config.u.type0.InterruptPin ) );
				KdPrint( ( "======================PCI_COMMON_CONFIG End=====================\n" ) );
			}
			memcpy( pMsg->m_pData, &pci_config, sizeof( PCI_COMMON_CONFIG ) );
			pMsg->m_lRetVal = 1;
			KdPrint( ( "[ddb_main.c]: DJDDB_MSG_PCI_CONFIG_READ ok.\n" ) );
		}
		break;
		case  DJKB_MSG_PCI_CONFIG_WRITE:
		{
			PCI_COMMON_CONFIG pci_config;

			if( pMsg->m_lParam1 < 0 || pMsg->m_lParam1 >= g_AllDevInfo.wCardNum
			    || pMsg->m_lParam2 < 0 || pMsg->m_lParam2 >= MAX_PCI_CONFIG_SIZE )
			{
				pMsg->m_lRetVal = 0;
				return status;
			}

			sizeof( BYTE );                                                                             //��С
			pMsg->m_lParam2;                                                                            //λ��
			pMsg->m_lParam3;                                                                            //ֵ

			status = ReadWriteConfigSpace( fdo, 1, &pMsg->m_lParam3, pMsg->m_lParam2, sizeof( BYTE ) ); //�ڶ������� 0:read 1:write

			pMsg->m_lRetVal = 1;
			KdPrint( ( "[ddb_main.c]: DdbPciConfigWrite ok! pos:%X  val:%X\n", pMsg->m_lParam2, pMsg->m_lParam3 ) );
		}

		break;/*
		case DJDDB_MSG_2040_READ:
			Ddb2040Read( pMsg );
			break;
		case DJDDB_MSG_2040_WRITE:
			Ddb2040Write( pMsg );
			break;
		case  DJDDB_MSG_SEND_PACKET:
			DdbSendPacket( pMsg );
			break;
		case DJDDB_MSG_DSP_READ:
			break;*/
	}
	status					   = Irp->IoStatus.Status;
	//Irp->IoStatus.Information  = Msg_GetTotalLen( pMsg );
	IoCompleteRequest( Irp, IO_NO_INCREMENT );

	KdPrint( ( "[ddb_main.c]:Leave DDBWDMDeviceControl()\n" ) );
	return status;
}

/***********************************************************
* Function:       // DDBWDMUnload
* Description:    // �������������ж�ز���
* Input:          // 1.DriverObject:��������
* Return:         // ����״̬
* Others:         //
***********************************************************/
void DjkbWdmUnload( IN PDRIVER_OBJECT DriverObject )
{
	KdPrint( ( "[ddb_main.c]:Enter DDBWDMUnload()\n" ) );
	KdPrint( ( "[ddb_main.c]:Leave DDBWDMUnload()\n" ) );
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
void DjkbWdmDelay( long millsecond )
{
	LARGE_INTEGER	delayValue, delayTrue;
	NTSTATUS		ntRet;

	// 10*1000*1000 is 1 second, so 10*1000 is 1 millsecond
	delayValue.QuadPart	   = 10 * 1000 * millsecond; // 320 millisecond
	delayTrue.QuadPart	   = -( delayValue.QuadPart );
	ntRet				   = KeDelayExecutionThread( KernelMode, FALSE, &delayTrue );
}

/************************************** The End Of File **************************************/

