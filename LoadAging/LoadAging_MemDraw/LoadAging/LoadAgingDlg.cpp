// LoadAgingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LoadAging.h"
#include "LoadAgingDlg.h"
#include "NT77Api.h"
#include "LoadAging_Tran.h"
#include "LongDlg.h"
#include "hangePasswordDlg.h"
#include "SysConfigDlg.h"
#include "ParamEditDlg.h"
#include "ImportParamDlg.h"
#include "LogFile.h"
#include "MyLog.h"
#include "SerialPort.h"
#include "SLinkList.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMyLog			g_data[MAX_CAR_COUNT];
extern CSerialPort		g_Ports[MAX_CAR_COUNT];						//串口类，处理每个测试车串口数据
extern SLinkList<char> sLinkList1;
extern SLinkList<char> sLinkList2;
extern SLinkList<char> sLinkList3;
extern SLinkList<char> sLinkList4;
extern SLinkList<char> sLinkList5;
extern SLinkList<char> sLinkList6;
extern SLinkList<char> sLinkList7;
extern SLinkList<char> sLinkList8;
extern SLinkList<char> sLinkList9;
extern SLinkList<char> sLinkList10;
extern SLinkList<char> sLinkList11;
extern SLinkList<char> sLinkList12;
extern SLinkList<char> sLinkList13;
extern SLinkList<char> sLinkList14;
extern SLinkList<char> sLinkList15;
extern SLinkList<char> sLinkList16;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	
	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
// No message handlers
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoadAgingDlg dialog

