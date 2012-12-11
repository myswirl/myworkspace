#ifndef _XMS_DIAL_SUB_H_
#define _XMS_DIAL_SUB_H_

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"

#define	MAX_FILE_NAME_LEN		256
#define MAX_E1_NUM				10
enum TRUNK_STATE {
	TRK_WAITOPEN = 0,
	TRK_FREE,
	TRK_RING,
	TRK_NOTHANDLE,
	TRK_CALLOUT,  //带语音检测
	TRK_CALLOUT1, //纯信令接续
	TRK_WAIT_LINKOK,
	TRK_PLAY,
	TRK_GETDTMF,
	TRK_FAIL,
	TRK_WAITHANGUP,
	TRK_WAITSTOP,

	TRK_RECORDFILE,
	TRK_PLAYRECORD,	

	// new add for XMS
	TRK_WAIT_ANSWERCALL,

};

typedef struct
{
	// ----------------
	DeviceID_t	deviceID;
	int			iSeqID;
	int			iModSeqID;
	int			iLineState;

	DeviceID_t	VocDevID;

	DJ_U8		u8PlayTag;	

	// -----------------
	TRUNK_STATE	State;


	int		DtmfCount;
	char	DtmfBuf[32];

	char CallerCode[20];
	char CalleeCode[20];

	int     iTotalCnt;
	int     iSucCnt;
	int     iCallErrCnt;
	int     iCheckErrCnt;
	int     iEstablishTime;
	int     iCallOutTime;
} TRUNK_STRUCT;

enum VOICE_STATE {
	VOC_WAITOPEN,
	VOC_FREE,
	VOC_USED
} ;

typedef struct
{
	// ----------------
	DeviceID_t	deviceID;
	int			iSeqID;

	DeviceID_t	UsedDevID;

	// ----------------
	VOICE_STATE	State;

} VOICE_STRUCT;

typedef struct
{
	// ----------------
	DeviceID_t	deviceID;
	int			iSeqID;
	//
	DJ_U8		u8E1Type;
	DJ_S32		s32AlarmVal;
} PCM_STRUCT;

// ----- decalure function -----
bool	InitSystem(void);
void	ExitSystem(void); 

void	OpenAllDevice_Dsp ( DJ_S8 s8DspModID );
void	CloseAllDevice_Dsp ( DJ_S8 s8DspModID );

void	HandleDevState ( Acs_Evt_t *pAcsEvt );

void	ReDrawAll (void);
void	OpenDeviceOK ( DeviceID_t *pDevice );

void	AnalogUserWork ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt );

void	AnalogTrunkWork ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt );
void	AnalogRecWork ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt );

void	TrunkWork_ISDN_CALLER ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt );
void	TrunkWork_CAS_CALLER ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt );

void	TrunkWork_ISDN_CALLED ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt );
void	TrunkWork_CAS_CALLED ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt );

void	AnalogTrunkWork ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt );

void	Change_State ( TRUNK_STRUCT *pOneTrunk, TRUNK_STATE NewState );

void	InitTrunkChannel ( TRUNK_STRUCT *pOneTrunk );

void    InitTrunkcounter(TRUNK_STRUCT *pOneTrunk);

void	SimulateCallOut(void);

void   TrunkWork ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt );

void    SetParam_AnalogTrunk(DeviceID_t *Dev);//设置模拟中继参数

void AddMsg ( char *str);


extern int				cfg_iCallerE1[MAX_E1_NUM];//主叫E1号
extern int				cfg_iCalledE1[MAX_E1_NUM];//被叫E1号char				

#endif