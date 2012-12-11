#if !defined(AFX_LAYER5DLG_H__8D877CE3_B4F5_40AE_B8AD_B214D8D581C2__INCLUDED_)
#define AFX_LAYER5DLG_H__8D877CE3_B4F5_40AE_B8AD_B214D8D581C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Layer5Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Layer5Dlg dialog

class Layer5Dlg : public CDialog
{
// Construction
public:
	Layer5Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Layer5Dlg)
	enum { IDD = IDD_DIALOG_LAYER5 };
	CComboBox	m_ComboLayer5LoadMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Layer5Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Layer5Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboLayer5Loadmode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAYER5DLG_H__8D877CE3_B4F5_40AE_B8AD_B214D8D581C2__INCLUDED_)
