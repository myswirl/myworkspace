// hangePasswordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LoadAging.h"
#include "hangePasswordDlg.h"
#include "LoadAging_Tran.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// ChangePasswordDlg dialog


ChangePasswordDlg::ChangePasswordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ChangePasswordDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ChangePasswordDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void ChangePasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ChangePasswordDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ChangePasswordDlg, CDialog)
	//{{AFX_MSG_MAP(ChangePasswordDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ChangePasswordDlg message handlers

void ChangePasswordDlg::OnOK() 
{
	char currentPassword[30]={0};
	char newPassword[30]={0};
	char newPassword2[30]={0};
	char tmpStr[64]={0};
	
	this->GetDlgItem ( IDC_EDIT_CURRENTPASSWORD )->GetWindowText( currentPassword, 30 );	//当前获取密码
	this->GetDlgItem ( IDC_EDIT_NEWPASSWORD )->GetWindowText( newPassword, 30 );			//新密码
	this->GetDlgItem ( IDC_EDIT_NEWPASSWORD2 )->GetWindowText( newPassword2, 30 );			//新密码重复输入

	if( strlen(currentPassword) == 0 )
	{
		sprintf(tmpStr,"当前密码不能为空!");
		SetDlgItemText(IDC_STATIC_CHANGEERRORMSG,tmpStr);
		return;
	}
	if( strlen(newPassword) == 0 )
	{
		sprintf(tmpStr,"新密码不能为空!");
		SetDlgItemText(IDC_STATIC_CHANGEERRORMSG,tmpStr);
		return;
	}
	if(strcmp(newPassword, newPassword2) != 0)
	{
		sprintf(tmpStr,"两次输入密码不一样!");
		SetDlgItemText(IDC_STATIC_CHANGEERRORMSG,tmpStr);
		return;
	}
	
	if (g_LoginCheckOK==0 )	//工程师
	{
		if(strcmp(currentPassword, cfg_SuperPassword) == 0)
		{
			strcpy(cfg_SuperPassword,newPassword);
			WriteToConfig();
			CDialog::OnOK();
		}else
		{
			sprintf(tmpStr,"当前密码错误!");
			SetDlgItemText(IDC_STATIC_CHANGEERRORMSG,tmpStr);
			return;
		}
	}else if(g_LoginCheckOK == 1)	//技术员
	{
		if(strcmp(currentPassword, cfg_NormalPassword) == 0)
		{
			strcpy(cfg_SuperPassword,newPassword);
			WriteToConfig();
			CDialog::OnOK();
		}else
		{
			sprintf(tmpStr,"当前密码错误!");
			SetDlgItemText(IDC_STATIC_CHANGEERRORMSG,tmpStr);
			return;
		}
	}
	//CDialog::OnOK();
}
