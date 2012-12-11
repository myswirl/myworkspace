//-------------------------------------------------------
//the load aging system, main transaction module
//
#ifndef __LOADAGING_TRAN_H__
#define __LOADAGING_TRAN_H__

#define		MAX_CAR_COUNT					16			//���ϵͳ�����֧�ֲ��Գ�������			
#define		MAX_LAYER_COUNT_PERCAR			6			//ÿ̨���Գ�������
#define		MAX_LOAD_COUNT_PERLAYER			10			//ÿ�������ģ����
#define		MAX_LOAD_PERCAR					63			//ÿ̨���Գ��������
#define		MAX_CHN_COUNT_PERLOAD			4			//ÿ�����Ը������ͨ����4
#define		MAX_ADD_TIMESERIES				20			//�����ӵ�ʱ����Ŀ
#define		MAX_PORT_NO						101			//��󴮿ں�,���ڷ�Χ[1,100]
#define		COMMAND_INTERVAL				100			//��������ʱ����
#define		SERIALPORT_BUFSIZE				39			//����д���ݴ�С
#define		SERIALPORT_BUFSIZE				39			//����д���ݴ�С
#define		MAX_LED_COUNT					240			//���LED�Ƹ���
#define		SYS_CHECK_DOG					1			//1:���м��ܹ�У��; 0:��У��
//ϵͳ��ʱ��ID
#define		TIMER_SYSRUN		100						//ϵͳ��ʱ����ͳ��ϵͳ����ʱ�䣬ÿ����1��
#define		TIMER_CAR_1			101						//�ϻ���1����ʱ��ID
#define		TIMER_CAR_2			102						//�ϻ���2����ʱ��ID
#define		TIMER_CAR_3			103						//�ϻ���3����ʱ��ID
#define		TIMER_CAR_4			104						//�ϻ���4����ʱ��ID
#define		TIMER_CAR_5			105						//�ϻ���5����ʱ��ID
#define		TIMER_CAR_6			106						//�ϻ���6����ʱ��ID
#define		TIMER_CAR_7			107						//�ϻ���7����ʱ��ID
#define		TIMER_CAR_8			108						//�ϻ���8����ʱ��ID
#define		TIMER_CAR_9			109						//�ϻ���9����ʱ��ID
#define		TIMER_CAR_10		110						//�ϻ���10����ʱ��ID
#define		TIMER_CAR_11		111						//�ϻ���11����ʱ��ID
#define		TIMER_CAR_12		112						//�ϻ���12����ʱ��ID
#define		TIMER_CAR_13		113						//�ϻ���13����ʱ��ID
#define		TIMER_CAR_14		114						//�ϻ���14����ʱ��ID
#define		TIMER_CAR_15		115						//�ϻ���15����ʱ��ID
#define		TIMER_CAR_16		116						//�ϻ���16����ʱ��ID


//ͨ��״̬��ÿ��LED��״̬��δ���ӣ��ϸ�Ƿѹ����ѹ��Ƿ��������
enum CHN_STATE
{
    CHN_STATE_NOLINK		= 0x00,		//δ���ӣ���ɫ
	CHN_STATE_OK			= 0x01,		//�ϸ���ɫ
	CHN_STATE_LOWVOL		= 0x02,		//Ƿѹ����ɫ
	CHN_STATE_OVERVOL		= 0x03,		//��ѹ����ɫ
	CHN_STATE_LOWCUR		= 0x04,		//Ƿ������ɫ
	CHN_STATE_OVERCUR		= 0x05,		//��������ɫ
	CHN_STATE_2_PARALELL	= 0x06,		//��·����
	CHN_STATE_4_PARALELL	= 0x07,		//��·����

};

//����״̬
enum LOAD_STATE
{
    LOAD_STATE_NOSELECT		= 0x00,		//δѡ�����ӣ���ɫ
	LOAD_STATE_SELECT		= 0x01,		//�Ѿ�ѡ�У���ɫ
	LOAD_STATE_NOLINK		= 0x02,		//δ���ӣ�Ҳ����������ص��ĸ�ͨ����û�нӵ�Դ����ɫ
	LOAD_STATE_NOSETPARAM	= 0x03,		//δ���ò�������Ҫ�������ò�������ɫ
	LOAD_STATE_WORKING		= 0x04,		//�����У�����״̬��ʵʱ��ȡ����
	LOAD_STATE_PROTECT		= 0x05,		//�¶ȹ��ߣ�ϵͳ���ڱ���״̬����ʱ����ֹͣ
};

