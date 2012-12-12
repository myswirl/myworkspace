#ifndef _DJACSDATATYPE_H
#define _DJACSDATATYPE_H

#include "ItpDataDefine.h"
#include "ITPCom.h"
#include "DJAcsSignalMonitor.h"

#define  ACS_MAX_CALL_NUM                  32
#define  ACS_SIGDATA_MAX_LEN               300
#define  ACS_MAX_IO_SIZE                   2048
#define  ACS_LOCALID_MAX_LEN               20
#define  ACS_REMOTEID_MAX_LEN              20
#define  ACS_MAX_DECODER_CFG_LEN           200
#define  XMS_MAX_REGNAME_LEN                16
#define  ACS_MAX_H245_SIG_PDU_LEN          4000

typedef	 DJ_Void (*EsrFunc)(DJ_U32 esrParam);

enum XMS_DEVMAIN_TYPE
{
	XMS_DEVMAIN_VOICE                = 0x02, /*Voice device*/
	XMS_DEVMAIN_FAX                  = 0x03, /*Fax device*/
	XMS_DEVMAIN_DIGITAL_PORT         = 0x04, /*Digital Port device*/
	XMS_DEVMAIN_INTERFACE_CH         = 0x05, /*Interface Ch device*/
	XMS_DEVMAIN_DSS1_LINK            = 0x06, /*DSS1 Link device*/
	XMS_DEVMAIN_SS7_LINK             = 0x07, /*SS7 Link device*/
	XMS_DEVMAIN_BOARD                = 0x08, /*DSP Board device*/
	XMS_DEVMAIN_CTBUS_TS             = 0x09, /*CTBus ts device*/
	XMS_DEVMAIN_VOIP                 = 0x0A, /*VoIP device*/
	XMS_DEVMAIN_CONFERENCE           = 0x0C, /*Conference Group Device*/
	XMS_DEVMAIN_VIDEO                = 0x0D, /*Video device*/
	XMS_DEVMAIN_CTX                  = 0x10, /*Ctx device*/
	XMS_DEVMAIN_RTPX                  = 0x11, /*RTPX device*/
	XMS_DEVMAIN_MEDIAEX              = 0x1F, /*MediaEx device*/
};

enum XMS_INTERFACE_DEVSUB_TYPE
{
	XMS_DEVSUB_BASE                       = 0x0,  /*Base*/
	XMS_DEVSUB_ANALOG_TRUNK               = 0x01, /*Analog Trunk*/
	XMS_DEVSUB_ANALOG_USER                = 0x02, /*Analog User*/
	XMS_DEVSUB_ANALOG_HIZ                 = 0x03, /*Analog Hiz*/
	XMS_DEVSUB_ANALOG_EMPTY               = 0x04, /*Analog Empty*/
	XMS_DEVSUB_E1_PCM                     = 0x05, /*E1 PCM*/
	XMS_DEVSUB_E1_CAS                     = 0x06, /*E1 CAS*/
	XMS_DEVSUB_E1_DSS1                    = 0x07, /*E1 DSS1*/
	XMS_DEVSUB_E1_SS7_TUP                 = 0x08, /*E1 SS7 TUP*/
	XMS_DEVSUB_E1_SS7_ISUP                = 0x09, /*E1 SS7 ISUP*/
	XMS_DEVSUB_ANALOG_VOC2W			      = 0x0A, /*Voice only 2 wire*/
    XMS_DEVSUB_ANALOG_VOC4W			      = 0x0B, /*Voice only 4 wire*/
    XMS_DEVSUB_ANALOG_EM			      = 0x0C, /*EM control module*/
    XMS_DEVSUB_ANALOG_MAG			      = 0x0D, /*magnetic module*/
    XMS_DEVSUB_E1_DCH				      = 0x0E, /*E1 6D25B's Dchannel*/
    XMS_DEVSUB_E1_BCH				      = 0x0F, /*E1 6D25B's Bchannel*/
    XMS_DEVSUB_UNUSABLE				      = 0x10, /*unusable timeslot, such as ts0 of E1*/
	XMS_DEVSUB_HIZ_CAS				      = 0x11, /*Hiz Cas*/
	XMS_DEVSUB_HIZ_PRI				      = 0x12, /*Hiz Pri*/
	XMS_DEVSUB_HIZ_SS7					  = 0x13, /*Hiz ss7*/
	XMS_DEVSUB_HIZ_PRI_LINK			      = 0x14, /*Hiz pri link*/
	XMS_DEVSUB_HIZ_SS7_64K_LINK  		  = 0x15, /*Hiz ss7 64k link*/
	XMS_DEVSUB_HIZ_SS7_2M_LINK    		  = 0x16, /*Hiz ss7 2M link*/
	XMS_DEVSUB_SS7_LINK		              = 0x17, /*ss7 link*/
	XMS_DEVSUB_LINESIDE		              = 0x18, /*LineSide E1*/
	XMS_DEVSUB_ANALOG_REC	              = 0x19, /*Analog phone Record*/
	XMS_DEVSUB_DIGITAL_REC	              = 0x20, /*Digital phone Record*/
	XMS_DEVSUB_HIZ_HDLC_N64K_LINK         = 0x1A, /*Hiz n*64K */
	XMS_DEVSUB_RAWDATA					  = 0x1C, /*Rawdata*/
};

enum XMS_MEDIA_DEVSUB_TYPE
{
	XMS_DEVSUB_MEDIA_VOC                 = 0x0,  /*voc device*/
	XMS_DEVSUB_MEDIA_324                 = 0x1,  /*324 device*/
	XMS_DEVSUB_MEDIA_RAW				 = 0x2,	 /*Raw device*/
};

enum XMS_CTBUS_DEVSUB_TYPE
{
	XMS_DEVSUB_CTBUS_LINK                 = 0x0, /*CTBus link ts*/	
	XMS_DEVSUB_CTBUS_CTX                  = 0x1, /*CTBus CTX ts*/	
};

enum XMS_E1_TYPE
{
	XMS_E1_TYPE_Analog30	         = 1, /* Analog Interface 30*/
	XMS_E1_TYPE_PCM31		         = 2, /* E1 Frame,ts31*/
	XMS_E1_TYPE_PCM30		         = 3, /* E1 Frame,ts 30*/
	XMS_E1_TYPE_CAS			         = 4, /* E1 Frame,CAS*/
	XMS_E1_TYPE_PRI			         = 5, /* E1 Frame,PRI*/
	XMS_E1_TYPE_SS7_TUP_0_Link	     = 6, /* E1 Frame,TUP: 0 link*/
	XMS_E1_TYPE_SS7_TUP_1_Link	     = 7, /* E1 Frame,TUP: 1 link*/
	XMS_E1_TYPE_SS7_TUP_2_Link	     = 8, /* E1 Frame,TUP: 2 link*/
	XMS_T1_TYPE_D4	                 = 9, /* T1 Frame*/
	XMS_T1_TYPE_ESF		             = 10, /* T1 Frame*/
	XMS_J1_TYPE_D4		             = 11, /* J1 Frame*/
	XMS_J1_TYPE_ESF		             = 12, /* J1 Frame*/
	XMS_SLC_TYPE_96    	             = 13, /* SLC_96 Frame*/
	XMS_E1_TYPE_SS7_ISUP_0_Link	     = 14, /* E1 Frame,ISUP: 0 link*/
	XMS_E1_TYPE_SS7_ISUP_1_Link	     = 15, /* E1 Frame,ISUP: 1 link*/
	XMS_E1_TYPESS7_ISUP_2_Link	     = 16, /*E1 Frame,ISUP: 2 link*/
	XMS_E1_TYPE_6D25B		         = 17, /* E1 Frame, 6 DChannel,25 BChannel*/
	XMS_E1_TYPE_1D30B                    = 18, /*E1 Frame, 1 DChannel, 30 BChannel*/
};

/*E1 STATE BITS DEFINE*/
enum XMS_E1PORT_MASK_STATE
{
	XMS_E1PORT_MASK_LOST_SIGNAL	     = 0x00000001,
	XMS_E1PORT_MASK_FAS_ALARM	     = 0x00000002,
	XMS_E1PORT_MASK_MFAS_ALARM	     = 0x00000004,
	XMS_E1PORT_MASK_CRC4_ALARM	     = 0x00000008,
	XMS_E1PORT_MASK_REMOTE_ALARM	 = 0x00000010,
	XMS_E1PORT_MASK_REMOTE_MF_ALARM	 = 0x00000020,
	XMS_E1PORT_MASK_TX_OPEN_ALARM	 = 0x00000040,
	XMS_E1PORT_MASK_TX_SHORT_ALARM	 = 0x00000080,
	XMS_E1PORT_MASK_RX_LEVEL	     = 0x00000F00,
	XMS_E1PORT_MASK_TYPE		     = 0x00FF0000,
};

enum XMS_VOIP_PROTOCOL_TYPE
{
	XMS_VOIP_PROTOCOL_H323           = 0x1, /*VoIP H323 protocol*/
	XMS_VOIP_PROTOCOL_SIP            = 0x2, /*VoIP SIP protocol*/
};

enum XMS_VOC_SRC_MODE
{
	XMS_SRC_8K                       = 0x0,
	XMS_SRC_6K                       = 0x1,
	XMS_SRC_GTG                      = 0x2,
	XMS_SRC_FSK                      = 0x3,
	XMS_SRC_RTP                      = 0x4,
	XMS_SRC_FAX                      = 0x5,
	XMS_SRC_3GVIDEO                  = 0x6,
	XMS_SRC_11K                      = 0x7,
	XMS_SRC_FAX_ECM                  = 0x8,
};

enum XMS_VOC_CODE_TYPE
{
	XMS_Alaw_type                    = 0x0,
	XMS_Ulaw_type                    = 0x1,
	XMS_Vox_type                     = 0x2,
	XMS_Linear_8bit                  = 0x3,
	XMS_Linear_16bit                 = 0x4,
	XMS_Amr_type                     = 0x5,
	XMS_G723_type                    = 0x6,
	XMS_CODE_STREAM                  = 0x7,
};

enum XMS_PLAYREC_STOP_MODE
{
	XMS_Normal_Stop                  = 0x0,
	XMS_Single_Code                  = 0x1,
	XMS_Any_Code                     = 0x2,
};

enum XMS_CSPREC_TYPE
{
	XMS_CSPREC_NONE                  = 0x0,
	XMS_CSPREC_BARGEIN               = 0x1,
	XMS_CSPREC_NON_BARGEIN           = 0x2,
};

#define XMS_MAX_PLAY_QUEUE_NUM        255

enum XMS_PLAY_TYPE
{
	XMS_PLAY_TYPE_FILE               = 0x0,
	XMS_PLAY_TYPE_INDEX              = 0x1,
	XMS_PLAY_TYPE_FILE_QUEUE         = 0x2,
	XMS_PLAY_TYPE_INDEX_QUEUE        = 0x3,
};

#define XMS_MAX_IDNEX_TABLE_NUM       1024
 
enum XMS_BUILD_INDEX_TYPE
{
	XMS_BUILD_INDEX_RAM              = 0x4,
	XMS_BUILD_INDEX_FILE             = 0x5,
};

enum XMS_CFG_TYPE
{
	XMS_CFG_TYPE_NON_INDEX           = 0x0,
	XMS_CFG_TYPE_INDEX               = 0x1,
};

enum XMS_CTRL_PLAY_TYPE
{
	XMS_STOP_PLAY                    = 0x0,
	XMS_PAUSE_PLAY                   = 0x1,
	XMS_RESUME_PLAY                  = 0x2,
	XMS_STEP_BACKWARD_PLAY           = 0x3,
	XMS_STEP_FORWARD_PLAY            = 0x4,

	XMS_CSP_STOP_PLAY                = 0x5,
	XMS_CSP_PAUSE_PLAY               = 0x6,
	XMS_CSP_RESUME_PLAY              = 0x7,
	XMS_CSP_STEP_BACKWARD_PLAY       = 0x8,
	XMS_CSP_STEP_FORWARD_PLAY        = 0x9,
	XMS_CSP_DELAY_STOP_PLAY          = 0xA,
};

enum XMS_PLAY_GAIN_MODE
{
	XMS_PLAY_GAIN_MODE_NOCHANGE     = 0x0,
	XMS_PLAY_GAIN_MODE_FIX          = 0x1,
	XMS_PLAY_GAIN_MODE_ALS          = 0x2,
	XMS_PLAY_GAIN_MODE_AGC          = 0x3,
};

enum XMS_VOC_OUTPUT_TYPE
{
	XMS_VOC_OUTPUT_SILENCE           = 0x0,
	XMS_VOC_OUTPUT_FROM_INPUT        = 0x1,
	XMS_VOC_OUTPUT_FROM_PLAY         = 0x2,
	XMS_VOC_OUTPUT_FROM_MIXER        = 0x3, 	
};

enum XMS_MIXER_TYPE
{
	XMS_MIXER_FROM_NULL              = 0x00,
	XMS_MIXER_FROM_INPUT             = 0x01,
	XMS_MIXER_FROM_PLAY              = 0x02,
	XMS_MIXER_FROM_CONF              = 0x03,
	XMS_MIXER_FROM_IP                = 0x04,
	XMS_MIXER_FROM_324               = 0x05,
	XMS_MIXER_FROM_RTPX              = 0x06,
};

enum XMS_AGC_MODE
{
	XMS_AGC_MODE_ALS		          = 0x00,
	XMS_AGC_MODE_AGC		          = 0x01,
};

enum XMS_EC_REF
{
	XMS_EC_REF_FROM_INPUT		      = 0x00,
	XMS_EC_REF_FROM_OUTPUT		      = 0x01,
};

enum XMS_CONF_INPUT_OPT
{
	XMS_CONF_INPUT_OPT_SILENCE        = 0x0,
	XMS_CONF_INPUT_OPT_NORMAL         = 0x1,
	XMS_CONF_INPUT_OPT_PLAY           = 0x2,	
};

enum XMS_CONF_OUTPUT_OPT
{
	XMS_CONF_OUTPUT_OPT_SILENCE		  = 0x0,
	XMS_CONF_OUTPUT_OPT_NORMAL	      = 0x1,
	XMS_CONF_OUTPUT_OPT_SUM	          = 0x2,
};

enum XMS_CTRL_RECORD_TYPE
{
	XMS_STOP_RECORD                   = 0x0,
	XMS_PAUSE_RECORD                  = 0x1,
	XMS_RESUME_RECORD                 = 0x2,
	XMS_STEP_BACKWARD_RECORD          = 0x3,

	XMS_CSP_STOP_RECORD               = 0x4,
	XMS_CSP_PAUSE_RECORD              = 0x5,
	XMS_CSP_RESUME_RECORD             = 0x6,
	XMS_CSP_STEP_BACKWARD_RECORD      = 0x7,
};

enum XMS_REC_OPENFILE_TYPE
{
	XMS_REC_FILE_TRUNC                = 0x0,
	XMS_REC_FILE_APPEND               = 0x1,
};

enum XMS_AUDIO_TRACKS
{
	XMS_AUDIO_TRACKS_MONO             = 0x1,
	XMS_AUDIO_TRACKS_STEREO           = 0x2,
};

enum XMS_VIDEO_CODE_TYPE
{
	XMS_VIDEO_CODE_TYPE_H263P      = 0x1,
    XMS_VIDEO_CODE_TYPE_H263       = 0x2,
    XMS_VIDEO_CODE_TYPE_MP4V       = 0x3,   
	XMS_VIDEO_CODE_TYPE_H264       = 0x4,
};

enum XMS_AUDIO_AMR_FRAME_TYPE
{
	XMS_AMR_FRAME_TYPE_4_75              = 0x0,
	XMS_AMR_FRAME_TYPE_5_15              = 0x1,
	XMS_AMR_FRAME_TYPE_5_90              = 0x2,
	XMS_AMR_FRAME_TYPE_6_70              = 0x3,
	XMS_AMR_FRAME_TYPE_7_40              = 0x4,
	XMS_AMR_FRAME_TYPE_7_95              = 0x5,
	XMS_AMR_FRAME_TYPE_10_2              = 0x6,
	XMS_AMR_FRAME_TYPE_12_2              = 0x7,
	XMS_AMR_FRAME_TYPE_AMR_SID           = 0x8,
	XMS_AMR_FRAME_TYPE_GSM_EFR_SID       = 0x9,
	XMS_AMR_FRAME_TYPE_TDMA_EFR_SID      = 0xA,
	XMS_AMR_FRAME_TYPE_PDC_EFR_SID       = 0xB,
	XMS_AMR_FRAME_TYPE_RESERVED1         = 0xC,
	XMS_AMR_FRAME_TYPE_RESERVED2         = 0xD,
	XMS_AMR_FRAME_TYPE_RESERVED3         = 0xE,
	XMS_AMR_FRAME_TYPE_NO_DATA           = 0xF,
};

