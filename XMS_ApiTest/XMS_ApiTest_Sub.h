#ifndef _XMS_APITEST_SUB_H_
#define _XMS_APITEST_SUB_H_

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"

typedef struct 
{
	DeviceID_t	DevID;
	bool		bOpenFlag;
	void*		pDevData;
} TYPE_MY_DEVICE_STRUCT;

int InitSystem( void );

#endif

