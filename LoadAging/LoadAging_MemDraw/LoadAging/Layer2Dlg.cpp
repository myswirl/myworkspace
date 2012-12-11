// Layer2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "loadaging.h"
#include "Layer2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Layer2Dlg dialog


Layer2Dlg::Layer2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(Layer2Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(Layer2Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void Layer2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Layer2Dlg)
	DDX_Control(pDX, IDC_COMBO_LAYER2_LOADMODE, m_ComboLayer2LoadMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Layer2Dlg, CDialog)
	//{{AFX_MSG_MAP(Layer2Dlg)
	ON_CBN_SELCHANGE(IDC_COMBO_LAYER2_LOADMODE, OnSelchangeComboLayer2Loadmode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Layer2Dlg message handlers

BOOL Layer2Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ComboLayer2LoadMode.AddString("CC模式");
	m_ComboLayer2LoadMode.AddString("CV模式");
	m_ComboLayer2LoadMode.SetCurSel(0);

		
	this->SetDlgItemText(IDC_EDIT_LAYER2_CHN1_VALUE,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER2_CHN2_VALUE,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER2_CHN3_VALUE,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER2_CHN4_VALUE,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER2_CHN1_MAX,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER2_CHN2_MAX,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER2_CHN3_MAX,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER2_CHN4_MAX,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER2_CHN1_MIN,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER2_CHN2_MIN,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER2_CHN3_MIN,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER2_CHN4_MIN,"0");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void Layer2Dlg::OnSelchangeComboLayer2Loadmode() 
{
	switch(this->m_ComboLayer2LoadMode.GetCurSel())
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