enum XMS_CSPPLAY_DATA_TYPE
{
	XMS_CSPPLAY_DATA_VOC                 = 0x0,
	XMS_CSPPLAY_DATA_VIDEO               = 0x1,
};

enum XMS_PRIVATE_EXT_TYPE
{
        XMS_EXT_TYPE_PATH                    = 0x01,             //extend to path's size from 128 to 260
};

enum XMS_ANALOG_TRUNK_CALLERID_OPT
{
	XMS_ANALOG_TRUNK_CALLERID_DISABLE      = 0x0,
	XMS_ANALOG_TRUNK_CALLERID_FSK          = 0x1,
	XMS_ANALOG_TRUNK_CALLERID_DTMF         = 0x2,
	XMS_ANALOG_TRUNK_CALLERID_FSKORDTMF    = 0x3,
};

enum XMS_ANALOG_TRUNK_DIALTONE_DETECT_OPT
{
	XMS_ANALOG_TRUNK_DIALTONE_DONTDETECT   = 0x0,
	XMS_ANALOG_TRUNK_DIALTONE_DETECT       = 0x1,
};

enum XMS_ANALOG_TRUNK_CALLOUT_METHOD_OPT
{
	XMS_ANALOG_TRUNK_CALLOUT_APP_FULLCONTROL     = 0x0,
	XMS_ANALOG_TRUNK_CALLOUT_NORMAL_PROGRESS     = 0x1,
	XMS_ANALOG_TRUNK_CALLOUT_POLARITY_REVERSE    = 0x2,
};

enum XMS_ANALOG_USER_CALLERID_OPT
{
	XMS_ANALOG_USER_CALLERID_DSIABLE            = 0x0,
	XMS_ANALOG_USER_CALLERID_FSK                = 0x1,
	XMS_ANALOG_USER_CALLERID_DTMF               = 0x2,
};

enum XMS_ANALOG_USER_RINGCADENCE_TYPE
{
	XMS_ANALOG_USER_RINGCADENCE_SINGLEPULSE    = 0x0,
	XMS_ANALOG_USER_RINGCADENCE_DOUBLEPULSE    = 0x1,
	XMS_ANALOG_USER_RINGCADENCE_TRIPLEPULSE    = 0x2,
};

enum XMS_ANALOG_USER_CALLIN_METHOD_TYPE
{
	XMS_ANALOG_USER_CALLIN_METHOD_APP_FULLCONTROL    = 0x0,
	XMS_ANALOG_USER_CALLIN_METHOD_AUTO_RECVCALLEE    = 0x1,
};

#define  ACS_SNAPSHOT_ALL                  0x0
#define  ACS_SNAPSHOT_INTERFACE            0x1
#define  ACS_SNAPSHOT_VOC                  0x2
#define  ACS_SNAPSHOT_CTBUS                0x3
#define  ACS_SNAPSHOT_VOIP                 0x4
#define  ACS_SNAPSHOT_CONFERENCE           0x5
#define  ACS_SNAPSHOT_VIDEO                0x6

enum XMS_FAX_MODEM_TYPE
{
	XMS_F48_Modem                        = 0x0,
	XMS_F96_Modem                        = 0x1,
	XMS_F144_Modem                       = 0x2,
};

enum XMS_FAX_BPS_TYPE
{
	XMS_BPS_2400                         = 0x0,
	XMS_BPS_4800                         = 0x1,
	XMS_BPS_7200                         = 0x2,
	XMS_BPS_9600                         = 0x3,
	XMS_BPS_12000                        = 0x4,
	XMS_BPS_14400                        = 0x5,
};

enum XMS_FAX_T4_TYPE
{
	XMS_T4_Low_Dencity                   = 0x0,
	XMS_T4_High_Dencity                  = 0x1,
	XMS_T4_MH_Encode                     = 0x0,
	XMS_T4_MR_Encode                     = 0x1,
};

#define  T6_Encode                         0x2

enum XMS_FAX_LINE_PIXEL_TYPE
{
	XMS_Line_Pixel_1728                  = 0x0,
	XMS_Line_Pixel_2048                  = 0x1,
	XMS_Line_Pixel_2432                  = 0x2,
	XMS_Line_Pixel_3456                  = 0x3,
	XMS_Line_Pixel_4096                  = 0x4,
	XMS_Line_Pixel_4864                  = 0x5,
};

enum XMS_FAX_PAGE_LENGTH
{
	XMS_Page_Length_A4                   = 0x0,	
	XMS_Page_Length_B4                   = 0x1,
	XMS_Page_Length_Unlinit              = 0x2,
};

enum XMS_FAX_SCANLINE_TIME
{
	XMS_Scanline_Time_0                  = 0x0,
	XMS_Scanline_Time_5                  = 0x1,
	XMS_Scanline_Time_10                 = 0x2,	
	XMS_Scanline_Time_20                 = 0x3,
	XMS_Scanline_Time_40                 = 0x4,
};

enum XMS_FAX_HOSTCTRL_TYPE
{
	XMS_Fax_Null                         = 0x0,
	XMS_Fax_Recv                         = 0x1,
	XMS_Fax_Send                         = 0x2,
	XMS_Fax_Stop                         = 0x3,
};

enum XMS_FAX_WORKMODE
{	
	XMS_FAX_WORKMODE_NORMAL              = 0x00,
	XMS_FAX_WORKMODE_SERVER              = 0x01,
	XMS_FAX_WORKMODE_GATEWAY             = 0x02,
};

enum XMS_FAX_ECM_MODE_TYPE
{
	XMS_FAX_ECM_MODE_TYPE_NORMAL         = 0x00,
	XMS_FAX_ECM_MODE_TYPE_ECM            = 0x01,	
};

enum XMS_ANSWERCALL_TYPE
{
	XMS_ANSWERCALL_ANC                   = 0x0,
	XMS_ANSWERCALL_ANN                   = 0x1,
	XMS_ANSWERCALL_CON                   = 0x2,
};

enum XMS_CTX_REG_TYPE
{
	XMS_CTX_REG                     = 0x1,/*ctx reg*/
	XMS_CTX_UNREG                   = 0x2,/*ctx unreg*/
};

enum XMS_CTX_LINK_OPTION
{
	CTX_LINK_OPTION_RESET			= 0x1,/*reset all link*/
	CTX_LINK_OPTION_LINK			= 0x2,/*single link*/
	CTX_LINK_OPTION_UNLINK			= 0x3,/*single unlink*/
	CTX_LINK_OPTION_DUALLINK		= 0x4,/*dual link*/
	CTX_LINK_OPTION_DUALUNLINK		= 0x5,/*dual unlink*/
};

enum XMS_MEDIA_TYPE
{
        MEDIA_TYPE_AUDIO                     = 0,   /*play audio only*/
        MEDIA_TYPE_VIDEO                     = 1,   /*play video only*/
        MEDIA_TYPE_ANV                       = 4,   /*play audio and video*/
};

enum XMS_SS7_FRAME_USER_TYPE
{
	XMS_SS7_USER_SCCP = 0x03,
	XMS_SS7_USER_TUP  = 0x04,
	XMS_SS7_USER_ISUP = 0x05,
};
 
enum XMS_SS7_FRAME_LINK_TYPE
{
	XMS_SS7_LINK_ITU  = 0x02,
	XMS_SS7_LINK_CHINA= 0x04,
};
 
enum XMS_SS7_FRAME_INFO_TYPE
{
	XMS_SS7_INFO_DATA   = 0x01,
	XMS_SS7_INFO_CMD    = 0x02, 
};

enum XMS_SYSMON_MODSTATE
{   
	SYSMON_MODSTATE_DISCONNECT      = 0x0,/*Module disconnect*/
    SYSMON_MODSTATE_CONNECT         = 0x1,/*Module connect*/    
};
#define  FetchEventData(pAcsEvt)  ((DJ_S8 *)pAcsEvt + sizeof(Acs_Evt_t))
#define  FetchDeviceList(pAcsEvt) ((DJ_S8 *)FetchEventData(pAcsEvt) + sizeof(Acs_Dev_List_Head_t))
#define  FetchIOData(pAcsEvt)     ((DJ_S8 *)FetchEventData(pAcsEvt) + sizeof(Acs_IO_Data))
#define  FetchParamData(pAcsEvt)  ((DJ_S8 *)FetchEventData(pAcsEvt) + sizeof(Acs_ParamProc_Data))

/*ACS server parm*/
typedef struct
{
	DJ_S8    m_s8ServerIp[32]; /*XMS server ip*/
	DJ_U32   m_u32ServerPort;  /*XMS server port*/
	DJ_S8    m_s8UserName[32]; /**/
    DJ_S8    m_s8UserPwd[32];  /**/
}ServerID_t;/**/

/*privatedata*/
typedef struct
{
	DJ_U32   m_u32DataSize; /*private data size*/
}PrivateData_t;/*PrivateData_t*/

/*CallID*/
typedef struct
{
    FlowType_t      m_s32FlowType;    /*CallType*/
    FlowChannel_t   m_s32FlowChannel; /*Call Channel*/
}CallID_t;/*CallID_t*/

/*DeviceID_t*/
typedef struct
{   
	DeviceMain_t    m_s16DeviceMain;  /*device main type*/
    DeviceSub_t     m_s16DeviceSub;   /*device sub type*/
    ModuleID_t      m_s8ModuleID;     /*device module ID*/
	DJ_S8           m_s8MachineID;    /*device machine ID*/
    ChannelID_t     m_s16ChannelID;   /*device channel ID*/
    DeviceGroup_t   m_s16DeviceGroup; /*device group ID*/	
	DJ_S8           m_Rfu2[2];        /*Reserved*/
	CallID_t        m_CallID;         /*Serveice ID*/
}DeviceID_t;/*DeviceID_t*/

/*Acs_Evt_t*/
typedef struct
{
	ACSHandle_t		  m_s32AcsHandle;       /*acs handle*/	
	DeviceID_t        m_DeviceID;           /*Device ID*/
	DJ_S32            m_s32EvtSize;         /*Evt size*/
	EventType_t		  m_s32EventType;       /*event type code*/
	DJ_U32            m_u32EsrParam;        /*App esr param*/
}Acs_Evt_t;/**/

/*ACS Event Head*/
typedef struct
{
	PKG_HEAD_STRUCT  m_PkgEvtHead;  /*ITP package head*/
	Acs_Evt_t        m_AcsEvt_t;    /*acs event package head*/
}ACS_EVT_HEAD;/**/

/*ACS playpropetry*/
typedef struct
{	
	DJ_U8                m_u8TaskID;         /*play task ID*/	
	DJ_U8                m_u8SrcMode;        /*file src mode, reference XMS_VOC_SRC_MODE*/
	DJ_U8                m_u8DecodeType;     /*decode type, reference XMS_VOC_CODE_TYPE*/	
	DJ_S8                m_s8Rfu1[1];        /*Reserved*/
	DJ_U32               m_u32MaxSize;       /*play max size(BYTES),  0: file real size; >0: max play size*/
	DJ_U32               m_u32MaxTime;       /*play max time(Seconds) 0: file real seconds; >0: max play seconds*/	
	DJ_U32               m_u32StopMode;      /*stop play mode,referecne XMS_PLAYREC_STOP_MODE*/
	DJ_U32               m_u32StopCode;      /*stop play when receive this dtmf code*/
	DJ_U16               m_u16FileOffset;    /*start play at the file offset*/
	DJ_U16               m_u16PlayType;      /*play type: 0-normal,1-index,referecnce XMS_PLAY_TYPE*/
	DJ_U16               m_u16PlayIndex;     /**/
	DJ_S8                m_s8PlayContent[ACS_MAX_FILE_NAME_LEN];/*play file name */
	DJ_U16               m_u16FileSubOffset; /*start play at the offset from m_u16FileOffset, and the granularity is 1 bytes, fileOffSet = m_u16FileOffset*8K + m_u16FileSubOffset*/
}PlayProperty_t;/*PlayProperty_t*/

/*ACS CSPPlayProperty_t*/
typedef struct
{	
	DJ_U8                m_u8TaskID;         /*play task ID*/	
	DJ_U8                m_u8SrcMode;        /*file src mode, reference XMS_VOC_SRC_MODE*/
	DJ_U8                m_u8DecodeType;     /*decode type, reference XMS_VOC_CODE_TYPE*/	
	DJ_S8                m_s8Rfu1[1];        /*Reserved*/
	DJ_U32               m_u32MaxSize;       /*play max size(BYTES),  0: file real size; >0: max play size*/
	DJ_U32               m_u32MaxTime;       /*play max time(Seconds) 0: file real seconds; >0: max play seconds*/	
	DJ_U32               m_u32StopMode;      /*stop play mode,referecne XMS_PLAYREC_STOP_MODE*/
	DJ_U32               m_u32StopCode;      /*stop play when receive this dtmf code*/
	DJ_S8                m_Rfu1[4];			
}CSPPlayProperty_t;/*CSPPlayProperty_t*/

/*CSPPlayDataInfo_t*/
typedef struct
{
	DJ_U16	m_u16DataLen;   		/*data length*/
	DJ_U8	m_u8DataType;		    /*data type,0: voc,1:video,reference XMS_CSPPLAY_DATA_TYPE*/
	DJ_U8	m_u8TaskID;	  		    /*task id*/
}CSPPlayDataInfo_t;/*CSPPlayDataInfo_t*/

/*ACS play3gpppropetry*/
typedef struct
{	
	DJ_U8                m_u8TaskID;              /*audio: play task ID*/	
	DJ_U8                m_u8SrcMode;             /*audio: file src mode, reference XMS_VOC_SRC_MODE*/
	DJ_U8                m_u8DecodeType;          /*audio: decode type, reference XMS_VOC_CODE_TYPE*/	
	DJ_S8                m_u8Audio_tracks;        /*audio: audio tracks*/
	DJ_S8                m_u8AmrFrameType;        /*audio: amr frame type*/
	DJ_U8                m_u8AudioNoDecode;       /*audio: FALSE: AMR to line-decodetype(default value); TRUE: No decode;*/
	DJ_U8                m_u8Rfu1[2];             /*reserved*/
	DJ_U32               m_u32MaxTime;            /*audio: play max time(Seconds) 0: file real seconds; >0: max play seconds*/	
	DJ_U32               m_u32StopMode;           /*audio: stop play mode,referecne XMS_PLAYREC_STOP_MODE*/
	DJ_U32               m_u32StopCode;           /*audio: stop play when receive this dtmf code*/
	DJ_U16               m_u16TimeOffset;         /*audio: start play at the file offset with time*/
	DJ_U16               m_u16PlayType;           /*audio: play type: 0-normal,1-index,referecnce XMS_PLAY_TYPE*/
	DJ_U16               m_u16PlayIndex;          /*audio: play index*/
	DJ_S16               m_s16Rfu2;               /*Reserved*/
	DJ_S8                m_s8PlayContent[ACS_MAX_FILE_NAME_LEN];/*audio: play file name */
	DJ_U8                m_u8VideoFrameRate;      /*video frame rate*/
	DJ_U8	             m_u8VideoDecode_Type;    /*video: video decode type*/
	DJ_U8	             m_u8VideoHigh;           /*video: video high*/
	DJ_U8	             m_u8VideoWidth;          /*video: video width*/	
	DJ_U32               m_u32VideoMaxTime;       /*video: play max time(Seconds) 0: file real seconds; >0: max play seconds*/	
	DJ_U16               m_u16VideoTimeOffset;    /*video: start play at the file offset with time*/
	DJ_U16               m_u16VideoPlayType;      /*video: play type: 0-normal,1-index,referecnce XMS_PLAY_TYPE*/
	DJ_U16               m_u16VideoPlayIndex;     /*video: play index*/
	DJ_U8                m_u8VideoTaskID;         /*video: play video task ID*/
	DJ_U8                m_u8MediaType;           /*common: media type XMS_MEDIA_TYPE*/        
	DJ_S8                m_s8VideoPlayContent[ACS_MAX_FILE_NAME_LEN];/*video: play file name */
	
}Play3gppProperty_t;/*PlayProperty_3gp_t*/

