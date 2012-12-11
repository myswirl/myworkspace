//-------------------------------------------------------
//the load aging system, main transaction module
//
#ifndef __LOADAGING_TRAN_H__
#define __LOADAGING_TRAN_H__

#define		MAX_CAR_COUNT					16			//监控系统，最大支持测试车的数量			
#define		MAX_LAYER_COUNT_PERCAR			6			//每台测试车最大层数
#define		MAX_LOAD_COUNT_PERLAYER			10			//每层最大负载模块数
#define		MAX_LOAD_PERCAR					63			//每台测试车最大负载数
#define		MAX_CHN_COUNT_PERLOAD			4			//每个测试负载最大通道数4
#define		MAX_ADD_TIMESERIES				20			//最多添加的时序数目
#define		MAX_PORT_NO						101			//最大串口号,串口范围[1,100]
#define		COMMAND_INTERVAL				100			//串口命令时间间隔
#define		SERIALPORT_BUFSIZE				39			//串口写数据大小
#define		SERIALPORT_BUFSIZE				39			//串口写数据大小
#define		MAX_LED_COUNT					240			//最多LED灯个数
#define		SYS_CHECK_DOG					1			//1:进行加密狗校验; 0:不校验
//系统定时器ID
#define		TIMER_SYSRUN		100						//系统定时器，统计系统运行时间，每秒钟1次
#define		TIMER_CAR_1			101						//老化车1，定时器ID
#define		TIMER_CAR_2			102						//老化车2，定时器ID
#define		TIMER_CAR_3			103						//老化车3，定时器ID
#define		TIMER_CAR_4			104						//老化车4，定时器ID
#define		TIMER_CAR_5			105						//老化车5，定时器ID
#define		TIMER_CAR_6			106						//老化车6，定时器ID
#define		TIMER_CAR_7			107						//老化车7，定时器ID
#define		TIMER_CAR_8			108						//老化车8，定时器ID
#define		TIMER_CAR_9			109						//老化车9，定时器ID
#define		TIMER_CAR_10		110						//老化车10，定时器ID
#define		TIMER_CAR_11		111						//老化车11，定时器ID
#define		TIMER_CAR_12		112						//老化车12，定时器ID
#define		TIMER_CAR_13		113						//老化车13，定时器ID
#define		TIMER_CAR_14		114						//老化车14，定时器ID
#define		TIMER_CAR_15		115						//老化车15，定时器ID
#define		TIMER_CAR_16		116						//老化车16，定时器ID


//通道状态，每个LED的状态，未连接，合格，欠压，过压，欠流，过流
enum CHN_STATE
{
    CHN_STATE_NOLINK		= 0x00,		//未连接，白色
	CHN_STATE_OK			= 0x01,		//合格，绿色
	CHN_STATE_LOWVOL		= 0x02,		//欠压，红色
	CHN_STATE_OVERVOL		= 0x03,		//过压，红色
	CHN_STATE_LOWCUR		= 0x04,		//欠流，红色
	CHN_STATE_OVERCUR		= 0x05,		//过流，红色
	CHN_STATE_2_PARALELL	= 0x06,		//二路并联
	CHN_STATE_4_PARALELL	= 0x07,		//四路并联

};

//负载状态
enum LOAD_STATE
{
    LOAD_STATE_NOSELECT		= 0x00,		//未选中连接，白色
	LOAD_STATE_SELECT		= 0x01,		//已经选中，灰色
	LOAD_STATE_NOLINK		= 0x02,		//未连接，也就是这个负载的四个通道都没有接电源，灰色
	LOAD_STATE_NOSETPARAM	= 0x03,		//未设置参数，需要重新设置参数，灰色
	LOAD_STATE_WORKING		= 0x04,		//工作中，此种状态可实时获取数据
	LOAD_STATE_PROTECT		= 0x05,		//温度过高，系统处于保护状态，此时拉载停止
};

//负载模式，界面设置的
enum LOAD_MODE
{
    LOAD_MODE_CC  = 0x00,		//CC模式
	LOAD_MODE_CV  = 0x01,		//CV模式
};

//负载模块测试命令
enum LOAD_TEST_CMD
{
	LOAD_COMMAND_SET		= 0x0E,	//负载命令，设置拉载参数
	LOAD_COMMAND_READ		= 0x0D,	//负载命令，读取
	LOAD_COMMAND_STOP		= 0x0C,	//负载命令，停止拉载命令
	LOAD_COMMAND_220V_ON	= 0x06,	//主控板命令，220V开
	LOAD_COMMAND_220V_OFF	= 0x07,	//主控板命令，220V关
	LOAD_COMMAND_110V_ON	= 0x08,	//主控板命令，110V开
	LOAD_COMMAND_110V_OFF	= 0x09,	//主控板命令，110V关
	LOAD_COMMAND_BEEPON		= 0x10,	//主控板命令，蜂鸣器开
	LOAD_COMMAND_BEEPOFF	= 0x11,	//主控板命令，蜂鸣器关
	LOAD_COMMAND_LIGHTON	= 0x12,	//主控板命令，LED开
	LOAD_COMMAND_LIGHTOFF	= 0x13,	//主控板命令，LED关

};

