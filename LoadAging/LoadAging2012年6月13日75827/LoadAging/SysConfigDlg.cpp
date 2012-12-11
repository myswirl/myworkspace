// SysConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LoadAging.h"
#include "SysConfigDlg.h"
#include "LoadAging_Tran.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// SysConfigDlg dialog


SysConfigDlg::SysConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(SysConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(SysConfigDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void SysConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SysConfigDlg)
	DDX_Control(pDX, IDC_COMBO_COMLIST_D4, m_ComboComListD4);
	DDX_Control(pDX, IDC_COMBO_COMLIST_D3, m_ComboComListD3);
	DDX_Control(pDX, IDC_COMBO_COMLIST_D2, m_ComboComListD2);
	DDX_Control(pDX, IDC_COMBO_COMLIST_D1, m_ComboComListD1);
	DDX_Control(pDX, IDC_COMBO_COMLIST_C4, m_ComboComListC4);
	DDX_Control(pDX, IDC_COMBO_COMLIST_C3, m_ComboComListC3);
	DDX_Control(pDX, IDC_COMBO_COMLIST_C2, m_ComboComListC2);
	DDX_Control(pDX, IDC_COMBO_COMLIST_C1, m_ComboComListC1);
	DDX_Control(pDX, IDC_COMBO_COMLIST_B4, m_ComboComListB4);
	DDX_Control(pDX, IDC_COMBO_COMLIST_B3, m_ComboComListB3);
	DDX_Control(pDX, IDC_COMBO_COMLIST_B2, m_ComboComListB2);
	DDX_Control(pDX, IDC_COMBO_COMLIST_B1, m_ComboComListB1);
	DDX_Control(pDX, IDC_COMBO_COMLIST_A4, m_ComboComListA4);
	DDX_Control(pDX, IDC_COMBO_COMLIST_A3, m_ComboComListA3);
	DDX_Control(pDX, IDC_COMBO_COMLIST_A2, m_ComboComListA2);
	DDX_Control(pDX, IDC_COMBO_COMLIST_A1, m_ComboComListA1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SysConfigDlg, CDialog)
	//{{AFX_MSG_MAP(SysConfigDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_COMLIST_A1, OnSelchangeComboComlistA1)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SysConfigDlg message handlers

void SysConfigDlg::OnOK() 
{
	int iCarIndex;
	char tmpDataRecordPath[256]={0};
	char tmpStr[3];
	CString strTemp[16];
	
	//获取当前选中文件保存路径
	this->GetDlgItem ( IDC_EDIT_DATARECORD_PATH )->GetWindowText( tmpDataRecordPath, 256 );	//数据保存路径
	strcpy(cfg_DataRecordPath,tmpDataRecordPath);
	WriteToConfig();
	
	//获取当前选中的串口值
	((CComboBox*)GetDlgItem(IDC_COMBO_COMLIST_A1))->GetWindowText(strTemp[0]);
	((CComboBox*)GetDlgItem(IDC_COMBO_COMLIST_A2))->GetWindowText(strTemp[1]);
	((CComboBox*)GetDlgItem(IDC_COMBO_COMLIST_A3))->GetWindowText(strTemp[2]);
	((CComboBox*)GetDlgItem(IDC_COMBO_COMLIST_A4))->GetWindowText(strTemp[3]);
	((CComboBox*)GetDlgItem(IDC_COMBO_COMLIST_B1))->GetWindowText(strTemp[4]);
	((CComboBox*)GetDlgItem(IDC_COMBO_COMLIST_B2))->GetWindowText(strTemp[5]);
	((CComboBox*)GetDlgItem(IDC_COMBO_COMLIST_B3))->GetWindowText(strTemp[6]);
	((CComboBox*)GetDlgItem(IDC_COMBO_COMLIST_B4))->GetWindowText(strTemp[7]);
	((CComboBox*)GetDlgItem(IDC_COMBO_COMLIST_C1))->GetWindowText(strTemp[8]);
	((CComboBox*)GetDlgItem(IDC_COMBO_COMLIST_C2))->GetWindowText(strTemp[9]);
	((CComboBox*)GetDlgItem(IDC_COMBO_COMLIST_C3))->GetWindowText(strTemp[10]);
	((CComboBox*)GetDlgItem(IDC_COMBO_COMLIST_C4))->GetWindowText(strTemp[11]);
	((CComboBox*)GetDlgItem(IDC_COMBO_COMLIST_D1))->GetWindowText(strTemp[12]);
	((CComboBox*)GetDlgItem(IDC_COMBO_COMLIST_D2))->GetWindowText(strTemp[13]);
	((CComboBox*)GetDlgItem(IDC_COMBO_COMLIST_D3))->GetWindowText(strTemp[14]);
	((CComboBox*)GetDlgItem(IDC_COMBO_COMLIST_D4))->GetWindowText(strTemp[15]);

	//判断,选中串口不能相同
	for(int i=0;i<16;i++)
	{
		for(int j=0;j<16;j++)
		{
			if(i!=j && !strTemp[i].IsEmpty() && !strTemp[j].IsEmpty() && strTemp[i]==strTemp[j])
			{	
				AfxMessageBox("同一个串口只能选中一次!");
				return;
			}
		}
	}


	//选中串口赋值到全局结构体g_AllCar
	for(iCarIndex=0; iCarIndex<MAX_CAR_COUNT; iCarIndex++)
	{
		
		if(!strTemp[iCarIndex].IsEmpty())
		{
			strcpy(tmpStr, strTemp[iCarIndex].Mid(3)); 
			sscanf(tmpStr, "%d",&g_AllCar[iCarIndex].m_ComID);
		}else
		{
			g_AllCar[iCarIndex].m_ComID=0;
		}	
	}
	cfg_EnableBeep = ((CButton *)this->GetDlgItem (IDC_CHECK_ENABLEBEEP))->GetCheck ( ) ;
	cfg_EnableLight = ((CButton *)this->GetDlgItem (IDC_CHECK_ENABLELIGHT))->GetCheck ( ) ;

	CDialog::OnOK();
}

BOOL SysConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strCom;
	int retIndex;
	this->m_ComboComListA1.AddString("");
	this->m_ComboComListA2.AddString("");
	this->m_ComboComListA3.AddString("");
	this->m_ComboComListA4.AddString("");
	this->m_ComboComListB1.AddString("");
	this->m_ComboComListB2.AddString("");
	this->m_ComboComListB3.AddString("");
	this->m_ComboComListB4.AddString("");
	this->m_ComboComListC1.AddString("");
	this->m_ComboComListC2.AddString("");
	this->m_ComboComListC3.AddString("");
	this->m_ComboComListC4.AddString("");
	this->m_ComboComListD1.AddString("");
	this->m_ComboComListD2.AddString("");
	this->m_ComboComListD3.AddString("");
	this->m_ComboComListD4.AddString("");
	
	//初始化可用串口列表
	for(int comIndex=0; comIndex<g_AvailableComCounter; comIndex++)
	{
		strCom.Format("COM%d", g_ComList[comIndex]);		
		this->m_ComboComListA1.AddString(strCom);
		this->m_ComboComListA2.AddString(strCom);
		this->m_ComboComListA3.AddString(strCom);
		this->m_ComboComListA4.AddString(strCom);
		this->m_ComboComListB1.AddString(strCom);
		this->m_ComboComListB2.AddString(strCom);
		this->m_ComboComListB3.AddString(strCom);
		this->m_ComboComListB4.AddString(strCom);
		this->m_ComboComListC1.AddString(strCom);
		this->m_ComboComListC2.AddString(strCom);
		this->m_ComboComListC3.AddString(strCom);
		this->m_ComboComListC4.AddString(strCom);
		this->m_ComboComListD1.AddString(strCom);
		this->m_ComboComListD2.AddString(strCom);
		this->m_ComboComListD3.AddString(strCom);
		this->m_ComboComListD4.AddString(strCom);	
		
	}
	//初始化当前选中项,查找当前选中串口字符串“COM3”的下标
	strCom.Format("COM%d", g_AllCar[0].m_ComID);
	retIndex = m_ComboComListA1.FindString(0,strCom);
	if(retIndex>=0)
	{
		m_ComboComListA1.SetCurSel(retIndex);
	}	
	strCom.Format("COM%d", g_AllCar[1].m_ComID);
	retIndex = m_ComboComListA2.FindString(0,strCom);
	if(retIndex>=0)
	{
		m_ComboComListA2.SetCurSel(retIndex);
	}
	strCom.Format("COM%d", g_AllCar[2].m_ComID);
	retIndex = m_ComboComListA3.FindString(0,strCom);
	if(retIndex>=0)
	{
		m_ComboComListA3.SetCurSel(retIndex);
	}
	strCom.Format("COM%d", g_AllCar[3].m_ComID);
	retIndex = m_ComboComListA4.FindString(0,strCom);
	if(retIndex>=0)
	{
		m_ComboComListA4.SetCurSel(retIndex);
	}
	//B----------------------------------------------
	strCom.Format("COM%d", g_AllCar[4].m_ComID);
	retIndex = m_ComboComListB1.FindString(0,strCom);
	if(retIndex>=0)
	{
		m_ComboComListB1.SetCurSel(retIndex);
	}	
	strCom.Format("COM%d", g_AllCar[5].m_ComID);
	retIndex = m_ComboComListB2.FindString(0,strCom);
	if(retIndex>=0)
	{
		m_ComboComListB2.SetCurSel(retIndex);
	}
	strCom.Format("COM%d", g_AllCar[6].m_ComID);
	retIndex = m_ComboComListB3.FindString(0,strCom);
	if(retIndex>=0)
	{
		m_ComboComListB3.SetCurSel(retIndex);
	}
	strCom.Format("COM%d", g_AllCar[7].m_ComID);
	retIndex = m_ComboComListB4.FindString(0,strCom);
	if(retIndex>=0)
	{
		m_ComboComListB4.SetCurSel(retIndex);
	}
	//C----------------------------------------------
	strCom.Format("COM%d", g_AllCar[8].m_ComID);
	retIndex = m_ComboComListC1.FindString(0,strCom);
	if(retIndex>=0)
	{
		m_ComboComListC1.SetCurSel(retIndex);
	}	
	strCom.Format("COM%d", g_AllCar[9].m_ComID);
	retIndex = m_ComboComListC2.FindString(0,strCom);
	if(retIndex>=0)
	{
		m_ComboComListC2.SetCurSel(retIndex);
	}
	strCom.Format("COM%d", g_AllCar[10].m_ComID);
	retIndex = m_ComboComListC3.FindString(0,strCom);
	if(retIndex>=0)
	{
		m_ComboComListC3.SetCurSel(retIndex);
	}
	strCom.Format("COM%d", g_AllCar[11].m_ComID);
	retIndex = m_ComboComListC4.FindString(0,strCom);
	if(retIndex>=0)
	{
		m_ComboComListC4.SetCurSel(retIndex);
	}
	//D----------------------------------------------
	strCom.Format("COM%d", g_AllCar[12].m_ComID);
	retIndex = m_ComboComListD1.FindString(0,strCom);
	if(retIndex>=0)
	{
		m_ComboComListD1.SetCurSel(retIndex);
	}	
	strCom.Format("COM%d", g_AllCar[13].m_ComID);
	retIndex = m_ComboComListD2.FindString(0,strCom);
	if(retIndex>=0)
	{
		m_ComboComListD2.SetCurSel(retIndex);
	}
	strCom.Format("COM%d", g_AllCar[14].m_ComID);
	retIndex = m_ComboComListD3.FindString(0,strCom);
	if(retIndex>=0)
	{
		m_ComboComListD3.SetCurSel(retIndex);
	}
	strCom.Format("COM%d", g_AllCar[15].m_ComID);
	retIndex = m_ComboComListD4.FindString(0,strCom);
	if(retIndex>=0)
	{
		m_ComboComListD4.SetCurSel(retIndex);
	}


	//初始化系统盘符列表--------------------------------------
	/*char szBuf[100];
    memset(szBuf,0,100);
    long len=GetLogicalDriveStrings(sizeof(szBuf)/sizeof(TCHAR),szBuf);
    for (char* s= szBuf;  *s;  s+=strlen(s)+1)
    {
         char* sDrivePath = s;
         this->m_ComboDiskList.AddString(sDrivePath);
    }
	this->m_ComboDiskList.SetCurSel(0);
	*/
	this->SetDlgItemText(IDC_EDIT_DATARECORD_PATH,cfg_DataRecordPath);
	((CButton *)this->GetDlgItem (IDC_CHECK_ENABLEBEEP))->SetCheck (cfg_EnableBeep);
	((CButton *)this->GetDlgItem (IDC_CHECK_ENABLELIGHT))->SetCheck (cfg_EnableLight);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void SysConfigDlg::OnSelchangeComboComlistA1() 
{
		
}

void SysConfigDlg::OnButtonBrowse() 
{
	CString str;
	CString m_Path;
	BROWSEINFO bi;
	char name[MAX_PATH];
	ZeroMemory(&bi,sizeof(BROWSEINFO));
	bi.hwndOwner=GetSafeHwnd();
	bi.pszDisplayName=name;
	bi.lpszTitle="Select folder";
	bi.ulFlags=BIF_RETURNONLYFSDIRS;//BIF_USENEWUI;
	LPITEMIDLIST idl=SHBrowseForFolder(&bi);
	if(idl==NULL)
		return;
	SHGetPathFromIDList(idl,str.GetBuffer(MAX_PATH));
	str.ReleaseBuffer();
	m_Path=str;
	if(str.GetAt(str.GetLength()-1)!='\\')
		m_Path+="\\";
	
	this->SetDlgItemText(IDC_EDIT_DATARECORD_PATH,m_Path);

}