/*����ͨ������*/
typedef struct
{
	DJ_U8	m_u8SRC1_Ctrl;  /*reference XMS_MIXER_TYPE*/
	DJ_U8	m_u8SRC2_Ctrl;  /*reference XMS_MIXER_TYPE*/
	DJ_U16	m_u16SRC_ChID1; /*Media ChID1*/
	DJ_U16	m_u16SRC_ChID2; /*Media ChID2*/
	DJ_U8	m_u8Video;
	DJ_S8   m_s8Rfu;     /*Rfu*/
}MixerControlParam_t;/*MixerControl_t*/


/*ACS record file propetry*/
typedef struct
{	
	DJ_U32                m_u32MaxSize;    /* record max size, 0: no limit file size; >0: record max size*/
	DJ_U32                m_u32MaxTime;    /* record max time, 0: no limit record time; >0: record max time*/	
	DJ_U8	              m_u8EncodeType;  /* encode type, reference XMS_VOC_CODE_TYPE*/	
	DJ_U8	              m_u8SRCMode;     /*file src mode, reference XMS_VOC_SRC_MODE*/
	DJ_U8	              m_u8StopMode;    /*stop record mode,referecne XMS_PLAYREC_STOP_MODE*/
	DJ_U8	              m_u8StopCode;    /*stop record when receive this dtmf code*/
	DJ_S8                 m_s8IsMixEnable; /*is Mix*/
	DJ_U8                 m_u8TaskID;      /*change from Reserved to m_u8TaskID on 090112*/
	MixerControlParam_t   m_MixerControl;  /*Mixer Contronl parameter*/
	DJ_S8                 m_s8IsAppend;    /*0: trunc record file; 1: append record file,reference XMS_REC_OPENFILE_TYPE*/
	FilePath_t            m_s8FileName[MAX_PATH];/*record file name*/
	DJ_S8                 m_s8Rfu2[1];      /*Reserved*/
}RecordProperty_t;/*RecordProperty_t*/

/*ACS 3gpp record propetry*/
typedef struct
{
	DJ_U32                m_u32MaxTime;          /*record max time, 0: no limit record time; >0: record max time*/	
	DJ_U8	              m_u8StopMode;          /*stop record mode,referecne XMS_PLAYREC_STOP_MODE*/	
	DJ_U8	              m_u8StopCode;	         /*stop record when receive this dtmf code*/

	DJ_S8                 m_s8IsAppend;          /*0: trunc record file; 1: append record file,reference XMS_REC_OPENFILE_TYPE*/	
	DJ_U8	              m_u8AudioEncodeType;  /*encode type, reference XMS_VOC_CODE_TYPE*/	
	DJ_U8	              m_u8AudioSRCMode;     /*file src mode, reference XMS_VOC_SRC_MODE*/
	DJ_S8                 m_u8AudioTracks;      /*audio channel,reference XMS_AUDIO_TRACKS*/
    DJ_U16                m_u16AudioMaxBitRate; /*audio max bitrate,0~65535*/
    DJ_U16                m_u16AudioAverageBitRate;/*audio average bitrate,0~65535*/
	DJ_S8                 m_Rfu1[1];
	DJ_S8                 m_s8AudioIsMixEnable; /*is Mix, 0 : no mix,1: mix*/
	MixerControlParam_t   m_AudioMixerControl;  /*Mixer Contronl parameter*/
	
	DJ_U8	              m_u8VideoEncodeType;  /*encode type, reference XMS_VIDEO_CODE_TYPE*/	
	DJ_U8	              m_u8VideoFrameRate;   /*video frame rate,10~30*/
	DJ_U16                m_u16VideoWidth;      /*video width,176*/
	DJ_U16                m_u16VideoHeight;     /*video heigth,144*/
	DJ_U16                m_u16VideoMaxBitRate; /*video max bit rate,0~65535*/
	DJ_U16                m_u16VideoAverageBitRate;/*video average bit rate,0~65535*/
	DJ_U8                 m_Rfu2;
	DJ_S8                 m_s8VideoIsMixEnable; /*is Mix, 0 : no mix,1: mix*/
	MixerControlParam_t   m_VideoMixerControl;  /*Mixer Contronl parameter*/

	DJ_U8                 m_u8AudioProfiles;    /*audio profiles?*/
    DJ_U8                 m_u8AudioLevel;       /*audio level ?*/
	DJ_U16                m_u16AudioDecoderCfgLen;/*audio decoder cfg length,1~ACS_MAX_DECODER_CFG_LEN*/
	DJ_U8                 m_u8AudioDecoderCfg[ACS_MAX_DECODER_CFG_LEN];/*audio decoder cfg*/
	
	DJ_U8                 m_u8VideoProfiles;    /*video profiles?*/
	DJ_U8                 m_u8VideoLevel;       /*video level?*/	
	DJ_U16                m_u16VideoDecoderCfgLen;  /*video decoder cfg length,1~ACS_MAX_DECODER_CFG_LEN*/
	DJ_U8                 m_u8VideoDecoderCfg[ACS_MAX_DECODER_CFG_LEN];/*video decoder cfg?*/
	FilePath_t            m_s8FileName[MAX_PATH];/*record file name*/
}Record3gppProperty_t;/*Record3gpProperty_t*/

/*ACS record Ram propetry*/
typedef struct
{	
	DJ_U8	              m_u8EncodeType;  /* encode type, reference XMS_VOC_CODE_TYPE*/	
	DJ_U8	              m_u8SRCMode;     /*file src mode, reference XMS_VOC_SRC_MODE*/
	DJ_U16                m_u16PacketLen;  /*Record Packet Length*/
	DJ_U8                 m_u8CSPRecType;  /*1: barge-in, 2: non barge-in, reference XMS_CSPREC_TYPE*/
	DJ_S8                 m_s8Rfu1[3];
}RecordCSPProperty_t;/*RecordCSPProperty_t*/

/*control play */
typedef struct
{
	DJ_U16             m_u16ControlType;  /*play control type,reference XMS_CTRL_PLAY_TYPE*/
	DJ_U16             m_u16StepSize;     /*play step size*/
	DJ_U16             m_u16TuneType;     /*tune type,reserved*/
	DJ_U16             m_u16SpeedSize;    /*speed size,reserved*/
}ControlPlay_t;/*ControlPlay_t*/

/*control 3gpp play */
typedef struct
{
	DJ_U16             m_u16ControlType;  /*play control type,reference XMS_CTRL_PLAY_TYPE*/
	DJ_U16             m_u16StepTime;     /*play step time*/
	DJ_U16             m_u16TuneType;     /*tune type,reserved*/
	DJ_U16             m_u16SpeedSize;    /*speed size,reserved*/
	DJ_U8              m_u8MediaType;     /*media type: 0: audio; 1: video; 4: A & V*/
	DJ_U8              m_u8Reserv1[3];    /*reserve*/
}Control3gppPlay_t;/*Control3gppPlay_t*/


/*ACS control record */
typedef struct
{
	DJ_U32             m_u32ControlType;  /*control record type,reference XMS_CTRL_RECORD_TYPE*/
	DJ_U32             m_u32StepSize;     /*step size*/
	DJ_U32             m_u32SpeedSize;    /*speed size,reserved*/
}ControlRecord_t;/*ControlRecord_t*/

/*ACS control 3gpp record */
typedef struct
{
	DJ_U32             m_u32ControlType;  /*control record type,reference XMS_CTRL_RECORD_TYPE*/
	DJ_U32             m_u32StepTime;     /*step time*/
	DJ_U32             m_u32SpeedSize;    /*speed size,reserved*/
}Control3gppRecord_t;/*Control3gpRecord_t*/


/*ACS Get 3GPP file config add by fys*/
typedef struct
{
	DJ_U8              m_u8MediaType;                /*media type: XMS_MEDIA_TYPE*/
	DJ_U8              m_u8AudioFileType;            /*audio file type: XMS_CFG_TYPE*/
	DJ_U16             m_u16AudioFileIndex;          /*audio file index: 0~1023*/
	DJ_S8              m_s8AudioFileName[MAX_PATH];  /*audio file path and name*/
	DJ_U8              m_u8VideoFileType;            /*video file type: XMS_CFG_TYPE*/
	DJ_U8              m_u8Rfu1;                     /*reserve*/
	DJ_U16             m_u16VideoFileIndex;          /*video file index: 0~1023*/
	DJ_S8              m_s8VideoFileName[MAX_PATH];  /*video file path and name*/
}Get3gppCfg_t;/*Get3gppCfg_t*/


/*SigMon_t*/
typedef struct
{
	DJ_U8        m_u8MonSigType;     /*singnal type,reference XMS_SIGMON_SIG_TYPE*/
	DJ_U8        m_u8MonPackType;    /*Signal Pakcet type,reference XMS_SIGMON_PACK_TYPE*/	
	DJ_S8        m_s8Rfu1[6];        /*Rfu*/	
}SigMon_t;/*SigMon_t*/

/*CmdParamData_RTPXInput_t*/
typedef struct
{
	DJ_U8    m_u8ChanEnable;       /*Enable flag, 0: disable; 1: enable*/
	DJ_U8    m_u8MediaType;        /*Data type,reference XMS_VOIP_MEDIA_CAPS*/
	DJ_U8    m_u8PayloadType;      /*Payload type*/
	DJ_U8    m_u8Rfu;              /*reserved*/ 
}CmdParamData_RTPXInput_t;/*CmdParamData_RTPXInput_t*/
 
/*CmdParamData_RTPXOutput_t*/
typedef struct
{
	DJ_U8    m_u8ChanEnable;       /*Enable flag, 0: disable; 1: enable*/
	DJ_U8    m_u8MediaType;        /*Data type,reference XMS_VOIP_MEDIA_CAPS*/
	DJ_U8    m_u8PayloadType;      /*Payload type*/
	DJ_U8    m_u8SRC_Ctrl1;        /*Source type,reference XMS_MIXER_TYPE*/
	DJ_U16   m_u16SRC_Id1;         /*Source channel ID*/
	DJ_U16   m_u16RemotePort;	   /*Remote port*/
	DJ_U32	 m_u32RemoteIP;		   /*Remote IP*/
}CmdParamData_RTPXOutput_t;/*CmdParamData_RTPXOutput_t*/

/*CmdParamData_RTPXAddr_t*/
typedef struct
{
	DJ_U32	 m_u32RemoteIP;		   /*Remote IP*/
	DJ_U16   m_u16RemotePort;	   /*Remote port*/
	DJ_U8	 m_u8Rfu[2];		   /*Reserved*/
}CmdParamData_RTPXAddr_t;/*CmdParamData_RTPXOutput_t*/

/*ACS answer call private param*/
typedef struct
{
	PrivateData_t   m_PrivateData;         /*privatedata head*/ 
	DJ_S32          m_s32AnswerType;       /*answer type*/
}AnswerCallPrivate_t;/*AnswerCallPrivate_t*/
/*ACS Fax private param*/
typedef struct
{
	PrivateData_t   m_PrivateData;         /*privatedata head*/ 
	DeviceID_t      m_VoIPDevice;          /*VoIP device ID*/
	DJ_U8           m_u8WorkMode;          /*Fax work mode,reference XMS_FAX_WORKMODE*/
	DJ_S8           m_s8Rfu1[3];           /*Rfu */
}FaxPrivate_t;/*FaxPrivate_t*/

/*ACS VoIP Call private param*/
typedef struct
{
	PrivateData_t   m_PrivateData;         /*privatedata head*/ 
	DJ_S32          m_s32Protocol;         /*protocol type*/
	DJ_S8           m_s8CallerAddress[20]; /*Caller Address*/
	DJ_S32          m_s32CallerPort;       /*Caller Port*/
	DJ_S8           m_s8CallerUserID[40];  /*Caller UserID*/		
	DJ_S8           m_s8CalleeAddress[20]; /*Callee Address*/
	DJ_S32          m_s32CalleePort;       /*Callee Port*/
	DJ_S8           m_s8CalleeUserID[40];  /*Callee UserID*/		
	DJ_U8           m_u8MediaCapNum;       /*media capability number*/
	DJ_U8           m_u8MediaCapList[10];  /*media capability list*/
}VoIPCallPrivate_t;/*VoIPCallPrivate_t*/

/*ACS VoIP Alert private param*/
typedef struct
{
	PrivateData_t   m_PrivateData;         /*privatedata head*/ 
	DJ_U8           m_u8MediaCapNum;       /*media capability number*/
	DJ_U8           m_u8MediaCapList[10];  /*media capability list*/
}VoIPAlertPrivate_t;/*VoIPAlertPrivate_t*/

/*ACS VoIP Answer private param*/
typedef struct
{
	PrivateData_t   m_PrivateData;         /*privatedata head*/ 
	DJ_U8           m_u8MediaCapNum;       /*media capability number*/
	DJ_U8           m_u8MediaCapList[10];  /*media capability list*/
}VoIPAnswerPrivate_t;/*VoIPAnswerPrivate_t*/

/* VoIP Media Capabilities */
enum XMS_VOIP_MEDIA_CAPS
{
// audio media
	VOIP_MEDIA_AUDIO_PCMU		= 0x00,	// pcm u-law
	VOIP_MEDIA_AUDIO_PCMA		= 0x01,	// pcm a-law
	VOIP_MEDIA_AUDIO_G723		= 0x02,	// g.723
	VOIP_MEDIA_AUDIO_G729		= 0x03,	// g.729
	VOIP_MEDIA_AUDIO_T38		= 0x04,	// t.38/rtp
	VOIP_MEDIA_AUDIO_AMR		= 0x05,	// amr-nb
// video media
	VOIP_MEDIA_VIDEO_H261		= 0x80,	// h.261
	VOIP_MEDIA_VIDEO_H263		= 0x81,	// h.263
	VOIP_MEDIA_VIDEO_H263P		= 0x82,	// h.263+
	VOIP_MEDIA_VIDEO_MPEG4		= 0x83,	// mpeg-4 visual
	VOIP_MEDIA_VIDEO_H264		= 0x84,	// h.264

	VOIP_MEDIA_IMAGE_T38		= 0xF0,	// t.38/udptl
};

/* Board Device Params */
enum XMS_BOARD_PARAM_TYPE
{
	BOARD_PARAM_GETBOARDINFO          = 0,
	BOARD_PARAM_SETFSK                = 1,
	BOARD_PARAM_SETFLASH              = 2,
	BOARD_PARAM_SETGTDFREQ            = 3,
	BOARD_PARAM_SETGTDTONE            = 4,
	BOARD_PARAM_SETTRUNKFLASH         = 5,
	BOARD_PARAM_SETGTGFREQ            = 6, //GTG Freq
	BOARD_PARAM_SETGTGTONE            = 7, //GTG Tone
	BOARD_PARAM_SETVSD                = 8, //VSD

};

enum XMS_FAX_PARAM_TYPE
{
	FAX_PARAM_FAXHEADER    = 0 ,
	FAX_PARAM_ADDFAXFILE   = 1 ,
	FAX_PARAM_STARTSENDFAX = 2,
    FAX_PARAM_MODE_TYPE    = 3,
};

/* Media Device Params */
enum XMS_VOC_PARAM_TYPE
{
	VOC_PARAM_UNIPARAM     = 0,// set universal vocice parameters.
	VOC_PARAM_BREAKEN_CALL = 1,// added by jerry for keyInterrupet .08.02.19
	VOC_PARAM_3GFAXTONE    = 2,// added by jerry for 3G FaxTone Eanble or Disable 08.02.20
};

