// Layer5Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "loadaging.h"
#include "Layer5Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Layer5Dlg dialog


Layer5Dlg::Layer5Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(Layer5Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(Layer5Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void Layer5Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Layer5Dlg)
	DDX_Control(pDX, IDC_COMBO_LAYER5_PARAMODE, m_ComboLayer5ParaMode);
	DDX_Control(pDX, IDC_COMBO_LAYER5_LOADMODE, m_ComboLayer5LoadMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Layer5Dlg, CDialog)
	//{{AFX_MSG_MAP(Layer5Dlg)
	ON_CBN_SELCHANGE(IDC_COMBO_LAYER5_LOADMODE, OnSelchangeComboLayer5Loadmode)
	ON_CBN_SELCHANGE(IDC_COMBO_LAYER5_PARAMODE, OnSelchangeComboLayer5Paramode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Layer5Dlg message handlers

BOOL Layer5Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ComboLayer5LoadMode.AddString("CC模式");
	m_ComboLayer5LoadMode.AddString("CV模式");
	m_ComboLayer5LoadMode.SetCurSel(0);	
	m_ComboLayer5ParaMode.AddString("无并联");
	m_ComboLayer5ParaMode.AddString("两路并联");
	m_ComboLayer5ParaMode.AddString("四路并联");
	m_ComboLayer5ParaMode.SetCurSel(0);

	this->SetDlgItemText(IDC_EDIT_LAYER5_CHN1_VALUE,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER5_CHN2_VALUE,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER5_CHN3_VALUE,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER5_CHN4_VALUE,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER5_CHN1_MAX,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER5_CHN2_MAX,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER5_CHN3_MAX,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER5_CHN4_MAX,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER5_CHN1_MIN,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER5_CHN2_MIN,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER5_CHN3_MIN,"0");
	this->SetDlgItemText(IDC_EDIT_LAYER5_CHN4_MIN,"0");	


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void Layer5Dlg::OnSelchangeComboLayer5Loadmode() 
{
	switch(this->m_ComboLayer5LoadMode.GetCurSel())
	{
	case 0:
		this->GetDlgItem(IDC_COMBO_LAYER5_PARAMODE)->EnableWindow(TRUE);
				this->m_ComboLayer5ParaMode.SetCurSel(0);

		this->SetDlgItemText(IDC_STATIC_VALUE,"定电流值");
		this->SetDlgItemText(IDC_STATIC_MAX,"电压上限");
		this->SetDlgItemText(IDC_STATIC_MIN,"电压下限");
		break;
	case 1:
		this->m_ComboLayer5ParaMode.SetCurSel(0);
		this->GetDlgItem(IDC_COMBO_LAYER5_PARAMODE)->EnableWindow(FALSE);
		this->SetDlgItemText(IDC_STATIC_VALUE,"定电压值");
		this->SetDlgItemText(IDC_STATIC_MAX,"电流上限");
		this->SetDlgItemText(IDC_STATIC_MIN,"电流下限");
		break;
	}	
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN1_VALUE)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN1_MAX)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN1_MIN)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN1_PRONAME)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN2_VALUE)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN2_MAX)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN2_MIN)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN2_PRONAME)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN3_VALUE)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN3_MAX)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN3_MIN)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN3_PRONAME)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN4_VALUE)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN4_MAX)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN4_MIN)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN4_PRONAME)->EnableWindow(TRUE);

}

void Layer5Dlg::OnSelchangeComboLayer5Paramode() 
{
	switch(this->m_ComboLayer5ParaMode.GetCurSel())
	{
	case 0://无并联
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN1_VALUE)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN1_MAX)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN1_MIN)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN1_PRONAME)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN2_VALUE)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN2_MAX)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN2_MIN)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN2_PRONAME)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN3_VALUE)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN3_MAX)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN3_MIN)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN3_PRONAME)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN4_VALUE)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN4_MAX)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN4_MIN)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN4_PRONAME)->EnableWindow(TRUE);

		break;
	case 1://二路并联
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN1_VALUE)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN1_MAX)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN1_MIN)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN1_PRONAME)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN2_VALUE)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN2_MAX)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN2_MIN)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN2_PRONAME)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN3_VALUE)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN3_MAX)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN3_MIN)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN3_PRONAME)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN4_VALUE)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN4_MAX)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN4_MIN)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN4_PRONAME)->EnableWindow(FALSE);

		break;
	case 2://四路并联
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN1_VALUE)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN1_MAX)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN1_MIN)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN1_PRONAME)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN2_VALUE)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN2_MAX)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN2_MIN)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN2_PRONAME)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN3_VALUE)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN3_MAX)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN3_MIN)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN3_PRONAME)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN4_VALUE)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN4_MAX)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN4_MIN)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER5_CHN4_PRONAME)->EnableWindow(FALSE);

		break;
	}			
}