//测试车状态
enum TESTING_CAR_STATE
{
	CAR_STATE_NOIMPORT	 = 0x00,	//未导入测试参数
	CAR_STATE_IMPORTED	 = 0x01,	//已经导入测试参数
	CAR_STATE_TESTING	 = 0x02,	//测试中
	CAR_STATE_TESTFINISH = 0x03,	//测试完成
};

//测试中状态
enum TEST_CAR_STATE
{
	CAR_STATE_TESTING_READ		= 0x01,	//读数据
	CAR_STATE_TESTING_220VON	= 0x02,	
	CAR_STATE_TESTING_220VOFF	= 0x03,	
	CAR_STATE_TESTING_110VON	= 0x04,	
	CAR_STATE_TESTING_110VOFF	= 0x05,	
};


//通道，每个负载四个通道
typedef	struct
{
	int			m_ChnState;				//通道的状态，未连接，合格，欠压，过压，欠流，过流
	int			m_ChnHaveBeenError;		//出现过Error data
	int			m_ChnHaveBeenRight;		//正确过Right data
	float		m_NowCurrent;			//获取的当前电流值
	float		m_NowVoltage;			//获取的当前电压值
	float		m_SetValue;				//定值电压、定值电流
	float		m_SetMax;				//上限
	float		m_SetMin;				//下限
	char		m_SetProName[16];		//通道1,产品名称
} CHANNEL_STRUCT;

//负载模块
typedef	struct
{
	int					m_LoadState;								//负载测试状态,底层返回的
	int					m_LoadMode;									//负载模式: CC,CV
	int					m_LayerID;									//此负载属于哪一层，取值范围[1,6]
	int					m_ParaMode;									//并联方式，0：无并联；1:2线并联；2:4线并联
	unsigned long		m_LoadErrorCounter;							//错误统计，欠压，欠流，过压，过流
	CHANNEL_STRUCT		m_Channel[MAX_CHN_COUNT_PERLOAD];			//每个负载4个通道
} LOAD_STRUCT;

//整区设置时只用第一层参数起作用，分层设置，实际只用了四个通道
typedef struct
{
	int		loadMode;						//负载模式: CC,CV
	int		paraMode;						//并联模式:0:无并联;1:两路并联;2:四路并联;
	float	chn1Value;						//通道1,定值电压或定值电流
	float	chn2Value;						//通道2,定值电压或定值电流
	float	chn3Value;
	float	chn4Value;
	float	chn5Value;
	float	chn6Value;
	float	chn1Max;						//上限
	float	chn2Max;
	float	chn3Max;
	float	chn4Max;
	float	chn5Max;
	float	chn6Max;
	float	chn1Min;						//下限
	float	chn2Min;
	float	chn3Min;
	float	chn4Min;
	float	chn5Min;
	float	chn6Min;
	char	chn1ProName[16];				//通道1,产品名称
	char	chn2ProName[16];				
	char	chn3ProName[16];				
	char	chn4ProName[16];				
	char	chn5ProName[16];				
	char	chn6ProName[16];				
}LAYER_STRUCT;


//添加时序测试结构体
typedef struct
{
	char timeSeries[128];
}TIMESEIRES_PARAM;

//根据时序参数转换成的测试项
typedef struct  
{
	int testItem;	//测试项
	int startTime;	//开始时间，单位秒
	int testTime;	//测试时间，单位秒
	int recvEvt;	//
}TIMESEIRES_STRUCT;

//测试参数设置结构体
typedef struct
{
	int layerPerCar;						//每台测试车层数
	int loadPerLayer;						//每层负载数
	int paramSetMode;						//参数设置方式：整区设置；分层设置
	int startLoadNum;						//扫描地址，开始
	int endLoadNum;							//扫描地址，结束
	int totalTestTime;						//总测试时间
	
	TIMESEIRES_PARAM timeSeries[MAX_ADD_TIMESERIES];		//添加的时序字符串

	int isSaveData;							//是否保存数据
	int saveDataInterval;					//保存数据时间间隔，单位秒
	int reserved;
	LAYER_STRUCT layerParam[MAX_LAYER_COUNT_PERCAR+1];		//每次的参数设置，每台测试车最多6层

}TEST_PARAM_STRUCT;