/* ISDN Device Param type*/
enum XMS_ISDN_PARAM_TYPE
{
     ISDN_PARAM_ORINUMBER = 0,
     ISDN_PARAM_CALLTYPE = 1,
     ISDN_PARAM_APPENDCALLEE = 2,
     ISDN_PARAM_CALLOUT =3,
     ISDN_PARAM_DISCONNECTREASON=4,
};
/* Conference Device Params */
enum XMS_CONF_PARAM_TYPE
{
	CONF_PARAM_UNIPARAM = 0,
};

/* Analog EM Device Params */
enum XMS_AEM_PARAM_TYPE
{
	XMS_AEM_PARAM_SND_M_SIGNAL = 1, //send M msg to endpoint
};

/* MediaEx Device Params */
enum XMS_MEDIAEX_PARAM_TYPE
{
	MEDIAEX_PARAM_INPUT = 0,
	MEDIAEX_PARAM_OUTPUT,
};

/* VoIP Call Slot Access Flag */
#define XMS_VOIP_ACCESS_NONE			0x00	/* no access */
#define XMS_VOIP_ACCESS_H323			0x01	/* h.323 call access */
#define XMS_VOIP_ACCESS_SIP				0x02	/* sip call access */
#define XMS_VOIP_ACCESS_INCOMING		0x04	/* incoming call access */
#define XMS_VOIP_ACCESS_OUTGOING		0x08	/* outgoing call access */
#define XMS_VOIP_ACCESS_REGISTRATION	0x10	/* registration access */

/* VoIP Device Params */
enum XMS_VOIP_PARAM_TYPE
{
	VOIP_PARAM_ACCESS = 0,
	VOIP_PARAM_VOICE,
	VOIP_PARAM_RTPSESSION,
	VOIP_PARAM_RTPADDR,
	VOIP_PARAM_RTCPADDR,
	VOIP_PARAM_RTPCODEC,
	VOIP_PARAM_RTPMIXER,
	VOIP_PARAM_RTPWANIP,
};

/* CAS Device Params */
enum XMS_CAS_PARAM_TYPE
{
	CAS_PARAM_UNIPARAM = 0,
        CAS_PARAM_MFCTYPE  = 1,
};

/* Analog User Device Params */
enum XMS_ANALOGUSER_PARAM_TYPE
{
	ANALOGUSER_PARAM_UNIPARAM = 0,
	ANALOGUSER_PARAM_UNBINDVOC     = 3 ,//Added by jerry For UnBind Voc From AnalogUser
        ANALOGUSER_PARAM_READSN        = 4,//get USER SN
};

/* Analog Trunk Device Params */
enum XMS_ANALOGTRUNK_PARAM_TYPE
{
	ANALOGTRUNK_PARAM_UNIPARAM = 0,
	ANALOGTRUNK_PARAM_SETFLASH = 1,
	ANALOGTRUNK_PARAM_UNSETUNIPARAM = 2 ,//added by jerry for UnSet UniParam(id = 0 )....
	ANALOGTRUNK_PARAM_UNBINDVOC     = 3 ,//Added by jerry For UnBind Voc From AnalogTrunk
        ANALOGTRUNK_PARAM_READSN        = 4 ,//get TRUNK SN
};

/* Analog HIZ Device Params */
enum XMS_ANALOGHIZ_PARAM_TYPE
{
    ANALOGHIZ_PARAM_VOLTAGE       = 1 ,//Set voltage
    ANALOGHIZ_PARAM_READSN        = 4 ,//get HIZ SN
};

/* 324 Config Params*/
enum XMS_324_PARAM_TYPE
{
	H324_PARAM_UNIPARAM  = 0x0,
	H324_PARAM_MEDIA     = 0x1,
	H324_PARAM_CH	     = 0x2,
	H324_PARAM_MIXER	 = 0x3,
};

/*Digtial machine Record param type*/
enum XMS_DIGREC_PARAM_TYPE
{
	XMS_DIGREC_PARAM_UNI  = 0x0,
};

enum MD_CFG_TYPE
{
	MD_CFG_TYPE_NON_INDEX      = 0x00,   /*����������*/
	MD_CFG_TYPE_INDEX          = 0X01,   /*��������*/
};

/*XMS_324_CMD_SESSION_TYPE*/
enum XMS_324_CMD_SESSION_TYPE
{	
	XMS_324_CMD_TYPE_NORMAL_START			= 0x0,	/* start session in normal mode */
	XMS_324_CMD_TYPE_MONITOR_START          = 0x1,	/* start session in monitor mode */
	XMS_324_CMD_TYPE_STOP                   = 0x2,	/* stop session */
	XMS_324_CMD_TYPE_RESET					= 0x3,	/* reset session */
};

/*XMS_324_CMD_CTRL_TYPE*/
enum XMS_324_CMD_CTRL_TYPE
{
	XMS_324_CMD_TYPE_SETMET					= 0x1,	/* set multiplex entry table */
	XMS_324_CMD_TYPE_OLC					= 0x2,	/* open rx/tx logical channel */
	XMS_324_CMD_TYPE_CLC					= 0x3,	/* close logical channel */
	XMS_324_CMD_TYPE_UII					= 0x4,	/* Send user input */	
};

/*XMS_324_EVT_TYPE*/
enum XMS_324_EVT_TYPE
{
	XMS_324_EVT_TYPE_START				    = 0x1,	/* start session event */
	XMS_324_EVT_TYPE_STOP				    = 0x2,	/* stop session event */
	XMS_324_EVT_TYPE_RESET					= 0x3,	/* reset session event */
	XMS_324_EVT_TYPE_UII					= 0x4,	/* receive user input event */
	XMS_324_EVT_TYPE_SIG_PDU                = 0x5,	/* receive h.245 signal message event */
};

/* XMS_324_STOP_CODE */
enum XMS_324_STOP_CODE
{
	XMS_324_STOP_CODE_UNKOWN				= 0x0,
	XMS_324_STOP_CODE_UPERSTOP				= 0x1,
	XMS_324_STOP_CODE_LOWERSTOP				= 0x2,
	XMS_324_STOP_CODE_HOSTSTOP				= 0x3,	
	XMS_324_STOP_CODE_REMOTESTOP			= 0x4,
};

/* XMS_324_AL_TYPE */
enum XMS_324_AL_TYPE
{
	XMS_324_AL1_FRAMED						= 0x1,	/* AL1 framed */
	XMS_324_AL1_NOTFRAMED					= 0x2,	/* AL1 not framed */
	XMS_324_AL2_WITHOUTSN					= 0x3,	/* AL2 */
	XMS_324_AL2_SN							= 0x4,	/* AL2 with sn */
	XMS_324_AL3								= 0x5,	/* AL3 */
};

/* encode type define for 3G324M module */
/*  encode type */
enum XMS_ENCODE_TYPE
{
	XMS_324_LC_CTRL							= 0x0,
	XMS_324_LC_AUDIO_G723					= 0x08,
	XMS_324_LC_AUDIO_AMR					= 0x14,
	XMS_324_LC_VIDEO_H263					= 0x83,
	XMS_324_LC_VIDEO_MPEG4					= 0x85,
};

//////////////////////////////////////////////////////////////////
/*XMS_SCCP_CMD_TYPE*/
enum XMS_SCCP_CMD_TYPE
{
	XMS_SCCP_CMD_SETRAWDATA             = 0x1,	
};

/*EXT_ENABLE_TYPE*/
enum EXT_ENABLE_TYPE
{
    EXT_ENABLE_DPD                          = 0x01,
    EXT_ENABLE_PVD                          = 0x02,	
    EXT_ENABLE_MSK                          = 0x04,
    EXT_ENABLE_VSD                          = 0x08,
};

/*DRS_EVT_MASK*/
enum DRS_EVT_MASK
{
    DRS_EVT_HOOK_MASK                       = 0x01,
    DRS_EVT_CONNECT_MASK                    = 0x02,	
    DRS_EVT_VAD_MASK                        = 0x04,
};

/*XMS_RTPX_PARAM_TYPE*/
enum XMS_RTPX_PARAM_TYPE
{
	XMS_RTPX_PARAM_INPUT               = 0x0,
	XMS_RTPX_PARAM_OUTPUT              = 0x1,
	XMS_RTPX_PARAM_MIXER		       = 0x2,
	XMS_RTPX_PARAM_ADDR				   = 0x3,
};
/*XMS_DRS_SWITCH_TYPE*/
enum XMS_DRS_SWITCH_TYPE
{ 
    XMS_DRS_SWITCH_AVAYA                = 0x01,
    XMS_DRS_SWITCH_SIMENZ               = 0x02,
    XMS_DRS_SWITCH_TADIRANT             = 0x03,
    XMS_DRS_SWITCH_ERRICSION            = 0x04,
    XMS_DRS_SWITCH_TOSHIBA              = 0x05,
};

/*VOC input control param*/
typedef struct 
{
	DJ_U8	m_u8AgcEnable;		/*0:disable;1:enable*/
	DJ_U8	m_u8AgcMode;		/*0:ALS mode; 1:AGC mode,reference XMS_AGC_MODE*/
	DJ_U8	m_u8EcEnable;		/*0:disable;1:enable*/
	DJ_U8	m_u8EcRefType;		/*0:ref from input;1:ref from output*/
	DJ_U8	m_u8VadEnable;		/*0:disable;1:enable*/
	DJ_U8	m_u8TadEnable;		/*0:disable;1:enable*/
	DJ_U8	m_u8Reserved[2];    /*Rfu*/
	DJ_U16	m_u16FixGain;		/*0x400 = 0dB*/
	DJ_U16	m_u16EcRefCh;       /*Rfu*/
	DJ_U32	m_u32Reserved;      /*Rfu*/
}VocInputControlParam_t;/*VocInputControl_t*/

/*VOC ouput control param*/
typedef struct 
{
	DJ_U8	             m_u8AgcEnable;	  /*0:disable;1:enable*/
	DJ_U8	             m_u8AgcMode;	  /*0:ALS mode; 1:AGC mode*/
	DJ_U8	             m_u8OutputType;  /*0:silence;1:from input;2:from play;3:from mixer,control by m_MixerControl,Reference XMS_VOC_OUTPUT_TYPE*/
	DJ_U8	             m_u8Reserved[1]; /**/
	DJ_U16	             m_u16FixGain;	  /*0x400 = 0dB*/
	DJ_S8                m_s8Rfu[2];      /*Rfu*/
	MixerControlParam_t	 m_MixerControl;  /**/
}VocOutputControlParam_t;/*VocOutputControl_t*/

/*VOC GTD control param*/
typedef struct
{
	DJ_U8	m_u8ChannelEnable; /*0: all disable; 1: control by next params*/
	DJ_U8	m_u8DTMFEnable;	   /*0:disable;1:enable*/
	DJ_U8	m_u8MR2FEnable;	   /*0:disable;1:enable*/
	DJ_U8	m_u8MR2BEnable;	   /*0:disable;1:enable*/
	DJ_U8	m_u8GTDEnable;	   /*0:disable;1:enable*/
	DJ_U8	m_u8FSKEnable;	   /*0:disable;1:enable*/
	DJ_U8   m_u8EXTEnable;     /*0 bit: DPD, 1 bit: PVD, 2 bit: MSK reference to EXT_ENABLE_TYPE enum*/
        DJ_U8   m_u8Reserved;      /*rfu*/
	DJ_U8	m_u8GTDID[8];      /**/
}VocGtdControlParam_t;/*VocGtdControl_t*/

/*Voice param*/
typedef struct
{
	DJ_U8                       m_u8InputCtrlValid;  /*is VocInputControl_t valid*/
	DJ_U8                       m_u8PlayGainMode;    /*Play gain mode,reference XMS_PLAY_GAIN_MODE*/  
	DJ_U16                      m_u16PlayFixGain;    /*Play fix gain*/ 
	VocInputControlParam_t	    m_VocInputControl;   /*valid to voc,not include RTP*/
	DJ_U8                       m_u8OutputCtrlValid; /*is VocOutputControl_t valid*/
	DJ_S8                       m_s8Rfu2[3];         /*Rfu*/
	VocOutputControlParam_t	    m_VocOutputControl;  /*valid to voc and RTP*/
	DJ_U8                       m_u8GtdCtrlValid;    /*is VocGtdControl_t valid*/
	DJ_S8                       m_s8Rfu3[3];         /*Rfu*/
	VocGtdControlParam_t	    m_VocGtdControl;	 /*valid to voc and RTP*/
}CmdParamData_Voice_t;/*CmdParamData_Voice_t*/

/* Acs VoIP Registration State struct */
typedef struct 
{
	DJ_U16	m_u16SlotID;			/* Registration Slot ID */
	DJ_U8	m_u8Protocol;			/* VoIP Protocol */
	DJ_U8	m_u8State;				/* Registration State */
	DJ_S8	m_s8TelNo[20];			/* Client Telephone-subscriber */
	DJ_S8	m_s8Address[20];		/* Client Address */
	DJ_U16	m_u16Port;				/* Client Port */
	DJ_S8	m_s8UserID[40];			/* Client UserID */
}Acs_VoIP_REGSTATE;/*Acs_VoIP_REGISTRATION*/

/* Acs VoIP Registraion Response struct */
typedef struct 
{
	DJ_U16	m_u16SlotID;			/* Registration Slot ID */
	DJ_U8	m_u8Granted;			/* grant registration */
	DJ_S8	m_s8Password[20];		/* password for sip authentication */
}Acs_VoIP_REGRESP;/*Acs_VoIP_REGRESP*/

/*analog flash param*/
typedef struct
{
	DJ_U8	m_u8MinHookFlashTime;		//default is 5, //in units of 20ms
	DJ_U8	m_u8MaxHookFlashTime;		//default is 25,in units of 20ms	
}CmdParamData_FlashParam_t;/*CmdParamData_FlashParam_t*/

typedef	struct
{
	DJ_U8	m_u8HookFlashTime;		//default is 10, //in units of 20ms
	DJ_U8	m_u8reserved[3];		
}CmdParamData_TrunkFlashParam_t;

typedef struct
{
        DJ_U8  m_u8ReadSn[13];
        DJ_U8  m_u8Reserved[3];
}CmdParamData_ReadSn_t;

/*silence voice detect param*/
typedef struct	
{ 	
    DJ_U32    m_u32SilenceLevel;            //silence detect level(db)        
    DJ_U32    m_u32SilenceTimer;            //silence detect timer(ms)
    DJ_U32    m_u32Reserved2;               //reserve
}CmdParam_SilenceDetectParam_t;/*CmdParam_SilenceDetectParam_t*/

/*FSK Send/Recv param*/
typedef	struct
{
	DJ_U16	rxFilterWindow;		/*default is 4, shouldn't change it       */
	DJ_U16	rxPeriodTheshold;	/*=80*rxFilterLength,shouldn't change it  */
	DJ_U16	rx55Count;			/*default is 10, can be changed as needed */
	DJ_U16	rxAllOneSamples;	/*default is 60, can be changed as needed */
	DJ_U16	rxDataTimeOut;		/*default is 20ms,can be changed as needed */
	DJ_U16	txBit1Freq;			/*default is 1350,can be changed as needed */
	DJ_U16	txBit0Freq;			/*default is 2150,can be changed as needed */
	DJ_U16	tx55Count;			/*default is 20, can be changed as needed  */
	DJ_U16	txAllOneSamples;	/*default is 600, can be changed as needed */
	DJ_U16	reserved[3];
}CmdParamData_FskParam_t;/*CmdParamData_FskParam_t*/

typedef struct
{	
	DJ_U16	m_u16Freq_Index;  /* Frequence index: 0-15*/
	DJ_U16  m_u16Freq_Coef;   /* Frequence*/
}CmdParamData_GtdFreq_t;/*CmdParamData_GtdFreq_t*/

typedef struct
{
	DJ_U16  m_u16GtdID;
	DJ_U16	m_u16Freq_Mask;
	DJ_U16	m_u16Amp_Threshold;
	DJ_U16	m_u16Envelope_Mode;
	DJ_U16	m_u16Repeat_Count;
	DJ_U16	m_u16Min_On_Time1;
	DJ_U16	m_u16Max_On_Time1;
	DJ_U16	m_u16Min_Off_Time1;
	DJ_U16	m_u16Max_Off_Time1;
	DJ_U16	m_u16Min_On_Time2;
	DJ_U16	m_u16Max_On_Time2;
	DJ_U16	m_u16Min_Off_Time2;
	DJ_U16	m_u16Max_Off_Time2;
}CmdParamData_GtdProtoType_t;/*CmdParamData_GtdProtoType_t*/

