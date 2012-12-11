// Layer6Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "loadaging.h"
#include "Layer6Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Layer6Dlg dialog


Layer6Dlg::Layer6Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(Layer6Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(Layer6Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void Layer6Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Layer6Dlg)
	DDX_Control(pDX, IDC_COMBO_LAYER6_LOADMODE, m_ComboLayer6LoadMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Layer6Dlg, CDialog)
	//{{AFX_MSG_MAP(Layer6Dlg)
	ON_CBN_SELCHANGE(IDC_COMBO_LAYER6_LOADMODE, OnSelchangeComboLayer6Loadmode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Layer6Dlg message handlers

BOOL Layer6Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ComboLayer6LoadMode.AddString("CC模式");
	m_ComboLayer6LoadMode.AddString("CV模式");
	m_ComboLayer6LoadMode.SetCurSel(0);
	
	this->SetDlgItemText(IDC_EDIT_LAYER6_CHN1_VALUE,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER6_CHN2_VALUE,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER6_CHN3_VALUE,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER6_CHN4_VALUE,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER6_CHN1_MAX,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER6_CHN2_MAX,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER6_CHN3_MAX,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER6_CHN4_MAX,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER6_CHN1_MIN,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER6_CHN2_MIN,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER6_CHN3_MIN,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER6_CHN4_MIN,"0");	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void Layer6Dlg::OnSelchangeComboLayer6Loadmode() 
{
	switch(this->m_ComboLayer6LoadMode.GetCurSel())
	{
	case 0:
		this->SetDlgItemText(IDC_STATIC_VALUE,"定电流值");
		this->SetDlgItemText(IDC_STATIC_MAX,"电压上限");
		this->SetDlgItemText(IDC_STATIC_MIN,"电压下限");
		break;
	case 1:
		this->SetDlgItemText(IDC_STATIC_VALUE,"定电压值");
		this->SetDlgItemText(IDC_STATIC_MAX,"电流上限");
		this->SetDlgItemText(IDC_STATIC_MIN,"电流下限");
		break;
	}	
}