//����ģʽ���������õ�
enum LOAD_MODE
{
    LOAD_MODE_CC  = 0x00,		//CCģʽ
	LOAD_MODE_CV  = 0x01,		//CVģʽ
};

//����ģ���������
enum LOAD_TEST_CMD
{
	LOAD_COMMAND_SET		= 0x0E,	//��������������ز���
	LOAD_COMMAND_READ		= 0x0D,	//���������ȡ
	LOAD_COMMAND_STOP		= 0x0C,	//�������ֹͣ��������
	LOAD_COMMAND_220V_ON	= 0x06,	//���ذ����220V��
	LOAD_COMMAND_220V_OFF	= 0x07,	//���ذ����220V��
	LOAD_COMMAND_110V_ON	= 0x08,	//���ذ����110V��
	LOAD_COMMAND_110V_OFF	= 0x09,	//���ذ����110V��
	LOAD_COMMAND_BEEPON		= 0x10,	//���ذ������������
	LOAD_COMMAND_BEEPOFF	= 0x11,	//���ذ������������
	LOAD_COMMAND_LIGHTON	= 0x12,	//���ذ����LED��
	LOAD_COMMAND_LIGHTOFF	= 0x13,	//���ذ����LED��

};

//���Գ�״̬
enum TESTING_CAR_STATE
{
	CAR_STATE_NOIMPORT	 = 0x00,	//δ������Բ���
	CAR_STATE_IMPORTED	 = 0x01,	//�Ѿ�������Բ���
	CAR_STATE_TESTING	 = 0x02,	//������
	CAR_STATE_TESTFINISH = 0x03,	//�������
};

//������״̬
enum TEST_CAR_STATE
{
	CAR_STATE_TESTING_READ		= 0x01,	//������
	CAR_STATE_TESTING_220VON	= 0x02,	
	CAR_STATE_TESTING_220VOFF	= 0x03,	
	CAR_STATE_TESTING_110VON	= 0x04,	
	CAR_STATE_TESTING_110VOFF	= 0x05,	
};


//ͨ����ÿ�������ĸ�ͨ��
typedef	struct
{
	int			m_ChnState;				//ͨ����״̬��δ���ӣ��ϸ�Ƿѹ����ѹ��Ƿ��������
	int			m_ChnHaveBeenError;		//���ֹ�Error data
	int			m_ChnHaveBeenRight;		//��ȷ��Right data
	float		m_NowCurrent;			//��ȡ�ĵ�ǰ����ֵ
	float		m_NowVoltage;			//��ȡ�ĵ�ǰ��ѹֵ
	float		m_SetValue;				//��ֵ��ѹ����ֵ����
	float		m_SetMax;				//����
	float		m_SetMin;				//����
	char		m_SetProName[16];		//ͨ��1,��Ʒ����
} CHANNEL_STRUCT;

//����ģ��
typedef	struct
{
	int					m_LoadState;								//���ز���״̬,�ײ㷵�ص�
	int					m_LoadMode;									//����ģʽ: CC,CV
	int					m_LayerID;									//�˸���������һ�㣬ȡֵ��Χ[1,6]
	int					m_ParaMode;									//������ʽ��0���޲�����1:2�߲�����2:4�߲���
	unsigned long		m_LoadErrorCounter;							//����ͳ�ƣ�Ƿѹ��Ƿ������ѹ������
	CHANNEL_STRUCT		m_Channel[MAX_CHN_COUNT_PERLOAD];			//ÿ������4��ͨ��
} LOAD_STRUCT;