/* Acs VoIP H.245 UserInputIndication struct */
typedef struct
{	
	DJ_U16	m_u16Freq_Index;  /* Frequence index: 0-15*/
	DJ_U16  m_u16Freq_Coef;   /* Frequence*/
}CmdParamData_GtgFreq_t;/*CmdParamData_GtgFreq_t*/

//---This Struct according to ITP_GTG_Template(DSP_PACKET_DEF.H)
typedef struct
{
	DJ_U8	m_u8GTG_id;	        /*Ӱ��GTG ID����Чֵ0x30-0x3F*/
	DJ_U8	m_u8Reserved1;		/*����*/
	DJ_U16	m_u16Reserved2;		/*����*/

	//-------ITP_GTG_Prototype--------
	DJ_U8	m_u8Freq1_Index;
	DJ_U8	m_u8Freq2_Index;
	DJ_U16	m_u16Freq1_Amp;
	DJ_U16	m_u16Freq2_Amp;
	DJ_U16	m_u16Envelope_Mode;
	DJ_U16	m_u16Repeat_Count;
	DJ_U16	m_u16On_Time1;
	DJ_U16	m_u16Off_Time1;
	DJ_U16	m_u16On_Time2;
	DJ_U16	m_u16Off_Time2;
	DJ_U16	m_u16Reserved;		/*����*/
}CmdParamData_GtgProtoType_t;/*CmdParamData_GtgProtoType_t*/

/* Acs VoIP H.245 UserInputIndication struct */
typedef struct
{
	DJ_S8	m_s8String[100];		/* UserInputIndication string (general string)*/
	DJ_S8	m_s8Signal;				/* UserInputIndication signal type (dtmf char in "0123456789#*ABCD!")*/
	DJ_S8   m_Rfu[3];               /* Reserved*/
	DJ_U32	m_u32Duration;			/* UserInputIndication signal duration (in milliseconds)*/
}Acs_VoIP_H245UII;/*Acs_VoIP_H245UII*/

/* Acs VoIP SIP INFO struct */
typedef struct
{
	DJ_S8	m_s8ContentType[50];	/* content type string (mime)*/
	DJ_S8	m_s8Body[800];			/* info message body string*/
}Acs_VoIP_SIPINFO;/*Acs_VoIP_SIPINFO*/

/* Acs VoIP RTP Session struct */
typedef struct 
{
	DJ_U8	m_u8Enable;				/* enable/disable rtp session */
	DJ_U8	m_u8PayloadDtmf;		/* dynamic payload type for rtp dtmf */
	DJ_U8	m_u8PayloadTone;		/* dynamic payload type for rtp tone */
}Acs_VoIP_RTPSESSION;/*Acs_VoIP_RTPSESSION*/

/* Acs VoIP RTP Address struct */
typedef struct 
{
	struct ChAddr
	{
		DJ_U8	m_u8Enable;			/* enable flag */
		DJ_S8	m_s8IP[20];			/* ip address */
		DJ_U16	m_u16Port;			/* port */
	} audio, video;
}Acs_VoIP_RTPADDR;/*Acs_VoIP_RTPADDR*/

/* Acs VoIP RTCP Address struct */
typedef Acs_VoIP_RTPADDR
Acs_VoIP_RTCPADDR;/*Acs_VoIP_RTCPADDR*/

/* Acs VoIP RTP Codec struct */
typedef struct
{
	struct ChCodec
	{
		DJ_U8	m_u8Enable;				/* enable flag */
		DJ_U8	m_u8TxCodec;			/* tx codec */
		DJ_U8	m_u8TxPayloadType;		/* tx payload type (if dynamical) */
		DJ_U8	m_u8RxCodec;			/* rx codec */
		DJ_U8	m_u8RxPayloadType;		/* rx payload type (if dynamical) */
	} audio, video;
}Acs_VoIP_RTPCODEC;/*Acs_VoIP_RTPCODEC*/

/* Acs VoIP RTP DTMF struct */
typedef struct
{
	DJ_S8	m_s8EventChar;			/* dtmf event char */
	DJ_U8	m_s8Volume;				/* volume (in dbm0 from -63 to 0) */
	DJ_U16	m_u16Duration;			/* duration (in timestamp units) */
}Acs_VoIP_RTPDTMF;/*Acs_VoIP_RTPDTMF*/

/* Acs VoIP RTP TONE struct */
typedef struct
{
	DJ_U16	m_u16Modulation;		/* modulation */
	DJ_U8	m_u8TriFlag;			/* tri-frequency flag */
	DJ_S8	m_s8Volume;				/* volume */
	DJ_U16	m_u16Duration;			/* duration */
	DJ_U8	m_u8FrequencyNum;		/* number of frequencies */
	DJ_U16	m_u16Frequency[10];		/* frequencies to mix the tone */
}Acs_VoIP_RTPTONE;/*Acs_VoIP_RTPTONE*/

/* Acs VoIP Mixer struct*/
typedef struct
{
	DJ_U8	m_u8SRC1_Ctrl;  /*reference XMS_MIXER_TYPE*/
	DJ_U8	m_u8SRC2_Ctrl;  /*reference XMS_MIXER_TYPE*/
	DJ_U16	m_u16SRC_ChID1; /*Media ChID1*/
	DJ_U16	m_u16SRC_ChID2; /*Media ChID2*/
	DJ_S8   m_s8Video;      /*Video Channel*/
	DJ_S8   m_s8Rfu;        /*Rfu*/
}Acs_VoIP_Mixer;/*Acs_VoIP_Mixer*/

/*ACS_Conf_Param*/
typedef struct
{
	DJ_U8               m_u8InputOpt;	 /*conf input option, reference XMS_CONF_INPUT_OPT*/
	DJ_U8               m_u8OutputOpt;	 /*conf output option, reference XMS_CONF_OUTPUT_OPT*/
	DJ_U8               m_u8Rfu[2];      /*Rfu*/
}CmdParamData_Conf_t;/*CmdParamData_Conf_t*/

/* Acs MediaEx Input Param */
typedef struct
{
	DJ_U16		m_u16Port;				/* local port */
	DJ_U8		m_u8Codec;				/* codec type */
	DJ_U8		m_u8PayloadType;		/* rtp payload type (if dynamic) */
	DJ_U32		m_u32ProfileLevelId;	/* codec extra attribute */
}Acs_MediaEx_Input;/*Acs_MediaEx_Input*/

/* Acs MediaEx Output Param */
typedef struct
{
	DJ_S8		m_s8IP[20];				/* dest address */
	DJ_U16		m_u16Port;				/* dest port */
	DJ_U8		m_u8Codec;				/* codec type */
	DJ_U8		m_u8PayloadType;		/* rtp payload type (if dynamic) */
	DJ_U16		m_u16ResX;				/* resolution x */
	DJ_U16		m_u16ResY;				/* resolution y */
	DJ_U8		m_u8MaxFps;				/* max fps */
	DJ_U16		m_u16Kbps;				/* bitrate in kbps */
}Acs_MediaEx_Output;/*Acs_MediaEx_Input*/

/* Called Number Table Description */ 
typedef struct 
{
	DJ_U8						m_u8NumHeadLen;			/* Compared Called Number Head Length */
	DJ_U8						m_u8NumHead[14];		/* Compared Called Number Head Content */
	DJ_U8						m_u8NumLen;				/* Total Called Number Length */
}CAS_CalledTableDesc_t;/**/					

/* CAS Parameter Type Define*/
typedef struct 
{
	DeviceID_t					m_VocDevID;				/* Voice Device ID which is used for MFC, alloced by Application */

	/* Call In Parameter */
	DJ_U8						m_u8CalledTableCount;	/* Called Table Count ( 0-16:Table Count) */
	DJ_U8                       m_Rfu1[3];              /* Reserved*/
	CAS_CalledTableDesc_t		m_CalledTable[16];		/* Max 16 items */
	DJ_U8						m_u8CalledLen;			/* Called Number Length (0:Variable, Other:Length) */

	DJ_U8						m_u8CalledTimeOut;		/* Time Out Seconds when Receive MFC */

	DJ_U8						m_u8NeedCaller;			/* Is Need Caller Number ( 1:Yes, 0:No ) */
	DJ_U8						m_u8AreaCodeLen;		/* Area Code Length when receive Called Number, Used for Receive Caller Number */

	/* Call Out Parameter */
	DJ_U8						m_u8KA;					/* KA ( Default KA = 1 ) */
	DJ_U8						m_u8KD;					/* KD ( Default KD = 3 ) */

	/* Other */
	DJ_U8						m_u8ControlMode;		/* Release Control Mode ( 0:None, 1:Caller Control, 2:Called Control ) */
	DJ_U8                       m_u8CountryCode;    
	DJ_U8						m_u8Rfu[8];				/* Reserved for Future Use */
}CmdParamData_CAS_t;/*CmdParamData_CAS_t*/

#define   COUNTRY_BEGIN 0x00

#define   CHINA      0x01
#define   KOREA      0x02
#define   INDIA      0x03
#define   VIETNAM    0x04

#define   COUNTRY_END 0x05
/*AnalogTrunk Parameter Define*/
typedef struct
{
	//CallIn Paramteter
	DJ_U16	m_u16CallInRingCount; //ring count before CallIn event
	DJ_U16  m_u16CallInRingTimeOut;//Unit:ms Ring TimeOut,when ring timed out ,state changed to DCS_Free
}CmdParamData_AnalogTrunk_t;/*CmdParamData_AnalogTrunk_t*/


typedef struct
{
	DJ_U8	m_u8Channel_Enable;
	DJ_U8	m_u8Type;
	DJ_U8	m_u8Tx_State;
	DJ_U8	m_u8Rx_State;
}CmdParamData_AnalogTrunkFlash_t;/*CmdParamData_AnalogTrunkFlash_t*/

/*AnalogUser Parameter Define*/
typedef struct
{	
	// Call Out Parameter 
	DJ_U8        m_u8CallerIdOption;//0:disabled; 1:Fsk CallerId; 2:DTMF CallerId;,reference XMS_ANALOG_USER_CALLERID_OPT
	DJ_U8        m_u8RingCadenceType;//0: single-pulse ring; 1:double-pulse ring; 2: triple-pulse ring
	DJ_U8        m_u8CallOutRingTimeOut;// unit: second ; Ringing TimeOut ,it timetout ,then state return to DCS_FREE
	DJ_U8        m_u8Rfu1;

	DJ_U8        m_u8Rfu2[12];
}CmdParamData_AnalogUser_t;/*CmdParamData_AnalogUser_t*/


/*DTMF control param*/
typedef struct
{
 DJ_U32                      m_u32MaxValidIntervalTime;  /* max interval time: ms*/
 DJ_U8                       m_u8MaxCodeNums;            /* max DTMF nums : 0 ~ 255*/
 DJ_U8                       m_u8Rfu1[3];                /*Rfu*/
 DJ_U8                       m_u8DTMF[32];               /*DTMF buffer*/
}CmdDtmfParamData_Voice_t;/*CmdDtmfParamData_Voice_t*/


/* Struct of AddFaxFile */
typedef struct 
{
	DJ_S8       m_s8FaxFileName[128]; /* FaxFileName ,Usual As Tiff Format */
}CmdParamData_AddFax_t;/*CmdParamData_AddFax_t*/

/* Struct of StartSendFax */
typedef struct
{
	DeviceID_t m_VocDev;
	DJ_S8      m_s8LocalID[ACS_LOCALID_MAX_LEN];
}CmdParamData_StartSendFax_t;/*CmdParamData_StartSendFax_t*/

/*CmdParamData_3GPP_t*/
typedef struct
{	
    DJ_U8			m_u8AudioDecodeType;  /*audio decode type*/
	DJ_U8           m_u8AudioFrameType;   /*audio frame type,reference XMS_AUDIO_AMR_FRAME_TYPE*/
    DJ_U16			m_u16AudioDecoderCfgLen;/*audio decoder cfg length,1~ACS_MAX_DECODER_CFG_LEN*/
	DJ_U8			m_u8AudioDecoderCfg[ACS_MAX_DECODER_CFG_LEN];/*audio decoder cfg*/
	DJ_U8           m_u8AudioProfiles;    /*audio profiles?*/
    DJ_U8           m_u8AudioLevel;		  /*audio level ?*/
    DJ_U8           m_Rfu1[2];

	DJ_U8           m_u8VideoDecodeType;      /*video decode type*/
	DJ_U8           m_u8VideoFrameRate;       /*video frame rate 10 ~ 30*/
	DJ_U16          m_u16VideoDecoderCfgLen;  /*video decoder cfg length,1~ACS_MAX_DECODER_CFG_LEN*/
	DJ_U8           m_u8VideoDecoderCfg[ACS_MAX_DECODER_CFG_LEN];/*video decoder cfg?*/
	DJ_U8           m_u8VideoProfiles;        /*video profiles?*/
	DJ_U8           m_u8VideoLevel;           /*video level?*/
	DJ_U8           m_Rfu2[2];
}CmdParamData_3GPP_t;/*CmdParamData_3GPP_t*/

/*DigRecSwitchMode*/
typedef struct
{
	DJ_U8           m_u8Enable;               /*the switch of the function, 0: disable; 1: enable*/
	DJ_U8           m_u8SwitchMode;           /*the digital switch type,referece to XMS_DRS_SWITCH_TYPE*/
	DJ_U8           m_u8EncodeType;           /*audio's encode type, referece to the A-Law or Mu-law of XMS_VOC_CODE_TYPE, and any other type is not supported by keygoe*/
	DJ_U8           m_u8IsRecordSigData;      /*the switch to wheather record the signal data from DSP, 0: disable; 1: enable*/
	DJ_S8           m_Rfu1[4];                /*reserve*/
}DigRecSwitchMode;/*DigRecSwitchMode*/
/*CmdParamData_SS7RawFrame_t*/
typedef struct 
{
	DJ_U8 m_u8UserType;
	DJ_U8 m_u8LinkType;
	DJ_U8 m_u8MsgType;
	DJ_U8 m_u8InfoLen;
	
	DJ_U8 m_u8UnitId;
	DJ_U8 m_u8E1Port;
	DJ_U8 m_u8Ts;
	DJ_U8 m_u8Reserved1;
	
	DJ_U8 m_u8Info[255];
	DJ_U8 Reserved2;
}CmdParamData_SS7RawFrame_t;/*CmdParamData_SS7RawFrame_t*/

/*CmdParamdata Body*/
typedef struct
{
	union
	{
		CmdParamData_Voice_t		m_cmdParamDataVoice;     /**/		
		CmdParamData_Conf_t		    m_CmdParamDataConf;      /**/
	};/**/
}CmdParamData_t;/**/

/************************************** Event define ******************************/

#define ACS_IOEVT_MASK_ALL   0xFFFF

/*Event filter mask*/
enum XMS_IOEVT_MASK_TYPE
{
	XMS_IOEVT_MASK_GTD              = 0x0001,
	XMS_IOEVT_MASK_FSK              = 0x0002,
	XMS_IOEVT_MASK_MR2F             = 0x0004,
	XMS_IOEVT_MASK_MR2B             = 0x0008,
};

enum XMS_MEDIA_IO_TYPE
{		
	XMS_IO_TYPE_DTMF                = 1,
	XMS_IO_TYPE_MR2F                = 2,
	XMS_IO_TYPE_MR2B                = 3,
	XMS_IO_TYPE_GTG                 = 4,
	XMS_IO_TYPE_GTD                 = 4,
	XMS_IO_TYPE_FSK                 = 5,
	XMS_IO_TYPE_DPD                 = 6,
	XMS_IO_TYPE_PVD                 = 7,
	XMS_IO_TYPE_MSK                 = 8,
	XMS_IO_TYPE_VSD                 = 9,
	XMS_IO_TYPE_MSKREND             = 10,
	XMS_IO_TYPE_DRS                 = 11,
};