CLoadAgingDlg::CLoadAgingDlg(CWnd* pParent /*=NULL*/)
: CDialog(CLoadAgingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoadAgingDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLoadAgingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoadAgingDlg)
	DDX_Control(pDX, IDC_BUTTON_SYSCONFIG, m_BtnSysSet);
	DDX_Control(pDX, IDC_BUTTON_PARAMIMPORT, m_BtnParamImport);
	DDX_Control(pDX, IDC_BUTTON_PARAMEDIT, m_BtnParamEdit);
	DDX_Control(pDX, IDC_BUTTON_PASSWORD, m_BtnPassword);
	DDX_Control(pDX, IDC_BUTTON_HELP, m_BtnHelp);
	DDX_Control(pDX, IDCANCEL, m_BtnExit);
	DDX_Control(pDX, IDC_BUTTON_TEST, m_BtnTest);
	
	DDX_Control(pDX, IDC_STATIC_CHN1, m_LED[0]);   
	DDX_Control(pDX, IDC_STATIC_CHN2, m_LED[1]);   
	DDX_Control(pDX, IDC_STATIC_CHN3, m_LED[2]);   
	DDX_Control(pDX, IDC_STATIC_CHN4, m_LED[3]);   
	DDX_Control(pDX, IDC_STATIC_CHN5, m_LED[4]);   
	DDX_Control(pDX, IDC_STATIC_CHN6, m_LED[5]);   
	DDX_Control(pDX, IDC_STATIC_CHN7, m_LED[6]);   
	DDX_Control(pDX, IDC_STATIC_CHN8, m_LED[7]);   
	DDX_Control(pDX, IDC_STATIC_CHN9, m_LED[8]);   
	DDX_Control(pDX, IDC_STATIC_CHN10, m_LED[9]);    
	DDX_Control(pDX, IDC_STATIC_CHN11, m_LED[10]);   
	DDX_Control(pDX, IDC_STATIC_CHN12, m_LED[11]);   
	DDX_Control(pDX, IDC_STATIC_CHN13, m_LED[12]);   
	DDX_Control(pDX, IDC_STATIC_CHN14, m_LED[13]);   
	DDX_Control(pDX, IDC_STATIC_CHN15, m_LED[14]);   
	DDX_Control(pDX, IDC_STATIC_CHN16, m_LED[15]);   
	DDX_Control(pDX, IDC_STATIC_CHN17, m_LED[16]);   
	DDX_Control(pDX, IDC_STATIC_CHN18, m_LED[17]);   
	DDX_Control(pDX, IDC_STATIC_CHN19, m_LED[18]);   	
	DDX_Control(pDX, IDC_STATIC_CHN20, m_LED[19]);
	DDX_Control(pDX, IDC_STATIC_CHN21, m_LED[20]);
	DDX_Control(pDX, IDC_STATIC_CHN22, m_LED[21]);
	DDX_Control(pDX, IDC_STATIC_CHN23, m_LED[22]);
	DDX_Control(pDX, IDC_STATIC_CHN24, m_LED[23]);
	DDX_Control(pDX, IDC_STATIC_CHN25, m_LED[24]);
	DDX_Control(pDX, IDC_STATIC_CHN26, m_LED[25]);
	DDX_Control(pDX, IDC_STATIC_CHN27, m_LED[26]);
	DDX_Control(pDX, IDC_STATIC_CHN28, m_LED[27]);
	DDX_Control(pDX, IDC_STATIC_CHN29, m_LED[28]);    
	DDX_Control(pDX, IDC_STATIC_CHN30, m_LED[29]); 
	DDX_Control(pDX, IDC_STATIC_CHN31, m_LED[30]); 
	DDX_Control(pDX, IDC_STATIC_CHN32, m_LED[31]); 
	DDX_Control(pDX, IDC_STATIC_CHN33, m_LED[32]); 
	DDX_Control(pDX, IDC_STATIC_CHN34, m_LED[33]); 
	DDX_Control(pDX, IDC_STATIC_CHN35, m_LED[34]); 
	DDX_Control(pDX, IDC_STATIC_CHN36, m_LED[35]); 
	DDX_Control(pDX, IDC_STATIC_CHN37, m_LED[36]); 
	DDX_Control(pDX, IDC_STATIC_CHN38, m_LED[37]); 
	DDX_Control(pDX, IDC_STATIC_CHN39, m_LED[38]);          
	DDX_Control(pDX, IDC_STATIC_CHN40, m_LED[39]); 
	DDX_Control(pDX, IDC_STATIC_CHN41, m_LED[40]); 
	DDX_Control(pDX, IDC_STATIC_CHN42, m_LED[41]); 
	DDX_Control(pDX, IDC_STATIC_CHN43, m_LED[42]); 
	DDX_Control(pDX, IDC_STATIC_CHN44, m_LED[43]); 
	DDX_Control(pDX, IDC_STATIC_CHN45, m_LED[44]); 
	DDX_Control(pDX, IDC_STATIC_CHN46, m_LED[45]); 
	DDX_Control(pDX, IDC_STATIC_CHN47, m_LED[46]); 
	DDX_Control(pDX, IDC_STATIC_CHN48, m_LED[47]); 
	DDX_Control(pDX, IDC_STATIC_CHN49, m_LED[48]); 
	DDX_Control(pDX, IDC_STATIC_CHN50, m_LED[49]); 
	DDX_Control(pDX, IDC_STATIC_CHN51, m_LED[50]); 
	DDX_Control(pDX, IDC_STATIC_CHN52, m_LED[51]); 
	DDX_Control(pDX, IDC_STATIC_CHN53, m_LED[52]); 
	DDX_Control(pDX, IDC_STATIC_CHN54, m_LED[53]); 
	DDX_Control(pDX, IDC_STATIC_CHN55, m_LED[54]); 
	DDX_Control(pDX, IDC_STATIC_CHN56, m_LED[55]); 
	DDX_Control(pDX, IDC_STATIC_CHN57, m_LED[56]); 
	DDX_Control(pDX, IDC_STATIC_CHN58, m_LED[57]); 
	DDX_Control(pDX, IDC_STATIC_CHN59, m_LED[58]);     
	DDX_Control(pDX, IDC_STATIC_CHN60, m_LED[59]); 
	DDX_Control(pDX, IDC_STATIC_CHN61, m_LED[60]); 
	DDX_Control(pDX, IDC_STATIC_CHN62, m_LED[61]); 
	DDX_Control(pDX, IDC_STATIC_CHN63, m_LED[62]); 
	DDX_Control(pDX, IDC_STATIC_CHN64, m_LED[63]); 
	DDX_Control(pDX, IDC_STATIC_CHN65, m_LED[64]); 
	DDX_Control(pDX, IDC_STATIC_CHN66, m_LED[65]); 
	DDX_Control(pDX, IDC_STATIC_CHN67, m_LED[66]); 
	DDX_Control(pDX, IDC_STATIC_CHN68, m_LED[67]); 
	DDX_Control(pDX, IDC_STATIC_CHN69, m_LED[68]); 
	DDX_Control(pDX, IDC_STATIC_CHN70, m_LED[69]); 
	DDX_Control(pDX, IDC_STATIC_CHN71, m_LED[70]); 
	DDX_Control(pDX, IDC_STATIC_CHN72, m_LED[71]); 
	DDX_Control(pDX, IDC_STATIC_CHN73, m_LED[72]); 
	DDX_Control(pDX, IDC_STATIC_CHN74, m_LED[73]); 
	DDX_Control(pDX, IDC_STATIC_CHN75, m_LED[74]); 
	DDX_Control(pDX, IDC_STATIC_CHN76, m_LED[75]); 
	DDX_Control(pDX, IDC_STATIC_CHN77, m_LED[76]); 
	DDX_Control(pDX, IDC_STATIC_CHN78, m_LED[77]); 
	DDX_Control(pDX, IDC_STATIC_CHN79, m_LED[78]); 
	DDX_Control(pDX, IDC_STATIC_CHN80, m_LED[79]); 
	DDX_Control(pDX, IDC_STATIC_CHN81, m_LED[80]); 
	DDX_Control(pDX, IDC_STATIC_CHN82, m_LED[81]); 
	DDX_Control(pDX, IDC_STATIC_CHN83, m_LED[82]); 
	DDX_Control(pDX, IDC_STATIC_CHN84, m_LED[83]); 
	DDX_Control(pDX, IDC_STATIC_CHN85, m_LED[84]); 
	DDX_Control(pDX, IDC_STATIC_CHN86, m_LED[85]); 
	DDX_Control(pDX, IDC_STATIC_CHN87, m_LED[86]); 
	DDX_Control(pDX, IDC_STATIC_CHN88, m_LED[87]); 
	DDX_Control(pDX, IDC_STATIC_CHN89, m_LED[88]); 
	DDX_Control(pDX, IDC_STATIC_CHN90, m_LED[89]); 
	DDX_Control(pDX, IDC_STATIC_CHN91, m_LED[90]); 
	DDX_Control(pDX, IDC_STATIC_CHN92, m_LED[91]); 
	DDX_Control(pDX, IDC_STATIC_CHN93, m_LED[92]); 
	DDX_Control(pDX, IDC_STATIC_CHN94, m_LED[93]); 
	DDX_Control(pDX, IDC_STATIC_CHN95, m_LED[94]); 
	DDX_Control(pDX, IDC_STATIC_CHN96, m_LED[95]); 
	DDX_Control(pDX, IDC_STATIC_CHN97, m_LED[96]); 
	DDX_Control(pDX, IDC_STATIC_CHN98, m_LED[97]); 
	DDX_Control(pDX, IDC_STATIC_CHN99, m_LED[98]); 
	DDX_Control(pDX, IDC_STATIC_CHN100, m_LED[99]);    
	DDX_Control(pDX, IDC_STATIC_CHN101, m_LED[100]);   
	DDX_Control(pDX, IDC_STATIC_CHN102, m_LED[101]);   
	DDX_Control(pDX, IDC_STATIC_CHN103, m_LED[102]);   
	DDX_Control(pDX, IDC_STATIC_CHN104, m_LED[103]);   
	DDX_Control(pDX, IDC_STATIC_CHN105, m_LED[104]);   
	DDX_Control(pDX, IDC_STATIC_CHN106, m_LED[105]);   
	DDX_Control(pDX, IDC_STATIC_CHN107, m_LED[106]);   
	DDX_Control(pDX, IDC_STATIC_CHN108, m_LED[107]);   
	DDX_Control(pDX, IDC_STATIC_CHN109, m_LED[108]);   
	DDX_Control(pDX, IDC_STATIC_CHN110, m_LED[109]);    
	DDX_Control(pDX, IDC_STATIC_CHN111, m_LED[110]);   
	DDX_Control(pDX, IDC_STATIC_CHN112, m_LED[111]);   
	DDX_Control(pDX, IDC_STATIC_CHN113, m_LED[112]);   
	DDX_Control(pDX, IDC_STATIC_CHN114, m_LED[113]);   
	DDX_Control(pDX, IDC_STATIC_CHN115, m_LED[114]);   
	DDX_Control(pDX, IDC_STATIC_CHN116, m_LED[115]);   
	DDX_Control(pDX, IDC_STATIC_CHN117, m_LED[116]);   
	DDX_Control(pDX, IDC_STATIC_CHN118, m_LED[117]);   
	DDX_Control(pDX, IDC_STATIC_CHN119, m_LED[118]);   	                         
	DDX_Control(pDX, IDC_STATIC_CHN120, m_LED[119]);
	DDX_Control(pDX, IDC_STATIC_CHN121, m_LED[120]);
	DDX_Control(pDX, IDC_STATIC_CHN122, m_LED[121]);
	DDX_Control(pDX, IDC_STATIC_CHN123, m_LED[122]);
	DDX_Control(pDX, IDC_STATIC_CHN124, m_LED[123]);
	DDX_Control(pDX, IDC_STATIC_CHN125, m_LED[124]);
	DDX_Control(pDX, IDC_STATIC_CHN126, m_LED[125]);
	DDX_Control(pDX, IDC_STATIC_CHN127, m_LED[126]);
	DDX_Control(pDX, IDC_STATIC_CHN128, m_LED[127]);
	DDX_Control(pDX, IDC_STATIC_CHN129, m_LED[128]);                                 
	DDX_Control(pDX, IDC_STATIC_CHN130, m_LED[129]); 
	DDX_Control(pDX, IDC_STATIC_CHN131, m_LED[130]); 
	DDX_Control(pDX, IDC_STATIC_CHN132, m_LED[131]); 
	DDX_Control(pDX, IDC_STATIC_CHN133, m_LED[132]); 
	DDX_Control(pDX, IDC_STATIC_CHN134, m_LED[133]); 
	DDX_Control(pDX, IDC_STATIC_CHN135, m_LED[134]); 
	DDX_Control(pDX, IDC_STATIC_CHN136, m_LED[135]); 
	DDX_Control(pDX, IDC_STATIC_CHN137, m_LED[136]); 
	DDX_Control(pDX, IDC_STATIC_CHN138, m_LED[137]); 
	DDX_Control(pDX, IDC_STATIC_CHN139, m_LED[138]);                                      
	DDX_Control(pDX, IDC_STATIC_CHN140, m_LED[139]); 
	DDX_Control(pDX, IDC_STATIC_CHN141, m_LED[140]); 
	DDX_Control(pDX, IDC_STATIC_CHN142, m_LED[141]); 
	DDX_Control(pDX, IDC_STATIC_CHN143, m_LED[142]); 
	DDX_Control(pDX, IDC_STATIC_CHN144, m_LED[143]); 
	DDX_Control(pDX, IDC_STATIC_CHN145, m_LED[144]); 
	DDX_Control(pDX, IDC_STATIC_CHN146, m_LED[145]); 
	DDX_Control(pDX, IDC_STATIC_CHN147, m_LED[146]); 
	DDX_Control(pDX, IDC_STATIC_CHN148, m_LED[147]); 
	DDX_Control(pDX, IDC_STATIC_CHN149, m_LED[148]);                          
	DDX_Control(pDX, IDC_STATIC_CHN150, m_LED[149]); 
	DDX_Control(pDX, IDC_STATIC_CHN151, m_LED[150]); 
	DDX_Control(pDX, IDC_STATIC_CHN152, m_LED[151]); 
	DDX_Control(pDX, IDC_STATIC_CHN153, m_LED[152]); 
	DDX_Control(pDX, IDC_STATIC_CHN154, m_LED[153]); 
	DDX_Control(pDX, IDC_STATIC_CHN155, m_LED[154]); 
	DDX_Control(pDX, IDC_STATIC_CHN156, m_LED[155]); 
	DDX_Control(pDX, IDC_STATIC_CHN157, m_LED[156]); 
	DDX_Control(pDX, IDC_STATIC_CHN158, m_LED[157]); 
	DDX_Control(pDX, IDC_STATIC_CHN159, m_LED[158]);     
	DDX_Control(pDX, IDC_STATIC_CHN160, m_LED[159]); 
	DDX_Control(pDX, IDC_STATIC_CHN161, m_LED[160]); 
	DDX_Control(pDX, IDC_STATIC_CHN162, m_LED[161]); 
	DDX_Control(pDX, IDC_STATIC_CHN163, m_LED[162]); 
	DDX_Control(pDX, IDC_STATIC_CHN164, m_LED[163]); 
	DDX_Control(pDX, IDC_STATIC_CHN165, m_LED[164]); 
	DDX_Control(pDX, IDC_STATIC_CHN166, m_LED[165]); 
	DDX_Control(pDX, IDC_STATIC_CHN167, m_LED[166]); 
	DDX_Control(pDX, IDC_STATIC_CHN168, m_LED[167]); 
	DDX_Control(pDX, IDC_STATIC_CHN169, m_LED[168]); 
	DDX_Control(pDX, IDC_STATIC_CHN170, m_LED[169]); 
	DDX_Control(pDX, IDC_STATIC_CHN171, m_LED[170]); 
	DDX_Control(pDX, IDC_STATIC_CHN172, m_LED[171]); 
	DDX_Control(pDX, IDC_STATIC_CHN173, m_LED[172]); 
	DDX_Control(pDX, IDC_STATIC_CHN174, m_LED[173]); 
	DDX_Control(pDX, IDC_STATIC_CHN175, m_LED[174]); 
	DDX_Control(pDX, IDC_STATIC_CHN176, m_LED[175]); 
	DDX_Control(pDX, IDC_STATIC_CHN177, m_LED[176]); 
	DDX_Control(pDX, IDC_STATIC_CHN178, m_LED[177]); 
	DDX_Control(pDX, IDC_STATIC_CHN179, m_LED[178]); 
	DDX_Control(pDX, IDC_STATIC_CHN180, m_LED[179]); 
	DDX_Control(pDX, IDC_STATIC_CHN181, m_LED[180]); 
	DDX_Control(pDX, IDC_STATIC_CHN182, m_LED[181]); 
	DDX_Control(pDX, IDC_STATIC_CHN183, m_LED[182]); 
	DDX_Control(pDX, IDC_STATIC_CHN184, m_LED[183]); 
	DDX_Control(pDX, IDC_STATIC_CHN185, m_LED[184]); 
	DDX_Control(pDX, IDC_STATIC_CHN186, m_LED[185]); 
	DDX_Control(pDX, IDC_STATIC_CHN187, m_LED[186]); 
	DDX_Control(pDX, IDC_STATIC_CHN188, m_LED[187]); 
	DDX_Control(pDX, IDC_STATIC_CHN189, m_LED[188]); 
	DDX_Control(pDX, IDC_STATIC_CHN190, m_LED[189]); 
	DDX_Control(pDX, IDC_STATIC_CHN191, m_LED[190]); 
	DDX_Control(pDX, IDC_STATIC_CHN192, m_LED[191]); 
	DDX_Control(pDX, IDC_STATIC_CHN193, m_LED[192]); 
	DDX_Control(pDX, IDC_STATIC_CHN194, m_LED[193]); 
	DDX_Control(pDX, IDC_STATIC_CHN195, m_LED[194]); 
	DDX_Control(pDX, IDC_STATIC_CHN196, m_LED[195]); 
	DDX_Control(pDX, IDC_STATIC_CHN197, m_LED[196]); 
	DDX_Control(pDX, IDC_STATIC_CHN198, m_LED[197]); 
	DDX_Control(pDX, IDC_STATIC_CHN199, m_LED[198]); 
	DDX_Control(pDX, IDC_STATIC_CHN200, m_LED[199]); 
	//--------------后加了52个LED   
	DDX_Control(pDX, IDC_STATIC_CHN201, m_LED[200]);   
	DDX_Control(pDX, IDC_STATIC_CHN202, m_LED[201]);   
	DDX_Control(pDX, IDC_STATIC_CHN203, m_LED[202]);   
	DDX_Control(pDX, IDC_STATIC_CHN204, m_LED[203]);   
	DDX_Control(pDX, IDC_STATIC_CHN205, m_LED[204]);   
	DDX_Control(pDX, IDC_STATIC_CHN206, m_LED[205]);   
	DDX_Control(pDX, IDC_STATIC_CHN207, m_LED[206]);   
	DDX_Control(pDX, IDC_STATIC_CHN208, m_LED[207]);   
	DDX_Control(pDX, IDC_STATIC_CHN209, m_LED[208]);   
	DDX_Control(pDX, IDC_STATIC_CHN210, m_LED[209]);    
	DDX_Control(pDX, IDC_STATIC_CHN211, m_LED[210]);   
	DDX_Control(pDX, IDC_STATIC_CHN212, m_LED[211]);   
	DDX_Control(pDX, IDC_STATIC_CHN213, m_LED[212]);   
	DDX_Control(pDX, IDC_STATIC_CHN214, m_LED[213]);   
	DDX_Control(pDX, IDC_STATIC_CHN215, m_LED[214]);   
	DDX_Control(pDX, IDC_STATIC_CHN216, m_LED[215]);   
	DDX_Control(pDX, IDC_STATIC_CHN217, m_LED[216]);   
	DDX_Control(pDX, IDC_STATIC_CHN218, m_LED[217]);   
	DDX_Control(pDX, IDC_STATIC_CHN219, m_LED[218]);   	                         
	DDX_Control(pDX, IDC_STATIC_CHN220, m_LED[219]);
	DDX_Control(pDX, IDC_STATIC_CHN221, m_LED[220]);
	DDX_Control(pDX, IDC_STATIC_CHN222, m_LED[221]);
	DDX_Control(pDX, IDC_STATIC_CHN223, m_LED[222]);
	DDX_Control(pDX, IDC_STATIC_CHN224, m_LED[223]);
	DDX_Control(pDX, IDC_STATIC_CHN225, m_LED[224]);
	DDX_Control(pDX, IDC_STATIC_CHN226, m_LED[225]);
	DDX_Control(pDX, IDC_STATIC_CHN227, m_LED[226]);
	DDX_Control(pDX, IDC_STATIC_CHN228, m_LED[227]);
	DDX_Control(pDX, IDC_STATIC_CHN229, m_LED[228]);                                 
	DDX_Control(pDX, IDC_STATIC_CHN230, m_LED[229]); 
	DDX_Control(pDX, IDC_STATIC_CHN231, m_LED[230]); 
	DDX_Control(pDX, IDC_STATIC_CHN232, m_LED[231]); 
	DDX_Control(pDX, IDC_STATIC_CHN233, m_LED[232]); 
	DDX_Control(pDX, IDC_STATIC_CHN234, m_LED[233]); 
	DDX_Control(pDX, IDC_STATIC_CHN235, m_LED[234]); 
	DDX_Control(pDX, IDC_STATIC_CHN236, m_LED[235]); 
	DDX_Control(pDX, IDC_STATIC_CHN237, m_LED[236]); 
	DDX_Control(pDX, IDC_STATIC_CHN238, m_LED[237]); 
	DDX_Control(pDX, IDC_STATIC_CHN239, m_LED[238]);                                      
	DDX_Control(pDX, IDC_STATIC_CHN240, m_LED[239]); 
