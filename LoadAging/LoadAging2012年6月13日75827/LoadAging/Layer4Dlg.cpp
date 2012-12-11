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
	DDX_Control(pDX, IDC_COMBO_LAYER4_PARAMODE, m_ComboLayer4ParaMode);
	DDX_Control(pDX, IDC_COMBO_LAYER4_LOADMODE, m_ComboLayer4LoadMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Layer4Dlg, CDialog)
	//{{AFX_MSG_MAP(Layer4Dlg)
	ON_CBN_SELCHANGE(IDC_COMBO_LAYER4_LOADMODE, OnSelchangeComboLayer4Loadmode)
	ON_CBN_SELCHANGE(IDC_COMBO_LAYER4_PARAMODE, OnSelchangeComboLayer4Paramode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Layer4Dlg message handlers

BOOL Layer4Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ComboLayer4LoadMode.AddString("CC模式");
	m_ComboLayer4LoadMode.AddString("CV模式");
	m_ComboLayer4LoadMode.SetCurSel(0);	
	m_ComboLayer4ParaMode.AddString("无并联");
	m_ComboLayer4ParaMode.AddString("两路并联");
	m_ComboLayer4ParaMode.AddString("四路并联");
	m_ComboLayer4ParaMode.SetCurSel(0);

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
		this->GetDlgItem(IDC_COMBO_LAYER4_PARAMODE)->EnableWindow(TRUE);
				this->m_ComboLayer4ParaMode.SetCurSel(0);

		this->SetDlgItemText(IDC_STATIC_VALUE,"定电流值");
		this->SetDlgItemText(IDC_STATIC_MAX,"电压上限");
		this->SetDlgItemText(IDC_STATIC_MIN,"电压下限");
		break;
	case 1:
		this->m_ComboLayer4ParaMode.SetCurSel(0);
		this->GetDlgItem(IDC_COMBO_LAYER4_PARAMODE)->EnableWindow(FALSE);
		this->SetDlgItemText(IDC_STATIC_VALUE,"定电压值");
		this->SetDlgItemText(IDC_STATIC_MAX,"电流上限");
		this->SetDlgItemText(IDC_STATIC_MIN,"电流下限");
		break;
	}	
			this->GetDlgItem(IDC_EDIT_LAYER4_CHN1_VALUE)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN1_MAX)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN1_MIN)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN1_PRONAME)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN2_VALUE)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN2_MAX)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN2_MIN)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN2_PRONAME)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN3_VALUE)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN3_MAX)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN3_MIN)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN3_PRONAME)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN4_VALUE)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN4_MAX)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN4_MIN)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN4_PRONAME)->EnableWindow(TRUE);

}

void Layer4Dlg::OnSelchangeComboLayer4Paramode() 
{
	switch(this->m_ComboLayer4ParaMode.GetCurSel())
	{
	case 0://无并联
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN1_VALUE)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN1_MAX)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN1_MIN)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN1_PRONAME)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN2_VALUE)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN2_MAX)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN2_MIN)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN2_PRONAME)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN3_VALUE)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN3_MAX)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN3_MIN)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN3_PRONAME)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN4_VALUE)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN4_MAX)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN4_MIN)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN4_PRONAME)->EnableWindow(TRUE);

		break;
	case 1://二路并联
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN1_VALUE)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN1_MAX)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN1_MIN)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN1_PRONAME)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN2_VALUE)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN2_MAX)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN2_MIN)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN2_PRONAME)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN3_VALUE)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN3_MAX)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN3_MIN)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN3_PRONAME)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN4_VALUE)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN4_MAX)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN4_MIN)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN4_PRONAME)->EnableWindow(FALSE);

		break;
	case 2://四路并联
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN1_VALUE)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN1_MAX)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN1_MIN)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN1_PRONAME)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN2_VALUE)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN2_MAX)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN2_MIN)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN2_PRONAME)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN3_VALUE)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN3_MAX)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN3_MIN)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN3_PRONAME)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN4_VALUE)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN4_MAX)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN4_MIN)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER4_CHN4_PRONAME)->EnableWindow(FALSE);

		break;
	}			
}