enum XMS_MEDIA_ERR_CODE
{
	XMS_ERR_CODE_OK                   = 0x00,
	XMS_ERR_CODE_APPSTOP              = 0x01,
	XMS_ERR_CODE_GTD                  = 0x02,
	XMS_ERR_CODE_OPENFILE_FAILED      = 0x03,
	XMS_ERR_CODE_WREADFILE_FAILED     = 0x04,
	XMS_ERR_CODE_CH_MISSING           = 0x05,
	XMS_ERR_CODE_EXE_FAILED           = 0x06,	
	XMS_ERR_CODE_BUILD_INDEX_OK       = 0x07,
	XMS_ERR_CODE_PLAY_QUEUE_FULL      = 0x08,
	XMS_ERR_CODE_3G_AUDIO_OK          = 0x09,
	XMS_ERR_CODE_3G_VIDEO_OK          = 0x0A,
	XMS_ERR_CODE_BUILD_INDEX_FAILED   = 0x0B,
	XMS_ERR_SYSTEM_OVERLOAD           = 0x0C,
};

enum XMS_VOIP_IO_TYPE
{
	XMS_IO_TYPE_VOIP_H245UII        = 1, /* VoIP H.245 UserInputIndication Event */
	XMS_IO_TYPE_VOIP_SIPINFO        = 2, /* VoIP SIP INFO Event */
	XMS_IO_TYPE_VOIP_RTPDTMF        = 3, /* VoIP RTP DTMF Event */
	XMS_IO_TYPE_VOIP_RTPTONE        = 4, /* VoIP RTP TONE */
	XMS_IO_TYPE_VOIP_FAXSTART       = 5, /* VoIP start fax mode (t.38 or t.30 pass-thru) */
	XMS_IO_TYPE_VOIP_FAXSTOP        = 6, /* VoIP stop fax mode(t.38 or t.30 pass-thru) */
	XMS_IO_TYPE_VOIP_REGSTATE       = 7, /* VoIP Registration State */
	XMS_IO_TYPE_VOIP_REGREQUEST     = 8, /* VoIP Registration Request */
	XMS_IO_TYPE_VOIP_REGRESPONSE    = 9, /* VoIP Registration Response */
	XMS_IO_TYPE_VOIP_TRANSFER       = 10,/* VoIP Device Transfer */ 
};

enum XMS_DCH_IO_TYPE
{
	XMS_IO_TYPE_DCH_I_FRAME         = 1,
	XMS_IO_TYPE_DCH_UI_FRAME        = 2,
	XMS_IO_TYPE_DCH_DEBUG           = 3,
};

enum XMS_LINESIDEE1_IO_TYPE
{
	XMS_IO_TYPE_LINESIDEE1          = 12,
};

enum XMS_ANALOG_INTERFACE_STATE
{
	XMS_ANALOG_USER_CH_OFFHOOK             = 0,
	XMS_ANALOG_USER_CH_ONHOOK              = 1,	
	XMS_ANALOG_USER_CH_FLASH               = 2,


};
enum XMS_ANALOG_INTERFACE_STATE2
{
	XMS_ANALOG_TRUNK_CH_RING               = 0, 
	XMS_ANALOG_TRUNK_CH_POSITEX_POLARITY   = 1, 
	XMS_ANALOG_TRUNK_CH_NEGATIVE_POLARITY  = 2, 
	XMS_ANALOG_TRUNK_CH_LOOPDROP           = 3,
	XMS_ANALOG_TRUNK_CH_NOLOOP             = 4, 
	XMS_ANALOG_TRUNK_CH_FLASH              = 8, 
};


enum XMS_SIG_RAWDATA_EVT_TYPE
{
	XMS_SIG_RAWDATA_EVT_BASE         = 0xFF00,	
};

enum XMS_SS7_SN_STATUS
{
	XMS_SN_PAUSE      = 0x01, // Pause
	XMS_SN_RESUME     = 0x02, // Resume
	XMS_SN_CONG       = 0x03, // Network Congested
	XMS_SN_RMTUSRUNAV = 0x04, // Remote User Unavailable
	XMS_SN_RSTBEG     = 0x05, // Restart Begins
	XMS_SN_RSTEND     = 0x06, // Restart Ends
	XMS_SN_STPCONG    = 0x07, // Stop Network Congestion
	XMS_SN_RESTRICT   = 0x08, // Restrict
	XMS_SN_LNK_CHANGE = 0x09, // Link status changed
};


enum XMS_SS7_MSG_TYPE
{
	XMS_SS7_MSG_DATA   = 0x01,
	XMS_SS7_MSG_CMD    = 0x02,
	XMS_SS7_MSG_STATUS = 0x03,
};


enum XMS_SS7_USER_TYPE
{
	XMS_SS7_USER_TYPE_SCCP = 0x03,
	XMS_SS7_USER_TYPE_TUP  = 0x04,
	XMS_SS7_USER_TYPE_ISUP = 0x05,
};


enum XMS_SS7_LINK_TYPE
{
	XMS_SS7_LINK_TYPE_ITU  = 0x02,
	XMS_SS7_LINK_TYPE_CHINA= 0x04,
};

/* Event class define*/
enum XMS_EVT_CLASS
{
	XMS_EVT_CLASS_GENERAL            = 0x0001,
	XMS_EVT_CLASS_CALLCONTROL        = 0x0002,
	XMS_EVT_CLASS_CONFPROC           = 0x0003,
	XMS_EVT_CLASS_VOIPPROC           = 0x0004,
	XMS_EVT_CLASS_MEDIAPROC          = 0x0005,
	XMS_EVT_CLASS_FAXPROC            = 0x0006,
	XMS_EVT_CLASS_IOPROC             = 0x0007,
	XMS_EVT_CLASS_SYSMONITOR         = 0x0008,
	XMS_EVT_CLASS_UNIFAILURE         = 0x0009,	
};

/*Event type define */
enum XMS_EVT_TYPE
{
	XMS_EVT_OPEN_STREAM                     = ((XMS_EVT_CLASS_GENERAL<<16)+0x01),	
	XMS_EVT_QUERY_DEVICE                    = ((XMS_EVT_CLASS_GENERAL<<16)+0x02),	
	XMS_EVT_QUERY_DEVICE_END                = ((XMS_EVT_CLASS_GENERAL<<16)+0x03),
	XMS_EVT_OPEN_DEVICE                     = ((XMS_EVT_CLASS_GENERAL<<16)+0x04),
	XMS_EVT_CLOSE_DEVICE                    = ((XMS_EVT_CLASS_GENERAL<<16)+0x05),
	XMS_EVT_RESET_DEVICE                    = ((XMS_EVT_CLASS_GENERAL<<16)+0x06),
	XMS_EVT_DEVICESTATE                     = ((XMS_EVT_CLASS_GENERAL<<16)+0x07),
	XMS_EVT_SETDEV_GROUP                    = ((XMS_EVT_CLASS_GENERAL<<16)+0x08),
	XMS_EVT_SETPARAM                        = ((XMS_EVT_CLASS_GENERAL<<16)+0x09),
	XMS_EVT_GETPARAM                        = ((XMS_EVT_CLASS_GENERAL<<16)+0x0A),
	XMS_EVT_QUERY_ONE_DSP_START             = ((XMS_EVT_CLASS_GENERAL<<16)+0x0B),
	XMS_EVT_QUERY_ONE_DSP_END               = ((XMS_EVT_CLASS_GENERAL<<16)+0x0C),
	XMS_EVT_QUERY_REMOVE_ONE_DSP_START      = ((XMS_EVT_CLASS_GENERAL<<16)+0x0D),
	XMS_EVT_QUERY_REMOVE_ONE_DSP_END        = ((XMS_EVT_CLASS_GENERAL<<16)+0x0E),
	XMS_EVT_OPEN_STREAM_EXT                 = ((XMS_EVT_CLASS_GENERAL<<16)+0x0F),	
	XMS_EVT_LICENSE_QUERY                   = ((XMS_EVT_CLASS_GENERAL<<16)+0x10),	
        XMS_EVT_E1STATE                         = ((XMS_EVT_CLASS_GENERAL<<16)+0x20),
	XMS_EVT_ACS_TIMER                       = ((XMS_EVT_CLASS_GENERAL<<16)+0xFF),
			
	XMS_EVT_CALLOUT							= ((XMS_EVT_CLASS_CALLCONTROL<<16)+0x01),
	XMS_EVT_CALLIN							= ((XMS_EVT_CLASS_CALLCONTROL<<16)+0x02),
	XMS_EVT_ALERTCALL		                = ((XMS_EVT_CLASS_CALLCONTROL<<16)+0x03),
	XMS_EVT_ANSWERCALL		                = ((XMS_EVT_CLASS_CALLCONTROL<<16)+0x04),
	XMS_EVT_LINKDEVICE				        = ((XMS_EVT_CLASS_CALLCONTROL<<16)+0x05),
	XMS_EVT_UNLINKDEVICE					= ((XMS_EVT_CLASS_CALLCONTROL<<16)+0x06),
	XMS_EVT_CLEARCALL						= ((XMS_EVT_CLASS_CALLCONTROL<<16)+0x07),
	XMS_EVT_ANALOG_INTERFACE                = ((XMS_EVT_CLASS_CALLCONTROL<<16)+0x08),
	XMS_EVT_CAS_MFC_START					= ((XMS_EVT_CLASS_CALLCONTROL<<16)+0x09),
	XMS_EVT_CAS_MFC_END						= ((XMS_EVT_CLASS_CALLCONTROL<<16)+0x0A),
	XMS_EVT_SENDSIGMSG						= ((XMS_EVT_CLASS_CALLCONTROL<<16)+0x0B),		
	XMS_EVT_SIGMON       					= ((XMS_EVT_CLASS_CALLCONTROL<<16)+0x0C),
	XMS_EVT_3G324M       					= ((XMS_EVT_CLASS_CALLCONTROL<<16)+0x0D),
	XMS_EVT_CTX_REG                         = ((XMS_EVT_CLASS_CALLCONTROL<<16)+0x0E),
	XMS_EVT_CTX_LINK                        = ((XMS_EVT_CLASS_CALLCONTROL<<16)+0x0F),
	XMS_EVT_CTX_APPDATA                     = ((XMS_EVT_CLASS_CALLCONTROL<<16)+0x10),
	XMS_EVT_DRS_CALL_NUMS                   = ((XMS_EVT_CLASS_CALLCONTROL<<16)+0x11),
	
	
	XMS_EVT_SS7RawFrame						= ((XMS_EVT_CLASS_CALLCONTROL<<16)+0x20),
	XMS_EVT_SS7SendRawFrame					= ((XMS_EVT_CLASS_CALLCONTROL<<16)+0x21),

		
	XMS_EVT_JOINTOCONF					    = ((XMS_EVT_CLASS_CONFPROC<<16)+0x01),
	XMS_EVT_LEAVEFROMCONF					= ((XMS_EVT_CLASS_CONFPROC<<16)+0x02),
	XMS_EVT_CLEARCONF					    = ((XMS_EVT_CLASS_CONFPROC<<16)+0x03),
		
	XMS_EVT_PLAY			                = ((XMS_EVT_CLASS_MEDIAPROC<<16)+0x01),
	XMS_EVT_INITINDEX				        = ((XMS_EVT_CLASS_MEDIAPROC<<16)+0x02),
	XMS_EVT_BUILDINDEX						= ((XMS_EVT_CLASS_MEDIAPROC<<16)+0x03),
	XMS_EVT_CONTROLPLAY				        = ((XMS_EVT_CLASS_MEDIAPROC<<16)+0x04),
	XMS_EVT_RECORD							= ((XMS_EVT_CLASS_MEDIAPROC<<16)+0x05),
	XMS_EVT_CONTROLRECORD				    = ((XMS_EVT_CLASS_MEDIAPROC<<16)+0x06),
	XMS_EVT_RECORDCSP   				    = ((XMS_EVT_CLASS_MEDIAPROC<<16)+0x07),
	XMS_EVT_CONTROLRECORDCSP    		    = ((XMS_EVT_CLASS_MEDIAPROC<<16)+0x08),
    XMS_EVT_3GPP_PLAY			            = ((XMS_EVT_CLASS_MEDIAPROC<<16)+0x09),
	XMS_EVT_3GPP_CONTROLPLAY		        = ((XMS_EVT_CLASS_MEDIAPROC<<16)+0x0A),
	XMS_EVT_3GPP_RECORD	    				= ((XMS_EVT_CLASS_MEDIAPROC<<16)+0x0B),
	XMS_EVT_3GPP_CONTROLRECORD    		    = ((XMS_EVT_CLASS_MEDIAPROC<<16)+0x0C),
	XMS_EVT_PLAYCSPREQ 				        = ((XMS_EVT_CLASS_MEDIAPROC<<16)+0x0D),
	XMS_EVT_3GPP_INIT_INDEX                           = ((XMS_EVT_CLASS_MEDIAPROC<<16)+0x0E),
	XMS_EVT_3GPP_BUILD_INDEX                          = ((XMS_EVT_CLASS_MEDIAPROC<<16)+0x0F),
	
	XMS_EVT_SENDFAX			                = ((XMS_EVT_CLASS_FAXPROC<<16)+0x01),
	XMS_EVT_RECVFAX					        = ((XMS_EVT_CLASS_FAXPROC<<16)+0x02),
			
	XMS_EVT_SENDIODATA						= ((XMS_EVT_CLASS_IOPROC<<16)+0x01),
	XMS_EVT_RECVIODATA						= ((XMS_EVT_CLASS_IOPROC<<16)+0x02),
		
	XMS_EVT_MODMONITOR				        = ((XMS_EVT_CLASS_SYSMONITOR<<16)+0x01),
	XMS_EVT_DEV_TIMER						= ((XMS_EVT_CLASS_SYSMONITOR<<16)+0x02),
	
	XMS_EVT_UNIFAILURE						= ((XMS_EVT_CLASS_UNIFAILURE<<16)+0x01),	
};

/*Error code define*/
#define  ACSPOSITIVE_ACK                0x01

#define  ACSERR_LOADLIBERR              0x01
#define  ACSERR_BADPARAMETER            0x02
#define  ACSERR_NOSERVER                0x03
#define  ACSERR_MAXCONN_EXCEED          0x04
#define  ACSERR_BADHDL                  0x05
#define  ACSERR_FAILSEND                0x06
#define  ACSERR_LINKBROKEN              0x07
#define  ACSERR_NOMESSAGE               0x08
#define  ACSERR_GETRESFAIL              0x09	
#define  ACSERR_NOSUPPORTCMD            0x0A
#define  ACSERR_DEVICEERR               0x0B
#define  ACSERR_CTX_REGOVERRUN     		0x0C
#define	 ACSERR_CTX_REG_NAMEEXIS        0x0D
#define	 ACSERR_CTX_UNREG_NOREG		    0x0E
#define  ACSERR_CTX_TRAN_NODEST		    0x0F
#define  ACSERR_CTX_TRAN_NOSRC		    0x10
#define  ACSERR_CTX_LINK_FAIL           0x11
#define  ACSERR_CTX_APPDATA_NODEST      0x12
#define  ACSERR_CTX_APPDATA_NOSRC       0x13


/*Acs_Dev_List_Head_t*/
typedef struct
{
	DJ_S32        m_s32DeviceMain;   /*device Main*/
	DJ_S32        m_s32ModuleID;     /*Device Module ID*/
	DJ_S32        m_s32DeviceNum;    /*device num*/
}Acs_Dev_List_Head_t;/**/

/*general proc evt data*/
typedef struct
{
	Acs_Evt_State_t         m_s32AcsEvtState;    /*acs event state, 1: success; 0: failed*/
	Acs_Evt_ErrCode_t       m_s32AcsEvtErrCode;  /*acs event errcode,reference XMS_ERROR_CODE_DEF*/
	DJ_S32                  m_s32DeviceState;    /*Device State*/
	DJ_S32                  m_s32Resrv;          /*reserved*/
	PrivateData_t           m_PrivData;          /*private data*/
}Acs_GeneralProc_Data;/**/

/*Acs_OpenDev_Evt*/
typedef struct
{
	ACS_EVT_HEAD              m_AcsEvtHead;     /*acs event head*/
	Acs_GeneralProc_Data      m_AcsGenLProcData; /*acs general proc event data*/
}Acs_GeneralProc_Evt;/**/

