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
	DDX_Control(pDX, IDC_COMBO_LAYER2_PARAMODE, m_ComboLayer2ParaMode);
	DDX_Control(pDX, IDC_COMBO_LAYER2_LOADMODE, m_ComboLayer2LoadMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Layer2Dlg, CDialog)
	//{{AFX_MSG_MAP(Layer2Dlg)
	ON_CBN_SELCHANGE(IDC_COMBO_LAYER2_LOADMODE, OnSelchangeComboLayer2Loadmode)
	ON_CBN_SELCHANGE(IDC_COMBO_LAYER2_PARAMODE, OnSelchangeComboLayer2Paramode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Layer2Dlg message handlers

BOOL Layer2Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ComboLayer2LoadMode.AddString("CCģʽ");
	m_ComboLayer2LoadMode.AddString("CVģʽ");
	m_ComboLayer2LoadMode.SetCurSel(0);
	m_ComboLayer2ParaMode.AddString("�޲���");
	m_ComboLayer2ParaMode.AddString("��·����");
	m_ComboLayer2ParaMode.AddString("��·����");
	m_ComboLayer2ParaMode.SetCurSel(0);

		
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
		this->GetDlgItem(IDC_COMBO_LAYER2_PARAMODE)->EnableWindow(TRUE);
		this->m_ComboLayer2ParaMode.SetCurSel(0);
		
		this->SetDlgItemText(IDC_STATIC_VALUE,"������ֵ");
		this->SetDlgItemText(IDC_STATIC_MAX,"��ѹ����");
		this->SetDlgItemText(IDC_STATIC_MIN,"��ѹ����");
		break;
	case 1:
		this->m_ComboLayer2ParaMode.SetCurSel(0);
		this->GetDlgItem(IDC_COMBO_LAYER2_PARAMODE)->EnableWindow(FALSE);
		this->SetDlgItemText(IDC_STATIC_VALUE,"����ѹֵ");
		this->SetDlgItemText(IDC_STATIC_MAX,"��������");
		this->SetDlgItemText(IDC_STATIC_MIN,"��������");
		break;
	}
	this->GetDlgItem(IDC_EDIT_LAYER2_CHN1_VALUE)->EnableWindow(TRUE);
	this->GetDlgItem(IDC_EDIT_LAYER2_CHN1_MAX)->EnableWindow(TRUE);
	this->GetDlgItem(IDC_EDIT_LAYER2_CHN1_MIN)->EnableWindow(TRUE);
	this->GetDlgItem(IDC_EDIT_LAYER2_CHN1_PRONAME)->EnableWindow(TRUE);
	this->GetDlgItem(IDC_EDIT_LAYER2_CHN2_VALUE)->EnableWindow(TRUE);
	this->GetDlgItem(IDC_EDIT_LAYER2_CHN2_MAX)->EnableWindow(TRUE);
	this->GetDlgItem(IDC_EDIT_LAYER2_CHN2_MIN)->EnableWindow(TRUE);
	this->GetDlgItem(IDC_EDIT_LAYER2_CHN2_PRONAME)->EnableWindow(TRUE);
	this->GetDlgItem(IDC_EDIT_LAYER2_CHN3_VALUE)->EnableWindow(TRUE);
	this->GetDlgItem(IDC_EDIT_LAYER2_CHN3_MAX)->EnableWindow(TRUE);
	this->GetDlgItem(IDC_EDIT_LAYER2_CHN3_MIN)->EnableWindow(TRUE);
	this->GetDlgItem(IDC_EDIT_LAYER2_CHN3_PRONAME)->EnableWindow(TRUE);
	this->GetDlgItem(IDC_EDIT_LAYER2_CHN4_VALUE)->EnableWindow(TRUE);
	this->GetDlgItem(IDC_EDIT_LAYER2_CHN4_MAX)->EnableWindow(TRUE);
	this->GetDlgItem(IDC_EDIT_LAYER2_CHN4_MIN)->EnableWindow(TRUE);
	this->GetDlgItem(IDC_EDIT_LAYER2_CHN4_PRONAME)->EnableWindow(TRUE);
	
	
}

void Layer2Dlg::OnSelchangeComboLayer2Paramode() 
{
	switch(this->m_ComboLayer2ParaMode.GetCurSel())
	{
	case 0://�޲���
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN1_VALUE)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN1_MAX)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN1_MIN)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN1_PRONAME)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN2_VALUE)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN2_MAX)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN2_MIN)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN2_PRONAME)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN3_VALUE)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN3_MAX)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN3_MIN)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN3_PRONAME)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN4_VALUE)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN4_MAX)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN4_MIN)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN4_PRONAME)->EnableWindow(TRUE);

		break;
	case 1://��·����
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN1_VALUE)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN1_MAX)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN1_MIN)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN1_PRONAME)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN2_VALUE)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN2_MAX)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN2_MIN)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN2_PRONAME)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN3_VALUE)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN3_MAX)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN3_MIN)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN3_PRONAME)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN4_VALUE)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN4_MAX)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN4_MIN)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN4_PRONAME)->EnableWindow(FALSE);

		break;
	case 2://��·����
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN1_VALUE)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN1_MAX)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN1_MIN)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN1_PRONAME)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN2_VALUE)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN2_MAX)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN2_MIN)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN2_PRONAME)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN3_VALUE)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN3_MAX)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN3_MIN)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN3_PRONAME)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN4_VALUE)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN4_MAX)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN4_MIN)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_LAYER2_CHN4_PRONAME)->EnableWindow(FALSE);

		break;
	}		
}
