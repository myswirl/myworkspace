#if !defined(AFX_LAYER6DLG_H__C28260A8_D275_4DFC_A82A_76D5DBC2B137__INCLUDED_)
#define AFX_LAYER6DLG_H__C28260A8_D275_4DFC_A82A_76D5DBC2B137__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Layer6Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Layer6Dlg dialog

class Layer6Dlg : public CDialog
{
// Construction
public:
	Layer6Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Layer6Dlg)
	enum { IDD = IDD_DIALOG_LAYER6 };
	CComboBox	m_ComboLayer6LoadMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Layer6Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Layer6Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboLayer6Loadmode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAYER6DLG_H__C28260A8_D275_4DFC_A82A_76D5DBC2B137__INCLUDED_)