/*	DDX_Control(pDX, IDC_STATIC_CHN241, m_LED[240]); 
	DDX_Control(pDX, IDC_STATIC_CHN242, m_LED[241]); 
	DDX_Control(pDX, IDC_STATIC_CHN243, m_LED[242]); 
	DDX_Control(pDX, IDC_STATIC_CHN244, m_LED[243]); 
	DDX_Control(pDX, IDC_STATIC_CHN245, m_LED[244]); 
	DDX_Control(pDX, IDC_STATIC_CHN246, m_LED[245]); 
	DDX_Control(pDX, IDC_STATIC_CHN247, m_LED[246]); 
	DDX_Control(pDX, IDC_STATIC_CHN248, m_LED[247]); 
	DDX_Control(pDX, IDC_STATIC_CHN249, m_LED[248]);                          
	DDX_Control(pDX, IDC_STATIC_CHN250, m_LED[249]); 
	DDX_Control(pDX, IDC_STATIC_CHN251, m_LED[250]); 
	DDX_Control(pDX, IDC_STATIC_CHN252, m_LED[251]);*/

	DDX_Control(pDX, IDC_TAB_TESTING_ALLCAR, m_TabCtrlTestAllCar);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLoadAgingDlg, CDialog)
//{{AFX_MSG_MAP(CLoadAgingDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_WM_TIMER()
ON_WM_SIZE()
ON_BN_CLICKED(IDC_BUTTON_PASSWORD, OnButtonPassword)
ON_BN_CLICKED(IDC_BUTTON_SYSCONFIG, OnButtonSysconfig)
ON_BN_CLICKED(IDC_BUTTON_PARAMEDIT, OnButtonParamedit)
ON_BN_CLICKED(IDC_BUTTON_PARAMIMPORT, OnButtonParamimport)
ON_BN_CLICKED(IDC_BUTTON_HELP, OnButtonHelp)
ON_WM_CTLCOLOR()
ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_TESTING_ALLCAR, OnSelchangeTabTestingAllcar)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoadAgingDlg message handlers

