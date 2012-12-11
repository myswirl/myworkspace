#if !defined(AFX_SYSCONFIGDLG_H__10F8505E_9CB9_48C1_91AC_1AADEA66FB03__INCLUDED_)
#define AFX_SYSCONFIGDLG_H__10F8505E_9CB9_48C1_91AC_1AADEA66FB03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SysConfigDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SysConfigDlg dialog

class SysConfigDlg : public CDialog
{
// Construction
public:
	SysConfigDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(SysConfigDlg)
	enum { IDD = IDD_DIALOG_SYSCONFIG };
	CComboBox	m_ComboComListD4;
	CComboBox	m_ComboComListD3;
	CComboBox	m_ComboComListD2;
	CComboBox	m_ComboComListD1;
	CComboBox	m_ComboComListC4;
	CComboBox	m_ComboComListC3;
	CComboBox	m_ComboComListC2;
	CComboBox	m_ComboComListC1;
	CComboBox	m_ComboComListB4;
	CComboBox	m_ComboComListB3;
	CComboBox	m_ComboComListB2;
	CComboBox	m_ComboComListB1;
	CComboBox	m_ComboComListA4;
	CComboBox	m_ComboComListA3;
	CComboBox	m_ComboComListA2;
	CComboBox	m_ComboComListA1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SysConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SysConfigDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboComlistA1();
	afx_msg void OnButtonBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSCONFIGDLG_H__10F8505E_9CB9_48C1_91AC_1AADEA66FB03__INCLUDED_)
