// Layer4Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "loadaging.h"
#include "Layer4Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Layer4Dlg dialog


Layer4Dlg::Layer4Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(Layer4Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(Layer4Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void Layer4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Layer4Dlg)
	DDX_Control(pDX, IDC_COMBO_LAYER4_LOADMODE, m_ComboLayer4LoadMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Layer4Dlg, CDialog)
	//{{AFX_MSG_MAP(Layer4Dlg)
	ON_CBN_SELCHANGE(IDC_COMBO_LAYER4_LOADMODE, OnSelchangeComboLayer4Loadmode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Layer4Dlg message handlers

BOOL Layer4Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ComboLayer4LoadMode.AddString("CCģʽ");
	m_ComboLayer4LoadMode.AddString("CVģʽ");
	m_ComboLayer4LoadMode.SetCurSel(0);	
	
	this->SetDlgItemText(IDC_EDIT_LAYER4_CHN1_VALUE,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER4_CHN2_VALUE,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER4_CHN3_VALUE,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER4_CHN4_VALUE,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER4_CHN1_MAX,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER4_CHN2_MAX,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER4_CHN3_MAX,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER4_CHN4_MAX,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER4_CHN1_MIN,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER4_CHN2_MIN,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER4_CHN3_MIN,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER4_CHN4_MIN,"0");	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void Layer4Dlg::OnSelchangeComboLayer4Loadmode() 
{
	switch(this->m_ComboLayer4LoadMode.GetCurSel())
	{
	case 0:
		this->SetDlgItemText(IDC_STATIC_VALUE,"������ֵ");
		this->SetDlgItemText(IDC_STATIC_MAX,"��ѹ����");
		this->SetDlgItemText(IDC_STATIC_MIN,"��ѹ����");
		break;
	case 1:
		this->SetDlgItemText(IDC_STATIC_VALUE,"����ѹֵ");
		this->SetDlgItemText(IDC_STATIC_MAX,"��������");
		this->SetDlgItemText(IDC_STATIC_MIN,"��������");
		break;
	}	
}
