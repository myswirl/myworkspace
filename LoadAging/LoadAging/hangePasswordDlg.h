#if !defined(AFX_HANGEPASSWORDDLG_H__544F08A3_4E88_4A50_9E56_5411FA00C456__INCLUDED_)
#define AFX_HANGEPASSWORDDLG_H__544F08A3_4E88_4A50_9E56_5411FA00C456__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// hangePasswordDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ChangePasswordDlg dialog

class ChangePasswordDlg : public CDialog
{
// Construction
public:
	ChangePasswordDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ChangePasswordDlg)
	enum { IDD = IDD_DIALOG_CHANGEPASSWORD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ChangePasswordDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ChangePasswordDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HANGEPASSWORDDLG_H__544F08A3_4E88_4A50_9E56_5411FA00C456__INCLUDED_)
