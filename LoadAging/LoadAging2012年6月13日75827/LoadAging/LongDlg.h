#if !defined(AFX_LONGDLG_H__4AB4841F_6270_46AE_AE89_BA8115D3282D__INCLUDED_)
#define AFX_LONGDLG_H__4AB4841F_6270_46AE_AE89_BA8115D3282D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LongDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// LongDlg dialog

class LongDlg : public CDialog
{
// Construction
public:
	LongDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(LongDlg)
	enum { IDD = IDD_DIALOG_LOGIN };
	CComboBox	m_ComboUserName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(LongDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(LongDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LONGDLG_H__4AB4841F_6270_46AE_AE89_BA8115D3282D__INCLUDED_)