//��������ʱֻ�õ�һ����������ã��ֲ����ã�ʵ��ֻ�����ĸ�ͨ��
typedef struct
{
	int		loadMode;						//����ģʽ: CC,CV
	int		paraMode;						//����ģʽ:0:�޲���;1:��·����;2:��·����;
	float	chn1Value;						//ͨ��1,��ֵ��ѹ��ֵ����
	float	chn2Value;						//ͨ��2,��ֵ��ѹ��ֵ����
	float	chn3Value;
	float	chn4Value;
	float	chn5Value;
	float	chn6Value;
	float	chn1Max;						//����
	float	chn2Max;
	float	chn3Max;
	float	chn4Max;
	float	chn5Max;
	float	chn6Max;
	float	chn1Min;						//����
	float	chn2Min;
	float	chn3Min;
	float	chn4Min;
	float	chn5Min;
	float	chn6Min;
	char	chn1ProName[16];				//ͨ��1,��Ʒ����
	char	chn2ProName[16];				
	char	chn3ProName[16];				
	char	chn4ProName[16];				
	char	chn5ProName[16];				
	char	chn6ProName[16];				
}LAYER_STRUCT;


//���ʱ����Խṹ��
typedef struct
{
	char timeSeries[128];
}TIMESEIRES_PARAM;

//����ʱ�����ת���ɵĲ�����
typedef struct  
{
	int testItem;	//������
	int startTime;	//��ʼʱ�䣬��λ��
	int testTime;	//����ʱ�䣬��λ��
	int recvEvt;	//
}TIMESEIRES_STRUCT;

//���Բ������ýṹ��
typedef struct
{
	int layerPerCar;						//ÿ̨���Գ�����
	int loadPerLayer;						//ÿ�㸺����
	int paramSetMode;						//�������÷�ʽ���������ã��ֲ�����
	int startLoadNum;						//ɨ���ַ����ʼ
	int endLoadNum;							//ɨ���ַ������
	int totalTestTime;						//�ܲ���ʱ��
	
	TIMESEIRES_PARAM timeSeries[MAX_ADD_TIMESERIES];		//��ӵ�ʱ���ַ���

	int isSaveData;							//�Ƿ񱣴�����
	int saveDataInterval;					//��������ʱ��������λ��
	int reserved;
	LAYER_STRUCT layerParam[MAX_LAYER_COUNT_PERCAR+1];		//ÿ�εĲ������ã�ÿ̨���Գ����6��

}TEST_PARAM_STRUCT;



//���Գ��ṹ��
typedef	struct
{
	int m_ComID;							//���ں�
	int m_TimerID;							//��ʱ��ID
	int m_CarState;							//���Գ�״̬: δ������������������������
	int	m_CarTestingState;					//������
	unsigned char recvData[SERIALPORT_BUFSIZE];				//����˸����ϴν�������
	int m_CMDBeepClear;							//����Clear����
	int m_CMDBeepOffCounter;					//�ط������������ط�������
	int m_CMDLightClear;							//����Clear����
	int m_CMDLightOffCounter;					//�ظ澯�������ط�������

	int m_CMDAlreadyBeepOn;					//1:�������Ѿ�����;0:��������
	int m_CMDBeepOnCounter;					//�������������ط�������
	int m_CMDAlreadyLightOn;				//1:�澯������;0.�澯��û����
	int m_CMDLightOnCounter;				//�澯�ƿ������ط�������

	int	testItemNum;						//��ʱ������ַ���ת�����Ĳ�������Ŀ
	TIMESEIRES_STRUCT *pTimeSeires;			//������MAX_ADD_TIMESERIES��ʱ�������
	
	int m_curLoadNum;						//��ǰ���Ը��غ�
	int	m_LostEventTimer;					//�������ݶ�ʧ��ʱͳ��
	unsigned long m_TimeCounter;			//ͳ�Ʋ���ʱ�䣬50ms����һ�Σ�������ʱ�䲻����365�죬���ᳬ����Χ
	unsigned long m_ErrorCounter;			//����ͳ�ƣ�Ƿѹ��Ƿ������ѹ������
	unsigned long m_ReadCommandCounter;		//�����·��Ĵ��ڶ�ȡ����ͳ��
	unsigned long m_ReadEventCounter;		//����Read�¼�ͳ��
	unsigned long m_LostReadEventCounter;		//��ʧ��������ͳ��
	long          iTestStartTime;			//��ʼ����ʱ��
	
	LOAD_STRUCT m_Load[MAX_LOAD_PERCAR];	//ÿ̨���Գ������63������ģ�飬Э����64Ϊ�㲥����
	
	char paramFileName[256];				//������Բ���ȫ·���ļ�����
	TEST_PARAM_STRUCT testParam;			//����Ĳ��Բ���

	CRITICAL_SECTION CriticalSectionClear;		//�������״̬������
	CRITICAL_SECTION CriticalSectionRecvBuffer;	//�������ݻ����������壬

} CAR_STRUCT;