/*OpenStreamExt evt data*/
typedef struct
{
	ServerID_t      m_ServerID;
	DJ_S8           m_s8Rfu1[16];
}Acs_OpenStreamExt_Data;/**/

/*Acs_OpenStreamExt_Evt*/
typedef struct
{
	ACS_EVT_HEAD                m_AcsEvtHead;           /*acs event head*/
	Acs_OpenStreamExt_Data      m_AcsOpenStreamExtData; /*acs general proc event data*/
}Acs_OpenStreamExt_Evt;/**/

typedef struct
{
	Acs_Evt_State_t         m_s32AcsEvtState;    /*acs event state, 1: success; 0: failed*/
	Acs_Evt_ErrCode_t       m_s32AcsEvtErrCode;  /*acs event errcode,reference XMS_ERROR_CODE_DEF*/
	DJ_U16                  m_u16ParamCmdType;  /*acs param Cmd Type*/
	DJ_U16                  m_u16ParamDataSize; /*acs param Data Size*/
}Acs_ParamProc_Data;/**/

typedef struct
{
	ACS_EVT_HEAD              m_AcsEvtHead;     /*acs event head*/
	Acs_ParamProc_Data        m_ParamProcData;
}Acs_ParamData_Evt;/**/

typedef struct
{
	DJ_U8	m_u8MacAddr[6];	   /* Mac address */
	DJ_U8	m_u8ChassisType;   /* chassis type��0: ITP1200; 1:3U chassis; 2:5U chassis; 3: 11U chassis */
	DJ_U8	m_u8ChassisIndex;  /* chassis  No */
	DJ_U8	m_u8ChassisSlot;   /* chassis slot number */
	DJ_U8	m_u8SubBoardIndex; /* module index inside one board*/
	DJ_U8	m_u8BoardType;	   /* Board type��0:itp1200;1:itp2400; 2: itp4800*/
	DJ_U8	rfu1[1];
	DJ_S8	m_s8SystemName[32];/* System name */	
	DJ_S8	m_s8PlatformType;  /* Platform: 1:callback; 2:keygoe 1000; 3:keygoe 3000; 4:keygoe 8000*/
	DJ_S8	m_s8UserCode[7];   /* User code */
	DJ_S32	m_s32SysId;        /* sys id*/
	DJ_S8	rfu2[20];

	DJ_S8	m_s8FirmwareName[8];	/* DSP firmwarename */
	DJ_S8	m_s8FirmwareVersion[8];	/* DSP firmware version*/
	DJ_S8	m_s8RevisionDate[8];	/* DSP firmware date*/
}Acs_ParamData_UserReadXmsBoardInfo;

/* CAS Extra Infomation, used by XMS_EVT_CAS_MFC_END*/
typedef struct
{
	/* Call In Infomation */
	DJ_U8                     m_u8KA; /* Caller's KA */
	DJ_U8                     m_u8KD; /* Caller's KD */
	/* Call Out Infomation */

	/* Other */
	DJ_U8                     m_u8Rfu[14];/* Reserved for Future Use */
}Acs_CAS_ExtraInfo_t;/*Acs_CAS_ExtraInfo_t*/

/*Acs_CallControl_Evt*/
typedef struct
{
	ACS_EVT_HEAD              m_AcsEvtHead;     /*acs event head*/
	Acs_CAS_ExtraInfo_t       m_AcsCASExtraInfo; /*acs CAS ExtraInfo event data*/
}Acs_CAS_MFC_Evt;/**/

/*Acs_CallControl_Data*/
typedef struct
{
	Acs_Evt_State_t         m_s32AcsEvtState;   /*acs event state, 1: success; 0: failed*/
	Acs_Evt_ErrCode_t       m_s32AcsEvtErrCode; /*acs event errcode,reference XMS_ERROR_CODE_DEF*/
	DJ_S32                  m_s32CallClearCause; /*Call clear cause code*/
	DJ_U8                   m_u8CallingCategory;/*Calling Category*/
	DJ_U8                   m_u8CallingAddressIndicator;/*Address Indicator*/
	DJ_U8                   m_u8CalledAddressIndicator;/*Address Indicator*/
	DJ_S8                   m_s8Rfu1[1];
	CallNum_t               m_s8CallingNum[ACS_MAX_CALL_NUM]; /*calling num*/
	CallNum_t               m_s8CalledNum[ACS_MAX_CALL_NUM];  /*called num*/
	DJ_U8                   m_u8DRSCallDirectFlag;            /*Call direction flag, 0: Call In; 1: Call Out, reference XMS_DRS_CALL_DIRECT_DEF*/
	DJ_S8                   m_s8Rfu2[31];
	PrivateData_t           m_PrivData;/*private data*/
}Acs_CallControl_Data;/**/
enum XMS_DRS_CALL_DIRECT_DEF
{
	XMS_DRS_CALL_DIRECT_CALL_IN           = 0,
	XMS_DRS_CALL_DIRECT_CALL_OUT          = 1,
};

/*Acs_CallControl_Evt*/
typedef struct
{
	ACS_EVT_HEAD              m_AcsEvtHead;     /*acs event head*/
	Acs_CallControl_Data      m_AcsCallControl; /*acs call control event data*/
}Acs_CallControl_Evt;/**/

/*Acs_LinkDev_Data*/
typedef struct
{
	Acs_Evt_State_t         m_s32AcsEvtState;   /*acs event state, 1: success; 0: failed*/
	Acs_Evt_ErrCode_t       m_s32AcsEvtErrCode; /*acs event errcode,reference XMS_ERROR_CODE_DEF*/
	DeviceID_t              m_SrcDevice;
	DeviceID_t              m_DestDevice;
}Acs_LinkDev_Data;/*Acs_LinkDev_Data*/

/*Acs_LinkDev_Evt*/
typedef struct
{
	ACS_EVT_HEAD              m_AcsEvtHead;     /*acs event head*/
	Acs_LinkDev_Data          m_AcsLinkDev;     /*acs Link Device event data*/
}Acs_LinkDev_Evt;/*Acs_LinkDev_Evt*/

/*Acs_CtxReg_Data*/
typedef struct
{
	Acs_Evt_State_t         m_s32AcsEvtState;   /*acs event state, 1: success; -1: failed*/
	Acs_Evt_ErrCode_t       m_s32AcsEvtErrCode; /*acs event errcode,reference XMS_ERROR_CODE_DEF*/
}Acs_CtxReg_Data;/**/

/*Acs_CtxReg_Evt*/
typedef struct
{
	ACS_EVT_HEAD              m_AcsEvtHead;     /*acs event head*/
	Acs_CtxReg_Data           m_AcsCtxReg;      /*acs ctxReg evt*/
}Acs_CtxReg_Evt;/**/

/*Acs_CtxLink_Data*/
typedef struct
{
	Acs_Evt_State_t         m_s32AcsEvtState;   /*acs event state, 1: success; 0: failed*/
	Acs_Evt_ErrCode_t       m_s32AcsEvtErrCode; /*acs event errcode,reference XMS_ERROR_CODE_DEF*/
}Acs_CtxLink_Data;/**/

/*Acs_CtxLink_Evt*/
typedef struct
{
	ACS_EVT_HEAD              m_AcsEvtHead;     /*acs event head*/
	Acs_CtxLink_Data          m_AcsCtxLink;
}Acs_CtxLink_Evt;/**/

/*CTX App Data Info*/
typedef struct
{
	DJ_U32      m_u32AppDataSize; /*user APP data size,not include XMS_AppData_Info_t*/
	DJ_U8       m_u8AppReqType;   /*user APP req cmd type,user self define*/
	DJ_U8       m_u8AcsUnitID;
	DJ_U8       m_Rfu1[2];      
	DJ_S8       m_s8SrcRegName[XMS_MAX_REGNAME_LEN]; /*src reg name*/
	DJ_S8       m_s8DestRegName[XMS_MAX_REGNAME_LEN];/*dest reg name*/
	DJ_U8       m_u8SrcAppUnitID;
	DJ_U8       m_u8DstAppUnitID;
	DJ_S8       m_Rfu2[2];
	DeviceID_t  m_srcDevice;
	DJ_S8       m_Rfu3[20];
	DJ_S32      m_s32Rst;
}CTX_AppData_Info;/**/

/*Acs_CtxAppData_Info*/
typedef struct
{
	Acs_Evt_State_t         m_s32AcsEvtState;   /*acs event state, 1: success; 0: failed*/
	Acs_Evt_ErrCode_t       m_s32AcsEvtErrCode; /*acs event errcode,reference XMS_ERROR_CODE_DEF*/
	CTX_AppData_Info        m_AppDataInfo;
}Acs_CtxAppData_Info;/**/

/*Acs_CtxAppData_Evt*/
typedef struct
{
	ACS_EVT_HEAD             m_AcsEvtHead;     /*acs event head*/
	Acs_CtxAppData_Info      m_AcsCtxAppData;
}Acs_CtxAppData_Evt;/**/

/*Acs_CtxState_Info*/
typedef struct
{
	Acs_Evt_State_t         m_s32AcsEvtState;   /*acs event state, 1: success; 0: failed*/
	Acs_Evt_ErrCode_t       m_s32AcsEvtErrCode; /*acs event errcode,reference XMS_ERROR_CODE_DEF*/
	DJ_U32                  m_u32CtxState;
	DJ_S8                   m_s8SrcRegName[XMS_MAX_REGNAME_LEN];
	DJ_S8                   m_s8DestRegName[XMS_MAX_REGNAME_LEN];
}Acs_CtxState_Info;/**/

/*Acs_Ctx_State_Evt*/
typedef struct
{
	ACS_EVT_HEAD             m_AcsEvtHead;     /*acs event head*/
	Acs_CtxState_Info        m_AcsCtxState;
}Acs_Ctx_State_Evt;/**/

/*Acs_SigMon_Data*/
typedef struct
{
	SMON_EVENT           m_SigMon;            /*sig monitor data*/
	PrivateData_t        m_PrivData;          /*private data*/
}Acs_SigMon_Data;/*Acs_SigMon_Data*/

/*Acs_SigMon_Evt*/
typedef struct
{
	ACS_EVT_HEAD              m_AcsEvtHead;     /*acs event head*/
	Acs_SigMon_Data           m_AcsSigMon;      /*acs call control event data*/
}Acs_SigMon_Evt;/**/

/*Acs_3G324_Data*/
typedef	struct
{
	DJ_U8	m_u8DataType;		    /*3G324 event data type*/
	DJ_S8	Rfu1;				    /*reserved*/
	DJ_U16	m_u16DataLen;	        /*3G324 event data size*/	
}Acs_3G324_Data;/*Acs_3G324_Data*/

/*Acs_3G324_Evt*/
typedef struct
{
	ACS_EVT_HEAD		m_AcsEvtHead;  /*acs event head*/	
	Acs_3G324_Data		m_Acs324Data;  /*acs 3G324 event*/
}Acs_3G324_Evt;/**/

/*Acs_AnalogInterface_Data*/
typedef struct
{
	Acs_Evt_State_t         m_s32AcsEvtState;   /*acs event state, 1: success; 0: failed*/
	Acs_Evt_ErrCode_t       m_s32AcsEvtErrCode; /*acs event errcode,reference XMS_ERROR_CODE_DEF*/
	DJ_U8                   m_u8AnalogInterfaceState;/*Analog Interface State,reference XMS_ANALOG_INTERFACE_STATE*/
	DJ_U8                   m_Rfu1[3];
	PrivateData_t           m_PrivData;          /*private data*/
}Acs_AnalogInterface_Data;/**/

/*Acs_AnalogInterface_Evt*/
typedef struct
{
	ACS_EVT_HEAD                  m_AcsEvtHead;     /*acs event head*/
	Acs_AnalogInterface_Data      m_AcsAnalogInterfaceData; /*acs Analog Interface event data*/
}Acs_AnalogInterface_Evt;/**/

/*Acs_SendSigMsg_Data*/
typedef struct
{
	Acs_Evt_State_t         m_s32AcsEvtState;    /*acs event state, 1: success; 0: failed*/
	Acs_Evt_ErrCode_t       m_s32AcsEvtErrCode;  /*acs event errcode,reference XMS_ERROR_CODE_DEF*/
	DJ_S16                  m_s16SignalMsgType;  /*acs signal msg type*/
	DJ_S8                   m_s8Rfu[2];          /*Rfu*/
}Acs_SendSigMsg_Data;/**/

/*Acs_SendSigMsg_Evt*/
typedef struct
{
	ACS_EVT_HEAD              m_AcsEvtHead;     /*acs event head*/
	Acs_SendSigMsg_Data       m_AcsSendSigMsg;  /*acs send signal msg event data*/
}Acs_SendSigMsg_Evt;/**/

/*Acs_ConfControl_Data*/
typedef struct
{
	Acs_Evt_State_t         m_s32AcsEvtState;    /*acs event state, 1: success; 0: failed*/
	Acs_Evt_ErrCode_t       m_s32AcsEvtErrCode;  /*acs event errcode,reference XMS_ERROR_CODE_DEF*/
	DJ_S32                  m_s32ConfMembers;    /*conference member num*/		
	DeviceID_t              m_MediaDevice;
	PrivateData_t           m_PrivData;/*private data*/
}Acs_ConfControl_Data;/**/

/*Acs_ConfControl_Evt*/
typedef struct
{
	ACS_EVT_HEAD              m_AcsEvtHead;      /*acs event head*/
	Acs_ConfControl_Data      m_AcsConfControl;  /*acs call control event data*/
}Acs_ConfControl_Evt;/**/

/*Acs_MediaProc_Data*/
typedef struct
{
	Acs_Evt_State_t         m_s32AcsEvtState;    /*acs event state, 1: success; 0: failed*/
	Acs_Evt_ErrCode_t       m_s32AcsEvtErrCode;  /*acs event errcode ,reference XMS_MEDIA_ERR_CODE*/	
	DJ_U8                   m_u8TaskID;          /*play Task ID*/
	DJ_U8                   m_u8GTD;             /*GTD ID*/	
	DJ_S8                   m_s8Rfu[2];
	DJ_U32                  m_u32CurFileIndex;   /*current play file index*/
	DJ_U32                  m_u32CurFilePos;     /*current play pos*/
	PrivateData_t           m_PrivData;          /*private data*/
}Acs_MediaProc_Data;/**/

/*Acs_MediaProc_Evt*/
typedef struct
{
	ACS_EVT_HEAD              m_AcsEvtHead;    /*acs event head*/
	Acs_MediaProc_Data        m_AcsMediaProc;  /*acs media proc event data*/
}Acs_MediaProc_Evt;/**/

/*Acs_CSPDataReq_Data*/
typedef	struct
{
	DJ_U16			m_u16ReqCspDataLen; /*DSP request data length*/
	DJ_U8			m_u8TaskID;	        /*Task ID*/
	DJ_U8			m_u8DataType;	    /*Data Type: reference XMS_CSPPLAY_DATA_TYPE*/
}Acs_CSPDataReq_Data;/*Acs_CSPDataReq_Data*/

/*Acs_CSPDataReq_Evt*/
typedef struct
{
	ACS_EVT_HEAD              m_AcsEvtHead;         /*acs event head*/
	Acs_CSPDataReq_Data       m_AcsCSPDataReqProc;  /*acs media CSP play request data*/
}Acs_CSPDataReq_Evt;/**/


#define		ITP_MAX_STREAM_DATA_LENGTH		4000

/*Acs_MediaCSPProc_Data*/
typedef	struct
{
	DJ_U16	m_u16DataLen;	        /*stream data size*/
	DJ_U8	m_u8DataType;		    /*stream data type*/
	DJ_U8	m_u8TagNumber;			/*Rfu*/
	DJ_U8	m_u8StreamData[ITP_MAX_STREAM_DATA_LENGTH ];	/*stream data*/
}Acs_MediaCSPProc_Data;

/*Acs_MediaCSPProc_Evt*/
typedef struct
{
	ACS_EVT_HEAD              m_AcsEvtHead;       /*acs event head*/
	Acs_MediaCSPProc_Data     m_AcsMediaCSPProc;  /*acs media CSP proc event data*/
}Acs_MediaCSPProc_Evt;/**/