//测试车结构体
typedef	struct
{
	int m_ComID;							//串口号
	int m_TimerID;							//定时器ID
	int m_CarState;							//测试车状态: 未导入参数，导入参数，测试中
	int	m_CarTestingState;					//测试中
	unsigned char recvData[SERIALPORT_BUFSIZE];				//保存此负载上次接收数据
	int m_CMDBeepClear;							//进行Clear操作
	int m_CMDBeepOffCounter;					//关蜂鸣器响命令重发计数器
	int m_CMDLightClear;							//进行Clear操作
	int m_CMDLightOffCounter;					//关告警灯命令重发计数器

	int m_CMDAlreadyBeepOn;					//1:蜂鸣器已经响了;0:蜂鸣器关
	int m_CMDBeepOnCounter;					//蜂鸣器响命令重发计数器
	int m_CMDAlreadyLightOn;				//1:告警灯亮了;0.告警灯没有亮
	int m_CMDLightOnCounter;				//告警灯开命令重发计数器

	int	testItemNum;						//从时序测试字符串转换来的测试项数目
	TIMESEIRES_STRUCT *pTimeSeires;			//最多添加MAX_ADD_TIMESERIES个时序测试项
	
	int m_curLoadNum;						//当前测试负载号
	int	m_LostEventTimer;					//串口数据丢失超时统计
	unsigned long m_TimeCounter;			//统计测试时间，50ms增加一次，最大测试时间不超过365天，不会超出范围
	unsigned long m_ErrorCounter;			//错误统计，欠压，欠流，过压，过流
	unsigned long m_ReadCommandCounter;		//所有下发的串口读取命令统计
	unsigned long m_ReadEventCounter;		//串口Read事件统计
	unsigned long m_LostReadEventCounter;		//丢失串口数据统计
	long          iTestStartTime;			//起始测试时间
	
	LOAD_STRUCT m_Load[MAX_LOAD_PERCAR];	//每台测试车，最多63个负载模块，协议中64为广播命令
	
	char paramFileName[256];				//导入测试参数全路径文件名称
	TEST_PARAM_STRUCT testParam;			//导入的测试参数

	CRITICAL_SECTION CriticalSectionClear;		//清除操作状态互斥体
	CRITICAL_SECTION CriticalSectionRecvBuffer;	//接收数据缓冲区互斥体，

} CAR_STRUCT;


//全局变量声明------------------------------------------------
extern long		g_SystemRunTimeCounter;					//系统运行时间计数器，用于加密
extern char		g_softwareVersion[16];					//软件版本标识
extern char		cfg_NormalPassword[128];				//技术人员密码
extern char		cfg_SuperPassword[128];					//工程师密码
extern char		cfg_DataRecordPath[256];				//数据保存路径
extern char		cfg_Title[256];							//标题
extern int		cfg_EnableBeep;							//使能蜂鸣器
extern int		cfg_EnableLight;						//使能指示灯
extern int		cfg_DisplayTestInfo;					//是否统计测试信息
extern int		cfg_DialNumMethod;						//

extern int		g_LoginCheckOK;							//登录校验是否通过,-1:校验失败；0:工程师登录成功；1:技术员登录成功
extern int		g_AvailableComCounter;					//可用串口数目
extern int		g_curSelTestCar;						//当前选中测试车
extern int		g_ComList[MAX_CAR_COUNT];				//可用串口列表
extern CAR_STRUCT g_AllCar[MAX_CAR_COUNT];				//最重要的全局结构体
extern int				g_curImportCar;					//当前选中导入数据的测试车
extern int		g_LastTimeSeriesCMD;					//上一个时序命令，110V还是220V，决定脉冲开关的命令类型
extern int				g_ValidHour;								//加密狗
			
//外部调用函数声明---------------------------------------------
bool	InitSystem(void);											//系统初始化函数
bool	IsMainBoardCommand(int funcNum);							//是否为主控板命令
void	ReadFromConfig(void);
void	WriteToConfig(void);
void	DrawTotalTestTime(char* testTimeStr,int iCarIndex);			//计算测试时间
void	GetAvailableComList(void);									//获取系统可用设备列表
char*	GetTestCarName(int carID);									//获取测试车名称
char*	GetLoadModeName(int modeID);
char*	GetParaModeName(int modeID);																
char*	GetChnTitleName(int modeID);	
char*   GetChnStateName(int chnMode);								//获取通道状态名称字符串
char*	GetTestCarStateName(int carID);									

char*	Led_GetLoadModeName(int modeID);									
char*	Led_GetFixedValue(int modeID);									
char*	Led_GetUnitName(int modeID);									
							
void	ProcessReceiveData( unsigned char* pRecvData, int portNo);
void	RefreshAllLED( int curCarID);							//刷新当前界面
void	RefreshOneLED( int curCarID, int loadIndex, int chnIndex);		//刷新单个LED
int		GetSerialPortCommandType(int carID, int loadNum);				//根据测试时间在测试时序中的位置，判断当前测试命令类型
int		GetCurLoadNum(int carID);										//获取当前测试负载号
void	WriteSerialPortCommand(int carID, int serialPortCommandType, int curLoadNum);
void	DealWithSerialPortData(int carID);								//处理串口接收到的数据
void	Proc_MutilMediaTimer(int carID,DWORD dwUser);					//通用定时器处理函数
void	SaveTestData(int carID);										//保存测试数据
void	DrawCurrentCarInfo(void);										//刷新主界面，当前测试车状态信息
void	SaveCurrentLoadTestData(int carID, int loadNum);	

#endif
