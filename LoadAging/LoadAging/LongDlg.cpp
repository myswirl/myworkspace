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
	
	GetDlgItemText(IDC_EDIT_PASSWORD,password);	//��ȡ����
	switch (this->m_ComboUserName.GetCurSel())
	{
	case 0:	//����ʦ
		if (strcmp(password, cfg_SuperPassword) == 0)
		{
			g_LoginCheckOK = 0;
			sprintf(logbuf, "%s-%s, ��½ϵͳ, Ȩ��:����ʦ", cfg_Title, cfg_SoftwareVersion);
			WriteLog(LEVEL_INFO,logbuf);
			CDialog::OnOK();	
		}
		break;
	case 1://����Ա
		if (strcmp(password, cfg_NormalPassword) == 0)
		{
			g_LoginCheckOK = 1;
			sprintf(logbuf, "%s-%s, ��½ϵͳ, Ȩ��:����Ա", cfg_Title, cfg_SoftwareVersion);
			WriteLog(LEVEL_INFO,logbuf);
			CDialog::OnOK();
		}
		break;
	}
	SetDlgItemText(IDC_EDIT_PASSWORD,"");	
	SetDlgItemText(IDC_STATIC_LOGINRESULT,"�������!");
	//CDialog::OnOK();
}

BOOL LongDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	this->m_ComboUserName.AddString("����ʦ");
	this->m_ComboUserName.AddString("����Ա");
	this->m_ComboUserName.SetCurSel(0);

	SetWindowText(cfg_Title);			//���öԻ������

	if(g_ValidHour>0 && g_ValidHour!=0xFFFD)//��ʱ��ʱ��Ȩ
	{
		char tmpStr[64]={0};
		//sprintf(tmpStr,"��ʱ��Ȩ,ʣ��%dСʱ",g_ValidHour/6);		
		//SetDlgItemText(IDC_STATIC_LOGINRESULT,tmpStr);
		sprintf(tmpStr, "%d",g_ValidHour/6);
		WritePrivateProfileString ( "ConfigInfo", "ValidHour", tmpStr, cfg_IniName);
		
	}
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
