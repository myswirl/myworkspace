#if !defined(AFX_LAYER2DLG_H__0DA39C11_B159_4584_9E03_B087D5C035F6__INCLUDED_)
#define AFX_LAYER2DLG_H__0DA39C11_B159_4584_9E03_B087D5C035F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Layer2Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Layer2Dlg dialog

class Layer2Dlg : public CDialog
{
// Construction
public:
	Layer2Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Layer2Dlg)
	enum { IDD = IDD_DIALOG_LAYER2 };
	CComboBox	m_ComboLayer2LoadMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Layer2Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Layer2Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboLayer2Loadmode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAYER2DLG_H__0DA39C11_B159_4584_9E03_B087D5C035F6__INCLUDED_)
