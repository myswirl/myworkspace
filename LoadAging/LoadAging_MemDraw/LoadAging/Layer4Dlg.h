#if !defined(AFX_LAYER4DLG_H__DC264189_0117_462B_891C_776110E4C49A__INCLUDED_)
#define AFX_LAYER4DLG_H__DC264189_0117_462B_891C_776110E4C49A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Layer4Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Layer4Dlg dialog

class Layer4Dlg : public CDialog
{
// Construction
public:
	Layer4Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Layer4Dlg)
	enum { IDD = IDD_DIALOG_LAYER4 };
	CComboBox	m_ComboLayer4LoadMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Layer4Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Layer4Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboLayer4Loadmode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAYER4DLG_H__DC264189_0117_462B_891C_776110E4C49A__INCLUDED_)