/*Acs_Media3GPPProc_Data*/
typedef struct
{
	Acs_Evt_State_t         m_s32AcsEvtState;         /*common: acs event state, 1: success; 0: failed*/
	Acs_Evt_ErrCode_t       m_s32AcsEvtErrCode;       /*common: acs event errcode ,reference XMS_ERROR_CODE_DEF*/	
	DJ_U8                   m_u8AudioTaskID;          /*audio:  play Task ID*/
	DJ_U8                   m_u8MediaType;            /*common: media type XMS_MEDIA_TYPE*/
	DJ_U8                   m_u8VideoTaskID;          /*video:  play Task ID*/
	DJ_S8                   m_s8Rfu;
	DJ_U32                  m_u32AudioCurFileIndex;   /*audio:current play index*/
	DJ_U32                  m_u32AudioCurFilePos;     /*audio:current play pos*/
	DJ_U32                  m_u32VideoCurFileIndex;   /*video:current play index*/
	DJ_U32                  m_u32VideoCurFilePos;     /*video:current play pos*/
	PrivateData_t           m_PrivData;/*private data*/
}Acs_Media3GPPProc_Data;/**/

/*Acs_Media3GPPProc_Evt*/
typedef struct
{
	ACS_EVT_HEAD              m_AcsEvtHead;    /*acs event head*/
	Acs_Media3GPPProc_Data    m_AcsMedia3GPPProc;  /*acs media proc event data*/
}Acs_Media3GPPProc_Evt;/**/

/*Acs_IO_Data*/
typedef struct
{
	Acs_Evt_State_t      m_s32AcsEvtState;   /*acs event state*/
	Acs_Evt_ErrCode_t    m_s32AcsEvtErrCode; /*acs event errcode*/	
    DJ_U16               m_u16IoType;     /*io proc type*/
	DJ_U16               m_u16IoDataLen;  /*io data len*/  
}Acs_IO_Data;/**/

/*Acs_IO_Evt*/
typedef struct
{
	ACS_EVT_HEAD          m_AcsEvtHead; /*acs event head*/	
	Acs_IO_Data           m_AcsIOData;  /*acs io event*/
}Acs_IO_Evt;/**/

/*Acs License Query event*/
typedef struct
{
	DJ_U8     m_u8ModuleID;          /*license alarm Board module ID*/
	DJ_U16    m_u16LicLeftDay;        /*license alarm Board left license days,0: No license or license overdue;100:license forever; else: left license days*/
	DJ_S8     m_Rfu1[1];
}Acs_LicenseQuery_Data;/**/

/*Acs_LicenseAlarm_Evt*/
typedef struct
{
	ACS_EVT_HEAD              m_AcsEvtHead;       /*acs event head*/
	Acs_LicenseQuery_Data     m_LicenseQueryData; /*universal failure confirmation event*/	
}Acs_LicenseQuery_Evt;/**/

/*Acs unifail event*/
typedef struct
{
	Acs_Evt_State_t          m_s32AcsEvtState;   /*acs event state, -1*/
	Acs_Evt_ErrCode_t        m_s32AcsEvtErrCode; /*acs event errcode,reference XMS_ERROR_CODE_DEF*/
}Acs_UniFailure_Data;/**/

/*Acs_UniFailure_Evt*/
typedef struct
{
	ACS_EVT_HEAD              m_AcsEvtHead;     /*acs event head*/
	Acs_UniFailure_Data       m_UniverFailData; /*universal failure confirmation event*/	
}Acs_UniFailure_Evt;/**/

/*Acs_FaxEnd_Data*/
typedef struct
{
	Acs_Evt_State_t      m_s32AcsEvtState;   /*acs event state*/
	Acs_Evt_ErrCode_t    m_s32AcsEvtErrCode; /*acs event errcode*/	
	DJ_U8                m_u8ErrorStep;      /*error state of last fax operation*/
	DJ_U8                m_u8T30SendState;   /*the state the T30 command just sent*/
	DJ_U8                m_u8RecvT30Cmd;     /*the T30 command just received*/
	DJ_U8                m_u8Rfu1;            /*Reserved*/ 
	DJ_U16               m_u16TotalPages;    /*total pages,send: sending total pages successfully  ;rev: receiving total pages successfully*/
    DJ_S8                m_s8RemoteID[ACS_REMOTEID_MAX_LEN];/*Remote ID*/	
	DJ_U8                m_u8Rfu2[2];           /*Reserved*/
	PrivateData_t        m_PrivData;/*private data*/
}Acs_FaxProc_Data;/**/

/*Acs_FaxProc_Evt*/
typedef struct
{
	ACS_EVT_HEAD          m_AcsEvtHead;     /*acs event head*/	
	Acs_FaxProc_Data      m_AcsFaxProcData; /*acs fax proc data*/
}Acs_FaxProc_Evt;/**/

/*ITP_3GPP_PARAM_CFG*/
typedef struct
{
    DJ_U8	    m_u8AudioDecodeType;      /*audio decode type*/
    DJ_U8           m_u8AudioFrameType;       /*audio frame type,reference XMS_AUDIO_AMR_FRAME_TYPE*/
    DJ_U16	    m_u16AudioDecoderCfgLen;  /*audio decoder cfg length,1~ACS_MAX_DECODER_CFG_LEN*/
    DJ_U8           m_u8AudioDecoderCfg[ACS_MAX_DECODER_CFG_LEN];/*audio decoder cfg*/
    DJ_U8           m_u8AudioProfiles;        /*audio profiles?*/
    DJ_U8           m_u8AudioLevel;		  /*audio level ?*/
    DJ_U8           m_Rfu1[2];

    DJ_U8           m_u8VideoDecodeType;      /*video decode type*/
    DJ_U8           m_u8VideoFrameRate;       /*video frame rate 10 ~ 30*/
    DJ_U16          m_u16VideoDecoderCfgLen;  /*video decoder cfg length,1~ACS_MAX_DECODER_CFG_LEN*/
    DJ_U8           m_u8VideoDecoderCfg[ACS_MAX_DECODER_CFG_LEN];/*video decoder cfg?*/
    DJ_U8           m_u8VideoProfiles;        /*video profiles?*/
    DJ_U8           m_u8VideoLevel;           /*video level?*/
    DJ_U8           m_Rfu2[2];  

    DJ_U32          m_u32TrackNums;           /*track numbers*/
    DJ_U16          m_u16AudioMaxBitRate;
    DJ_U16          m_u16AudioAverageBitRate;
    DJ_U32          m_u32AudioBitMask;
    DJ_U16          m_u16VideoMaxBitRate;
    DJ_U16          m_u16VideoAverageBitRate;
    DJ_U32          m_u32VideoBitMask;
    DJ_U16          m_u16VideoWidth;
    DJ_U16          m_u16VideoHeight;
	
}ITP_3GPP_PARAM_CFG;/*ITP_3GPP_PARAM_CFG*/

/*CmdParamData_324CH_t*/
typedef struct {
	DJ_U8	m_u8Channel_Enable;		/* channel enable flag, 1 - enable, 0 - disable*/
	
	DJ_U8	m_u8AudioSRC1_Ctrl;		/* audio channel source 1 type */
	DJ_U8	m_u8AudioSRC2_Ctrl;		/* audio channel source 2 type */
	DJ_U8	m_u8VideoSRC1_Ctrl;		/* video channel source type */
	DJ_U16	m_u16AudioSRC1_ID;		/* audio channel source 1 id */
	DJ_U16	m_u16AudioSRC2_ID;		/* audio channel source 2 id */
	DJ_U16	m_u16VideoSRC1_ID;		/* video channel source id */

	DJ_U8 	m_u8AudioInDecodeFlag;	/* 1 - input decode, 0 - no decode */
	DJ_U8 	m_u8AudioOutEncodeFlag; /* 1 - output encode, 0 - no encode */
	DJ_U8 	m_u8AudioInCodec;		/* audio input codec */
	DJ_U8	m_u8AudioOutCodec;		/* audio output codec */
	DJ_U8	m_u8VideoInCodec;		/* video input codec */
	DJ_U8	m_u8VideoOutCodec;		/* video output codec */
}CmdParamData_324CH_t;

/*EvtH245SIG_PDU*/
typedef struct {

	DJ_U8	m_u8H245SigPdu[ACS_MAX_H245_SIG_PDU_LEN];		/* H.245 signal data */
}Evt_H245_SIG_PDU;

/*EvtH245USER_INPUT*/
typedef struct{
	DJ_U8 m_u8Type;		/* type=1: signal is valid, type=2: str is valid*/
	DJ_S8 m_s8Signal;		
	DJ_S8 str[126];     
}H245_UII;

typedef  struct
{
	DJ_U16  m_u16ChanEnable;  // 1: enable; 0: disable
}ITP_3G_FaxToneStruct_t;/*ITP_3G_FaxToneStruct_t;*/

typedef  struct
{
	DJ_U16   m_u16ChanState;  //1��ofax_tone; 0: voice tone
}ITP_3G_FaxToneEventStruct_t;/*ITP_3G_FaxToneEventStruct_t;*/

typedef struct
{
	DJ_S32   s32WithHeader;   /* 0-NoHeader ,1-WithHeader*/
	DJ_S8    s8HeaderUser[20];/* User*/
	DJ_S8    s8HeaderFrom[20];/* From*/
	DJ_S8    s8HeaderTo[20];  /* To */
}CmdParamData_FaxSetHeader_t;/*CmdParamData_FaxSetHeader_t*/


/*Acs_SS7RawFrame_Data*/
typedef struct
{
	DJ_U8 m_u8UserType; /*SS7 frame user type,reference XMS_SS7_FRAME_USER_TYPE*/
	DJ_U8 m_u8LinkType; /*SS7 frame link type,reference XMS_SS7_FRAME_LINK_TYPE*/
	DJ_U8 m_u8InfoType; /*SS7 frame info type,reference XMS_SS7_FRAME_INFO_TYPE*/
	DJ_U8 m_u8InfoLen;  /*SS7 frame info len*/
	DJ_U8 m_u8Rfu1[4];
	DJ_U8 m_u8Info[255];/*SS7 frame info*/
	DJ_U8 m_u8Rfu2;
}Acs_SS7RawFrame_Data;/**/

/*Acs_SS7RawFrame_Evt*/
typedef struct
{
	ACS_EVT_HEAD              m_AcsEvtHead;         /*acs event head*/	
	Acs_SS7RawFrame_Data		  m_AcsSS7RawFrameData; /*acs ss7 ras frame data*/
}Acs_SS7RawFrame_Evt;/**/


/*Acs_MediaPrivateExt_Data*/
typedef struct
{
	DJ_U32 m_u32ExtType;        //reference to XMS_PRIVATE_EXT_TYPE
	DJ_U8  m_u8Buf[MAX_PATH];
}Acs_MediaPrivateExt_Data;/*Acs_MediaPrivateExt_Data*/ 


/*Acs_MediaECM_OffSet*/
typedef struct 
{
	DJ_U32  m_u32Play_FileOffset; //�ļ�ƫ��,��λΪ1bytes
	DJ_U32  m_u32Play_Max_Length; //������������(��λ���ֽ�)
}Acs_MediaECM_OffSet;/*Acs_MediaECM_OffSet*/

/*Acs_MediaECM_CMD*/
typedef struct 
{
	DJ_U16  m_u16FrameNums;
	DJ_U16  m_u16Reserved[3];
	Acs_MediaECM_OffSet m_stOffSet[256];
}Acs_MediaECM_CMD;/*Acs_MediaECM_CMD*/

/*Acs_AnalogEM_CMD*/
typedef struct 
{
	DJ_U8	m_u8Channel_Enable;
	DJ_U8	m_u8Type;
	DJ_U8	m_u8Tx_State;
	DJ_U8	m_u8Rx_State;
}Acs_AnalogEM_CMD;/*Acs_AnalogEM_CMD*/

/*Acs_FAXECM_MODE_Cmd*/
typedef struct
{
	DJ_U8  m_u8FaxECMMode;           /*referent to XMS_FAX_ECM_MODE_TYPE*/	
	DJ_U8  m_u8Ref[3];               /*reserve*/
}Acs_FAXECM_MODE_Cmd;/*Acs_FAXECM_MODE_Cmd*/

typedef struct
{    
    DJ_U8       m_u8ModuleType;       /*Module type*/
    DJ_U8       m_u8UnitID;           /*Module unitid*/        
    DJ_U16      m_u16ModuleState;     /*Module state,reference XMS_SYSMON_MODSTATE*/        
    DJ_S8       m_s8IPAddress[32];    /*Module IP address*/
    DJ_U32      m_u32Port;            /*Module port*/
    DJ_S8       m_s8Rfu[16];          /*rfu*/
}Acs_SysMon_Data;/**/

/*Acs_SysMon_Evt*/
typedef struct
{
	ACS_EVT_HEAD          m_AcsEvtHead;     /*acs event head*/	
	Acs_SysMon_Data       m_AcsSysMonData; /*acs fax proc data*/
}Acs_SysMon_Evt;/**/
typedef struct {
	DJ_U8	m_u8Lost_signal;	//1:���źţ�0�����ź�
	DJ_U8	m_u8Fas_align;	//0��ͬ����1��ʧ��
	DJ_U8	m_u8Mf_align; 	//0��ͬ����1��ʧ��
	DJ_U8	m_u8Crc4_align; 	//0��ͬ����1��ʧ��
	DJ_U8	m_u8Remote_alarm; 	//0���޸澯��1��Զ�˸澯
	DJ_U8	m_u8Remote_MF_alarm;	//0���޸澯��1��Զ�˸澯
	DJ_U8	m_u8Rx_level;	//0-15: *(-2.5db)
	DJ_U8	m_u8Tx_open;	//1:open circuit,
	DJ_U8	m_u8Tx_overlimit;	//1:current over limit
	DJ_U8	m_u8Port_Current_State;	//0: error state; 1: normal working state
	DJ_U8	m_u8Port_Normal_Count;	//seconds of digital port normal working
	DJ_U8	m_u8Port_Error_Count;	// seconds of digital port error
	DJ_U32	m_u32total_seconds;	//ͳ����ʱ��
	DJ_U32	m_u32RLOS_seconds;	//����ʧ��������
	DJ_U32 	m_u32LRCL_seconds;	//�������ź�������
	DJ_U32 	m_u32RUA1_seconds;	//����ȫ1�ź�������
	DJ_U32 	m_u32RRA_seconds;	//Զ�˸澯������
	DJ_U32 	m_u32RDMA_seconds;	//Զ��CAS��֡�澯������
	DJ_U32 	m_u32ILUT_seconds;	//���յ�ƽ����������
	DJ_U32 	m_u32JALT_seconds;	//��������������
	DJ_U32 	m_u32TOCD_seconds;	//���Ϳ�·����������
	DJ_U32 	m_u32TCLE_seconds;	//���Ͷ�·����������
	DJ_U32 	m_u32RSLIP_seconds;	//���ջ������������
	DJ_U32 	m_u32TSLIP_seconds;	//���ͻ������������
	DJ_U32 	m_u32LCVCR_count;	//������·���ʹ����ܼ���
	DJ_U32 	m_u32PCVCR_count;	//����CRC4�����ܼ���
	DJ_U32 	m_u32FOSCR_count;	//����FAS�����ܼ���
	DJ_U32 	m_u32EBCR_count;		//Զ��CRC4 E-BIT�ܼ���
	DJ_U32	m_u32HDLC1_Tx_packets;	//HDLC1���Ͱ�����
	DJ_U32	m_u32HDLC1_Rx_goodpk;	//HDLC1������ȷ������
	DJ_U32	m_u32HDLC1_Rx_badpk;		//HDLC1���մ��������
	DJ_U32	m_u32HDLC2_Tx_packets;	//HDLC2���Ͱ�����
	DJ_U32	m_u32HDLC2_Rx_goodpk;	//HDLC2������ȷ������
	DJ_U32	m_u32HDLC2_Rx_badpk;		//HDLC2���մ��������
	DJ_U8	m_u8E1Type;		/*E1 type*/
	DJ_U8	m_u8Rfu[3];	/**/
} ACS_Digital_PortState_Data;

typedef struct {

    ACS_EVT_HEAD                 m_AcsEvtHead;
    ACS_Digital_PortState_Data   m_AcsDigitalPortStateData;
   
}ACS_Digital_PortState_Evt;

#endif