BOOL CLoadAgingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	char tmpStr[64]={0};
	// Add "About..." menu item to system menu.
	
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	
	//判断是否已经运行, 系统同时只能运行一次
	HANDLE m_hMutex;   
	m_hMutex = CreateMutex(NULL, TRUE, "LoadAging");   //
    ASSERT(m_hMutex);
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        MessageBox("已经运行");  
		exit(0);
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	m_BtnSysSet.SetIcon(IDI_ICON_SYSSET);
	m_BtnSysSet.DrawTransparent(TRUE);
	
	m_BtnParamImport.SetIcon(IDI_ICON_PARAMIMPORT);
	m_BtnParamImport.DrawTransparent(TRUE);
	
	m_BtnParamEdit.SetIcon(IDI_ICON_PARAMEDIT);
	m_BtnParamEdit.DrawTransparent(TRUE);
	
	m_BtnPassword.SetIcon(IDI_ICON_PASSWORD);
	m_BtnPassword.DrawTransparent(TRUE);
	
	m_BtnHelp.SetIcon(IDI_ICON_HELP);
	m_BtnHelp.DrawTransparent(TRUE);
	
	m_BtnExit.SetIcon(IDI_ICON_EXIT);
	m_BtnExit.DrawTransparent(TRUE);
	
	m_BtnTest.SetIcon(IDI_ICON_STARTTEST);
	m_BtnTest.DrawTransparent(TRUE);
	
	m_brshBack.CreateSolidBrush(g_ColorBlue);//对话框背景颜色
	
	//加密信息校验,加密狗---------------------
	int ret=1;
	//ret = SystemAuthorizeCheck();	
	if(ret<0)
	{
		AfxMessageBox("加密狗校验失败!");
		//AfxMessageBox("Error, System Authorized Check Fail!");
		exit(1);
	}
	
	//读取ini文件-----------------------------
	ReadFromConfig();
	SetWindowText(cfg_Title);			//设置对话框标题
	
	//系统初始化
	InitSystem();

	m_TabCtrlTestAllCar.InsertItem(0, _T("A1区"));
	m_TabCtrlTestAllCar.InsertItem(1, _T("A2区"));
	m_TabCtrlTestAllCar.InsertItem(2, _T("A3区"));
	m_TabCtrlTestAllCar.InsertItem(3, _T("A4区"));
	m_TabCtrlTestAllCar.InsertItem(4, _T("B1区"));
	m_TabCtrlTestAllCar.InsertItem(5, _T("B2区"));
	m_TabCtrlTestAllCar.InsertItem(6, _T("B3区"));
	m_TabCtrlTestAllCar.InsertItem(7, _T("B4区"));
	m_TabCtrlTestAllCar.InsertItem(8, _T("C1区"));
	m_TabCtrlTestAllCar.InsertItem(9, _T("C2区"));
	m_TabCtrlTestAllCar.InsertItem(10, _T("C3区"));
	m_TabCtrlTestAllCar.InsertItem(11, _T("C4区"));
	m_TabCtrlTestAllCar.InsertItem(12, _T("D1区"));
	m_TabCtrlTestAllCar.InsertItem(13, _T("D2区"));
	m_TabCtrlTestAllCar.InsertItem(14, _T("D3区"));
	m_TabCtrlTestAllCar.InsertItem(15, _T("D4区"));	
		
	this->SetDlgItemText(IDC_BUTTON_TEST,"A1开始测试");
	
	//刷新当前测试车状态
	DrawCurrentCarInfo();

	// CG: The following block was added by the ToolTips component.
	{
		// Create the ToolTip control.
		m_tooltip.Create(this);
		m_tooltip.Activate(TRUE);
		
		// TODO: Use one of the following forms to add controls:
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), "<text>");
		
	}
	//初始化LED灯
	CWnd*   pW;
	for(int iLedIndex=0; iLedIndex<MAX_LED_COUNT; iLedIndex++)
	{
		m_LED[iLedIndex].pWnd = GetDlgItem(IDC_STATIC_CHN1+iLedIndex); // 获得对话框上的picture的窗口句柄
		m_LED[iLedIndex].pWnd->GetClientRect(&m_LED[iLedIndex].rect);         // 获取绘制坐标的文本框
		
		m_LED[iLedIndex].pDC = m_LED[iLedIndex].pWnd->GetDC();                 // 获得对话框上的picture的设备指针
		m_LED[iLedIndex].pOldBmp = NULL;                           // 将旧的位图指针置空
		// 创建内存绘图设备，使内存位图的DC与控件的DC关联
		m_LED[iLedIndex].memDC.CreateCompatibleDC(m_LED[iLedIndex].pDC); 
		m_LED[iLedIndex].memBitmap.CreateCompatibleBitmap(m_LED[iLedIndex].pDC,m_LED[iLedIndex].rect.right,m_LED[iLedIndex].rect.bottom);
		m_LED[iLedIndex].pOldBmp = m_LED[iLedIndex].memDC.SelectObject(&m_LED[iLedIndex].memBitmap);

		char tmpStr[128]={0};
		sprintf(tmpStr,"%d",iLedIndex+1);
		m_LED[iLedIndex].SetOnText(tmpStr);
		m_LED[iLedIndex].SetOnFgColor(g_ColorNotSelect);
		
		pW=GetDlgItem(1400+1+iLedIndex);
		sprintf(tmpStr,"位置:%d,  状态:未选中\n设定\n实时:功率= \n电流= , 电压=",iLedIndex+1);
		m_tooltip.AddTool(pW,tmpStr); 	
		m_tooltip.SetMaxTipWidth(200);
		
	}

	//登录密码校验----------------------------
	LongDlg loginDlg;
	int nRes = loginDlg.DoModal();
	if ( nRes != IDOK )
	{
		exit(0);
	}

	if (g_LoginCheckOK == 1)//技术员
	{
		GetDlgItem(IDC_BUTTON_PARAMEDIT)->EnableWindow(FALSE);		//开始测试
	}
	
	//每1s产生一次定时器中断，定时器的ID为1
	SetTimer(TIMER_SYSRUN,1000,NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLoadAgingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLoadAgingDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		for(int iLedIndex=0; iLedIndex<MAX_LED_COUNT; iLedIndex++)
		{
			m_LED[iLedIndex].pDC->BitBlt(m_LED[iLedIndex].rect.left,m_LED[iLedIndex].rect.top,m_LED[iLedIndex].rect.right,m_LED[iLedIndex].rect.bottom,&m_LED[iLedIndex].memDC,0,0,SRCCOPY); 
		}
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLoadAgingDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLoadAgingDlg::OnTimer(UINT nIDEvent) 
{
	char logBuf[512];
	memset(logBuf,0,sizeof(logBuf));

	if(nIDEvent == TIMER_SYSRUN)  //系统运行时间统计，1秒钟定时器
	{
		int ret;
		char tmpStr[64]={0};
		g_SystemRunTimeCounter++;
		if(g_SystemRunTimeCounter>3600)	//每小时进行一次授权校验
		{
			g_SystemRunTimeCounter=0;
			ret = SystemAuthorizeCheck();
			sprintf(logBuf,"SystemAuthorizeCheck() ret=%d.",ret);
			WriteLog(LEVEL_DEBUG,logBuf);
			if(ret<0)
			{
				AfxMessageBox("Error, System Authorized Check Fail!");
				exit(1);//授权校验失败，退出系统
			}
			if(ret<12)
			{
				sprintf(tmpStr,"Valid time %d hours, please contact us!", ret);
				AfxMessageBox(tmpStr);
			}
		}
		
		DrawCurrentCarInfo();//刷新当前测试车状态
		//RefreshAllLED(g_curSelTestCar);
	}
	
	if(nIDEvent >= TIMER_CAR_1 && nIDEvent<= TIMER_CAR_16)
	{
		int		selTab;					//主界面当前选中车		
		int		serialPortCommandType;	//串口命令类型
		int     curLoadNum;				//当前测试负载号, [1,63]
		int     curCarID;				//当前测试车ID, [0,15]
		char	testTimeStr[16]={0};
		char	tmpStr[64]={0};
		curCarID = nIDEvent-TIMER_CAR_1;				//当前测试车ID
		selTab=m_TabCtrlTestAllCar.GetCurSel();			//当前选中测试车

		//Clear LED error state
		if(g_AllCar[curCarID].m_CMDClear==1)
		{
			for(int iLoad=g_AllCar[curCarID].testParam.startLoadNum-1;iLoad<g_AllCar[curCarID].testParam.endLoadNum;iLoad++)
			{
				g_AllCar[curCarID].m_Load[iLoad].m_Channel[0].m_ChnHaveBeenError=0;
				g_AllCar[curCarID].m_Load[iLoad].m_Channel[1].m_ChnHaveBeenError=0;
				g_AllCar[curCarID].m_Load[iLoad].m_Channel[2].m_ChnHaveBeenError=0;
				g_AllCar[curCarID].m_Load[iLoad].m_Channel[3].m_ChnHaveBeenError=0;
			}
			EnterCriticalSection(&g_AllCar[curCarID].CriticalSectionClear);
			g_AllCar[curCarID].m_CMDClear=0;//这里要互斥
			LeaveCriticalSection(&g_AllCar[curCarID].CriticalSectionClear);
			
			sprintf(logBuf,"Ontimer(), carID:%d, 清除操作, Clear",curCarID);
			WriteLog(LEVEL_DEBUG,logBuf);
			GetDlgItem(IDC_BUTTON_CLEAR)->EnableWindow(TRUE);
		}

		//分析串口数据
		DealWithSerialPortData(curCarID);
		
				
		//当前测试车状态，停止测试
		if(	g_AllCar[curCarID].m_CarState == CAR_STATE_TESTFINISH ||
			g_AllCar[curCarID].m_CarState == CAR_STATE_IMPORTED )
		{	
			KillTimer(nIDEvent);		//关闭定时器

			WriteSerialPortCommand(curCarID, LOAD_COMMAND_STOP, 64);	//下发串口命令，停止测试

			GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(FALSE);
			
			Sleep(50);			
			WriteSerialPortCommand(curCarID, LOAD_COMMAND_BEEPOFF, 0);	//下发串口命令，停止测试

			Sleep(50);
			WriteSerialPortCommand(curCarID, LOAD_COMMAND_LIGHTOFF, 0);	//下发串口命令，停止测试

			GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(TRUE);

			if(selTab == curCarID)		//如果主界面是当前测试车
			{
				sprintf(tmpStr,"%s开始测试",GetTestCarName(curCarID));
				this->SetDlgItemText(IDC_BUTTON_TEST, tmpStr);
				m_BtnTest.SetIcon(IDI_ICON_STARTTEST);
			}
			sprintf(logBuf,"Ontimer(), carID:%d, 停止操作.",curCarID);
			WriteLog(LEVEL_DEBUG,logBuf);
	
			return;//停止测试

		}else if ( g_AllCar[curCarID].m_CarState == CAR_STATE_TESTING )
		{
			//测试流程，下发命令
			curLoadNum = GetCurLoadNum(curCarID);										//获取当前待测试负载号
			serialPortCommandType = GetSerialPortCommandType(curCarID,curLoadNum);		//当前测试车，当前测试负载，获取应下发命令类型
			WriteSerialPortCommand(curCarID, serialPortCommandType, curLoadNum);		//下发串口命令
			RefreshOneLED(curCarID,curLoadNum-1,0);
			RefreshOneLED(curCarID,curLoadNum-1,1);
			RefreshOneLED(curCarID,curLoadNum-1,2);
			RefreshOneLED(curCarID,curLoadNum-1,3);
		}
		
		g_AllCar[curCarID].m_TimeCounter++;							//测试时间统计
		g_AllCar[curCarID].m_curLoadNum++;							//当前测试负载号，循环测试负载

		//保存测试数据
		SaveTestData(curCarID);

		//总的测试时间到了,单位ms
		//LONGLONG lTotlalTime = (GetTickCount() - g_AllCar[curCarID].iTestStartTime)/1000;//运行了多少秒
		LONGLONG lTotlalTime = g_AllCar[curCarID].m_TimeCounter*COMMAND_INTERVAL;//运行了多少ms

		if ( lTotlalTime>= g_AllCar[curCarID].testParam.totalTestTime*60*1000 )
		{	
			sprintf(tmpStr,"%s:停止测试, 时间到了.",GetTestCarName(curCarID));
			WriteLog(LEVEL_DEBUG,tmpStr);	
			g_AllCar[curCarID].m_CarState = CAR_STATE_TESTFINISH;		//测试状态置为停止
						
		}

	}
	CDialog::OnTimer(nIDEvent);
}

void CLoadAgingDlg::OnButtonPassword() 
{
	ChangePasswordDlg cpwDlg;
	int	nRes = cpwDlg.DoModal ();
	if ( nRes != IDOK )
	{
		return;
	}
	
}

void CLoadAgingDlg::OnButtonSysconfig() 
{
	//int		selTab;		//当前选中的Tab	
	char tmpStr[64]={0};
	SysConfigDlg scDlg;
	int iCarIndex;
	char logBuf[64]={0};
	//判断是否有正在测试项
	for(iCarIndex=0; iCarIndex<MAX_CAR_COUNT; iCarIndex++)
	{
		if(g_AllCar[iCarIndex].m_CarState == CAR_STATE_TESTING)
		{
			sprintf(logBuf,"请先停止所有测试, 测试中:%s",GetTestCarName(iCarIndex));
			AfxMessageBox(logBuf);
			return ;
		}
	}
	
	int	nRes = scDlg.DoModal ();
	if ( nRes != IDOK )
	{
		return;
	}

}

void CLoadAgingDlg::OnButtonParamedit() 
{
	ParamEditDlg peDlg;
	int	nRes = peDlg.DoModal ();
	if ( nRes != IDOK )
	{
		return;
	}	
}
void CLoadAgingDlg::OnButtonHelp() 
{
	char cFileName[100];
	char curDir[100];
	CString csFileName;
	memset(cFileName, 0, sizeof(cFileName));

	/*	char	tmpStr[256];
	char    logBuf[4096];

	int		selTab;	
	memset(tmpStr,0,sizeof(tmpStr));
	memset(logBuf,0,sizeof(logBuf));
	
	selTab=m_TabCtrlTestAllCar.GetCurSel();		//当前选中测试车

	//当前测试车串口
	if(g_AllCar[selTab].m_ComID == 0 )
	{
		sprintf(tmpStr,"%s, 请先选择串口!",GetTestCarName(selTab));
		AfxMessageBox(tmpStr);
		return;
	}
	//当前测试车状态
	if(g_AllCar[selTab].m_CarState != CAR_STATE_IMPORTED)
	{
		sprintf(tmpStr,"%s, 请先导入测试参数!",GetTestCarName(selTab));
		AfxMessageBox(tmpStr);
		return;
	}	
	//串口初始化
	if ( g_Ports[selTab].InitPort(this, g_AllCar[selTab].m_ComID, 38400, 'N',8,1,EV_RXCHAR | EV_CTS, SERIALPORT_BUFSIZE) )
	{
		g_Ports[selTab].StartMonitoring();
		sprintf(logBuf,"InitPort OK. carID:%02d, comID:%02d",selTab,g_AllCar[selTab].m_ComID);
		WriteLog(LEVEL_DEBUG,logBuf);
		
	}else
	{
		// port not found
		sprintf(logBuf,"Error,InitPort fail! carID:%02d, comID:%02d",selTab,g_AllCar[selTab].m_ComID);
		WriteLog(LEVEL_ERROR,logBuf);
		AfxMessageBox(logBuf);
		return;
	}
	
	SerialPortFuncTestThread();//串口数据测试线程
	*/
							   /*
	int sleepTime=100;
	for (int i=0;i<20;i++)
	{
		if(i%2==0 )
		{
			if (i==2)
			{
				Sleep(sleepTime);
				WriteSerialPortCommand(g_curSelTestCar,LOAD_COMMAND_LIGHTON,0);
			}else if (i==4)
			{
				Sleep(sleepTime);
				WriteSerialPortCommand(g_curSelTestCar,LOAD_COMMAND_LIGHTOFF,0);
			}else if (i==6)
			{
				Sleep(sleepTime);
				WriteSerialPortCommand(g_curSelTestCar,LOAD_COMMAND_BEEPON,0);
			}else if (i==8)
			{
				Sleep(sleepTime);
				WriteSerialPortCommand(g_curSelTestCar,LOAD_COMMAND_BEEPOFF,0);
			}else if (i==10)
			{
				Sleep(sleepTime);
				WriteSerialPortCommand(g_curSelTestCar,LOAD_COMMAND_110V,0);
			}else if (i==12)
			{
				Sleep(sleepTime);
				WriteSerialPortCommand(g_curSelTestCar,LOAD_COMMAND_220V,0);
			}
		}else
		{
			if (i>3)
			{
				Sleep(sleepTime);
				//	WriteSerialPortCommand(g_curSelTestCar,LOAD_COMMAND_SET,3);
				WriteSerialPortCommand(g_curSelTestCar,LOAD_COMMAND_SET,1);
		
			}
		
		}
		if (i==14)
		{
			i=0;
		}
	}*/
	
	GetCurrentDirectory(256,curDir);
	sprintf(cFileName,"%s\\help.pdf",curDir);
	csFileName = cFileName;
	csFileName.Replace("\\","\\\\"); //将'\'替换为"\\"
	ShellExecute(this->m_hWnd, NULL, csFileName,NULL,NULL, SW_SHOWNORMAL);
	
}
void CLoadAgingDlg::OnButtonParamimport() 
{
	ImportParamDlg ipDlg;
	int	nRes = ipDlg.DoModal();
	if ( nRes != IDOK )
	{
		return;
	}		
}
void CLoadAgingDlg::OnCancel() 
{
	char tmpStr[100]={0};
	for (int carID=0; carID<MAX_CAR_COUNT; carID++)
	{
		if (CAR_STATE_TESTING == g_AllCar[carID].m_CarState)
		{
			sprintf(tmpStr,"%s在测试中, 请先停止!",GetTestCarName(carID));
			AfxMessageBox(tmpStr);
			return;
		}
	}
	for(int iLedIndex=0; iLedIndex<MAX_LED_COUNT; iLedIndex++)
	{
		m_LED[iLedIndex].memDC.SelectObject(m_LED[iLedIndex].pOldBmp);
		m_LED[iLedIndex].memDC.DeleteDC();
		m_LED[iLedIndex].memBitmap.DeleteObject();
	}
	

	WriteLog(LEVEL_DEBUG,"退出系统");
	CloseLogFile();

	CDialog::OnCancel();
}

HBRUSH CLoadAgingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
    if ( nCtlColor == CTLCOLOR_DLG )
    { 
        return (HBRUSH)m_brshBack; 
    }
	return hbr;
}
/************************************************************************/
/* 情况测试记录，当前测试车                                             */
/************************************************************************/
void CLoadAgingDlg::OnButtonClear() 
{
	char logBuf[256];
	memset(logBuf,0,sizeof(logBuf));
	
	//清除当前测试车，错误记录
	if(g_AllCar[g_curSelTestCar].m_CarState == CAR_STATE_TESTING)
	{
		EnterCriticalSection(&g_AllCar[g_curSelTestCar].CriticalSectionClear);
		g_AllCar[g_curSelTestCar].m_CMDClear=1;		//需要做互斥
		LeaveCriticalSection(&g_AllCar[g_curSelTestCar].CriticalSectionClear);

	}
	//Disable test Button 
	GetDlgItem(IDC_BUTTON_CLEAR)->EnableWindow(FALSE);

	sprintf(logBuf,"carID:%d, carState:%s, 清除操作, Clear",
		g_curSelTestCar,GetTestCarStateName(g_curSelTestCar));
	WriteLog(LEVEL_DEBUG,logBuf);


}
void CLoadAgingDlg::OnSize(UINT nType, int cx, int cy) 
{
	
	if (	nType != SIZE_MINIMIZED)
	{
		CWnd * pWnd;
		CWnd *buttonWnd[10];
		buttonWnd[0]=GetDlgItem(IDC_STATIC_COMMAND);
		buttonWnd[1]=GetDlgItem(IDC_BUTTON_SYSCONFIG);
		buttonWnd[2]=GetDlgItem(IDC_BUTTON_PARAMIMPORT);
		buttonWnd[3]=GetDlgItem(IDC_BUTTON_PARAMEDIT);
		buttonWnd[4]=GetDlgItem(IDC_BUTTON_PASSWORD);
		buttonWnd[5]=GetDlgItem(IDC_BUTTON_HELP);
		buttonWnd[6]=GetDlgItem(IDC_BUTTON_TEST);
		buttonWnd[7]=GetDlgItem(IDC_TAB_TESTING_ALLCAR);
		buttonWnd[8]=GetDlgItem(IDC_STATIC_CURCAR);
		buttonWnd[9]=GetDlgItem(IDC_BUTTON_CLEAR);
		int i;
		for (i=0;i<10;i++)
		{
			CRect rect;
			if(buttonWnd[i])
			{
				buttonWnd[i]->GetWindowRect(&rect);
				ScreenToClient(&rect);
				rect.left=rect.left*cx/m_rect.Width();
				rect.right=rect.right*cx/m_rect.Width();
				rect.top=rect.top*cy/m_rect.Height();
				rect.bottom=rect.bottom*cy/m_rect.Height();
				buttonWnd[i]->MoveWindow(rect);
			}
		}
        for (i=0;i<=240;i++)
		{
			CRect rect;
			pWnd=GetDlgItem(IDC_STATIC_CHN1+i);
			if(pWnd)
			{
				pWnd->GetWindowRect(&rect);
				ScreenToClient(&rect);
				rect.left=rect.left*cx/m_rect.Width();
				rect.right=rect.right*cx/m_rect.Width();
				rect.top=rect.top*cy/m_rect.Height();
				rect.bottom=rect.bottom*cy/m_rect.Height();
				
				pWnd->MoveWindow(rect);
				
				int iLedIndex=i;
				m_LED[iLedIndex].pWnd = GetDlgItem(IDC_STATIC_CHN1+iLedIndex); // 获得对话框上的picture的窗口句柄
				m_LED[iLedIndex].pWnd->GetClientRect(&m_LED[iLedIndex].rect);         // 获取绘制坐标的文本框
				
			//	m_LED[iLedIndex].pDC = m_LED[iLedIndex].pWnd->GetDC();                 // 获得对话框上的picture的设备指针
			//	m_LED[iLedIndex].pOldBmp = NULL;                           // 将旧的位图指针置空
				// 创建内存绘图设备，使内存位图的DC与控件的DC关联
			//	m_LED[iLedIndex].memDC.CreateCompatibleDC(m_LED[iLedIndex].pDC); 
				m_LED[iLedIndex].memBitmap.SetBitmapDimension(m_LED[iLedIndex].rect.right,m_LED[iLedIndex].rect.bottom);
				m_LED[iLedIndex].pOldBmp = m_LED[iLedIndex].memDC.SelectObject(&m_LED[iLedIndex].memBitmap);

			}
			
		}
		GetClientRect(&m_rect);
		RefreshAllLED(g_curSelTestCar);	
	}

	CDialog::OnSize(nType, cx, cy);
	//UPDATE_EASYSIZE;
}
/************************************************************************/
/* 开始测试                                                             */
/************************************************************************/
void CLoadAgingDlg::OnButtonTest() 
{
	int		selTab;	
	char	testBtnStr[10]={0};
	char	tmpStr[256];
	char    logBuf[4096];

	memset(tmpStr,0,sizeof(tmpStr));
	memset(logBuf,0,sizeof(logBuf));
	
	selTab=m_TabCtrlTestAllCar.GetCurSel();		//当前选中测试车
	
	this->GetDlgItem ( IDC_BUTTON_TEST )->GetWindowText( testBtnStr, 30 );
	
	if ( strcmp(&testBtnStr[2],"开始测试") == 0)
	{
		//当前测试车串口
		if(g_AllCar[selTab].m_ComID == 0 )
		{
			sprintf(tmpStr,"%s, 请先选择串口!",GetTestCarName(selTab));
			AfxMessageBox(tmpStr);
			return;
		}
		//当前测试车状态
		if(g_AllCar[selTab].m_CarState == CAR_STATE_NOIMPORT )
		{
			sprintf(tmpStr,"%s, 请先导入测试参数!",GetTestCarName(selTab));
			AfxMessageBox(tmpStr);
			return;
		}	
		//串口初始化
		if ( g_Ports[selTab].InitPort(this, g_AllCar[selTab].m_ComID, 38400, 'N',8,1,EV_RXCHAR | EV_CTS, SERIALPORT_BUFSIZE) )
		{
			g_Ports[selTab].StartMonitoring();
			sprintf(logBuf,"InitPort OK. carID:%02d, comID:%02d",selTab,g_AllCar[selTab].m_ComID);
			WriteLog(LEVEL_DEBUG,logBuf);
			
		}else
		{
			// port not found
			sprintf(logBuf,"Error,InitPort fail! carID:%02d, comID:%02d",selTab,g_AllCar[selTab].m_ComID);
			WriteLog(LEVEL_ERROR,logBuf);
			AfxMessageBox(logBuf);
			return;
		}
		
		//清空当前选中测试车的单链表	
		//是否保存测试数据, 初始化日志配置,日志级别:WARN,日志最大:20M
		char logPath[256]={0};
		switch (selTab)
		{
		case 0:
			sLinkList1.clear();		//清空单链表
			sprintf(logPath,"%s\\A1",cfg_DataRecordPath);	
			break;
		case 1:
			sLinkList2.clear();		//清空单链表
			sprintf(logPath,"%s\\A2",cfg_DataRecordPath);	
			break;
		case 2:
			sLinkList3.clear();		//清空单链表
			sprintf(logPath,"%s\\A3",cfg_DataRecordPath);	
			break;
		case 3:
			sLinkList4.clear();		//清空单链表
			sprintf(logPath,"%s\\A4",cfg_DataRecordPath);	
			break;
		case 4:
			sLinkList5.clear();		//清空单链表
			sprintf(logPath,"%s\\B1",cfg_DataRecordPath);	
			break;
		case 5:
			sLinkList6.clear();		//清空单链表
			sprintf(logPath,"%s\\B2",cfg_DataRecordPath);	
			break;
		case 6:
			sLinkList7.clear();		//清空单链表
			sprintf(logPath,"%s\\B3",cfg_DataRecordPath);	
			break;
		case 7:
			sLinkList8.clear();		//清空单链表
			sprintf(logPath,"%s\\B4",cfg_DataRecordPath);	
			break;
		case 8:
			sLinkList9.clear();		//清空单链表
			sprintf(logPath,"%s\\C1",cfg_DataRecordPath);	
			break;
		case 9:
			sLinkList10.clear();		//清空单链表
			sprintf(logPath,"%s\\C2",cfg_DataRecordPath);	
			break;
		case 10:
			sLinkList11.clear();		//清空单链表
			sprintf(logPath,"%s\\C3",cfg_DataRecordPath);	
			break;
		case 11:
			sLinkList12.clear();		//清空单链表
			sprintf(logPath,"%s\\C4",cfg_DataRecordPath);	
			break;
		case 12:
			sLinkList13.clear();		//清空单链表
			sprintf(logPath,"%s\\D1",cfg_DataRecordPath);	
			break;
		case 13:
			sLinkList14.clear();		//清空单链表
			sprintf(logPath,"%s\\D2",cfg_DataRecordPath);	
			break;
		case 14:
			sLinkList15.clear();		//清空单链表
			sprintf(logPath,"%s\\D3",cfg_DataRecordPath);	
			break;
		case 15:
			sLinkList16.clear();		//清空单链表
			sprintf(logPath,"%s\\D4",cfg_DataRecordPath);	
			break;
		}
		//是否保存测试数据
		if (g_AllCar[selTab].testParam.isSaveData)
		{
			//查找数据保存路径
			WIN32_FIND_DATA   wfd;
			BOOL rValue = FALSE;
			HANDLE hFind = FindFirstFile(cfg_DataRecordPath, &wfd);
			if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				rValue = TRUE;  
			}
			FindClose(hFind);
			if (rValue == FALSE)
			{
				CreateDirectory(cfg_DataRecordPath,NULL);
			}
			
			if (g_data[selTab].InitLogCfg(LEVEL_DEBUG, logPath, 20*1024*1024) < 0)	
			{
				sprintf(tmpStr,"Error, %s InitLogCfg() Fail!",GetTestCarName(selTab));
				WriteLog(LEVEL_ERROR,tmpStr);
				AfxMessageBox(tmpStr);
				return;
			}
			sprintf(logBuf,"所属:,%s",GetTestCarName(selTab));
			g_data[selTab].WriteData(logBuf);
			
			SYSTEMTIME	sysTime;			//系统时间
			GetLocalTime( &sysTime );		//获得系统当前时间
			sprintf(logBuf,"老化开始时间:,%04d年%02d月%02d日 %02d时%02d分%02d秒\n",
				sysTime.wYear, 
				sysTime.wMonth, 
				sysTime.wDay, 
				sysTime.wHour, 
				sysTime.wMinute, 
				sysTime.wSecond);
			g_data[selTab].WriteData(logBuf);
			
			sprintf(logBuf,"老化时间:,%d分钟\n",g_AllCar[selTab].testParam.totalTestTime);
			g_data[selTab].WriteData(logBuf);
			
			g_data[selTab].WriteData("负载参数设定值:\n");
			
			sprintf(logBuf,"第1层,%s, , , , , 第2层,%s, , , , , 第3层,%s, , , , , 第4层,%s, , , , ,\n",
				GetLoadModeName(g_AllCar[selTab].testParam.layerParam[0].loadMode),
				GetLoadModeName(g_AllCar[selTab].testParam.layerParam[1].loadMode),
				GetLoadModeName(g_AllCar[selTab].testParam.layerParam[2].loadMode),
				GetLoadModeName(g_AllCar[selTab].testParam.layerParam[3].loadMode));
			g_data[selTab].WriteData(logBuf);
			
			sprintf(logBuf,",%s, ,,%s, ,,%s, ,,%s, ,,\n",
				GetChnTitleName(g_AllCar[selTab].testParam.layerParam[0].loadMode),
				GetChnTitleName(g_AllCar[selTab].testParam.layerParam[1].loadMode),
				GetChnTitleName(g_AllCar[selTab].testParam.layerParam[2].loadMode),
				GetChnTitleName(g_AllCar[selTab].testParam.layerParam[3].loadMode));
			g_data[selTab].WriteData(logBuf);
			
			sprintf(logBuf,"通道1,%0.3f,%0.3f,%0.3f,%s,,通道1,%0.3f,%0.3f,%0.3f,%s,,通道1,%0.3f,%0.3f,%0.3f,%s,,通道1,%0.3f,%0.3f,%0.3f,%s,,\n",
				g_AllCar[selTab].testParam.layerParam[0].chn1Value,
				g_AllCar[selTab].testParam.layerParam[0].chn1Max,
				g_AllCar[selTab].testParam.layerParam[0].chn1Min,
				g_AllCar[selTab].testParam.layerParam[0].chn1ProName,
				g_AllCar[selTab].testParam.layerParam[1].chn1Value,
				g_AllCar[selTab].testParam.layerParam[1].chn1Max,
				g_AllCar[selTab].testParam.layerParam[1].chn1Min,
				g_AllCar[selTab].testParam.layerParam[1].chn1ProName,
				g_AllCar[selTab].testParam.layerParam[2].chn1Value,
				g_AllCar[selTab].testParam.layerParam[2].chn1Max,
				g_AllCar[selTab].testParam.layerParam[2].chn1Min,
				g_AllCar[selTab].testParam.layerParam[2].chn1ProName,
				g_AllCar[selTab].testParam.layerParam[3].chn1Value,
				g_AllCar[selTab].testParam.layerParam[3].chn1Max,
				g_AllCar[selTab].testParam.layerParam[3].chn1Min,
				g_AllCar[selTab].testParam.layerParam[3].chn1ProName);
			g_data[selTab].WriteData(logBuf);
			
			sprintf(logBuf,"通道2,%0.3f,%0.3f,%0.3f,%s,,通道2,%0.3f,%0.3f,%0.3f,%s,,通道2,%0.3f,%0.3f,%0.3f,%s,,通道2,%0.3f,%0.3f,%0.3f,%s,,\n",
				g_AllCar[selTab].testParam.layerParam[0].chn2Value,
				g_AllCar[selTab].testParam.layerParam[0].chn2Max,
				g_AllCar[selTab].testParam.layerParam[0].chn2Min,
				g_AllCar[selTab].testParam.layerParam[0].chn2ProName,
				g_AllCar[selTab].testParam.layerParam[1].chn2Value,
				g_AllCar[selTab].testParam.layerParam[1].chn2Max,
				g_AllCar[selTab].testParam.layerParam[1].chn2Min,
				g_AllCar[selTab].testParam.layerParam[1].chn2ProName,
				g_AllCar[selTab].testParam.layerParam[2].chn2Value,
				g_AllCar[selTab].testParam.layerParam[2].chn2Max,
				g_AllCar[selTab].testParam.layerParam[2].chn2Min,
				g_AllCar[selTab].testParam.layerParam[2].chn2ProName,
				g_AllCar[selTab].testParam.layerParam[3].chn2Value,
				g_AllCar[selTab].testParam.layerParam[3].chn2Max,
				g_AllCar[selTab].testParam.layerParam[3].chn2Min,
				g_AllCar[selTab].testParam.layerParam[3].chn2ProName);
			g_data[selTab].WriteData(logBuf);
			
			sprintf(logBuf,"通道3,%0.3f,%0.3f,%0.3f,%s,,通道3,%0.3f,%0.3f,%0.3f,%s,,通道3,%0.3f,%0.3f,%0.3f,%s,,通道3,%0.3f,%0.3f,%0.3f,%s,,\n",
				g_AllCar[selTab].testParam.layerParam[0].chn3Value,
				g_AllCar[selTab].testParam.layerParam[0].chn3Max,
				g_AllCar[selTab].testParam.layerParam[0].chn3Min,
				g_AllCar[selTab].testParam.layerParam[0].chn3ProName,
				g_AllCar[selTab].testParam.layerParam[1].chn3Value,
				g_AllCar[selTab].testParam.layerParam[1].chn3Max,
				g_AllCar[selTab].testParam.layerParam[1].chn3Min,
				g_AllCar[selTab].testParam.layerParam[1].chn3ProName,
				g_AllCar[selTab].testParam.layerParam[2].chn3Value,
				g_AllCar[selTab].testParam.layerParam[2].chn3Max,
				g_AllCar[selTab].testParam.layerParam[2].chn3Min,
				g_AllCar[selTab].testParam.layerParam[2].chn3ProName,
				g_AllCar[selTab].testParam.layerParam[3].chn3Value,
				g_AllCar[selTab].testParam.layerParam[3].chn3Max,
				g_AllCar[selTab].testParam.layerParam[3].chn3Min,
				g_AllCar[selTab].testParam.layerParam[3].chn3ProName);
			g_data[selTab].WriteData(logBuf);
			
			sprintf(logBuf,"通道4,%0.3f,%0.3f,%0.3f,%s,,通道4,%0.3f,%0.3f,%0.3f,%s,,通道4,%0.3f,%0.3f,%0.3f,%s,,通道4,%0.3f,%0.3f,%0.3f,%s,,\n",
				g_AllCar[selTab].testParam.layerParam[0].chn4Value,
				g_AllCar[selTab].testParam.layerParam[0].chn4Max,
				g_AllCar[selTab].testParam.layerParam[0].chn4Min,
				g_AllCar[selTab].testParam.layerParam[0].chn4ProName,
				g_AllCar[selTab].testParam.layerParam[1].chn4Value,
				g_AllCar[selTab].testParam.layerParam[1].chn4Max,
				g_AllCar[selTab].testParam.layerParam[1].chn4Min,
				g_AllCar[selTab].testParam.layerParam[1].chn4ProName,
				g_AllCar[selTab].testParam.layerParam[2].chn4Value,
				g_AllCar[selTab].testParam.layerParam[2].chn4Max,
				g_AllCar[selTab].testParam.layerParam[2].chn4Min,
				g_AllCar[selTab].testParam.layerParam[2].chn4ProName,
				g_AllCar[selTab].testParam.layerParam[3].chn4Value,
				g_AllCar[selTab].testParam.layerParam[3].chn4Max,
				g_AllCar[selTab].testParam.layerParam[3].chn4Min,
				g_AllCar[selTab].testParam.layerParam[3].chn4ProName);
			g_data[selTab].WriteData(logBuf);
			
			g_data[selTab].WriteData("实际测量值------------------------------------\n");
			g_data[selTab].WriteData("时间,负载号,ch1电压,ch1电流,ch1状态,ch2电压,ch2电流,ch2状态,ch3电压,ch3电流,ch3状态,ch4电压,ch4电流,ch4状态\n");
		}	
		
		//初始化测试数据，情况之前的测试数据
		g_AllCar[selTab].iTestStartTime =  GetTickCount();
		g_AllCar[selTab].m_CarState = CAR_STATE_TESTING;
		g_AllCar[selTab].m_ErrorCounter = 0;
		g_AllCar[selTab].m_TimeCounter = 0;
		g_AllCar[selTab].m_TimerID = selTab+TIMER_CAR_1;
		g_AllCar[selTab].m_curLoadNum = 0;
		g_AllCar[selTab].m_CMDAlreadyLightOn=0;
		g_AllCar[selTab].m_CMDAlreadyBeepOn=0;
		for(int iLoad=0;iLoad<MAX_LOAD_PERCAR;iLoad++)
		{
			g_AllCar[selTab].m_Load[iLoad].m_LoadErrorCounter = 0;
		}

		//Disable test Button 
		GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(FALSE);
		//下发串口命令, 设置参数, 当前待测试的每个负载分别下发命令
		for (int chn=g_AllCar[selTab].testParam.startLoadNum; chn<=g_AllCar[selTab].testParam.endLoadNum;chn++)
		{
			g_AllCar[selTab].m_Load[chn-1].m_LoadState = LOAD_STATE_SELECT;		//更改负载状态为选中		
			Sleep(20);
			WriteSerialPortCommand(selTab, LOAD_COMMAND_SET, chn);
		}
		Sleep(3200);//make sure the data is right,负载设置参数后延时3秒后读取数据，确保开始读取到的数据不会出错
		
		//启动定时器
		SetTimer(selTab+TIMER_CAR_1,COMMAND_INTERVAL,NULL);	//每100mss产生一次定时器中断，定时器的ID为selTab+TIMER_CAR_1
		sprintf(tmpStr,"%s停止测试",GetTestCarName(selTab));
		//Enable test button
		GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(TRUE);
		this->SetDlgItemText(IDC_BUTTON_TEST,tmpStr);
		m_BtnTest.SetIcon(IDI_ICON_STOPTEST);
		
		sprintf(tmpStr,"%s:开始测试",GetTestCarName(selTab));
		WriteLog(LEVEL_DEBUG, tmpStr);																																						 WriteLog(LEVEL_DEBUG,tmpStr);
																																								 
	}else if ( strcmp(&testBtnStr[2],"停止测试") == 0)
	{
		
		g_AllCar[selTab].m_CarState = CAR_STATE_IMPORTED;//这里要互斥

		sprintf(tmpStr,"%s停止测试",GetTestCarName(selTab));
		this->SetDlgItemText(IDC_BUTTON_TEST,tmpStr);
		m_BtnTest.SetIcon(IDI_ICON_STARTTEST);

		sprintf(tmpStr,"%s:停止测试, 主动下发.",GetTestCarName(selTab));
		WriteLog(LEVEL_DEBUG,tmpStr);

	}
	
}
/************************************************************************/
/* 程序主界面，测试车切换                                               */
/************************************************************************/
void CLoadAgingDlg::OnSelchangeTabTestingAllcar(NMHDR* pNMHDR, LRESULT* pResult) 
{
	char	tmpStr[64]={0};
	int		selTab;	
	selTab=m_TabCtrlTestAllCar.GetCurSel();	//当前选中的Tab, 当前测试车	
	g_curSelTestCar = selTab;
	
	RefreshAllLED(g_curSelTestCar);//刷新当前界面所有LED
	
	DrawCurrentCarInfo();//刷新当前测试车状态

	//当前串口
	if(g_AllCar[selTab].m_ComID == 0)
	{
		sprintf(tmpStr,"无");
	}else
	{
		sprintf(tmpStr,"COM%d",g_AllCar[selTab].m_ComID);		
	}
	this->SetDlgItemText(IDC_EDIT_CURCOM,tmpStr);
	
	//测试车当前状态，以及测试时间、测试按钮显示
	switch(g_AllCar[selTab].m_CarState)
	{
	case CAR_STATE_TESTFINISH:
	case CAR_STATE_NOIMPORT:
		sprintf(tmpStr,"%s开始测试",GetTestCarName(selTab));
		this->SetDlgItemText(IDC_BUTTON_TEST,tmpStr);
		m_BtnTest.SetIcon(IDI_ICON_STARTTEST);
		break;
	case CAR_STATE_IMPORTED:
		sprintf(tmpStr,"%s开始测试",GetTestCarName(selTab));
		this->SetDlgItemText(IDC_BUTTON_TEST,tmpStr);
		m_BtnTest.SetIcon(IDI_ICON_STARTTEST);
		
		break;
	case CAR_STATE_TESTING:
		sprintf(tmpStr,"%s停止测试",GetTestCarName(selTab));
		this->SetDlgItemText(IDC_BUTTON_TEST,tmpStr);
		m_BtnTest.SetIcon(IDI_ICON_STOPTEST);		
		break;
	}

	*pResult = 0;
}

BOOL CLoadAgingDlg::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following block was added by the ToolTips component.
	{
		// Let the ToolTip process this message.
		m_tooltip.RelayEvent(pMsg);	
	}
	return CDialog::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}