//ȫ�ֱ�������------------------------------------------------
extern long		g_SystemRunTimeCounter;					//ϵͳ����ʱ������������ڼ���
extern char		g_softwareVersion[16];					//����汾��ʶ
extern char		cfg_NormalPassword[128];				//������Ա����
extern char		cfg_SuperPassword[128];					//����ʦ����
extern char		cfg_DataRecordPath[256];				//���ݱ���·��
extern char		cfg_Title[256];							//����
extern int		cfg_EnableBeep;							//ʹ�ܷ�����
extern int		cfg_EnableLight;						//ʹ��ָʾ��
extern int		cfg_DisplayTestInfo;					//�Ƿ�ͳ�Ʋ�����Ϣ
extern int		cfg_DialNumMethod;						//

extern int		g_LoginCheckOK;							//��¼У���Ƿ�ͨ��,-1:У��ʧ�ܣ�0:����ʦ��¼�ɹ���1:����Ա��¼�ɹ�
extern int		g_AvailableComCounter;					//���ô�����Ŀ
extern int		g_curSelTestCar;						//��ǰѡ�в��Գ�
extern int		g_ComList[MAX_CAR_COUNT];				//���ô����б�
extern CAR_STRUCT g_AllCar[MAX_CAR_COUNT];				//����Ҫ��ȫ�ֽṹ��
extern int				g_curImportCar;					//��ǰѡ�е������ݵĲ��Գ�
extern int		g_LastTimeSeriesCMD;					//��һ��ʱ�����110V����220V���������忪�ص���������
extern int				g_ValidHour;								//���ܹ�
			
//�ⲿ���ú�������---------------------------------------------
bool	InitSystem(void);											//ϵͳ��ʼ������
bool	IsMainBoardCommand(int funcNum);							//�Ƿ�Ϊ���ذ�����
void	ReadFromConfig(void);
void	WriteToConfig(void);
void	DrawTotalTestTime(char* testTimeStr,int iCarIndex);			//�������ʱ��
void	GetAvailableComList(void);									//��ȡϵͳ�����豸�б�
char*	GetTestCarName(int carID);									//��ȡ���Գ�����
char*	GetLoadModeName(int modeID);
char*	GetParaModeName(int modeID);																
char*	GetChnTitleName(int modeID);	
char*   GetChnStateName(int chnMode);								//��ȡͨ��״̬�����ַ���
char*	GetTestCarStateName(int carID);									

char*	Led_GetLoadModeName(int modeID);									
char*	Led_GetFixedValue(int modeID);									
char*	Led_GetUnitName(int modeID);									
							
void	ProcessReceiveData( unsigned char* pRecvData, int portNo);
void	RefreshAllLED( int curCarID);							//ˢ�µ�ǰ����
void	RefreshOneLED( int curCarID, int loadIndex, int chnIndex);		//ˢ�µ���LED
int		GetSerialPortCommandType(int carID, int loadNum);				//���ݲ���ʱ���ڲ���ʱ���е�λ�ã��жϵ�ǰ������������
int		GetCurLoadNum(int carID);										//��ȡ��ǰ���Ը��غ�
void	WriteSerialPortCommand(int carID, int serialPortCommandType, int curLoadNum);
void	DealWithSerialPortData(int carID);								//�����ڽ��յ�������
void	Proc_MutilMediaTimer(int carID,DWORD dwUser);					//ͨ�ö�ʱ��������
void	SaveTestData(int carID);										//�����������
void	DrawCurrentCarInfo(void);										//ˢ�������棬��ǰ���Գ�״̬��Ϣ
void	SaveCurrentLoadTestData(int carID, int loadNum);	

#endif
