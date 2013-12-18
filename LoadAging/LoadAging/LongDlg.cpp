// LongDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LoadAging.h"
#include "LongDlg.h"
#include "LoadAging_Tran.h"
#include "LogFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern char			cfg_IniName[256];


/////////////////////////////////////////////////////////////////////////////
// LongDlg dialog


LongDlg::LongDlg(CWnd* pParent /*=NULL*/)
	: CDialog(LongDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(LongDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void LongDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LongDlg)
	DDX_Control(pDX, IDC_COMBO_USERNAME, m_ComboUserName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(LongDlg, CDialog)
	//{{AFX_MSG_MAP(LongDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LongDlg message handlers

void LongDlg::OnOK() 
{
	char logbuf[128]={0};
	CString password;
	
	GetDlgItemText(IDC_EDIT_PASSWORD,password);	//获取密码
	switch (this->m_ComboUserName.GetCurSel())
	{
	case 0:	//工程师
		if (strcmp(password, cfg_SuperPassword) == 0)
		{
			g_LoginCheckOK = 0;
			sprintf(logbuf, "%s-%s, 登陆系统, 权限:工程师", cfg_Title, cfg_SoftwareVersion);
			WriteLog(LEVEL_INFO,logbuf);
			CDialog::OnOK();	
		}
		break;
	case 1://技术员
		if (strcmp(password, cfg_NormalPassword) == 0)
		{
			g_LoginCheckOK = 1;
			sprintf(logbuf, "%s-%s, 登陆系统, 权限:技术员", cfg_Title, cfg_SoftwareVersion);
			WriteLog(LEVEL_INFO,logbuf);
			CDialog::OnOK();
		}
		break;
	}
	SetDlgItemText(IDC_EDIT_PASSWORD,"");	
	SetDlgItemText(IDC_STATIC_LOGINRESULT,"密码错误!");
	//CDialog::OnOK();
}

BOOL LongDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	this->m_ComboUserName.AddString("工程师");
	this->m_ComboUserName.AddString("技术员");
	this->m_ComboUserName.SetCurSel(0);

	SetWindowText(cfg_Title);			//设置对话框标题

	if(g_ValidHour>0 && g_ValidHour!=0xFFFD)//此时临时授权
	{
		char tmpStr[64]={0};
		//sprintf(tmpStr,"临时授权,剩余%d小时",g_ValidHour/6);		
		//SetDlgItemText(IDC_STATIC_LOGINRESULT,tmpStr);
		sprintf(tmpStr, "%d",g_ValidHour/6);
		WritePrivateProfileString ( "ConfigInfo", "ValidHour", tmpStr, cfg_IniName);
		
	}
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
